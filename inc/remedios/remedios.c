#include "remedios.h"

// Cadastro dos remédios
Reminders remedios[NUMERO_DE_REMEDIOS] = {
    // Remédio 1: Dipirona
    {
        .medicine_name      = "Dipirona",
        .rgb_matrix_pattern = cross_pattern,
        .rgb_color          = {YELLOW},
        .chosen_melody      = &melody_1,
        .chosen_time        = {
            .year   = -1,
            .month  = -1,
            .day    = -1, 
            .dotw   = -1,
            .hour   = 12,
            .min    = 0,
            .sec    = 15
        }
    },

    // Remédio 2: Dorflex
    {
        .medicine_name      = "Dorflex",
        .rgb_matrix_pattern = x_pattern,
        .rgb_color          = {BLUE},
        .chosen_melody      = &melody_2,
        .chosen_time        = {
            .year   = -1,
            .month  = -1,
            .day    = -1, 
            .dotw   = -1,
            .hour   = 12,
            .min    = 0,
            .sec    = 40
        }
    }
};