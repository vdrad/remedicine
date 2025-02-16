/*
[DESCRIÇÃO DO PROJETO]
*/

#include <stdio.h>                          // Biblioteca de propósito geral.
#include "pico/stdlib.h"                    // Biblioteca geral com códigos pertinentes à RP2040.
#include "hardware/timer.h"                 // Biblioteca contendo funções relacionadas ao hardware de timer da RP2040
#include "inc/pinout.h"                     // Biblioteca contendo a relação dos pinos da RP2040 utilizados no projeto.
#include "inc/rgb_matrix/rgb_matrix.h"      // Biblioteca da Matriz de LEDs RGB.
#include "inc/oled_display/oled_display.h"  // Biblioteca do Display OLED.
#include "inc/buzzer/buzzer.h"              // Biblioteca do Buzzer.
#include "inc/buttons/buttons.h"            // Biblioteca do Botão.
#include "inc/microphone/microphone.h"      // Biblioteca do Microfone.
#include "inc/alarm/alarm.h"                // Biblioteca do Alarme.
#include "inc/remedios/remedios.h"          // Biblioteca de cadastro dos remédios.

// extern struct repeating_timer display_timer;

/* ==================================== CÓDIGO PRINCIPAL ==================================== */
int main() {
    stdio_init_all();       // Inicializa entradas e saídas
    rgb_matrix_init();      // Inicializa matriz de LEDs NeoPixel
    oled_display_init();    // Inicializa o Display OLED
    buzzer_init('L');       // Inicializa o Buzzer Esquerdo
    button_init('A');       // Inicializa o Botão A
    button_init('B');       // Inicializa o Botão B
    microphone_init();      // Inicializa o Microfone
    alarm_init();           // Inicializa o Alarme

    // Espera até que o usuário pressione o Botão A ou B para avançar no código
    uint8_t a_button_pressed = get_a_button_state();
    uint8_t b_button_pressed = get_b_button_state();
    while((!a_button_pressed) && (!b_button_pressed)) {
        a_button_pressed = get_a_button_state();
        b_button_pressed = get_b_button_state();
    };

    // Utiliza o recurso timer para atualizar o horário no display OLED
    static struct repeating_timer display_timer;                                   // Cria a estrutura de timer ciclico
    add_repeating_timer_us(100 *1000, display_current_time, NULL, &display_timer); // Configura a função button_callback() para ser executada a cada 100 ms

    // Loop infinito
    while (true) {
        // Constantemente checa se há algum alarme para ser acionado.
        int8_t medicine_index = check_for_medicine_alarm();
        if (medicine_index > -1) {
            cancel_repeating_timer(&display_timer);     // Desabilita atualização padrão do display para exibir informações do remédio
            notify_medicine(&remedios[medicine_index]); // Notifica ao usuário do remédio a ser tomado
            add_repeating_timer_us(100 *1000, display_current_time, NULL, &display_timer);  // Volta a exibir o horário atual
        }
    }

    return 0;
}