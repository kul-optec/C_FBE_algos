#include "function_evaluator_test.h"
#include "../../PANOC/function_evaluator.h"

#include "../../globals/globals.h"
#include <stddef.h>
#include <stdlib.h>

static real_t (*proxg)( real_t* state);
static real_t (*f)(const real_t* input);
static void (*df)(const real_t* input, real_t* output);

static size_t dimension;

int function_evaluator_init(const struct optimizer_problem* problem){return FAILURE;}
size_t function_evaluator_get_dimension(){return dimension;}

/* test init function */
int function_evaluator_test_init(size_t dimension_, 
    real_t (*proxg_)(real_t* state),
    real_t (*f_)(const real_t* input),
    void (*df_)(const real_t* input, real_t* output)){
    
    f=f_;proxg=proxg_;df=df_;
    dimension=dimension_;
    return SUCCESS;
}

/* cost functions */
real_t function_evaluator_f(const real_t* input){
    return f(input);
}
void function_evaluator_df(const real_t* input,real_t* output){
    df(input,output);
}
real_t function_evaluator_f_df(const real_t* input,real_t* output){
    df(input,output); /* get gradient */
    return f(input); /* get function value */
}
real_t  function_evaluator_proxg(real_t* state){
    return proxg(state);
}