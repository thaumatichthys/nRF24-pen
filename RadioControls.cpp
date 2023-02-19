#include "RadioControls.hpp"


RF24 radio(1, 2);
static void (*_motorHandler)(bool);

void RadioControls::Init(uint8_t CE, uint8_t CS, const char* address1, const char* address2, void (*motorHandler)(bool)) {
    _motorHandler = motorHandler;

    radio.begin(CE, CS);
    radio.setPayloadSize(2);
    radio.setDataRate(RF24_250KBPS);
    radio.setPALevel(RF24_PA_MAX);
    
    radio.openReadingPipe(1, (uint8_t*) address1);
    radio.openWritingPipe((uint8_t*) address2);
    radio.startListening();
}

bool RadioControls::UpdateReceive() {
    if (radio.available()) {
        _motorHandler(true);
        // clear read fifo
        while(radio.available()) {
            uint8_t dummy;
            radio.read(&dummy, 1);
        }
        return true;
    }
    _motorHandler(false);
    return false;
}

bool RadioControls::UpdateTransmit(bool buttonState) {
    bool success = true;
    if (buttonState) {
        success = false;
        radio.stopListening();
        for (int i = 0; i < SEND_REPEATS; i++) {
            uint8_t buffer[2];
            if (radio.write(buffer, 2)) {
                success = true;
                break;
            }
        }
        radio.startListening();
    }
    return success;
}

void RadioControls::PowerDown() {
    radio.powerDown();
}

void RadioControls::PowerUp() {
    radio.powerUp();
    radio.startListening();
}