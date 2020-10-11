#include "mbed.h"

#define MINHUMIDITY      0.57F  // Breakpoint on which humidity the system will react [0.65]
#define WAIT_TIME        1800   // how long the system will stay in the WAIT State [1800 - every 30 minutes]
#define PUMP_TIME        2      // how long the system will stay in the PUMP State [2]
#define DATA_RESOLUTION  10     // Resolution for the Serial Data which will be printed to the Terminal [10]

Serial      PC(PA_2,PA_3);      // UART
AnalogIn    adSensor(PC_0);    
DigitalOut  pump(PA_4);
DigitalOut  onBoardLED(PA_5);
Ticker      messureTimer;
Timer       stopWatchTimer;


// --- Variables ---
    float testPeak = 0;
    float adinRaw = 0;
    int nStateChanged = 0;
    int nPrintChanged = 0;
    int nTime = 0;
    int stateFlag = 1;
    
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
    
    void testSequenceLED(){
        onBoardLED = 1;
        wait(1);
        onBoardLED = 0;
        wait(1);
        onBoardLED = 1;
        wait(0.2);
        onBoardLED = 0;
        wait(0.5);
        onBoardLED = 1;
        wait(0.2);
        onBoardLED = 0;
    }

// Tiefpass (Mittelwert)
    float analogInput() {
        static float  analog = adSensor;
        float adin = adSensor  ;
    
        analog = (analog * 49 + adin) /50;
    
        return analog;
    }

    void setNextStateAndTheWaitTime(STATE state, int time) {
        nStateChanged = nTime + time;
        enState = state;
    }
    
    void printDataToTerminal(int time) {
        if (nPrintChanged - nTime <= 0) {
            adinRaw = adSensor;
            PC.printf("\x1B""A%f\rB%f\rC%f\rD%f\r\n", analogInput(), adinRaw, MINHUMIDITY, testPeak);
            PC.printf("\x1B""Eanalog_wert=%f\r\n", analogInput());   
            PC.printf("\x1B""Eanalog_wert_Raw=%f\r\n",adinRaw);  
                        
            nPrintChanged = nTime + time;
        }
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
                pump = stOFF;
                enState = stCHECK;
                break;
    
            case stCHECK:
            PC.printf("%d:%d:%d - CHECK\r\n", nHour, nMin, nSek);
                testSequenceLED();
                if(analogInput() > MINHUMIDITY){
                    setNextStateAndTheWaitTime(stPUMP, PUMP_TIME); //pump for x second
                    break;
                }
                
                setNextStateAndTheWaitTime(stWAIT, WAIT_TIME);    //wait y minutes
                break;
                
            case stPUMP:
                if(stateFlag == 1) { 
                    PC.printf("%d:%d:%d - PUMP\r\n", nHour, nMin, nSek);
                    pump = stON;
                    stateFlag = 0;
                }
                
                if (nStateChanged - nTime <= 0) {
                    pump = stOFF;
                    stateFlag = 1;
                    setNextStateAndTheWaitTime(stWAIT, WAIT_TIME);    //wait y minutes
                }
                break;
    
            case stWAIT:
                if(stateFlag == 1) 
                { 
                    PC.printf("%d:%d:%d - WAIT\r\n", nHour, nMin, nSek);
                    stateFlag = 0;
                }
  
                if (nStateChanged - nTime <= 0) {
                    stateFlag = 1;
                    enState = stCHECK;                  //check Status    
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
            printDataToTerminal(DATA_RESOLUTION); 
        }
    }
