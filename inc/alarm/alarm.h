#ifndef __ALARM_H__ // Caso já tenha sido declarada em algum outro lugar, não declare novamente.
#define __ALARM_H__

#include "pico/stdlib.h" //Biblioteca geral com códigos pertinentes à RP2040.
#include "inc/buzzer/buzzer.h"              // Biblioteca do Buzzer.

// Estrutura de Alarme
typedef struct {
    char medicine_name[256];                    // Nome do remédio
    uint8_t *rgb_matrix_pattern;                // Padrão da Matriz RGB
    uint8_t rgb_color[3];                       // Cores da Matriz RGB
    melody *chosen_melody;                      // Melodia escolhida
    datetime_t chosen_time;                     // Horário do alarme
} Reminders;

// Funções
void alarm_init();
void validate_time();
void notify_medicine(Reminders *medicine);
int8_t check_for_medicine_alarm();
void get_current_time(char *time_string);
void set_alarm_being_played(bool value);
bool get_if_alarm_being_played();
bool display_current_time();

#endif