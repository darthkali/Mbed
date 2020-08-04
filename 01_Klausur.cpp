// Protokoll zur Terminal Console (Kurvenschreiber)
// #<Kanal A..D><Wert><CR><NL>
// #<Kanal><Wert><CR><Kanal><Wert><CR><NL>

// startStop == 1 ? startStop = 0 : startStop = 1;
// #char Rotor1[]   = "DMTWSILRUYQNKFEJCAZBPGXOHV";
// enum STATE {stINIT, stGREEN, stYELLOW, stRED, stWAIT, stYELLOW_RED };

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

Serial      pc(PA_2, PA_3);                      // UART
Ticker      tiTimer;
DigitalIn   btn(PC_13);
DigitalOut  led(PB_3);
PwmOut      pwm(PB_10);
InterruptIn btnInter(PC_13);
Timer       interruptTimer;
AnalogIn    adSensor(PC_0);

// --- Types ---

// --- Variables ---
int nTime = 0;
bool passwordChecked = false;
string password = "9137";


// --- Functions ---
// --- invert button ---
int BtnDown() {
    return btn == 0;
}

// --- Btn Presses with InterruptTimer ---
void btnPressed() {
    pc.printf("Timer = %f\r\n", interruptTimer.read());
    if (interruptTimer.read() > 0.1F) {
        // Do Stuff
        interruptTimer.reset();
    }
}

// --- get a char ---
void RxChar() {
    char ch = pc.getc();

}

// --- system tick ---
void Tick() {
    nTime += 1;
}

// --- Tiefpass (Mittelwert) ---
float analogInput() {
    static float analog = 0.0F;
    float adin = adSensor;

    analog = (analog * 9 + adin) / 10;

    return analog;
}

// --- Main ---
int main() {
    // tiTimer.attach(Tick, 1);

    // btnInter.fall(btnPressed);
    // interruptTimer.start();

    pc.baud(115200);
    // pc.attach(RxChar);
    // pc.printf("\x0C""Enter Text: ");
    // pc.printf("#A%f\r\n", analog);

    while (1) {


    }
}
