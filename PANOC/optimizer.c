#include "../globals/globals.h"
#include "panoc.h"
#include "function_evaluator.h"

#include"math.h"
#include<stdlib.h>
#include<stdio.h>

#include "../include/optimizer.h"
#include "constraint_functions.h"

#define INDICATOR_BOX_SQUARED 0
#define COSTUM_MODE 1

static struct optimizer_problem* problem; /* data related to the problem */
static struct indicator_box_square_function* indicator_box_square_function_data; /* data related to the constraints used by the prox operator */

static unsigned char initialized=FALSE;
static unsigned char mode;
static real_t* new_solution;

static void save_solution(real_t* solution);
static int optimizer_init(void);

int solve_problem(real_t* solution){
    /* 
     * take implicitly the previous inputs as the starting position for the algorithm 
     */
    unsigned int i_panoc;
    real_t current_residual=problem->solver_params.tolerance*10;
    for (i_panoc= 0; i_panoc < problem->solver_params.max_interations ; i_panoc++){
        if(current_residual<problem->solver_params.tolerance){
            /* if the residual is low enough stop iterating */
            break;
        }
        current_residual = panoc_get_new_location(solution,new_solution);

        /*
         * if the residual was larger then the machine accuracy
         * -> set the new_input as input for the next iteration 
         * WARNING: if the residual was smaller then the machine 
         *  accuracy you might get NaN thats why we won't use it
         */
        if(current_residual>MACHINE_ACCURACY)
            save_solution(solution);
            
    }

    panoc_reset_cycli(); /* reset all counters/buffers */

    return i_panoc;
}

static void save_solution(real_t* solution){
    unsigned int i;
    for (i = 0; i < problem->dimension; i++)
    {
        solution[i]=new_solution[i];
    }
}

static int optimizer_init(void){
    if(initialized==TRUE) /* already initialized, remove old stuff first */
        optimizer_cleanup();
    
    /* Prepare the solver */
    if(panoc_init(problem->dimension,problem->solver_params.buffer_size)==FAILURE) return FAILURE;

    /* Prepare the cost function */
    if(function_evaluator_init(problem)==FAILURE) return FAILURE;

    initialized=TRUE;
    return SUCCESS;
}

int optimizer_init_with_costum_constraint(struct optimizer_problem* problem_,real_t (*proxg)(real_t* x)){
    mode=COSTUM_MODE;
    problem=problem_;

    /* prepare proxg(x) */
    problem->proxg=proxg;

    if(optimizer_init()==FAILURE) goto fail_1;

    new_solution = malloc(sizeof(real_t)*problem->dimension);
    if(new_solution==NULL) goto fail_2;

    return SUCCESS;

    fail_2:
        optimizer_cleanup();        
    fail_1:
        return FAILURE;
}

int optimizer_init_with_box(struct optimizer_problem* problem_,real_t lower_bound,real_t upper_bound){
    mode=INDICATOR_BOX_SQUARED;
    problem=problem_;    

    /* prepare proxg(x) */
    constraint_functions_init_box(&indicator_box_square_function_data,problem->dimension,lower_bound,upper_bound);
    if(indicator_box_square_function_data==NULL) goto fail_1;
    problem->proxg=indicator_box_square_function_data->proxg;

    if(optimizer_init()==FAILURE) goto fail_2;

    new_solution = malloc(sizeof(real_t)*problem->dimension);
    if(new_solution==NULL) goto fail_3;

    return SUCCESS;

    fail_3:
        optimizer_cleanup();        
    fail_2:
        constraint_functions_cleanup_box(indicator_box_square_function_data);
    fail_1:
        return FAILURE;
}

int optimizer_cleanup(void){
    panoc_cleanup();
    function_evaluator_cleanup();
    switch (mode)
    {
    case INDICATOR_BOX_SQUARED:
        free(indicator_box_square_function_data);
        break;
    default:
        break;
    }
    free(new_solution);
    initialized=FALSE;
    return SUCCESS;
}