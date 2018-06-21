#include<stdio.h>
#include<math.h>

#include"../globals/globals.h"
#include"example_problems.h"
#include"../include/optimizer.h"

static int test_rosen_box(void);
static int test_rosen_costum_box();
static real_t rosen_cost_gradient(const real_t* input,real_t* output);

int main(){
    return test_rosen_box()+test_rosen_costum_box();
}

static int test_rosen_box(void){
    real_t lower_bound = -2;
    real_t upper_bound = 2;

    struct optimizer_problem problem;
    problem.cost_gradient_function=rosen_cost_gradient;
    problem.dimension=2;

    problem.solver_params.tolerance=1e-12;
    problem.solver_params.buffer_size=20;
    problem.solver_params.max_interations=200;

    optimizer_init_with_box(&problem,lower_bound,upper_bound);

    real_t solution[2]={0,0};
    int iterations = solve_problem(solution);

    printf("solution in %d iterations = [%f ; %f] \n",iterations,solution[0],solution[1]);

	real_t error = ABS(solution[0] - 1.0);
    if(error > problem.solver_params.tolerance)
        return FAILURE;
	error = ABS(solution[1] - 1.0);
    if(error > problem.solver_params.tolerance)
        return FAILURE;

    optimizer_cleanup();
    return SUCCESS;
}

static real_t costum_proxg(real_t* x, real_t gamma){
    real_t lower_bound = -2;
    real_t upper_bound = 2;
    real_t g_x=0;
    unsigned int i;
    for (i = 0; i < 2; i++)
    {
        if(x[i]<lower_bound)
        {
            g_x = LARGE;
            x[i]=lower_bound;
        }
        else if (x[i]>upper_bound)
        {
            g_x = LARGE;
            x[i]=upper_bound;
        }
    }
    return g_x;
}

static int test_rosen_costum_box(void){
    struct optimizer_problem problem;
    problem.cost_gradient_function=rosen_cost_gradient;
    problem.dimension=2;

    problem.solver_params.tolerance=1e-12;
    problem.solver_params.buffer_size=20;
    problem.solver_params.max_interations=200;

    optimizer_init_with_costum_constraint(&problem,costum_proxg);

    real_t solution[2]={0,0};
    int iterations = solve_problem(solution);

    printf("solution in %d iterations = [%f ; %f] \n",iterations,solution[0],solution[1]);

	real_t error = ABS(solution[0] - 1.0);
    if(error > problem.solver_params.tolerance)
        return FAILURE;
	error = ABS(solution[1] - 1.0);
    if(error > problem.solver_params.tolerance)
        return FAILURE;

    optimizer_cleanup();
    return SUCCESS;
}
static real_t rosen_cost_gradient(const real_t* input,real_t* output){
    df_rosenbrock(input,output);
    return f_rosenbrock(input);
}