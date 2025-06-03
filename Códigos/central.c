#include <stdio.h>
#include <wiringPi.h>

#define SENSOR_PORTA 14
#define SENSOR_PIR   15
#define SENSOR_GAS   18

#define LED_VERMELHO 23
#define LED_VERDE    24
#define BUZZER       25

void portaHandler() {
    if (digitalRead(SENSOR_PORTA) == LOW) {
        digitalWrite(LED_VERMELHO, HIGH);  // Porta aberta
    } else {
        digitalWrite(LED_VERMELHO, LOW);   // Porta fechada
    }
}

void pirHandler() {
    if (digitalRead(SENSOR_PIR) == HIGH) {
        digitalWrite(LED_VERDE, HIGH);     // Movimento detectado
    } else {
        digitalWrite(LED_VERDE, LOW);      // Sem movimento
    }
}

void gasHandler() {
    if (digitalRead(SENSOR_GAS) == HIGH) {
        digitalWrite(BUZZER, HIGH);        // Gás detectado
    } else {
        digitalWrite(BUZZER, LOW);         // Sem gás
    }
}

int main(void) {
    wiringPiSetupGpio();  // Usa numeração BCM

    // Configura os pinos
    pinMode(SENSOR_PORTA, INPUT);
    pinMode(SENSOR_PIR, INPUT);
    pinMode(SENSOR_GAS, INPUT);

    pinMode(LED_VERMELHO, OUTPUT);
    pinMode(LED_VERDE, OUTPUT);
    pinMode(BUZZER, OUTPUT);

    // Estado inicial
    digitalWrite(LED_VERMELHO, LOW);
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(BUZZER, LOW);

    // Liga interrupções nos sensores
    wiringPiISR(SENSOR_PORTA, INT_EDGE_BOTH, &portaHandler);
    wiringPiISR(SENSOR_PIR, INT_EDGE_BOTH, &pirHandler);
    wiringPiISR(SENSOR_GAS, INT_EDGE_BOTH, &gasHandler);

    printf("Sistema iniciado. Aguardando eventos...\n");

    while (1) {
        delay(1000);  // Loop ocioso só para manter o programa rodando
    }

    return 0;
}