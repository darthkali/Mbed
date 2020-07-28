// Protokoll zur Terminal Console (Kurvenschreiber)
// #<Kanal A..D><Wert><CR><NL>
// #<Kanal><Wert><CR><Kanal><Wert><CR><NL>

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
Ticker      tiTimer;
DigitalIn   btn(PC_13);
DigitalOut  ledGreen(PB_3);
PwmOut      pwm(PB_10);
InterruptIn btn(PC_13);
Timer       interruptTimer;


// --- Variables ---
    int nTime = 0;
    bool passwordChecked = false;
    string password = "9137";
    #char Rotor1[]   = "DMTWSILRUYQNKFEJCAZBPGXOHV";


// --- Functions ---
    int BtnDown()
    {
        return Button == 0;
    }

// --- Btn Presses with InterruptTimer ---
    void btnPressed(){
        PC.printf("Timer = %f\r\n",interruptTimer.read());
        if(interruptTimer.read()> 0.1F){
            ShiftLed();
            interruptTimer.reset();
        }
    }

// --- get a char ---
    void RxChar()
    {
        char ch = PC.getc();

        if(ch == password[index]) {
            index++;
        }else {
            index = 0;
        }

        if (index == password.length()){
            index = 0;
            PC.printf(" - Passwort korrekt!\r\n");
            passwordChecked = true;
        }
        passwordChecked = false;
    }

// --- system tick ---
    void Tick()
    {
        nTime += 1;
    }

// --- Tiefpass (Mittelwert) ---
    float analogInput()
    {
        static float  analog = 0.0F;
        float adin = adSensor  ;

        analog = (analog * 9 + adin) /10;

        return analog;
    }

// --- Main ---
    int main()
    {
        tiTimer.attach(Tick, 1);

        btn.fall(BtnPressed);
        interruptTimer.start();

        pc.baud(115200);
        pc.attach(RxChar);
        pc.printf("\x0C""Enter Text: ");
        pc.printf("#A%f\r\n", analog);

        while(1)
        {


        }
    }
