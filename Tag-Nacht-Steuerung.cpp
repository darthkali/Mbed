#include "mbed.h"

Serial pc(PA_2, PA_3);                      // UART
Ticker messureTimer;
AnalogIn adSensor(PC_0);
PwmOut pwm(PB_10);
DigitalOut led(PA_5);

#define BREAKPOINT 0.3F
#define PWM_UP_DOWN 0.02F

// --- Types ---
enum STATE {
    stINIT, stUP, stDOWN, stWAIT
};

// --- Variables ---
int nStateChanged = 0;
int nTime = 0;
int ledStatus = 0;

STATE enState = stINIT;

// --- Functions ---

// --- system tick ---
void Tick() {
    nTime += 1;
}

// --- Tiefpass (Mittelwert) ---
float analogInput() {
    static float analog = 0.0F;
    float adin = adSensor;

    analog = (analog * 9 + adin) / 10;

    return analog;
}

void SetNextStateAndTheWaitTime(STATE state, int time) {
    nStateChanged = nTime + time;
    enState = state;
}

void StateMachine() {
    switch (enState) {
        case stINIT:
            pwm = 0;
            enState = stUP;
            break;

        case stUP:
            pwm = pwm + PWM_UP_DOWN;

            if (pwm >= 1) {
                SetNextStateAndTheWaitTime(stWAIT, 5);
            }
            break;

        case stDOWN:
            pwm = pwm - PWM_UP_DOWN;

            if (pwm <= 0) {
                SetNextStateAndTheWaitTime(stWAIT, 5);
            }
            break;

        case stWAIT:
            if (nStateChanged - nTime <= 0) {
                if (pwm <= 0) {
                    SetNextStateAndTheWaitTime(stUP, 0);
                } else {
                    SetNextStateAndTheWaitTime(stDOWN, 0);
                }
            }
            break;
    }
}

// --- Main ---
int main() {
    pc.baud(115200);
    pwm.period(0.001F);         // T für 10kHz
    pwm = 0.0F;                 // Duty Cycle in %
    messureTimer.attach(Tick, 1);

    while (1) {
        StateMachine();

        float analog = analogInput();

        analog <= BREAKPOINT ? led = 1 : led = 0;
        led == 1 ? ledStatus = 1 : ledStatus = 0;

        pc.printf("#A%f\r\n", analog);
        pc.printf("#B%d\r\n", ledStatus);
        pc.printf("#C%f\r\n", BREAKPOINT);

        wait(0.1);
    }
}
