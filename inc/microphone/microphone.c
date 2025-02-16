#include "inc/microphone/microphone.h"

#include <math.h>               // Biblioteca geral com códigos de operações matemáticas.
#include <stdlib.h>             // Biblioteca geral com códigos úteis em C.
#include "pico/stdlib.h"        // Biblioteca geral com códigos pertinentes à RP2040.
#include "hardware/adc.h"       // Biblioteca com funções acerca do ADC (hardware) da RP2040.
#include "hardware/dma.h"       // Biblioteca com funções acerca do DMA (hardware) da RP2040.
#include "inc/pinout.h"         // Biblioteca contendo a relação dos pinos da RP2040 utilizados no projeto.
#include "inc/alarm/alarm.h"    // Biblioteca do Alarme.

// Cria o canal DMA
uint dma_channel;
dma_channel_config dma_cfg;

uint16_t adc_buffer[SAMPLES];   // Cria um buffer para armazenar leituras do ADC


/**
 * Inicializa o periférico microfone da BitDogLab
 */
void microphone_init() {
    // Inicializações do ADC
    adc_gpio_init(MICROPHONE_PIN);
    adc_init();
    adc_select_input(MICROPHONE_CHANNEL);
    adc_fifo_setup(
        true,   // Habilita a FIFO
        true,   // Habilita requisição de dados via DMA
        1,      // Requisita DMA a cada 1 leitura do ADC
        false,  // Não reserva bit para indicar erro
        false   // Não faz downscale das amostras de 12 para 8 bits
    );
    adc_set_clkdiv(ADC_CLOCK_DIV); // Ajusta o divisor de clock do ADC

    // Inicializações do DMA
    dma_channel = dma_claim_unused_channel(true);                   // Reserva um canal DMA
    dma_cfg     = dma_channel_get_default_config(dma_channel);      // Importa as configurações padrão do DMA
    channel_config_set_transfer_data_size(&dma_cfg, DMA_SIZE_16);   // Define o tamanho da transferência de dados como 16 bits (uint16_t)
    channel_config_set_read_increment(&dma_cfg, false);             // Desabilita o incremento automático de endereço de registrador (assim lemos continuamente no mesmo registrador)
    channel_config_set_write_increment(&dma_cfg, true);             // Habilita o incremento automático do endereço de escrita (escreve sucessivamente index a index)
    channel_config_set_dreq(&dma_cfg, DREQ_ADC);                    // Configura a requisição de dados do ADC

    // Utiliza o recurso timer para monitorar o microfone
    static struct repeating_timer  microphone_timer;                                    // Cria a estrutura de timer ciclico
    add_repeating_timer_us(500 *1000, microphone_detect_blow, NULL, &microphone_timer); // Configura a função para ser executada a cada 500 ms
}

/**
 * Lê o ADC atrelado ao microfone e armazena as leituras no buffer
 */
void micrphone_read_sample() {
    adc_fifo_drain();   // Limpa a FIFO do ADC, eliminando leituras antigas
    adc_run(false);     // Desliga o ADC para efetuar as devidas configurações

    // Configura o canal DMA
    dma_channel_configure(
        dma_channel,        // Informa o canal
        &dma_cfg,           // Informa as configurações
        adc_buffer,         // Informa o buffer
        &(adc_hw->fifo),    // Informa a fila de leituras
        SAMPLES,            // Informa a quantidade de amostrar
        true                // Liga o modo DMA
    );

    // Liga o ADC e espera o fim de um ciclo de leituras
    adc_run(true);
    dma_channel_wait_for_finish_blocking(dma_channel);

    adc_run(false); // Desliga o ADC após o fim de um ciclo de leituras
}

/**
 * Calcula a tensão RMS do sinal detectado pelo microfone
 * @return sqrt(média(leitura^2))
 */
float microphone_read_rms_voltage() {
    float average = 0.f;

    for (uint32_t current_reading = 0; current_reading < SAMPLES; ++current_reading) {
        average += (float)adc_buffer[current_reading] * (float)adc_buffer[current_reading];
    }
    average /= (float)SAMPLES;
    average = sqrt(average);

    return average;
}

/**
 * Detecta se houve uma situação de sopro 
 * @return 0 caso não. 2 caso sim.
 */
bool microphone_detect_blow() {
    micrphone_read_sample();                                                    // Faz as leituras do microfone
    float average_reading   = microphone_read_rms_voltage();                    // Converte pra tensão RMS
    average_reading         = 2.f * abs(ADC_READ_VOLTS(average_reading));       // Converte a escala

    if (round(average_reading) >= 2) {
        if (get_if_alarm_being_played()) set_stop_melody(1);
    }
    return true;
}