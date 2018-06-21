#include "../globals/globals.h"
#include "panoc.h"
#include "function_evaluator.h"
#include "./matrix_operations.h"

#include"math.h"
#include<stdlib.h>
#include<stdio.h>

#include "../include/optimizer.h"

static real_t compute_constraint_part_LA(const real_t* x,real_t* output_gradient);
static int compute_lagrangian_multipliers(real_t* new_x);

static real_t* lagrangian_multipliers;
static real_t* slacked_constraint_violations; /* The violation of the constraint with it's slack variables */
static real_t* constraint_evaluations;
static real_t* gradient_constraint;

static real_t (*cost_gradient_function)(const real_t* input,real_t* output_gradient); /* old cost function/gradient */
static real_t cost_gradient_function_LA(const real_t* input,real_t* output_gradient); /* new cost function/gradient */

static struct optimizer_extended_problem* extended_problem;
static real_t constraint_weight;

int optimizer_init_extended_box(struct optimizer_extended_problem* extended_problem_,real_t lower_bound,real_t upper_bound,real_t constraint_weight_){
    extended_problem = extended_problem_;
    constraint_weight = constraint_weight_;
    cost_gradient_function = extended_problem->problem.cost_gradient_function;

    if(optimizer_init_with_box(&extended_problem->problem,lower_bound ,upper_bound)==FAILURE)
        goto fail_1;

    /* If the box constraint PANOC optimizer was succesfull created */
    cost_gradient_function  = extended_problem->problem.cost_gradient_function; /* save the cost/gradient function */
    extended_problem->problem.cost_gradient_function=cost_gradient_function_LA; /* set the cost/gradient to the Lagragian cost/gradient*/

    lagrangian_multipliers = malloc(sizeof(real_t)*extended_problem->problem.dimension);
    if(lagrangian_multipliers==NULL)goto fail_2;

    constraint_evaluations = malloc(sizeof(real_t)*extended_problem->number_of_constraints);
    if(constraint_evaluations==NULL) goto fail_3;

    slacked_constraint_violations = malloc(sizeof(real_t)*extended_problem->number_of_constraints);
    if(slacked_constraint_violations==NULL) goto fail_4;

    gradient_constraint = malloc(sizeof(real_t)*extended_problem->problem.dimension);
    if(gradient_constraint==NULL) goto fail_5;
    
    return SUCCESS;

    fail_5:
        free(slacked_constraint_violations);
    fail_4:
        free(constraint_evaluations);
    fail_3:
        free(lagrangian_multipliers);
    fail_2:
        optimizer_cleanup();
    fail_1:
        return FAILURE;
}

int optimizer_cleanup_extended(void){
    free(gradient_constraint);
    free(slacked_constraint_violations);
    free(constraint_evaluations);
    free(lagrangian_multipliers);
    optimizer_cleanup();
}

/*
 * Replacement function for the cost and gradient function
 */
static real_t cost_gradient_function_LA(const real_t* input,real_t* output_gradient){
    /* fill up output_gradient with the gradient of f */
    real_t buffer =  cost_gradient_function(input,output_gradient);

    /* compute constraint related stuff for this position */
    buffer += compute_constraint_part_LA(input,output_gradient);
   
    return buffer;
}

/*
 * This function is called by the user externally to get the solution
 */
int solve_extended_problem(real_t* solution){
    unsigned i;

    /* init the slack variables to zero */
    for (i = 0; i < extended_problem->number_of_constraints; i++){
        lagrangian_multipliers[i]=0;
    }

    int interations_till_convergence = 0;
    for( i = 0; i < extended_problem->number_of_iterations; i++){
        
        /* solve the problem with the current slack variables */
        interations_till_convergence += solve_problem(solution);

        compute_lagrangian_multipliers(solution);
    }

    return interations_till_convergence;
}

/*
 * The gradient of the lagragian is defined as: df(x) + constraint part
 *      constraint part = constraint_weight*(dh(x)*(h(x)+(1/c)*lagrangian_multiplier - z_c(x,lagrangian_multiplier)))
 */
static real_t add_constraint_gradient_part(const real_t constraint_evaluation,const real_t lagrangian_multiplier,\
        const real_t slacked_constraint_violation, real_t* output_gradient){

    real_t buffer = constraint_evaluation + (1/constraint_weight)*lagrangian_multiplier - slacked_constraint_violation;

    /* add the gradient part */
    vector_add_ntimes(output_gradient,gradient_constraint,extended_problem->problem.dimension,constraint_weight);

    /* return the function part */
    return sq(buffer)*(constraint_weight/2)-1/(2*constraint_weight)*sq(lagrangian_multiplier); 
}

/*
 * Compute the constraint part of the lagrangian (cost and gradient)
 */
static real_t compute_constraint_part_LA(const real_t* x,real_t* output_gradient){
    unsigned int index_constraint;real_t LA=0;
    for (index_constraint = 0; index_constraint < extended_problem->number_of_constraints; index_constraint++){

        /* evaluate constraint */
        constraint_evaluations[index_constraint]=extended_problem->constraints[index_constraint](x,gradient_constraint);

        /* calculate the violation */
        slacked_constraint_violations[index_constraint] = MIN(extended_problem->upper_bounds_constraints[index_constraint],\
            MAX(constraint_evaluations[index_constraint],extended_problem->lower_bounds_constraints[index_constraint])\
        );

        /* calculate the gradient part and add it to the current gradient, it returns the cost function */
        LA += add_constraint_gradient_part(constraint_evaluations[index_constraint], lagrangian_multipliers[index_constraint],\
            slacked_constraint_violations[index_constraint], output_gradient);
    }
    return LA;
}

/*
 * Compute the lagrangian multipliers with the formula
 *     y = y + constraint_weight*(g(new_x)-z_c(x,y))
 */
static int compute_lagrangian_multipliers(real_t* new_x){
    unsigned int index_constraint;
    for (index_constraint = 0; index_constraint < extended_problem->number_of_constraints; index_constraint++){
        real_t constraint_evaluation = extended_problem->constraints[index_constraint](new_x,gradient_constraint);/* evaluate constraint in new position */

        lagrangian_multipliers[index_constraint] = lagrangian_multipliers[index_constraint] + constraint_weight*(constraint_evaluation - lagrangian_multipliers[index_constraint]);
    }
    return SUCCESS;
}