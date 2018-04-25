/*
 * test.c - Matlab Interface to panoc
 *
 * solves non linear programming problems
 *
 * The calling syntax is:
 *
 *		outMatrix = arrayProduct(multiplier, inMatrix)
 *
 * This is a MEX file for MATLAB.
*/

#include "mex.h"
#include "matrix.h"
#include"stdio.h"
#include"stdlib.h"
#include <string.h>
#include"../include/optimizer.h"

#define SUCCESS 0
#define FAILURE 1

/* call modes */
#define INVALID_MODE 0
#define INIT_MODE 1
#define SOLVE_MODE 2
#define CLEANUP_MODE 3

/* constraint modes */
#define BOX_MODE 1

static int mode;
static struct optimizer_problem* problem;
static char* name_cost_function;
static unsigned char constraint_mode;
static mxArray * mx_struct_problem;

static int check_input_arguments(int nrhs, const mxArray *prhs[]);
static int check_input_arguments_init_mode(int nrhs, const mxArray *prhs[]);

static int validate_mode(const mxArray *prhs[]);

static int parse_problem(const mxArray *prhs[]);
static void set_problem_parameter(const char* propertie_name, const mxArray* data);

static int init_solver_box(void);


/* 
 * The gateway function 
 *  nlhs	Number of output (left-side) arguments, or the size of the plhs array.
 *  plhs	Array of output arguments.
 *  nrhs	Number of input (right-side) arguments, or the size of the prhs array.
 *  prhs	Array of input arguments.
 */
void mexFunction(int nlhs, mxArray *plhs[],int nrhs, const mxArray *prhs[])
{
    check_input_arguments(nrhs, prhs);
    switch (mode)
    {
    case INIT_MODE:
        problem = malloc(sizeof(struct optimizer_problem));
        if (problem == NULL) {
            mexErrMsgTxt("Cannot allocate problem struct, not enough heap memory available"); 
            break;
        }
        parse_problem(prhs);

        break;
    case SOLVE_MODE:
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
/*
 * find box constraint specific parameters and call the right init function
 */
static int init_solver_box(void) {
    double lower_bound=0;
    double upper_bound=0;

    /* Loop over all problem related fields, get the values that are only relevant to the constraint*/
    int number_of_fields_problem = mxGetNumberOfFields(mx_struct_problem);
    mwIndex  field_number;
    for (field_number = 0; field_number < number_of_fields_problem; field_number++) {

        const char* name = mxGetFieldNameByNumber(mx_struct_problem, field_number); /* get name */
        mxArray* data = mxGetField(mx_struct_problem, field_number, name); /* get data */

        /* set the right value if it's relevant */
        if (strcmp(name, "lower_boud") == 0)
            lower_bound = mxGetScalar(data);
        if (strcmp(name, "upper_boud") == 0)
            upper_bound = mxGetScalar(data);
    }

    return SUCCESS;
}
static void set_problem_parameter(const char* propertie_name,const mxArray* data){
    if (strcmp(propertie_name, "cost_function" )==0)
        name_cost_function = mxArrayToString(data);
    if (strcmp(propertie_name, "dimension") == 0)
        if (mxIsScalar(data)) {
            double dimension_dbl = mxGetScalar(data);
            problem->dimension = (int)(mxGetScalar(data) + 0.5); /* make sure it rounds down to the right value*/
        }
    if (strcmp(propertie_name, "constraint_type") == 0) {
        /* at the moment only one constraint so no need to check */
        constraint_mode = BOX_MODE;
    }
}
/*
 * Parse parameters related to all types of problems.
 */
static int parse_problem(const mxArray *prhs[]) {
    int number_of_fields_problem = mxGetNumberOfFields(prhs[1]);
    int number_of_fields_solver = mxGetNumberOfFields(prhs[2]);

    mwIndex  field_number;
    mx_struct_problem = prhs[1];
    for (field_number = 0; field_number < number_of_fields_problem; field_number++){
        const char* name = mxGetFieldNameByNumber(mx_struct_problem,field_number); /* get name */
        const mxArray* data = mxGetField(mx_struct_problem, field_number, name); /* get data */
        mxIsScalar(data);
        set_problem_parameter(name, data);
    }

    return SUCCESS;
}

static int check_input_arguments(int nrhs, const mxArray *prhs[]) {
    const char * error_message_invalid_mode = "The first input argument must be either 'init','solve' or 'cleanup'";
    if (!mxIsChar(prhs[0])) {
        mexErrMsgTxt(error_message_invalid_mode);
    }else {
        mode = validate_mode(prhs);
        if (mode == INVALID_MODE)  mexErrMsgTxt(error_message_invalid_mode);
    }

    if (mode == INIT_MODE)
        return check_input_arguments_init_mode(nrhs, prhs);

    return SUCCESS;
}

/*
 * check if the mode is either: init,solve or cleanup
 * -> return the mode
 */
static int validate_mode(const mxArray *prhs[]) {
    const char* mode_in_char = mxGetChars(prhs[0]);

    if (mode_in_char[0] == 'i') return INIT_MODE;
    if (mode_in_char[0] == 's') return CLEANUP_MODE;
    if (mode_in_char[0] == 'c') return INIT_MODE;
    return INVALID_MODE;
}

/* 
 * panoc(mode,name_const_function,type_constraints,data_constraint)
 * 
 * mode : char array (already checked)
 * name_const_function : char array
 * type_constraints : char array
 */
static int check_input_arguments_init_mode(int nrhs, const mxArray *prhs[]) {
    const char* error_message_invalid_init_args = "invalid options with init, use panoc(mode,problem,solver_params)";

    if(nrhs != 3) mexErrMsgTxt(error_message_invalid_init_args);
    if (!mxIsStruct(prhs[1]) || !mxIsStruct(prhs[2]))mexErrMsgTxt(error_message_invalid_init_args);

    return SUCCESS;
}