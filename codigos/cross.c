#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define GPIO_BASE 0x3F200000  // Endereço base do GPIO para Raspberry Pi 3
#define BLOCK_SIZE 4096
#define GPFSEL2    0x08
#define GPSET0     0x1C
#define GPCLR0     0x28

int main() {
    int fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd < 0) {
        perror("Erro ao abrir /dev/mem (execute como root)");
        return 1;
    }

    volatile uint32_t *gpio = (volatile uint32_t *)mmap(
        NULL,
        BLOCK_SIZE,
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        fd,
        GPIO_BASE
    );

    if (gpio == MAP_FAILED) {
        perror("Erro no mmap");
        close(fd);
        return 1;
    }

    // Configura GPIO23, 24 e 25 como saída
    uint32_t sel = *(gpio + GPFSEL2/4);
    sel &= ~((7 << 9) | (7 << 12) | (7 << 15)); // Limpa bits dos GPIOs 23,24,25
    sel |=  ((1 << 9) | (1 << 12) | (1 << 15)); // Seta como saída
    *(gpio + GPFSEL2/4) = sel;

    // Primeiro: coloca GPIO25 em LOW
    *(gpio + GPCLR0/4) = 1 << 25;

    // Segundo: coloca todos (23, 24 e 25) em HIGH
    *(gpio + GPSET0/4) = (1 << 23) | (1 << 24) | (1 << 25);

    // Terceiro: seta GPIO25 em HIGH novamente (duplicidade)
    *(gpio + GPSET0/4) = 1 << 25;

    printf("Teste alarmes");
    sleep(5); // Mantém por 5 segundos

    // Limpa todos (23, 24 e 25)
    *(gpio + GPCLR0/4) = (1 << 23) | (1 << 24) | (1 << 25);
    printf("GPIOs em nível baixo\n");

    munmap((void*)gpio, BLOCK_SIZE);
    close(fd);
    return 0;
}
