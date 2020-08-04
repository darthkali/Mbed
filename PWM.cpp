#include "mbed.h"

Serial pc(PA_2, PA_3);
PwmOut pwm(PB_10);
AnalogIn adc(PC_0);
DigitalIn btn(PC_13);

// Protokoll zur Terminal Console (Kurvenschreiber)
// #<Kanal A..D><Wert><CR><NL>
// #<Kanal><Wert><CR><Kanal><Wert><CR><NL>

// --- main ---
int main() {
    float f = 0.0F;

    pc.baud(115200);            // baud rate

    pwm.period(0.0001);         // T für 10kHz
    pwm = 0.25F;                // Duty Cycle in %

    while (true) {
        f += 0.025F;
        if (f > 6.28F) f = 0.0F;
        pwm = 0.5F + 0.5F * sin(f);

        wait(0.05);

        float analog = adc.read();
        float soll = pwm.read();

        pc.printf("#A%f\rB%f\r\n", analog, soll);
    }
}