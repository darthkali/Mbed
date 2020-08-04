#include "mbed.h"

DigitalOut RCLK(PA_9);
DigitalOut SRCLK(PA_5);
DigitalOut SER(PA_6);

InterruptIn Btn(PC_13);
Timer interruptTimer;

Serial PC(PA_2, PA_3);


unsigned char pattern = 0;

// --- Send byte to HC595 ---
void SendHC595(unsigned char byData) {
    for (int nCount = 0; nCount < 8; nCount += 1) {
        SER = byData & 0x01;
        byData >>= 1;
        SRCLK = 1;
        wait(0.01);
        SRCLK = 0;
    }

    RCLK = 1;
    RCLK = 0;
}

void ShiftLed() {

    if (pattern) {
        pattern <<= 1;
    } else {
        pattern = 1;
    }

    //wait(0.05);
}

void BtnPressed() {


    PC.printf("Timer = %f\r\n", interruptTimer.read());

    if (interruptTimer.read() > 0.1F) {
        ShiftLed();
        interruptTimer.reset();

    }

}


// --- Main ---
int main() {

    Btn.fall(BtnPressed);
    PC.baud(115200);
    interruptTimer.start();


    while (1) {
        SendHC595(pattern);
        wait(0.001);
    }
}