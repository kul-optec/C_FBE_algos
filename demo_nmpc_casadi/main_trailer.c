#include "stdio.h"
#include "../include/optimizer.h"
#include "./casadi_interface.h"

#define DIMENSION_STATE 3
#define DIMENSION_INPUT 2
#define HORIZON 40

#define PI (3.141592)


static int prepare_casadi(real_t* static_casadi_parameters,const real_t* current_state,const real_t* state_reference,\
        const real_t* input_reference){
    int i;
    for(i=0;i<DIMENSION_STATE;i++){
        static_casadi_parameters[i] = current_state[i];
        static_casadi_parameters[i+DIMENSION_STATE] = state_reference[i];
    }
    for(i=0;i<DIMENSION_INPUT;i++){
        static_casadi_parameters[i+2*DIMENSION_STATE] = input_reference[i];
    }
    casadi_prepare_cost_function(static_casadi_parameters);
    return SUCCESS;
}

int main(){
    printf("Debugging controller: \n");
    size_t number_of_steps=60;
    size_t print_limit=10;

    double ref_state[3]={7.,5.,0.8};
    double ref_input[3]={0,0};

    double state[3]={0,-0.5,PI/2};
    double new_state[3];
    double input[2];

    /*
     * solve specific variables
     */
    real_t lower_bound = -4;
    real_t upper_bound = 4;

    struct optimizer_problem problem;
    problem.cost_gradient_function=casadi_interface_f_df;
    problem.dimension=HORIZON*DIMENSION_INPUT;

    problem.solver_params.tolerance=1e-3;
    problem.solver_params.buffer_size=50;
    problem.solver_params.max_interations=1000;

    optimizer_init_with_box(&problem,lower_bound,upper_bound);

    /*
     * casadi specific things
     */
    double static_casadi_parameters[DIMENSION_STATE*2 + DIMENSION_INPUT];
    const int number_of_constraints = 4;
    casadi_interface_init(number_of_constraints);

    casadi_set_weight_constraints(0,700.);
    casadi_set_weight_constraints(1,700.);
    casadi_set_weight_constraints(2,700.);
    casadi_set_weight_constraints(3,700.);

    printf("# initial state x=%f y=%f theta=%f \n \n",state[0],state[1],state[2]);
    real_t* input_horizon = calloc(HORIZON*DIMENSION_INPUT,sizeof(real_t));

    unsigned int i;
    for (i = 0; i < number_of_steps; i++)
    {
        prepare_casadi(static_casadi_parameters,state,ref_state,ref_input);
        int iterations = solve_problem(input_horizon);
        unsigned int j;
        for (j = 0; j < DIMENSION_INPUT; j++) input[j]=input_horizon[j];

        if(i<print_limit)
            printf("- Optimal input Ux=%f Uy=%f solved in %d interations\n",input[0],input[1],iterations);

        casadi_integrate(state,input,new_state);
        for (j = 0; j < DIMENSION_STATE; j++) state[j]=new_state[j];

        if(i<print_limit)
            printf("# new state x=%f y=%f theta=%f \n \n",state[0],state[1],state[2]);

        /* no shift on the horizon */
    }

    free(input_horizon);
    printf("# Final state x=%f y=%f theta=%f \n \n",state[0],state[1],state[2]);
    optimizer_cleanup();

    return 0;/* return success */
}
