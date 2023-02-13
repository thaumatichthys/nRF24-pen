#include "RadioControls.hpp"


RF24 radio(5, 13);
static void (*_motorHandler)(bool);

void RadioControls::Init(uint8_t CE, uint8_t CS, const char* address1, const char* address2, void (*motorHandler)(bool)) {
    _motorHandler = motorHandler;

    //RadioControls::radio = new RF24(CE, CS);
    radio.begin();
    radio.setPayloadSize(2);
    radio.setDataRate(RF24_250KBPS);
    radio.setPALevel(RF24_PA_MAX);
    
    radio.openReadingPipe(1, (uint8_t*) address1);
    radio.openWritingPipe((uint8_t*) address2);
    radio.startListening();
}

void RadioControls::UpdateReceive() {
    if (radio.available()) {
        uint8_t received[2];
        radio.read(received, 2);
        if (received[1] == 123)
            _motorHandler(true);
        else if (received[1] == 234)
            _motorHandler(false);
        // clear read fifo
        while(radio.available()) {
            uint8_t dummy;
            radio.read(&dummy, 1);
        }
    }
}

void RadioControls::UpdateTransmit(bool buttonState) {
    RadioControls::UpdateReceive();
    for (int i = 0; i < SEND_REPEATS; i++) {
        uint8_t buffer[2];
        if (buttonState)
            buffer[1] = 123;
        else 
            buffer[1] = 234;
        radio.stopListening();
        if (radio.write(buffer, 2))
            break;
    }
    radio.startListening();
}

/*
    read radio
    if nothing: 
        send if necessary
        if it doesnt go through:
            send again
        if it keeps not going through for 5 times, just give up lmao
    else: 
        activate the motor

*/