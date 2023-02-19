/*
    Configuration stuff
*/

// pin configuration
#define RADIO_CE    5
#define RADIO_CS    13
#define MOTOR_PIN   8
#define BUTTON_PIN  11
#define LED_PIN     1

// nRF24 addresses
#define ADDRESS1    "hello"
#define ADDRESS2    "asdff"

// polling times
#define READ_DELAY  30
#define WRITE_DELAY 20

// idle timeout
#define TIMEOUT     (uint32_t) 2 /* minutes */ * 60 /* seconds*/ * 1000 /* milliseconds */
