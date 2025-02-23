#include "buzzer.h"

#include <stdio.h>              // Biblioteca para as funções gerais de pino e UART.
#include "pico/stdlib.h"        // Biblioteca geral com códigos pertinentes à RP2040.
#include "hardware/clocks.h"    // Biblioteca com funções acerca do clock (hardware) da RP2040.
#include "hardware/pwm.h"       // Biblioteca com funções de PWM da RP2040.
#include "inc/pinout.h"         // Biblioteca contendo a relação dos pinos da RP2040 utilizados no projeto.

uint8_t stop_melody = false;    // Variável para indicar parada do toque da melodia.

// Melodia pré-definida 1
melody melody_1 = {
    .melody_chords  = {NOTE_F5,NOTE_F5,NOTE_F5,NOTE_F5,NOTE_F5,NOTE_F5,NOTE_F5},
    .melody_tempo   = {0.25,0.75,0.25,0.75,0.25,0.75,1}
};

// Melodia pré-definida 2
melody melody_2 = {
    .melody_chords  = {NOTE_G5,NOTE_A5,NOTE_A5,NOTE_G5,NOTE_F5,NOTE_G5,NOTE_F5},
    .melody_tempo   = {0.25,0.5,0.5,0.5,0.25,0.25,0.25}
};

// Melodia pré-definida 3
melody melody_3 = {
    .melody_chords  = {NOTE_G5,NOTE_D5,NOTE_F5, NOTE_F5,NOTE_F5,NOTE_F5,NOTE_F5},
    .melody_tempo   = {0.5,1,0.25,0.75,0.25,0.75,0.25}
};

// Melodia pré-definida 4
melody melody_4 = {
    .melody_chords  = {NOTE_F5,NOTE_F5,NOTE_G5,NOTE_A5,NOTE_A5,NOTE_G5,NOTE_G5},
    .melody_tempo   = {0.75,1,0.25,0.5,0.75,0.25,0.5}
};

// Melodia pré-definida 5
melody melody_5 = {
    .melody_chords  = {NOTE_F5,NOTE_AS4,NOTE_F5,NOTE_F5,NOTE_C5,NOTE_C5,NOTE_A5},
    .melody_tempo   = {0.75,0.5,0.5,1,0.25,0.25,0.5}
};

// Melodia pré-definida 6
melody melody_6 = {
    .melody_chords  = {NOTE_A5,NOTE_AS4,NOTE_F5,NOTE_F5,NOTE_C5,NOTE_C5,NOTE_A5},
    .melody_tempo   = {1,0.5,0.5,0.75,0.25,0.5,0.5}
};

// Melodia pré-definida 6
melody melody_7 = {
    .melody_chords  = {NOTE_A5,NOTE_AS4,NOTE_F5,NOTE_F5,NOTE_C5,NOTE_A5,NOTE_A5},
    .melody_tempo   = {1,0.5,0.5,1,0.25,0.5,0.75}
};

melody smile_melody = {
    .melody_chords = {NOTE_C7,NOTE_D7,NOTE_E7,NOTE_F7,NOTE_G7,NOTE_A7,NOTE_C7},
    .melody_tempo  = {0.2,0.25,0.5,0.25,0.5,0.25,0.5}
};

melody sad_melody = {
    .melody_chords = {NOTE_C3,NOTE_D3,NOTE_E3,NOTE_F3,NOTE_G3,NOTE_A3,NOTE_C3},
    .melody_tempo  = {0.2,0.25,0.5,0.25,0.5,0.25,0.5}
};

/**
 * Inicializa o periférido Buzzer.
 * @param side o buzzer a ser configurado. 'R' ou 'r' para direito. 'L' ou 'l' para esquerdo.
 */
void buzzer_init(char side) {
    uint8_t pin = LEFT_BUZZER_PIN;                          // Define por padrão o pino do Buzzer Esquerdo, evitando erros caso o parâmetro side seja definido erroneamente
    if (side == 'R' || side == 'r') pin = RIGHT_BUZZER_PIN; // Caso qualquer outro caractere seja escrito por engano, será declarado o buzzer esquerdo
    gpio_set_function(pin, GPIO_FUNC_PWM);                  // Configura o pino do Buzzer como PWM
    uint slice_num = pwm_gpio_to_slice_num(pin);            // Captura o slice associado ao pino do Buzzer
    pwm_set_clkdiv(slice_num, BUZZER_PWM_DIV);              // Configura o divisor de clock
    pwm_set_wrap(slice_num, 2500);                          // Inicializa com um valor arbitrário que será modificado sempre que uma nota for tocada
    pwm_set_gpio_level(pin, 0);                             // Zera o pwm inicialmente para que não se produza sons aleatórios
    pwm_set_enabled(slice_num, true);                       // Habilita o slice em questão
}  

/**
 * Toca uma frequência específica no buzzer
 * @param side o buzzer a ser tocado. 'R' ou 'r' para direito. 'L' ou 'l' para esquerdo.
 * @param frequency a frequência em Hz.
 * @param duration_ms a duração da nota em milisegundos.
 * @param pause_ms a pausa entre notas em milisegundos.
 */
void buzzer_play_tone(char side, uint frequency, uint duration_ms, uint pause_ms) {
    // Configura a frequência da nota
    uint8_t pin = LEFT_BUZZER_PIN;                          // Define por padrão o pino do Buzzer Esquerdo, evitando erros caso o parâmetro side seja definido erroneamente
    if (side == 'R' || side == 'r') pin = RIGHT_BUZZER_PIN; // Caso qualquer outro caractere seja escrito por engano, será declarado o buzzer esquerdo
    uint slice_num          = pwm_gpio_to_slice_num(pin);   // Captura o slice associado ao pino do Buzzer 
    uint32_t clock_freq     = clock_get_hz(clk_sys);        // Captura a frequência de clock da RP2040
    uint32_t current_wrap   = clock_freq/(BUZZER_PWM_DIV * (float)frequency); // Calcula o wrap necessário para gerar a frequência solicitada

    // Toca a nota
    pwm_set_wrap(slice_num, current_wrap);                  // Configura a frequência solicitada com base no wrap calculado
    pwm_set_gpio_level(pin, current_wrap/2);                // Configura o Duty Cycle para 50%
    sleep_ms(duration_ms);              

    // Pausa
    pwm_set_gpio_level(pin, 0); // Desliga o som após a duração
    sleep_ms(pause_ms);         // Pausa entre notas
}

/**
 * Para de tocar a nota musical.
 * @param side o buzzer a ser tocado. 'R' ou 'r' para direito. 'L' ou 'l' para esquerdo.
 */
void buzzer_stop(char side) {
    uint8_t pin = LEFT_BUZZER_PIN;                          // Define por padrão o pino do Buzzer Esquerdo, evitando erros caso o parâmetro side seja definido erroneamente
    if (side == 'R' || side == 'r') pin = RIGHT_BUZZER_PIN; // Caso qualquer outro caractere seja escrito por engano, será declarado o buzzer esquerdo

    pwm_set_gpio_level(pin, 0); // Desliga o som após a duração
}

/**
 * Toca uma melodia pré-definida
 * @param side o buzzer a ser tocado. 'R' ou 'r' para direito. 'L' ou 'l' para esquerdo.
 * @param chosen_melody a melodia escolhida.
 * @return 1 ao final.
 */
uint8_t buzzer_play_melody(char side, melody *chosen_melody) {
    for (int this_note = 0; this_note < MELODY_FIXED_SIZE; this_note++) {
        int note_duration = (int)(NOTE_MULTIPLIER * chosen_melody->melody_tempo[this_note]);
        int pause_between_notes = note_duration * 1.05;
        
        if (stop_melody) return 0;
        buzzer_play_tone(side, chosen_melody->melody_chords[this_note], note_duration, pause_between_notes);
    }
    return 1;
}

/**
 * Atualiza o valor de stop_melody. Ou seja, habilita ou desabilita a interrupção da melodia atual.
 * @param value 1 para interromper a melodia atual. 0 para deixá-la tocando.
 */
void set_stop_melody(uint8_t value) {
    stop_melody = value;
}

/**
 * Toca uma melodia de teste para validar o buzzer.
 * @param side o buzzer a ser tocado. 'R' ou 'r' para direito. 'L' ou 'l' para esquerdo.
 */
void buzzer_validate(char side) {
    for (int this_note = 0; this_note < 7; this_note++) {
        int note_duration = (int)(NOTE_MULTIPLIER * melody_7.melody_tempo[this_note]);
        int pause_between_notes = note_duration * 1.05;

        buzzer_play_tone(side, melody_7.melody_chords[this_note], note_duration, pause_between_notes);
    }
}

