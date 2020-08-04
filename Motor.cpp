#include "mbed.h"


Ticker tick;
Serial ser(PA_2, PA_3);
PwmOut pwm(PB_10);
InterruptIn encoder(PA_0);



// Protocol to terminal
// #<Channel A..D><Value><CR><NL>
// #<Channel><Value><CR><Channel><Value><CR><NL>

int nPulses = 0;
int nLastPulses = 0;

float fSollSpeed = 9.0F;
float fSpeed = 0.0F;

float ek = 0.0F;
float esum = 0.0F;

void Pulses() {
    nPulses += 1;

}


void MotorSpeed() {

    float fDiv = nPulses - nLastPulses;
    fSpeed = (fSpeed * 49.0F + fDiv) / 50.0F;
    nLastPulses = nPulses;


}

void PIControl() {

    float kp = 0.9F;        //Verstärkung
    float ki = 0.7F;        //Nachstellzeit
    float ta = 0.01F;       //Abtastrate

    MotorSpeed();

    ek = fSollSpeed - fSpeed;   //Regelabweichung
    esum = esum + ek;

    float u = kp * ek + ki * ta * esum;

    if (u < 0.0F) u = 0.0F;
    if (u > 1.0F) u = 1.0F;

    pwm = u;
}

int main() {

    ser.baud(115200);
    pwm.period(0.0001);
    encoder.rise(Pulses);
    tick.attach(PIControl, 0.01F);


    while (1) {
        ser.printf("#A%f\r\n", fSpeed);
        wait(0.01);
    }
}


