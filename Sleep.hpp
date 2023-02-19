#include "Arduino.h"


void SleepInit(); // set RTC periodic interrupt, 1024Hz ULP + 2^14 cycle interrupt
void Sleep(uint8_t nRF24_CE); // set pin states and sleep mcu
