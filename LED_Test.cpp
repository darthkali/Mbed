#include "mbed.h"

Serial PC(PA_2, PA_3);

DigitalOut ledRed(PB_4);
DigitalOut ledGreen(PB_5);


void SetLed(int red, int green) {
    ledRed = red;
    ledGreen = green;
}

// get a char
void RxChar() {
    char ch = PC.getc();
    ch == '1' ? ledRed = 1 : ledRed = 0;

}


// --- Main ---
int main() {
    PC.baud(115200);
    PC.attach(RxChar);

    PC.printf("\x0C""Enter Text: ");


    while (1) {

    }
}
