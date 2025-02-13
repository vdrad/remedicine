/*
[DESCRIÇÃO DO PROJETO]
*/

#include "pico/stdlib.h"                    // Biblioteca geral com códigos pertinentes à RP2040.
#include "inc/pinout.h"                     // Biblioteca contendo a relação dos pinos da RP2040 utilizados no projeto.
#include "inc/rgb_matrix/rgb_matrix.h"      // Biblioteca da Matriz de LEDs RGB.
#include "inc/oled_display/oled_display.h"  // Biblioteca do Display OLED.
#include "inc/buzzer/buzzer.h"              // Biblioteca do Buzzer.
#include "inc/buttons/buttons.h"            // Biblioteca do Botão.

int y = 0;

/* ==================================== CÓDIGO PRINCIPAL ==================================== */
int main() {
    stdio_init_all();       // Inicializa entradas e saídas
    rgb_matrix_init();      // Inicializa matriz de LEDs NeoPixel
    oled_display_init();    // Inicializa o Display OLED
    buzzer_init('L');       // Inicializa o Buzzer Esquerdo
    button_init('A');       // Inicializa o Botão A
    button_init('B');       // Inicializa o Botão B

    // Não faz mais nada. Loop infinito.
    while (true) {
        if (get_a_button_state()) {
            rgb_matrix_write_pattern(smile_pattern, PURPLE);
            sleep_ms(2000);
            rgb_matrix_write_pattern(clear_pattern, BLANK_COLOR);
        }

        if (get_b_button_state()) {
            rgb_matrix_write_pattern(sad_pattern, EMERALD);
            sleep_ms(2000);
            rgb_matrix_write_pattern(clear_pattern, BLANK_COLOR);
        }
        // sleep_ms(5000);
    }

    return 0;
}