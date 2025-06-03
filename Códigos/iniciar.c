#include <unistd.h>
#include <stdlib.h>
#include <wiringPi.h>

#define LED_VERMELHO 23

int main(void) {
    if (wiringPiSetupGpio() == -1) {
        printf("Erro na wiringPi\n");
        return 1;
    }

    pinMode(LED_VERMELHO, OUTPUT);

    digitalWrite(LED_VERMELHO, HIGH);
    delay(100);
    digitalWrite(LED_VERMELHO, LOW);

    return 0;
}