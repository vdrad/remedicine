#include "rgb_matrix.h"

#include "pico/stdlib.h"        // Biblioteca geral com códigos pertinentes à RP2040.
#include "hardware/pio.h"       // Biblioteca com funções de PIO (Programmable Input/Output) da RP2040.
#include "hardware/clocks.h"    // Biblioteca com funções acerca do clock (hardware) da RP2040.
#include "ws2818b.pio.h"        // Biblioteca para controle dos LEDs RGB da Matriz 5x5
#include "inc/pinout.h"         // Biblioteca contendo a relação dos pinos da RP2040 utilizados no projeto.

// // Definição de desenhos padronizados para serem desenhados na matriz de LEDs
// Padrão de "+"
uint8_t cross_pattern[RGB_MATRIX_COUNT] = {
  0, 0, 1, 0, 0,
  0, 0, 1, 0, 0,
  1, 1, 1, 1, 1,
  0, 0, 1, 0, 0,
  0, 0, 1, 0, 0,
};

// Padrão de "X"
uint8_t x_pattern[RGB_MATRIX_COUNT] = {
  1, 0, 0, 0, 1,
  0, 1, 0, 1, 0,
  0, 0, 1, 0, 0,
  0, 1, 0, 1, 0,
  1, 0, 0, 0, 1,
};

// Padrão de "O"
uint8_t circle_pattern[RGB_MATRIX_COUNT] = {
  0, 1, 1, 1, 0,
  1, 0, 0, 0, 1,
  1, 0, 0, 0, 1,
  1, 0, 0, 0, 1,
  0, 1, 1, 1, 0,
};

// Padrão de [QUADRADO]
uint8_t square_pattern[RGB_MATRIX_COUNT] = {
  1, 1, 1, 1, 1,
  1, 0, 0, 0, 1,
  1, 0, 0, 0, 1,
  1, 0, 0, 0, 1,
  1, 1, 1, 1, 1,
};

// Padrão de [TRIÂNGULO]
uint8_t triangle_pattern[RGB_MATRIX_COUNT] = {
  0, 0, 0, 0, 0,
  1, 1, 1, 1, 1,
  0, 1, 1, 1, 0,
  0, 0, 1, 0, 0,
  0, 0, 0, 0, 0,
};

// Padrão de :)
uint8_t smile_pattern[RGB_MATRIX_COUNT] = {
  0, 0, 0, 0, 0,
  0, 1, 1, 1, 0,
  1, 0, 0, 0, 1,
  0, 1, 0, 1, 0,
  0, 0, 0, 0, 0,
};

// Padrão de :(
uint8_t sad_pattern[RGB_MATRIX_COUNT] = {
  1, 0, 0, 0, 1,
  0, 1, 1, 1, 0,
  0, 0, 0, 0, 0,
  0, 0, 0, 0, 0,
  0, 1, 0, 1, 0,
};

// Padrão Vazio
uint8_t clear_pattern[RGB_MATRIX_COUNT] = {
  0, 0, 0, 0, 0,
  0, 0, 0, 0, 0,
  0, 0, 0, 0, 0,
  0, 0, 0, 0, 0,
  0, 0, 0, 0, 0,
};

// Definição de pixel GRB
struct pixel_t {
  uint8_t G, R, B; // Três valores de 8-bits compõem um pixel.
};
typedef struct pixel_t pixel_t;
typedef pixel_t RGB_LED_t; // Cria instância da estrutura

// Declaração do buffer de pixels que formam a matriz.
RGB_LED_t rgb_leds[RGB_MATRIX_COUNT];

// Variáveis para uso da máquina PIO.
PIO np_pio;
uint sm;

/**
 * Reverte a ordem de um byte LSB -> MSB e sucessivamente.
 * @param b o byte a ser revertido
 */
uint8_t reverse_byte(uint8_t b) {
  uint8_t reversed = 0; // Inicializa o byte a ser revertido

  // Faz a reversão bit a bit seguindo a lógica: 
  // 1. Captura o i-ésimo bit do byte original ((b >> i) & 0x1)
  // 2. Envia-o para a variável reversed com o operador |
  // 3. Arrasta o bit transferido para a esquerda (reversed << 1)
  for (uint8_t i = 0; i < 8; i++) {
    reversed = reversed << 1; // 
    reversed = (reversed | ((b >> i) & 0x1)); 
  }

  return reversed;
}

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
  for (uint i = 0; i < RGB_MATRIX_COUNT; ++i) {
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
  // É preciso reverter a ordem dos bytes pois o LED RGB processa primeiro o MSB.
  uint8_t reversed_r = reverse_byte(r);
  uint8_t reversed_g = reverse_byte(g);
  uint8_t reversed_b = reverse_byte(b);

  rgb_leds[index].R = reversed_r;
  rgb_leds[index].G = reversed_g;
  rgb_leds[index].B = reversed_b;
}

/**
 * Limpa o buffer de pixels.
 */
void np_clear() {
  for (uint8_t i = 0; i < RGB_MATRIX_COUNT; ++i)
    np_set_led(i, 0, 0, 0);
}

/**
 * Escreve os dados do buffer nos LEDs.
 */
void np_write() {
  // Escreve cada dado de 8-bits dos pixels em sequência no buffer da máquina PIO.
  for (uint i = 0; i < RGB_MATRIX_COUNT; ++i) {
    pio_sm_put_blocking(np_pio, sm, rgb_leds[i].G);
    pio_sm_put_blocking(np_pio, sm, rgb_leds[i].R);
    pio_sm_put_blocking(np_pio, sm, rgb_leds[i].B);
  }
  sleep_us(100); // Espera 100us, sinal de RESET do datasheet.
}

/**
 * Inicializa a matriz de LEDs RGB.
 */
void rgb_matrix_init() {
  np_init(RGB_MATRIX_PIN);
  np_clear();
  np_write();
}

/**
 * Exibe um desenho padronizado na Matriz de LEDs RGB com a cor especificada.
 * @param pattern o padrão desejado
 * @param r
 * @param g
 * @param b
 */
void rgb_matrix_write_pattern(const uint8_t *pattern, const uint8_t r, const uint8_t g, const uint8_t b) {
    // Passa por todos os LEDs da Matriz. Caso aquele LED esteja ativo no padrão escolhido, escreve a cor desejada.
    // Caso o LED não esteja ativo, torna-o apagado.
    for (uint8_t currentLED = 0; currentLED < RGB_MATRIX_COUNT; currentLED++) {
        if (pattern[currentLED]) {
            np_set_led(currentLED, r, g, b);
        } else {
            np_set_led(currentLED, 0, 0, 0);
        }
    }
    np_write(); // Ao fim, escreve os valores para a matriz.
}

/**
 * Exibe todos os padrões configurados a cada 2s para teste.
 */
void rgb_matrix_validate() {
  while (true) {
    rgb_matrix_write_pattern(cross_pattern, RED);
    sleep_ms(2000);
  
    rgb_matrix_write_pattern(x_pattern, GREEN);
    sleep_ms(2000);
  
    rgb_matrix_write_pattern(circle_pattern, BLUE);
    sleep_ms(2000);
  
    rgb_matrix_write_pattern(square_pattern, PURPLE);
    sleep_ms(2000);
  
    rgb_matrix_write_pattern(triangle_pattern, ORANGE);
    sleep_ms(2000);
  }
}