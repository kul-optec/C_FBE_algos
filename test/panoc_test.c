#include<stdio.h>
#include<math.h>

#include"../globals/globals.h"
#include"example_problems.h"
#include"../include/optimizer.h"

int test_rosen(void);
real_t rosen_cost_gradient(const real_t* input,real_t* output);

int main(){
    return test_rosen();
}

int test_rosen(void){
    real_t lower_bound = -2;
    real_t upper_bound = 2;

    struct optimizer_problem problem;
    problem.cost_gradient_function=rosen_cost_gradient;
    problem.dimension=2;

    problem.solver_params.tolerance=1e-3;
    problem.solver_params.buffer_size=20;
    problem.solver_params.max_interations=200;

    optimizer_init_with_box(&problem,lower_bound,upper_bound);

    real_t solution[2]={0,0};
    int iterations = solve_problem(solution);

    printf("solution in %d iterations = [%f ; %f] \n",iterations,solution[0],solution[1]);
    if(ABS(solution[0]-1.0) > 1e-15)
        return FAILURE;
    if(ABS(solution[1]-1.0) > 1e-15)
        return FAILURE;

    optimizer_cleanup();
    return SUCCESS;
}

real_t rosen_cost_gradient(const real_t* input,real_t* output){
    df_rosenbrock(input,output);
    return f_rosenbrock(input);
}