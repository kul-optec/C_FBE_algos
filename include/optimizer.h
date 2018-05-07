#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include "../globals/globals.h"
struct solver_parameters{
    unsigned int max_interations;
    real_t tolerance;
    unsigned int buffer_size;
};

struct optimizer_problem{
    unsigned int dimension;
    real_t (*proxg)(real_t* input, real_t gamma);
    real_t (*cost_gradient_function)(const real_t* input,real_t* output_gradient);
    struct solver_parameters solver_params;
};

/*
 * Initializes a optimizer that uses the PANOC algorithm.
 */
int optimizer_init(struct optimizer_problem* problem_);

/*
 * This interfaces a optimization problem onto the panoc algorithm:
 *      min cost(x)
 *          subject to  L<x<H
 */
int optimizer_init_with_box(struct optimizer_problem* problem,real_t lower_bound,real_t upper_bound);
/*
 * This interfaces a optimization problem onto the panoc algorithm:
 *      min cost(x)
 *          subject to  g(x)=0
 */
int optimizer_init_with_costum_constraint(struct optimizer_problem* problem_,real_t (*proxg)(real_t* x, real_t gamma));

int optimizer_cleanup(void);
int solve_problem(real_t* solution);

#endif