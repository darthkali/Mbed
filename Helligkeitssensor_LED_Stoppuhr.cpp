// Protokoll zur Terminal Console (Kurvenschreiber)
// #<Kanal A..D><Wert><CR><NL>
// #<Kanal><Wert><CR><Kanal><Wert><CR><NL>

// startStop == 1 ? startStop = 0 : startStop = 1;

// Style guide vorschlag!
// ch -> char
// b  -> bool
// by -> byte
// n  -> int
// d  -> double
// f  -> float
// p  -> pointer
// arr -> array
// psz -> pointer to string zero


#include "mbed.h"
#include <string>

Serial      pc(PA_2,PA_3);                      // UART
Timer       messureTimer;
PwmOut      pwm(PB_10);
InterruptIn btnInter(PC_13);
Timer       interruptTimer;
AnalogIn    adSensor(PC_0);

#define BREAKPOINT 0.3F
#define PWM_UP_DOWN 0.05F

// --- Variables ---
float f = 0.0F;
bool btnCheck = false;
bool pwmUpDown = false;

// --- Functions ---

// --- Btn Presses with InterruptTimer ---
void btnPressed(){

    if(interruptTimer.read() > 0.1F){

        if(!btnCheck)
        {
            messureTimer.start();

            pwmUpDown = true;
            btnCheck = true;
        }else{
            messureTimer.stop();
            messureTimer.reset();

            pwmUpDown = false;
            btnCheck = false;
        }
        interruptTimer.reset();
    }
}

// --- Tiefpass (Mittelwert) ---
float analogInput()
{
    static float  analog = 0.0F;
    float adin = adSensor  ;

    analog = (analog * 9 + adin) /10;

    return analog;
}

// --- pwm for LED On/Off ---
void sinLED(){

    if(pwmUpDown){
        pwm = pwm + PWM_UP_DOWN;
    }
    else
    {
        pwm = pwm - PWM_UP_DOWN;
    }
}

// --- Main ---
int main()
{
    pc.baud(115200);

    btnInter.fall(btnPressed);
    interruptTimer.start();

    pwm.period(0.001F);         // T für 10kHz
    pwm = 0.0F;                // Duty Cycle in %

    while(1)
    {

        float analog = analogInput();
        float analog2 = adSensor;

        sinLED();

        if(analogInput() >= BREAKPOINT && btnCheck)
        {
            messureTimer.stop();
            pc.printf("\x0C""Zeit bis 0,3: %f\r\n",messureTimer.read());
        }else if(analogInput() < BREAKPOINT && btnCheck){
            pc.printf("\x0C""Zeit bis 0,3: wird gemessen\r\n");
        }else{
            pc.printf("\x0C""Zum messen bitte den Button betaetigen\r\n");
        }

        pc.printf("#A%f\r\n", analog);
        pc.printf("#B%f\r\n", analog2);
        pc.printf("#C%f\r\n", BREAKPOINT);

        wait(0.1);
    }
}
