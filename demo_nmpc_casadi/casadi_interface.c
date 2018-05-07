#include "../globals/globals.h"

#include "cost_function_derivative_combined.h"

#include <stddef.h>
#include <stdlib.h>
#include "casadi_interface.h"

#include "integrator.c"

#define MEM_CASADI 0
#define  DEFAULT_CONSTRAINT_WEIGHT 100

static real_t* constraint_weights;
static const real_t* state;

static unsigned int number_of_constraints;

int casadi_interface_init(const int number_of_constraints_){
    number_of_constraints = number_of_constraints_;
    if(number_of_constraints>0){
        constraint_weights=malloc(sizeof(real_t)*number_of_constraints);
        if(constraint_weights==NULL) goto fail_1;
        else{
            unsigned int i;
            for (i = 0; i < number_of_constraints; i++){
                constraint_weights[i]=DEFAULT_CONSTRAINT_WEIGHT;
            }
        }
    }
    return SUCCESS;

    fail_1:
        return FAILURE;
}
int casadi_interface_cleanup(void){
    free(constraint_weights);
    return SUCCESS;
}
int casadi_integrate(const real_t* current_state,const real_t* input,real_t* new_state){
    const real_t* input_casadi[2]={current_state,input};
    real_t* output_casadi[1]={new_state};

    return integrator(input_casadi, output_casadi, NULL, NULL, MEM_CASADI);
}

int casadi_prepare_cost_function(const real_t* _current_state){
    state = _current_state;
    return SUCCESS;
}

real_t casadi_interface_f_df(const real_t* input,real_t* data_output){
    real_t f_value;
    real_t* output[2] = {&f_value,data_output};
    const real_t* input_function[3]={state,input,constraint_weights};

    cost_function_derivative_combined(input_function, output, NULL, NULL, MEM_CASADI);

    return f_value;
}

int casadi_set_weight_constraints(int index_constraint,real_t weight){
    if(index_constraint<number_of_constraints && index_constraint>=0){
        constraint_weights[index_constraint]=weight;
        return SUCCESS;
    }
    return FAILURE;
}