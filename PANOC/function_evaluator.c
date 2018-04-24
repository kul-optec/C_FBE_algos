#include "../globals/globals.h"
#include "function_evaluator.h"
#include "../include/optimizer.h"

#include <stddef.h>
#include <stdlib.h>

static real_t (*g)(const real_t* input);
static void (*proxg)(real_t* input);
static real_t (*cost_gradient_function)(const real_t* input,real_t* output);

int function_evaluator_init(const struct optimizer_problem* problem){
    g = problem->g;
    proxg = problem->proxg;

    cost_gradient_function = problem->cost_gradient_function;

    return SUCCESS;
}

int function_evaluator_cleanup(void){
    return SUCCESS;
}

/* 
 * returns the cost over the stack
 * returns the gradient in real_t* output
 */
real_t function_evaluator_f_df(const real_t* input,real_t* output){
    return cost_gradient_function(input,output);
}

real_t function_evaluator_g(const real_t* input){
    return g(input);
}
void function_evaluator_proxg(real_t* input){
    proxg(input);
}