/*
 * panoc.c - Matlab Interface to panoc
 *
 * solves non linear programming problems.
 *
 * The calling syntax is:
 *
 *		panoc('init',problem,solver_params);
 *      number_of_iterations = panoc('solve',solution,@functionhandle);
 *      panoc('cleanup');
 *
 * This is a MEX file for MATLAB.
*/

#include "mex.h"
#include "matrix.h"
#include"stdio.h"
#include"stdlib.h"
#include <string.h>

#include"../include/optimizer.h"

#include"panoc.h"
#include"panoc_arguments_check.h"
#include "panoc_arguments_parse.h"

/* read out parameters and set the optimizer_problem struct */ 
static struct optimizer_problem* problem;

/* initiaze the indicator box function */
static int init_solver_box(const mxArray* mx_struct_problem);

static int init_solver_costum_constraint(const mxArray* mx_struct_problem);

/* callback function of the cost/gradient */
double callback_cost_gradient_function(double* input,double* output);

/* 
 * callback function of the constraint function g/proxg 
 * -> returns value g(x) 
 * -> saves proxg(x) in proxg_x
 */
double callback_proxg(double* x);

static mxArray* function_handle;
static mxArray* function_handle_constraint;/* only used with costum constraint */

/* 
 * The gateway function 
 *  nlhs  :  Number of output (left-side) arguments, or the size of the plhs array.
 *  plhs  :  Array of output arguments.
 *  nrhs  :  Number of input (right-side) arguments, or the size of the prhs array.
 *  prhs  :  Array of input arguments.
 */
void mexFunction(int nlhs, mxArray *plhs[],int nrhs, const mxArray *prhs[])
{
    const int mode = check_input_arguments(nrhs, prhs);
    switch (mode)
    {
    case INIT_MODE:
        problem = malloc(sizeof(struct optimizer_problem));
        if (problem == NULL) {
            mexErrMsgTxt("Cannot allocate problem struct, not enough heap memory available"); 
            break;
        }
        parse_problem(prhs,problem);
        parse_solver(prhs,problem);

        /* set the cost/gradient function to the callback function */
        problem->cost_gradient_function = callback_cost_gradient_function;

        const mxArray* mx_struct_problem = prhs[1];
        if (parser_get_constraint_mode() == BOX_MODE)
            init_solver_box(mx_struct_problem);
        if (parser_get_constraint_mode() == COSTUM_CONSTRAINT_MODE)
            init_solver_costum_constraint(mx_struct_problem);

        break;
    case SOLVE_MODE:
        double* init_solution = mxGetPr(prhs[1]);
        function_handle =(mxArray*) (prhs[2]); /* get the function handle */
        
        int number_of_iterations = solve_problem(init_solution);
        if(nlhs>0) plhs[0] = mxCreateDoubleScalar(number_of_iterations);
        break;
    case CLEANUP_MODE:
        if (problem!=NULL) free(problem);
        optimizer_cleanup();
        break;
    default:
        mexErrMsgTxt("error invalid mode");
        break;
    }

}
static int init_solver_costum_constraint(const mxArray* mx_struct_problem){
    /* Loop over all problem related fields, get the values that are only relevant to the constraint*/
    int number_of_fields_problem = mxGetNumberOfFields(mx_struct_problem);
    mwIndex  field_number;
    for (field_number = 0; field_number < number_of_fields_problem; field_number++) {
        const char* name = mxGetFieldNameByNumber(mx_struct_problem, field_number); /* get name */
        mxArray* data = mxGetField(mx_struct_problem, FIRST_ELEMENT_INDEX, name); /* get data */

        /* set the right value if it's relevant */
        if (strcmp(name, "constraint") == 0)
            function_handle_constraint =(mxArray*) (data); /* get the function handle */
            
    }
    
    optimizer_init_with_costum_constraint(problem,callback_proxg);
    
    return SUCCESS;
}
/*
 * Find box constraint specific parameters and call the right init function.
 */
static int init_solver_box(const mxArray* mx_struct_problem) {
    double lower_bound=0;
    double upper_bound=0;

    /* Loop over all problem related fields, get the values that are only relevant to the constraint*/
    int number_of_fields_problem = mxGetNumberOfFields(mx_struct_problem);
    mwIndex  field_number;
    for (field_number = 0; field_number < number_of_fields_problem; field_number++) {
        const char* name = mxGetFieldNameByNumber(mx_struct_problem, field_number); /* get name */
        mxArray* data = mxGetField(mx_struct_problem, FIRST_ELEMENT_INDEX, name); /* get data */

        /* set the right value if it's relevant */
        if (strcmp(name, "lower_bound") == 0)
            lower_bound = mxGetScalar(data);
        if (strcmp(name, "upper_bound") == 0)
            upper_bound = mxGetScalar(data);
    }

    optimizer_init_with_box(problem, lower_bound, upper_bound);

    return SUCCESS;
}

double callback_cost_gradient_function(const double* input, double* output) {    
    mwSize ONE = 1;
    mwSize DIMENSION = problem->dimension;

    mxArray* mx_input_matlab[2];
    mx_input_matlab[0] = function_handle;
    mx_input_matlab[1] = mxCreateDoubleMatrix(DIMENSION, ONE, mxREAL);

    mxArray* mx_output_matlab[2];

    /* set the input values */
    double* input_matlab = mxGetPr(mx_input_matlab[1]);
    int i;
    for (i = 0; i < problem->dimension; i++)
        input_matlab[i] = input[i];

    /* call the cost/gradient function handle from MATLAB */
    int state = mexCallMATLAB(2, mx_output_matlab, 2, mx_input_matlab, "feval");
    
    /* get the data from the MATLAB matrices */
    double* gradient = mxGetPr(mx_output_matlab[1]);
    double* cost = mxGetPr(mx_output_matlab[0]);

    /* set the gradient to output */
    for (i = 0; i < problem->dimension; i++)
        output[i] = gradient[i];

    return cost[0];/* return the cost by value*/
}

double callback_proxg(double* input){
    mwSize ONE = 1;
    mwSize DIMENSION = problem->dimension;
    
    mxArray* mx_input_matlab[2];
    mx_input_matlab[0] = function_handle_constraint;
    mx_input_matlab[1] = mxCreateDoubleMatrix(DIMENSION, ONE, mxREAL);

    mxArray* mx_output_matlab[2];
    
    /* set the input values */
    double* input_matlab = mxGetPr(mx_input_matlab[1]);
    int i;
    for (i = 0; i < problem->dimension; i++)
        input_matlab[i] = input[i];
    
    /* call the cost/gradient function handle from MATLAB */
    int state = mexCallMATLAB(2, mx_output_matlab, 2, mx_input_matlab, "feval");
    
    /* get the data from the MATLAB matrices */
    double* proxg = mxGetPr(mx_output_matlab[0]);
    double g = mxGetScalar(mx_output_matlab[1]);
    
    for (i = 0; i < problem->dimension; i++)
        input[i] = proxg[i];
    
    return g;
}