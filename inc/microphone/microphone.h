#ifndef __MICROPHONE_H__ // Caso já tenha sido declarada em algum outro lugar, não declare novamente.
#define __MICROPHONE_H__

#include "pico/stdlib.h"                                    // Biblioteca geral com códigos pertinentes à RP2040.

// Parâmetros de ADC
#define ADC_CLOCK_DIV       96.f                            // Divisor de clock para definir taxa de amostragem do ADC
#define SAMPLES             200                             // Número de amostras a serem coletadas no ADC
#define ADC_READ_VOLTS(x)   (x * 3.3f / (1 << 12u) - 1.65f) // Retorna o valor lido pelo ADC em Volts
#define ADC_MAX_VOLTS       3.3f                            // Máximo valor lido pelo ADC em Volts
#define ADC_STEP            (3.3f/5.f)                      // Intervalos de leitura do microfone

// Funções
void microphone_init();
bool microphone_detect_blow();
uint8_t get_microphone_read();

#endif