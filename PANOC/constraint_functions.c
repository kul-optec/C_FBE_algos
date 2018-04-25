#include "../globals/globals.h"
#include "constraint_functions.h"
#include<stdlib.h>

static struct indicator_box_square_function* indicator_box_square_function_data;
static real_t g_indicator_box_square_function(const real_t* input);
static void proxg_indicator_box_square_function(real_t* input);

int constraint_functions_init_box(struct indicator_box_square_function** box_function,\
                                    unsigned int dimension_,real_t lower_bound,real_t upper_bound){

    *box_function = malloc(sizeof(struct indicator_box_square_function));
    if(*box_function==NULL) return FAILURE;

    indicator_box_square_function_data = *box_function;

    indicator_box_square_function_data->dimension=dimension_;
    indicator_box_square_function_data->g=g_indicator_box_square_function;
    indicator_box_square_function_data->proxg=proxg_indicator_box_square_function;
    indicator_box_square_function_data->lower_bound=lower_bound;
    indicator_box_square_function_data->upper_bound=upper_bound;
    indicator_box_square_function_data->inf=LARGE;

    return SUCCESS;
}

int constraint_functions_cleanup_box(struct indicator_box_square_function* box_function){
    free(box_function);
    indicator_box_square_function_data=NULL;
    return SUCCESS;
}

/*
 * if input is outside the box, return large number otherwise return zero
 */
static real_t g_indicator_box_square_function(const real_t* input){
    unsigned int i;
    for (i = 0; i < indicator_box_square_function_data->dimension; i++)
    {
        if(input[i]<indicator_box_square_function_data->lower_bound || input[i]<indicator_box_square_function_data->upper_bound )
            return indicator_box_square_function_data->inf;
    }
    return 0;
}

static void proxg_indicator_box_square_function(real_t* input){
    unsigned int i;
    for (i = 0; i < indicator_box_square_function_data->dimension; i++)
    {
        if(input[i]<indicator_box_square_function_data->lower_bound)
        {
            input[i]=indicator_box_square_function_data->lower_bound;
        }
        else if (input[i]>indicator_box_square_function_data->upper_bound)
        {
            input[i]=indicator_box_square_function_data->upper_bound;
        }
    }
}