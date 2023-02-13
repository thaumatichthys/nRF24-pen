#include "TaskScheduler.hpp"
#include "config.hpp"
#include "RadioControls.hpp"


RadioControls controls;

void setup() {
    controls.Init(RADIO_CE, RADIO_CS, ADDRESS1, ADDRESS2, MotorHandler);
    pinMode(BUTTON_PIN, INPUT);
    pinMode(MOTOR_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), ButtonISR, CHANGE);
    flip();
}

int a = 0;
void flip() {
    a = 1 - a;
    digitalWrite(MOTOR_PIN, a);
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

void ButtonISR() {
    AddTask(Transmit, 0);
}

void loop() {
    UpdateTasks();
}