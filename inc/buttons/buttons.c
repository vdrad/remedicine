#include "buttons.h"

#include <stdbool.h>                // Biblioteca para tratar variáveis bool
#include "pico/stdlib.h"            // Biblioteca geral com códigos pertinentes à RP2040
#include "inc/pinout.h"             // Biblioteca contendo a relação dos pinos da RP2040 utilizados no projeto
#include "hardware/timer.h"         // Biblioteca contendo funções relacionadas ao hardware de timer da RP2040
#include "inc/alarm/alarm.h"      // Biblioteca do Buzzer.

bool a_button_used = false;         // Flag para identificar se o Botão A está sendo utilizado no código
bool b_button_used = false;         // Flag para identificar se o Botão B está sendo utilizado no código

bool a_button_last_state = false;   // Armazena o último estado do Botão A
bool b_button_last_state = false;   // Armazena o último estado do Botão B

/**
 * Função a ser chamada periodicamente pelo hardware timer
 * @param t
 */
bool button_callback(struct repeating_timer *t) {
    if (a_button_used) {
        static absolute_time_t last_a_button_press_time = 0;    // Armazena o tempo da última pressão do Botão A
        bool a_button_pressed = !gpio_get(A_BUTTON_PIN);        // Botão pressionado -> 0 | Botão solto -> 1

        /*
            Para considerar uma leitura válida, o botão precisa sair do estado NÃO PRESSIONADO para PRESSIONADO.
            Além disso, devem ter passado 200 ms desde a última vez que um pressionamento válido ocorreu
        */ 
        if (a_button_pressed && 
            !a_button_last_state &&
            absolute_time_diff_us(last_a_button_press_time, get_absolute_time()) > 200000
        ) {
            last_a_button_press_time = get_absolute_time(); // Atualiza o tempo da última pressão do Botão A
            a_button_last_state = true;
            if (get_if_alarm_being_played()) set_stop_melody(1); // Se o botão foi pressionado enquanto um alarme está tocando, interrompa-o.
        } else if (!a_button_pressed) {
            a_button_last_state = false; // Atualiza o status do Botão A para NÃO PRESSIONADO
        }
    }

    if (b_button_used) {
        static absolute_time_t last_b_button_press_time = 0;    // Armazena o tempo da última pressão do Botão B
        bool b_button_pressed = !gpio_get(B_BUTTON_PIN);        // Botão pressionado -> 0 | Botão solto -> 1

        /*
            Para considerar uma leitura válida, o botão precisa sair do estado NÃO PRESSIONADO para PRESSIONADO.
            Além disso, devem ter passado 200 ms desde a última vez que um pressionamento válido ocorreu
        */ 
        if (b_button_pressed && 
            !b_button_last_state &&
            absolute_time_diff_us(last_b_button_press_time, get_absolute_time()) > 200000
        ) {
            last_b_button_press_time = get_absolute_time(); // Atualiza o tempo da última pressão do Botão B
            b_button_last_state = true;
            if (get_if_alarm_being_played()) set_stop_melody(1); // Se o botão foi pressionado enquanto um alarme está tocando, interrompa-o.
        } else if (!b_button_pressed) {
            b_button_last_state = false; // Atualiza o status do Botão B para NÃO PRESSIONADO
        }
    }

    return true;
}

/**
 * Inicializa o periférico Botão
 * @param button 'A' ou 'a' para o Botão A. 'B' ou 'b' para o Botão B.
 */
void button_init(char button) { 
    uint8_t pin = A_BUTTON_PIN;                             // Define por padrão o pino do Botão A, evitando erros caso o parâmetro side seja definido erroneamente
    if (button == 'B' || button == 'b') pin = B_BUTTON_PIN; // Caso qualquer outro caractere seja escrito por engano, será declarado o botão B

    if (pin == A_BUTTON_PIN) a_button_used = true;  // Sinaliza que o Botão A está sendo utilizado
    if (pin == B_BUTTON_PIN) b_button_used = true;  // Sinaliza que o Botão B está sendo utilizado

    gpio_init(pin);             // Inicializa o pino do botão
    gpio_set_dir(pin, GPIO_IN); // Configura o pino do botão como entrada
    gpio_pull_up(pin);          // Ativa o resistor interno de pullup do GPIO do botão

    static struct repeating_timer timer; // Cria a estrutura de timer ciclico
    add_repeating_timer_us(100, button_callback, NULL, &timer); // Configura a função button_callback() para ser executada a cada 100us
}

/**
 * Retorna o estado do Botão A
 * @return true se foi pressionado. false caso contrário.
 */
bool get_a_button_state() {
    return a_button_last_state;
}

/**
 * Retorna o estado do Botão B
 * @return true se foi pressionado. false caso contrário.
 */
bool get_b_button_state() {
    return b_button_last_state;
}