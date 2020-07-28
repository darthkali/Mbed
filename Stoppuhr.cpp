#include "mbed.h"

Serial      pc(PA_2,PA_3);
Ticker      tiTimer;
InterruptIn btn(PC_13);
Timer       interruptTimer;

// --- Variables ---
int nTime = 0; //59000
int _min = 0;
int _sek = 0;
int _mill = 0;
int startStop = 0;


void Tick()
{
    nTime += 1;
}

void btnPressed(){
    if(interruptTimer.read()> 0.1F){
        nTime = 0;
        startStop == 1 ? startStop = 0 : startStop = 1;
        interruptTimer.reset();
    }
}

void calcTime()
{
    //minute
    _min = (nTime / 1000 / 60) % 60;
    //second
    _sek = (nTime / 1000) % 60;
    //hundredths
    _mill = nTime % 1000;
}


// --- main ---
int main()
{
    pc.baud(115200);            // baud rate
    tiTimer.attach(Tick, 0.001F);
    btn.fall(btnPressed);
    interruptTimer.start();

    while (true)
    {
        if(startStop == 1){
            pc.printf("\x0C""Stoppuhr: ");

            calcTime();

            pc.printf("%d:%d,%d \r\n",_min, _sek, _mill);

            wait(0.1);
        }
    }
}