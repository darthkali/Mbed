#include "mbed.h"

Serial pc(PA_2,PA_3);
PwmOut pwm(PB_10);
AnalogIn adc(PC_1);
DigitalIn btn(PC_13);

// Protokoll zur Terminal Console (Kurvenschreiber)
// #<Kanal A..D><Wert><CR><NL>
// #<Kanal><Wert><CR><Kanal><Wert><CR><NL>

float f = 0.0F;
int pwmUpDown = 0;

void sinLED(){

    if(pwmUpDown == 0){

        pwm = pwm +0.05f;

        if(pwm >= 1){
            pwmUpDown = 1;
        }
    }
    else
    {
        pwm = pwm - 0.05f;

        if(pwm <= 0){
            pwmUpDown = 0;
        }
    }
}


// --- main ---
int main()
{

    pc.baud(115200);            // baud rate
    pwm.period(0.01F);         // T für 10kHz
    pwm = 0.0F;                // Duty Cycle in %

    while (true)
    {

        sinLED();



        f += 0.025F;
        if (f > 6.28F) f = 0.0F;
        pwm = 0.5F + 0.5F * sin(f);

        float soll = pwm.read();
        float analog = adc.read();

        pc.printf("Flag: %d --- PWM: %f --- Analog: %f\r\n",pwmUpDown, soll, analog);
        pc.printf("#A%f\rB%f\r\n",soll, analog);


        wait(0.05);
        /*

         f += 0.025F;
        if (f > 6.28F) f = 0.0F;
        pwm = 0.5F + 0.5F * sin(f);

        wait(0.005);

        float analog = adc.read();
        // float soll = pwm.read();
*/
        // pc.printf("#A%f\r\n",pwm);
    }
}