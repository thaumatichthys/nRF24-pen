#include "TaskScheduler.hpp"
#include "config.hpp"
#include "RadioControls.hpp"


RadioControls controls;

void setup() {
    //controls.Init(RADIO_CE, RADIO_CS, ADDRESS1, ADDRESS2, MotorHandler);
    controls.Init(RADIO_CE, RADIO_CS, ADDRESS2, ADDRESS1, MotorHandler);
    pinMode(BUTTON_PIN, INPUT);
    pinMode(MOTOR_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    flip();
    Receive();
    Transmit();
}

int a = 0;
void flip() {
    a = 1 - a;
    digitalWrite(LED_PIN, a);
    AddTask(flip, 200);
}

void MotorHandler(bool state) {
    digitalWrite(MOTOR_PIN, state);
}

void Receive() {
    controls.UpdateReceive();
    AddTask(Receive, 30);
}

void Transmit() {
    controls.UpdateTransmit(!digitalRead(BUTTON_PIN));
    AddTask(Transmit, 20);
}

void loop() {
    UpdateTasks();
}
