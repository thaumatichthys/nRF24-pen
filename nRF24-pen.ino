#include "TaskScheduler.hpp"
#include "config.hpp"
#include "RadioControls.hpp"


RadioControls controls;

void setup() {
    controls.Init(RADIO_CE, RADIO_CS, ADDRESS1, ADDRESS2, MotorHandler);
    //controls.Init(RADIO_CE, RADIO_CS, ADDRESS2, ADDRESS1, MotorHandler);
    pinMode(BUTTON_PIN, INPUT);
    pinMode(MOTOR_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), ButtonISR, CHANGE);
    flip();
}

int a = 0;
void flip() {
    //a = 1 - a;
    //digitalWrite(MOTOR_PIN, a);
    digitalWrite(LED_PIN, a);
    AddTask(flip, 200);
}

void MotorHandler(bool state) {
    digitalWrite(MOTOR_PIN, state);
}

void Receive() {
    controls.UpdateReceive();
    AddTask(Receive, 20);
}

void Transmit() {
    controls.UpdateTransmit(!digitalRead(BUTTON_PIN));
}

bool button_switched = false;

void ResetCooldown() {
    button_switched = false;
}

void ButtonISR() {
    if (!button_switched) {
        AddTask(Transmit, 0); // avoids needing volatile variables
        AddTask(ResetCooldown, 30);
        button_switched = true;

        a = 1 - a;
        digitalWrite(LED_PIN, a);
    }
}

void loop() {
    UpdateTasks();
}
