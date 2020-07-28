#include "mbed.h"

DigitalOut led(PA_4);
DigitalOut led2(PA_5);
DigitalOut led3(PA_6);
DigitalOut led4(PA_7);
DigitalOut led5(PA_8);
DigitalOut led6(PA_9);
DigitalOut led7(PA_10);

int main()
{
    while(1)
    {
        led = !led;
        wait(0.1);
        led2 = !led2;
        wait(0.1);
        led3 = !led3;
        wait(0.1);
        led4 = !led4;
        wait(0.1);
        led5 = !led5;
        wait(0.1);
        led6 = !led6;
        wait(0.1);
        led7 = !led7;
        wait(0.1);
    }
}


