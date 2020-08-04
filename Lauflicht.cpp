#include "mbed.h"

#define WAIT 1

DigitalOut ledGreen(PB_3);
DigitalOut ledRed(PB_4);
DigitalOut ledYellow(PB_5);

DigitalIn Button(PC_13);

DigitalOut *arrLeds[3] = {&ledGreen, &ledRed, &ledYellow};


int main() {
    while (1) {
        for (int i = 0; i < 3; i += 1) {
            *arrLeds[0] = i == 0;
            *arrLeds[1] = i == 1;
            *arrLeds[2] = i == 2;

            wait(0.5);
        }
    }
}
