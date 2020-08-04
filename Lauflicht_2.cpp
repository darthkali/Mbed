#include "mbed.h"

DigitalOut arrLeds[3] = {PB_3, PB_5, PB_4};

DigitalIn Button(PC_13);


int main() {
    while (1) {
        for (int i = 0; i < 3; i += 1) {
            arrLeds[0] = i == 0;
            arrLeds[1] = i == 1;
            arrLeds[2] = i == 2;

            wait(0.05);
        }


    }
}
    