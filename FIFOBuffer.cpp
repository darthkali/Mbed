#include "mbed.h"

// --- Our FIFO buffer serial class ---
class BufferedSerial : public Serial
{
    static const int BUFFER_SIZE = 128;

private:

    char m_arrBuffer[BUFFER_SIZE];
    int  m_nWriteIndex;
    int  m_nReadIndex;
    int  m_nCount;


    // --- Callback ---
    void RxReceive()
    {
        char chRx = getc();

        if (m_nCount < BUFFER_SIZE)
        {
            m_arrBuffer[m_nWriteIndex++] = chRx;

            if (m_nWriteIndex >= BUFFER_SIZE)
                m_nWriteIndex = 0;
            m_nCount += 1;
        }
    }


public:

    // --- ctor ---
    BufferedSerial(PinName txPin, PinName rxPin) : Serial(txPin, rxPin)
    {
        m_nWriteIndex = 0;
        m_nReadIndex = 0;
        m_nCount = 0;

        attach(this,&BufferedSerial::RxReceive,RxIrq);
        baud(115200);
    }


    // --- Read char ---
    char ReadChar()
    {
        if (m_nCount > 0)
        {
            __disable_irq();    // lock
            char chRx = m_arrBuffer[m_nReadIndex++];

            if (m_nReadIndex >= BUFFER_SIZE)
                m_nReadIndex = 0;

            m_nCount -= 1;
            __enable_irq();     // unlock
            return chRx;
        }
        return 0;
    }


    // --- Get string ---
    char* GetString(int nSize)
    {
        char* pString = new char[nSize + 1];
        int nCount = 0;

        while (nCount < nSize)
        {
            if (m_nCount > 0)
            {
                char chRx = ReadChar();

                if (chRx == '\r')
                    break;

                pString[nCount++] = chRx;
            }
        }

        pString[nCount] = 0;
        return pString;
    }


    // --- Send strings ---
    void PutString(char* pszString)
    {
        while (*pszString)
            putc(*pszString++);
    }


    // --- Buffer count ---
    int GetCount()
    {
        return m_nCount;
    }
};



int main()
{
    BufferedSerial* pPC = new BufferedSerial(PA_2, PA_3);

    while (true)
    {
        char* pString = pPC->GetString(20);

        wait(0.001);// muss sein, damit es geht!?
        pPC->PutString(pString);

        delete pString;
    }
}