#include "mbed.h"

Serial      pc(PA_2,PA_3);
Ticker      tiTimer;
InterruptIn Btn(PC_13);
Timer       interruptTimer;

// --- Variables ---
int nTime = 0; //59000
int _min = 0;
int _sek = 0;
int _hund = 0;
int startStopp = 0;



// Protokoll zur Terminal Console (Kurvenschreiber)
// #<Kanal A..D><Wert><CR><NL>
// #<Kanal><Wert><CR><Kanal><Wert><CR><NL>


void Tick()
{
    nTime += 1;
}


// helper function to invert the Button
bool BtnDown()
{
    return button == 0;
}

void BtnPressed(){



    if(interruptTimer.read()> 0.1F){
        nTime = 0;
        if(startStopp == 1)
        {
            startStopp = 0;
            nTime = 0;
        }else{
            startStopp = 1;
        }

        interruptTimer.reset();
    }



}


// --- main ---
int main()
{
    pc.baud(115200);            // baud rate
    tiTimer.attach(Tick, 0.001F);
    Btn.fall(BtnPressed);
    interruptTimer.start();

    while (true)
    {

        if(startStopp == 1){
            pc.printf("\x0C""Stoppuhr: ");



            //Minuten
            _min = (nTime / 1000 / 60) % 60;
            //Sekunden
            _sek = (nTime / 1000) % 60;
            //Hundertstel
            _hund = nTime % 1000;

            // nTime += 1;
            //pc.printf("#A%d\r\n",nTime);
            pc.printf("%d:%d,%d \r\n",_min, _sek, _hund);

            wait(0.1);
        }

    }
}