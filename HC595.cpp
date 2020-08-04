#include "mbed.h"

DigitalOut RCLK(PA_9);
DigitalOut SRCLK(PA_5);
DigitalOut SER(PA_6);

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
    wait(0.1);
    RCLK = 0;
}

// --- Main ---
int main() {
    unsigned char pattern = 0;

    while (1) {
        SendHC595(pattern++);
        wait(0.5);
    }
}