#include "TaskScheduler.hpp"
#include "config.hpp"
#include "RadioControls.hpp"
#include "Sleep.hpp"


RadioControls controls;
uint32_t last_activity_time;

void setup() {
    // The two pens' code should be different here
    /*********************************************************************/

    controls.Init(RADIO_CE, RADIO_CS, ADDRESS1, ADDRESS2, MotorHandler);
    //controls.Init(RADIO_CE, RADIO_CS, ADDRESS2, ADDRESS1, MotorHandler);

    /*********************************************************************/
    pinMode(BUTTON_PIN, INPUT);
    pinMode(MOTOR_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);

    SleepInit();

    Receive();
    Transmit();
    CheckTimeout();

    last_activity_time = millis();
}

void MotorHandler(bool state) {
    digitalWrite(MOTOR_PIN, state);
    if (state) 
        last_activity_time = millis();
}

void Receive() {
    controls.UpdateReceive();
    AddTask(Receive, READ_DELAY);
}

void Transmit() {
    bool value = !digitalRead(BUTTON_PIN);
    digitalWrite(LED_PIN, controls.UpdateTransmit(value) && value);
    if (value) 
        last_activity_time = millis();
    AddTask(Transmit, WRITE_DELAY);
}

void CheckTimeout() {
    if ((millis() - last_activity_time) > TIMEOUT)
        PowerSave();
    AddTask(CheckTimeout, 50);
}

void PowerSave() {
    while(1) {
        controls.PowerDown();
        Sleep(RADIO_CE);
        controls.PowerUp();
        delay(READ_DELAY);
        if (controls.UpdateReceive() || (!digitalRead(BUTTON_PIN)))
            break;
    }
    last_activity_time = millis();
}

void loop() {
    UpdateTasks();
}
