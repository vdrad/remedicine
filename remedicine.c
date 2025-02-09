/*
[DESCRIÇÃO DO PROJETO]
*/

#include <stdio.h>              // Biblioteca para as funções gerais de pino e UARTA
#include "pico/stdlib.h"        // Biblioteca geral com códigos pertinentes à RP2040
#include "hardware/pio.h"       // Biblioteca com funções de PIO (Programmable Input/Output) da RP2040
#include "hardware/clocks.h"    // Biblioteca com funções acerca do clock (hardware) da RP2040

/* ==================================== MATRIZ DE LEDs RGB 5X5 ==================================== */
// Biblioteca para controle dos LEDs RGB da Matriz 5x5
#include "ws2818b.pio.h"

// Definição do número de LEDs e pino.
#define RGB_LED_COUNT   25
#define RGB_LED_PIN     7

// Definição de cores padronizadas para os LEDs
#define RED             255, 0, 0
#define ORANGE          240, 48, 0
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

// Definição de desenhos padronizados para serem desenhados na matriz de LEDs
// Padrão de "+"
uint8_t cross_pattern[RGB_LED_COUNT] = {
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    1, 1, 1, 1, 1,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
};

// Padrão de "X"
uint8_t x_pattern[RGB_LED_COUNT] = {
    1, 0, 0, 0, 1,
    0, 1, 0, 1, 0,
    0, 0, 1, 0, 0,
    0, 1, 0, 1, 0,
    1, 0, 0, 0, 1,
};

// Padrão de "O"
uint8_t circle_pattern[RGB_LED_COUNT] = {
    0, 1, 1, 1, 0,
    1, 0, 0, 0, 1,
    1, 0, 0, 0, 1,
    1, 0, 0, 0, 1,
    0, 1, 1, 1, 0,
};

// Padrão de [QUADRADO]
uint8_t square_pattern[RGB_LED_COUNT] = {
    1, 1, 1, 1, 1,
    1, 0, 0, 0, 1,
    1, 0, 0, 0, 1,
    1, 0, 0, 0, 1,
    1, 1, 1, 1, 1,
};

// Padrão de [TRIÂNGULO]
uint8_t triangle_pattern[RGB_LED_COUNT] = {
    0, 0, 0, 0, 0,
    1, 1, 1, 1, 1,
    0, 1, 1, 1, 0,
    0, 0, 1, 0, 0,
    0, 0, 0, 0, 0,
};

// Definição de pixel GRB
struct pixel_t {
  uint8_t G, R, B; // Três valores de 8-bits compõem um pixel.
};
typedef struct pixel_t pixel_t;
typedef pixel_t RGB_LED_t; // Cria instância da estrutura

// Declaração do buffer de pixels que formam a matriz.
RGB_LED_t rgb_leds[RGB_LED_COUNT];

// Variáveis para uso da máquina PIO.
PIO np_pio;
uint sm;

/**
 * Inicializa a máquina PIO para controle da matriz de LEDs.
 * @param pin 
 */
void np_init(uint pin) {

  // Cria programa PIO.
  uint offset = pio_add_program(pio0, &ws2818b_program);
  np_pio = pio0;

  // Toma posse de uma máquina PIO.
  sm = pio_claim_unused_sm(np_pio, false);
  if (sm < 0) {
    np_pio = pio1;
    sm = pio_claim_unused_sm(np_pio, true); // Se nenhuma máquina estiver livre, panic!
  }

  // Inicia programa na máquina PIO obtida.
  ws2818b_program_init(np_pio, sm, offset, pin, 800000.f);

  // Limpa buffer de pixels.
  for (uint i = 0; i < RGB_LED_COUNT; ++i) {
    rgb_leds[i].R = 0;
    rgb_leds[i].G = 0;
    rgb_leds[i].B = 0;
  }
}

/**
 * Atribui uma cor RGB a um LED.
 * @param index
 * @param r
 * @param g
 * @param b
 */
void np_set_led(const uint index, const uint8_t r, const uint8_t g, const uint8_t b) {
  rgb_leds[index].R = r;
  rgb_leds[index].G = g;
  rgb_leds[index].B = b;
}

/**
 * Limpa o buffer de pixels.
 */
void np_clear() {
  for (uint8_t i = 0; i < RGB_LED_COUNT; ++i)
    np_set_led(i, 0, 0, 0);
}

/**
 * Escreve os dados do buffer nos LEDs.
 */
void np_write() {
  // Escreve cada dado de 8-bits dos pixels em sequência no buffer da máquina PIO.
  for (uint i = 0; i < RGB_LED_COUNT; ++i) {
    pio_sm_put_blocking(np_pio, sm, rgb_leds[i].G);
    pio_sm_put_blocking(np_pio, sm, rgb_leds[i].R);
    pio_sm_put_blocking(np_pio, sm, rgb_leds[i].B);
  }
  sleep_us(100); // Espera 100us, sinal de RESET do datasheet.
}

/**
 * Inicializa a biblioteca dos LEDs RGB.
 */
void rgb_leds_init() {
    np_init(RGB_LED_PIN);
    np_clear();
}

/**
 * Exibe um desenho padronizado na Matriz de LEDs RGB com a cor especificada.
 * @param pattern o padrão desejado
 * @param r
 * @param g
 * @param b
 */
void rgb_write_pattern(const uint8_t *pattern, const uint8_t r, const uint8_t g, const uint8_t b) {
    // Passa por todos os LEDs da Matriz. Caso aquele LED esteja ativo no padrão escolhido, escreve a cor desejada.
    // Caso o LED não esteja ativo, torna-o apagado.
    for (uint8_t currentLED = 0; currentLED < RGB_LED_COUNT; currentLED++) {
        if (pattern[currentLED]) {
            np_set_led(currentLED, r, g, b);
        } else {
            np_set_led(currentLED, 0, 0, 0);
        }
    }
    np_write(); // Ao fim, escreve os valores para a matriz.
}

/* ==================================== CÓDIGO PRINCIPAL ==================================== */
int main() {
    stdio_init_all(); // Inicializa entradas e saídas.
    rgb_leds_init(); // Inicializa matriz de LEDs NeoPixel.

    // Não faz mais nada. Loop infinito.
    while (true) {
        sleep_ms(1000);
    }
}