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
            .sec    = 30
        }
    },

    // Remédio 3: Paracetamol
    {
        .medicine_name      = "Paracetamol",
        .rgb_matrix_pattern = circle_pattern,
        .rgb_color          = {PINK},
        .chosen_melody      = &melody_3,
        .chosen_time        = {
            .year   = -1,
            .month  = -1,
            .day    = -1, 
            .dotw   = -1,
            .hour   = 12,
            .min    = 0,
            .sec    = 45
        }
    },

    // Remédio 4: Paracetamol
    {
        .medicine_name      = "Omeprazol",
        .rgb_matrix_pattern = square_pattern,
        .rgb_color          = {ORANGE},
        .chosen_melody      = &melody_4,
        .chosen_time        = {
            .year   = -1,
            .month  = -1,
            .day    = -1, 
            .dotw   = -1,
            .hour   = 12,
            .min    = 1,
            .sec    = 15
        }
    },

    // Remédio 5: Paracetamol
    {
        .medicine_name      = "Omeprazol",
        .rgb_matrix_pattern = triangle_pattern,
        .rgb_color          = {EMERALD},
        .chosen_melody      = &melody_5,
        .chosen_time        = {
            .year   = -1,
            .month  = -1,
            .day    = -1, 
            .dotw   = -1,
            .hour   = 12,
            .min    = 2,
            .sec    = 0
        }
    }
};