#include "alarm.h"

#include <stdio.h>                          // Biblioteca de funcionalidades gerais.
#include <string.h>                         // Biblioteca para lidar com strings.
#include "pico/stdlib.h"                    // Biblioteca geral com códigos pertinentes à RP2040.
#include "hardware/rtc.h"                   // Biblioteca com funções acerca do RTC (hardware) da RP2040.
#include "pico/util/datetime.h"             // Biblioteca com funções de data e hora da RP2040.
#include "inc/rgb_matrix/rgb_matrix.h"      // Biblioteca da Matriz de LEDs RGB.
#include "inc/buzzer/buzzer.h"              // Biblioteca do Buzzer.
#include "inc/oled_display/oled_display.h"  // Biblioteca do Display OLED.
#include "inc/remedios/remedios.h"          // Biblioteca de cadastro dos remédios.
#include "inc/buttons/buttons.h"            // Biblioteca do Botão.

bool alarm_being_played = false;            // Flag para identificar se algum alarme está sendo tocado

/**
 * Identifica se é hora de sinalizar o alarme de algum remédio
 * @return o índice correspondente ao remédio cujo alarme está ativo.
 * -1 significa que não há alarme ativo.
 */
int8_t check_for_medicine_alarm() {
    // Lê a data e hora atuais pelo RTC da RP2040.
    datetime_t current_time = {0};                                          // Cria a struct que armazena a data e hora
    rtc_get_datetime(&current_time);                                        // Carrega a data e hora
    char datetime_buf[256];                                                 // Buffer para data e hora em formato de texto
    char *datetime_str = &datetime_buf[0];                                  // Armazena a data e hora em formato de texto
    datetime_to_str(datetime_str, sizeof(datetime_buf), &current_time);     // Converte a data e hora em formato de texto
    printf("\r%s      \n", datetime_str);                                   // Exibe a data e hora em formato de texto

    // Checa se a data e hora atuais condizem com a de algum remédio cadastrado
    for (int i = 0; i < NUMERO_DE_REMEDIOS; i++) {
        if (current_time.hour != remedios[i].chosen_time.hour) continue;    // Caso as horas sejam diferentes, avalie o outro index
        if (current_time.min  != remedios[i].chosen_time.min)  continue;    // Caso os minutos sejam diferentes, avalie o outro index
        if (current_time.sec  != remedios[i].chosen_time.sec)  continue;    // Caso os segundos sejam diferentes, avalie o outro index

        set_alarm_being_played(true);
        return i; // Caso todos sejam iguais, retorna o index desse remédio no vetor de remédios
    }

    return -1;  // Caso não, retorna -1 indicando que não há remédio cadastrado para a data e hora atuais.
}

/**
 * Inicializa a rotina de alarme e check de data e hora
 */
void alarm_init() {
    datetime_t initial_time = {
        .year   = 2025,
        .month  = 1,
        .day    = 15,
        .dotw   = 6,
        .hour   = 12,
        .min    = 00,
        .sec    = 00
    };

    rtc_init();
    rtc_set_datetime(&initial_time);
}

/**
 * Notifica do remédio cujo alarme está ativo no momento.
 * @param medicine o vetor de remédios
 */
void notify_medicine (Reminders *medicine) {
    uint8_t melody_played = 1;  // Comando para habilitar que o buzzer toque melodias

    // Envia o padrão e cor selecionados para a Matriz de LEDs RGB 
    rgb_matrix_write_pattern(
        medicine->rgb_matrix_pattern,
        medicine->rgb_color[0],
        medicine->rgb_color[1],
        medicine->rgb_color[2]
    );

    // Repete a melodia cadastrada 10 vezes para dar tempo suficiente de que seja escutada
    for (uint8_t i = 0; i < 10; i++) {
        melody_played = buzzer_play_melody('L', medicine->chosen_melody);
    }
    set_stop_melody(0); // Habilita o buzzer a tocar melodias

    // Caso a melodia tenha sido interrompida antes do fim, significa que o usuário reconheceu o alarme.
    // Assim, envie uma carinha sorridente :) na cor verde parabenizando por ter tomado o remédio
    if (melody_played == 0) {
        rgb_matrix_write_pattern(smile_pattern, GREEN);
        buzzer_play_melody('L', &smile_melody);
    } 
    // Caso a melodia tenha tocado até o fim, significa que o usuário não reconheceu o alarme.
    // Assim, envie uma carinha triste :( na cor vermelha indicando que o horário do remédio foi perdido.
    else {
        rgb_matrix_write_pattern(sad_pattern, RED);
        buzzer_play_melody('L', &sad_melody);
    }

    rgb_matrix_write_pattern(clear_pattern, BLANK_COLOR);   // Limpa a matriz de LEDs
}

/**
 * Valida a leitura de data e hora do RTC, exibindo os valores no Monitor Serial.
 */
void validate_time() {
    datetime_t current_time;
    char datetime_buf[256];
    char *datetime_str = &datetime_buf[0];
    rtc_get_datetime(&current_time);
    datetime_to_str(datetime_str, sizeof(datetime_buf), &current_time);
    printf("\r%s      \n", datetime_str);
    sleep_ms(100);
}

/**
 * Armazena a hora atual em uma string.
 * @param hour_string a string que irá receber o horário
 */
void get_current_time(char *time_string) {
    datetime_t current_time;
    rtc_get_datetime(&current_time);
    char time_string_buffer[15];

    // Adiciona o '0' na frente do número caso seja de apenas um dígito
    if ((current_time.hour > 9) && (current_time.min > 9) && (current_time.sec > 9)) {
        sprintf(time_string, "   %d:%d:%d", current_time.hour, current_time.min, current_time.sec);
    } else if ((current_time.hour > 9) && (current_time.min > 9) && !(current_time.sec > 9)) {
        sprintf(time_string, "   %d:%d:0%d", current_time.hour, current_time.min, current_time.sec);
    } else if ((current_time.hour > 9) && !(current_time.min > 9) && (current_time.sec > 9)) {
        sprintf(time_string, "   %d:0%d:%d", current_time.hour, current_time.min, current_time.sec);
    } else if ((current_time.hour > 9) && !(current_time.min > 9) && !(current_time.sec > 9)) {
        sprintf(time_string, "   %d:0%d:0%d", current_time.hour, current_time.min, current_time.sec);
    } else if (!(current_time.hour > 9) && (current_time.min > 9) && (current_time.sec > 9)) {
        sprintf(time_string, "   0%d:%d:%d", current_time.hour, current_time.min, current_time.sec);
    } else if (!(current_time.hour > 9) && (current_time.min > 9) && !(current_time.sec > 9)) {
        sprintf(time_string, "   0%d:%d:0%d", current_time.hour, current_time.min, current_time.sec);
    } else if (!(current_time.hour > 9) && !(current_time.min > 9) && (current_time.sec > 9)) {
        sprintf(time_string, "   0%d:0%d:%d", current_time.hour, current_time.min, current_time.sec);
    } else {
        sprintf(time_string, "   0%d:0%d:0%d", current_time.hour, current_time.min, current_time.sec);
    }
}


/**
 * Exibe o horário atual no display OLED
 */
bool display_current_time() {
    char *time_string[4] = {
        "  REMEDICINE",
        "",
        "",
        "00:00:00"
    };
    char buffer[15];
    get_current_time(buffer);
    time_string[3] = buffer;
    oled_display_write(time_string, count_of(time_string), 8);

    return true; // Libera a execução do timer
}

/**
 * Seta a flag alarm_being_played
 * @param value valor a inserir
 */
void set_alarm_being_played(bool value) {
    alarm_being_played = value;
}

/**
 * Indica se algum alarme está sendo tocado
 * @return true caso sim. false caso não.
 */
bool get_if_alarm_being_played() {
    return alarm_being_played;
}