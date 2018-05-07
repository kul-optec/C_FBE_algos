#ifndef EXAMPLE_PROBLEMS_H
#define EXAMPLE_PROBLEMS_H

#include "../globals/globals.h"
#include <stddef.h>
#include <stdlib.h>

/* a polynomial as f */
int f_poly_init(unsigned int dimension,unsigned int degree );
real_t f_poly(const real_t* input);
void df_poly(const real_t* input,real_t* output);

/* extra test function, rosenbrock function, minimum at (1,1)*/
real_t f_rosenbrock(const real_t* x);
void df_rosenbrock(const real_t* x,real_t* output);

/* g1 and its prox*/
int example_problems_set_init_problem1(real_t w,unsigned int dimension);
real_t g_1(const real_t* x);
real_t proxg_1(real_t* x, real_t gamma);

real_t g_2(const real_t* x);
real_t proxg_2(real_t* x, real_t gamma);

int example_problems_set_init_problem3(real_t u_min,real_t u_max);
real_t g_3(const real_t* x);
real_t proxg_3(real_t* x, real_t gamma);

#endif