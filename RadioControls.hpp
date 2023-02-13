#include "Arduino.h"
#include "SPI.h"
#include "RF24.h"


#define SEND_REPEATS 5

class RadioControls {
    //static RF24* radio;
    public:
    static void Init(uint8_t CE, uint8_t CS, const char* address1, const char* address2, void (*motorHandler)(bool));
    static void UpdateReceive();
    static void UpdateTransmit(bool buttonState);
    static void Update(bool buttonState);
};