#include "mbed.h"
#include <cctype>

#define NEXTLETTER 1

Serial PC(PA_2,PA_3);

char Rotor1[]   = "DMTWSILRUYQNKFEJCAZBPGXOHV";
char Rotor2[]   = "HQZGPJTMOBLNCIFDYAWVEUSRKX";
char Rotor3[]   = "UQNTLSZFMREHDPXKIBVYGJCWOA";
char Reflect[]  = "RULQMZJSYGOCETKWDAHNBXPVIF";


char LookUp(char arrRotor[], char chLetter)
{
    return arrRotor[chLetter - 'A'];
}

char LookDown(char arrRotor[], char chLetter)
{
    char chChar = 'A';

    while (arrRotor[chChar - 'A'] != chLetter){
        chChar += 1;
    };

    return chChar;
}


void RotateRotor(char arrRotor[])
{
    char chFirst = arrRotor[0];

    for ( int i = 0 ; i < 25 ; i++)
    {
        arrRotor[i] = arrRotor[i + 1];
    }

    arrRotor[25] = chFirst;

    // PC.printf("%s\r\n", arrRotor);
}


// --- any key pressed ---
void KeyDown()
{
    // ASCII-Tabelle
    // 'a' -> 97
    // 'b' -> 98

    // 'A' -> 65
    // 'B' -> 66

    char chKey = PC.getc();
    char chChar = toupper(chKey);

    static int nCounter = 0;

    chChar = LookUp(Rotor1,  chChar);
    chChar = LookUp(Rotor2,  chChar);
    chChar = LookUp(Rotor3,  chChar);

    chChar = LookUp(Reflect, chChar);

    chChar = LookDown(Rotor3,  chChar);
    chChar = LookDown(Rotor2,  chChar);
    chChar = LookDown(Rotor1,  chChar);

    RotateRotor(Rotor1);
    nCounter += 1;

    if(nCounter % 26 == 0){
        RotateRotor(Rotor2);
    }

    if(nCounter % 676 == 0){
        RotateRotor(Rotor3);
    }

    PC.putc(chChar);
}


int main()
{
    PC.baud(115200);
    PC.attach(KeyDown);

    PC.printf("\x0C""Enter Text: ");

    while(1)
    {

    }
}