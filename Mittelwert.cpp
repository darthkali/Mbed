#include "mbed.h"

Serial myPC(PA_2, PA_3);
AnalogIn adSensor(PC_0);
DigitalOut myLed(PB_3);

// Tiefpass (Mittelwert)
float analogInput() {
    static float analog = 0.0F;
    float adin = adSensor;

    analog = (analog * 9 + adin) / 10;

    return analog;
}

int main() {


    myPC.baud(115200);

    while (1) {

        float analog = analogInput();
        float analog2 = adSensor;

        myPC.printf("Analogwert = %f\r\n", analog);
        myPC.printf("#A%f\r\n", analog);
        myPC.printf("#B%f\r\n", analog2);

        myLed = analog <= 0.6F;

        wait(0.05);
    }
}
