#include "Arduino.h"
#include "SPI.h"
#include "RF24.h"


#define SEND_REPEATS 5

class RadioControls {
    public:
    static void Init(uint8_t CE, uint8_t CS, const char* address1, const char* address2, void (*motorHandler)(bool));
    static bool UpdateReceive();
    static bool UpdateTransmit(bool buttonState);
    static void Update(bool buttonState);

    static void PowerUp();
    static void PowerDown();
};