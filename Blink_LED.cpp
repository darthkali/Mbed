#include "mbed.h"

DigitalOut Led(PA_5);
DigitalIn Button(PC_13);
Serial PC(PA_2, PA_3);                      // UART


// enable Debugging
#define DEBUGGING 1


#if DEBUGGING == 1
#define TRACE(text) Trace(text)
#define DEBUG(name) Debug(#name, name)
#else
#define TRACE(text)
#define DEBUG(name)
#endif

void Trace(char *pszText) {

    if (pszText != 0) {
        PC.printf("%s\r\n", pszText);
    }
}

void Debug(char *pszName, int nValue) {

    if (pszName != 0) {
        PC.printf("%s = %d\r\n", pszName, nValue);
    }

}

// helper function to invert the Button
bool BtnDown() {
    return Button == 0;
}


// get a char
void RxChar() {
    char ch = PC.getc();
    ch == '1' ? Led = 1 : Led = 0;

}


// main function
int main() {

    PC.baud(115200);
    PC.attach(RxChar);

    TRACE("Start");


    int nCount = 0;
    int nIndex = 0;

    while (1) {

        nCount -= 1;
        nIndex += 1;

        DEBUG(nCount);
        DEBUG(nIndex);

        wait(1);
    }
}
