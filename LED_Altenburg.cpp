#include "mbed.h"

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

DigitalOut Led(PA_5);
DigitalIn Btn(PC_13);
Serial PC(PA_2, PA_3);           // UART

// Schater für Debug modus
#define DEBUGGING           1

// Debug macros
#if DEBUGGING == 1
#define TRACE(text) Trace(text)
#define DEBUG(name) Debug(#name,name)
#define SCOPE(value) Scope(value)
#define SCOPE2(valueA, valueB) Scope (valueA, valueB)
#else
#define TRACE(text)
#define DEBUG(name)
#define SCOPE(value)
#endif

// Trace-Ausgabe
void Trace(char *pszText) {
    if (pszText != 0)
        PC.printf("%s\r\n", pszText);
}

// Debug-Ausgabe
void Debug(char *pszName, int nValue) {
    if (pszName != 0)
        PC.printf("%s = %d\r\n", pszName, nValue);
}

// Scope output
void Scope(float fValueA, float fValueB) {
    PC.printf("#A%f\rB%f\r\n", fValueA, fValueB);
}


// Hilfsfunktion
bool BtnDown() {
    return Btn == 0;
}

// ein char empfangen
void RxChar() {
    char ch = PC.getc();

    if (ch == '0')
        Led = 0;
    else if (ch == '1')
        Led = 1;
}

// main loop
int main() {
    PC.baud(115200);
    PC.attach(RxChar);

    TRACE("Start!");

    int nCount = 0;
    int nIndex = 0;
    float fSin = 0.00;
    float fCos = 0.00;
    float fRad = 0.00;

    while (1) {
        nCount -= 1;
        nIndex += 1;

        DEBUG(nCount);  // Debug("nCount",nCount)
        DEBUG(nIndex);  // Debug("nIndex",nIndex)

        if (BtnDown() == 1) {
            fRad += 0.1;
        }
        fSin = 10 * sin(fRad);
        fCos = 6 * cos(fRad);
        fRad += 0.01;
        SCOPE2(fSin, fCos);

        wait(0.1);
    }
}