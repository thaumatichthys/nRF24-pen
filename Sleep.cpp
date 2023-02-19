#include "Sleep.hpp"


void SleepInit() {
    cli(); // disable interrupts for now
    // RTC = real time clock, PIT = periodic interrupt timer
    RTC.CLKSEL = 0x1; // select 1024 Hz internal ULP oscillator
    // configure periodic interrupt
    RTC.PITCTRLA = (0xC << 3) | 0x1; // see page 311 of the datasheet, tldr, set period to 2048 cycles and enable the interrupt
    RTC.PITINTCTRL = 0x0; // disable the periodic interrupt for now
    RTC.PITINTFLAGS = 0x1;
    sei();
}

void Sleep(uint8_t nRF24_CE) {
    //power_down_gpio();
    uint8_t pinctrls[3][8];
    cli();
    for (int i = 0; i < 8; i++) {
        register8_t* ptr_a = &PORTA.PIN0CTRL + i;
        register8_t* ptr_b = &PORTB.PIN0CTRL + i;
        register8_t* ptr_c = &PORTC.PIN0CTRL + i;
        if (!(PORTA.DIR & (1U << i))) { pinctrls[0][i] = *ptr_a; *ptr_a |= ((1U << 3) | 0x4); }
        if (!(PORTB.DIR & (1U << i))) { pinctrls[1][i] = *ptr_b; *ptr_b |= ((1U << 3) | 0x4); }
        if (!(PORTC.DIR & (1U << i))) { pinctrls[2][i] = *ptr_c; *ptr_c |= ((1U << 3) | 0x4); }
    }
    RTC.CNT = 0; // set the rtc count register to 0 
    RTC.PITINTCTRL = 0x1; // turn on the interrupt
    SLPCTRL.CTRLA = 0b00000101; // power down mode, sleep enabled
    sei();
    asm("SLEEP"); // initiate the power down mode via sleep instruction
    cli();
    RTC.PITINTCTRL = 0x0; // turn off the interrupt
    RTC.PITINTFLAGS = 1; // reset the interrupt flag
    SLPCTRL.CTRLA = 0; // clear the sleep mode register
    for (int i = 0; i < 8; i++) {
        register8_t* ptr_a = &PORTA.PIN0CTRL + i;
        register8_t* ptr_b = &PORTB.PIN0CTRL + i;
        register8_t* ptr_c = &PORTC.PIN0CTRL + i;
        if (!(PORTA.DIR & (1U << i))) { *ptr_a = pinctrls[0][i]; }
        if (!(PORTB.DIR & (1U << i))) { *ptr_b = pinctrls[1][i]; }
        if (!(PORTC.DIR & (1U << i))) { *ptr_c = pinctrls[2][i]; }
    }
    sei();
}

ISR(RTC_PIT_vect) {
}