/*
[DESCRIÇÃO DO PROJETO]
*/

#include "pico/stdlib.h"                    // Biblioteca geral com códigos pertinentes à RP2040.
#include "inc/pinout.h"                     // Biblioteca contendo a relação dos pinos da RP2040 utilizados no projeto.
 #include "inc/rgb_matrix/rgb_matrix.h"     // Biblioteca da Matriz de LEDs RGB.
#include "inc/oled_display/oled_display.h"  // Biblioteca do Display OLED.
#include "inc/buzzer/buzzer.h"              // Biblioteca do Buzzer.

int y = 0;

/* ==================================== CÓDIGO PRINCIPAL ==================================== */
int main() {
    stdio_init_all(); // Inicializa entradas e saídas.
    rgb_matrix_init(); // Inicializa matriz de LEDs NeoPixel.
    oled_display_init();
    buzzer_init('R');

    buzzer_validate('R');
    oled_display_validate();

    // Não faz mais nada. Loop infinito.
    while (true) {
        sleep_ms(1000);
    }

    return 0;
}