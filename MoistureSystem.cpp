#include "mbed.h"

#define MINHUMIDITY 0.7F        // Breakpoint on which humidity the system will react

Serial      PC(PA_2,PA_3);      // UART
AnalogIn    adSensor(PC_0);    
DigitalOut  pump(PA_4);
Ticker      messureTimer;
Timer       stopWatchTimer;

// --- Variables ---
    float testPeak = 0;
    int nStateChanged = 0;
    int nTime = 0;
    
    // StopWatch
    int nTimer = 0;
    int nHour = 0;
    int nMin = 0;
    int nSek = 0;
    int nMill = 0;

// --- Types ---
    enum STATE { stINIT, stCHECK, stPUMP, stWAIT };
    enum PUMPSTATUS{ stON, stOFF };

    STATE enState = stINIT;

// --- system tick ---
    void Tick() {
        nTime += 1;
    }

// Tiefpass (Mittelwert)
    float analogInput() {
        static float  analog = 0.0F;
        float adin = adSensor  ;
    
        analog = (analog * 99 + adin) /100;
    
        return analog;
    }

    void setNextStateAndTheWaitTime(STATE state, int time) {
        nStateChanged = nTime + time;
        enState = state;
    }

    void calcTime() {
        int nTimer = (int) (stopWatchTimer.read() * 1000); //+59000 - test the Minutes (1000*60*60)- test the Hours
        //minute
        nHour = (nTimer / 1000 / 60 / 60) % 60;
        //minute
        nMin = (nTimer / 1000 / 60) % 60;
        //second
        nSek = (nTimer / 1000) % 60;
        //hundredths
        nMill = nTimer % 1000;
    }
    
    
    void StateMachine() {
        calcTime();
        switch (enState) {
            case stINIT:
                //PC.printf("INIT\r\n");
                pump = stOFF;
                enState = stCHECK;
                break;
    
            case stCHECK:
                //PC.printf("%d:%d:%d,%d - SensorOutput: %f - Status: CHECK\r\n", nHour, nMin, nSek, nMill, analogInput());
                //testPeak = 1;
                if(analogInput() > MINHUMIDITY){
                    setNextStateAndTheWaitTime(stPUMP, 1); //pump for 1 second
                    break;
                }
                
                setNextStateAndTheWaitTime(stWAIT, 120);    //wait 2 minutes
                break;
            case stPUMP:
                //PC.printf("PUMP\r\n");
                pump = stON;
                if (nStateChanged - nTime <= 0) {
                    
                    PC.printf("%d:%d:%d,%d - SensorOutput: %f - Status: PUMP \r\n", nHour, nMin, nSek, nMill, analogInput());
                    pump = stOFF;
                    setNextStateAndTheWaitTime(stWAIT, 120);    //wait 2 minutes
                }
                break;
    
            case stWAIT:
                //PC.printf("WAIT\r\n");
                //testPeak = 0;
                pump = stOFF;
                if (nStateChanged - nTime <= 0) {
                   setNextStateAndTheWaitTime(stCHECK, 0);      //check Status
                }
                break;
        }
    }

// main loop
    int main() {
        PC.baud(115200);
        messureTimer.attach(Tick, 1); 
        stopWatchTimer.start();
        
        PC.printf("Start Watering System!\r\n");
          
        while(1)
        {
            StateMachine();
            float adinRaw = adSensor;
            PC.printf("#A%f\rB%f\rC%f\rD%f\r\n", analogInput(), adinRaw, MINHUMIDITY, testPeak);
            wait(0.1);
        }
    }
