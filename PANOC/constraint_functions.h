#ifndef CONSTRAINT_FUNCTIONS_H
#define CONSTRAINT_FUNCTIONS_H

#include "../globals/globals.h"

struct indicator_box_square_function{
    unsigned int dimension;
    real_t lower_bound;
    real_t upper_bound;
    real_t (*proxg)(real_t* input);
    real_t inf;
};

int constraint_functions_init_box(struct indicator_box_square_function** box_function,\
    unsigned int dimension_,real_t lower_bound,real_t upper_bound);
int constraint_functions_cleanup_box(struct indicator_box_square_function* box_function);

#endif