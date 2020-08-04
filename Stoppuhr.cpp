#include "mbed.h"

Serial pc(PA_2, PA_3);
InterruptIn btn(PC_13);
Timer interruptTimer;
Timer stopWatchTimer;

// --- Variables ---
int nTime = 0;
int nMin = 0;
int nSek = 0;
int nMill = 0;
int nStartStop = 0;

void btnPressed() {
    if (interruptTimer.read() > 0.1F) {
        stopWatchTimer.reset();
        nStartStop == 1 ? nStartStop = 0 : nStartStop = 1;
        interruptTimer.reset();
    }
}

void calcTime() {
    int nTime = (int) (stopWatchTimer.read() * 1000); //+59000 - test the Minutes
    //minute
    nMin = (nTime / 1000 / 60) % 60;
    //second
    nSek = (nTime / 1000) % 60;
    //hundredths
    nMill = nTime % 1000;
}


// --- main ---
int main() {
    pc.baud(115200);            // baud rate
    stopWatchTimer.start();
    interruptTimer.start();
    btn.fall(btnPressed);

    while (true) {
        if (nStartStop == 1) {
            pc.printf("\x0C""Stoppuhr: ");

            calcTime();
            pc.printf("%d:%d,%d \r\n", nMin, nSek, nMill);

            wait(0.1);
        }
    }
}