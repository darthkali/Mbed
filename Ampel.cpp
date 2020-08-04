#include "mbed.h"

Ticker tiTimer;
DigitalIn Button(PC_13);
DigitalOut ledGreen(PB_3);
DigitalOut ledYellow(PB_5);
DigitalOut ledRed(PB_4);

// --- Types ---
enum STATE {
    stINIT, stGREEN, stYELLOW, stRED, stWAIT, stYELLOW_RED
};

// --- Variables ---
STATE enState = stINIT;

int nTime = 0;
int nStateChanged = 0;

// helper function to invert the Button
int BtnDown() {
    return Button == 0;
}

// --- system tick ---
void Tick() {
    nTime += 1;
}

void SetLed(int red, int yellow, int green) {
    ledRed = red;
    ledYellow = yellow;
    ledGreen = green;
}


void SetNextStateAndTheWaitTime(STATE state, int time) {
    nStateChanged = nTime + time;
    enState = state;
}


void StateMachine() {
    switch (enState) {
        case stINIT:
            SetLed(0, 0, 1);
            enState = stGREEN;
            break;

        case stGREEN:
            if (BtnDown()) {
                SetNextStateAndTheWaitTime(stWAIT, 5);
            }
            break;

        case stWAIT:
            if (nStateChanged - nTime <= 0) {
                SetLed(0, 1, 0);
                SetNextStateAndTheWaitTime(stYELLOW, 1);
            }
            break;

        case stYELLOW:
            if (nStateChanged - nTime <= 0) {
                SetLed(1, 0, 0);
                SetNextStateAndTheWaitTime(stRED, 5);
            }
            break;

        case stRED:
            if (nStateChanged - nTime <= 0) {
                SetLed(1, 1, 0);
                SetNextStateAndTheWaitTime(stYELLOW_RED, 2);
            }
            break;

        case stYELLOW_RED:
            if (nStateChanged - nTime <= 0) {
                SetLed(0, 0, 1);
                SetNextStateAndTheWaitTime(stGREEN, 1);
            }
            break;
    }
}

// --- Main ---
int main() {
    tiTimer.attach(Tick, 1);

    while (1) {
        StateMachine();

    }
}
