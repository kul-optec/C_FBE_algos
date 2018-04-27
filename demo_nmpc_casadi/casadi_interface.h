#ifndef CASADI_INTERFACE_H
#define CASADI_INTERFACE_H

#include "../globals/globals.h"
#include <stddef.h>
#include <stdlib.h>

int casadi_interface_init(const int number_of_constraints_);
int casadi_interface_cleanup(void);
int casadi_prepare_cost_function(const real_t* current_state);
int casadi_integrate(const real_t* state,const real_t* input,real_t* new_state);

/* size_t casadi_interface_get_dimension(); */

/* constraint related functions */
real_t casadi_get_weight_constraints(int index_constraint);/* returns zero if index was out of range */
int casadi_set_weight_constraints(int index_constraint,real_t weight);/* returns failure if the index is out of range */

real_t casadi_interface_f_df(const real_t* input,real_t* output);

#endif