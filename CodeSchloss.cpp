#include "mbed.h"
#include <string>

Ticker      tiTimer;
DigitalIn   Button(PC_13);

Serial PC(PA_2,PA_3);

DigitalOut  ledRed(PB_4);
DigitalOut  ledGreen(PB_5);

// --- Types ---
enum STATE {stINIT, stBLOCKED, stUNBLOCKED, stWAIT };

// --- Variables ---
STATE enState = stINIT;

int nTime = 0;
int index = 0;
int nStateChanged = 0;
bool passwordChecked = false;
string password = "9137";

// --- system tick ---
void Tick()
{
    nTime += 1;
}

void SetLed(int red,  int green){
    ledRed = red;
    ledGreen = green;
}

void SetNextStateAndTheWaitTime(STATE state, int time){
    nStateChanged = nTime + time;
    enState = state;
}


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

void StateMachine()
{
    switch (enState)
    {
        case stINIT:
            SetLed(0,0);
            enState = stBLOCKED;
            break;


        case stBLOCKED:
            SetLed(1,0);
            if(passwordChecked){
                SetNextStateAndTheWaitTime(stUNBLOCKED, 5);
            }
            break;

        case stUNBLOCKED:
            SetLed(0,1);
            if(nStateChanged - nTime <= 0){
                SetLed(0,1);
                enState = stBLOCKED;
            }
            break;
    }
}


// --- Main ---
int main()
{
    tiTimer.attach(Tick, 10);
    PC.attach(RxChar);

    PC.baud(115200);
    PC.printf("Passwort eingeben:\r\n");

    while(1)
    {
        StateMachine();

    }
}
