#ifndef __REMEDIOS_H__ // Caso já tenha sido declarada em algum outro lugar, não declare novamente.
#define __REMEDIOS_H__

#include "pico/stdlib.h"                //Biblioteca geral com códigos pertinentes à RP2040.
#include "inc/alarm/alarm.h"            // Biblioteca do Alarme.
#include "inc/rgb_matrix/rgb_matrix.h"  // Biblioteca da Matriz de LEDs RGB.

// Declaração dos remédios
// Por se tratar de interação com o usuário, o código está escrito em português
#define NUMERO_DE_REMEDIOS 2
extern Reminders remedios[NUMERO_DE_REMEDIOS];

#endif