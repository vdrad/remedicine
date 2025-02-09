#ifndef __RGB_MATRIX_H__ // Caso já tenha sido declarada em algum outro lugar, não declare novamente.
#define __RGB_MATRIX_H__

#include "pico/stdlib.h"        // Biblioteca geral com códigos pertinentes à RP2040.
#include "inc/pinout.h"         // Biblioteca contendo a relação dos pinos da RP2040 utilizados no projeto.

// Definição de cores padronizadas para os LEDs
#define RED             255, 0, 0
#define ORANGE          255, 48, 0
#define YELLOW          255, 115, 0
#define LIME_GREEN      163, 251, 0
#define GREEN           0, 230,   0
#define EMERALD         0, 250, 40
#define CIAN            0, 255, 255
#define LIGHT_BLUE      0, 90, 255
#define BLUE            0,   0, 255
#define PURPLE          137,   0, 255
#define PINK            240,   0, 80
#define SCARLET         255, 0, 20
#define WHITE           200, 200, 200
#define BLANK_COLOR     0,   0,   0

// Declaração dos padrões como extern para que possam ser acessados externamente
extern uint8_t cross_pattern[RGB_LED_COUNT];
extern uint8_t x_pattern[RGB_LED_COUNT];
extern uint8_t circle_pattern[RGB_LED_COUNT];
extern uint8_t square_pattern[RGB_LED_COUNT];
extern uint8_t triangle_pattern[RGB_LED_COUNT];

// Funções de acesso externo
void rgb_matrix_init();
void rgb_matrix_write_pattern(const uint8_t *pattern, const uint8_t r, const uint8_t g, const uint8_t b);
void rgb_matrix_test();

#endif