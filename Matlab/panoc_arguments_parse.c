#include "panoc_arguments_parse.h"
#include"../include/optimizer.h"
#include "mex.h"
#include "panoc.h"
#include <string.h>

/* Set specific parameter in the problem structure */
static int set_solver_parameter(const char* propertie_name, const mxArray* data,struct optimizer_problem* problem);
static int set_problem_parameter(const char* propertie_name,const mxArray* data,struct optimizer_problem* problem);

/* static variables only needed in mex file */
static unsigned char constraint_mode;

static int set_problem_parameter(const char* propertie_name,const mxArray* data,struct optimizer_problem* problem){
    if (strcmp(propertie_name, "dimension") == 0)
        if (mxIsScalar(data))
            problem->dimension = (int)(mxGetScalar(data) + 0.5); /* make sure it rounds down to the right value*/

    if (strcmp(propertie_name, "constraint_type") == 0)
        constraint_mode = BOX_MODE;/* at the moment only one constraint so no need to check for now */

    return SUCCESS;
}
int parse_problem(const mxArray *prhs[],struct optimizer_problem* problem){
    int number_of_fields_problem = mxGetNumberOfFields(prhs[1]);

    mwIndex  field_number;
    const mxArray* mx_struct_problem = prhs[1];
    for (field_number = 0; field_number < number_of_fields_problem; field_number++){
        const char* name = mxGetFieldNameByNumber(mx_struct_problem,field_number); /* get name */
        const mxArray* data = mxGetField(mx_struct_problem, FIRST_ELEMENT_INDEX, name); /* get data */
        set_problem_parameter(name, data,problem);
    }

    return SUCCESS;
}
static int set_solver_parameter(const char* propertie_name, const mxArray* data,struct optimizer_problem* problem){
    if (strcmp(propertie_name, "tolerance") == 0)
        if (mxIsScalar(data))
            problem->solver_params.tolerance = mxGetScalar(data);

    if (strcmp(propertie_name, "buffer_size") == 0)
        if (mxIsScalar(data)) 
            problem->solver_params.buffer_size = (int)(mxGetScalar(data) + 0.5); /* make sure it rounds down to the right value*/

    if (strcmp(propertie_name, "max_iterations") == 0)
        if (mxIsScalar(data)) 
            problem->solver_params.max_interations = (int)(mxGetScalar(data) + 0.5); /* make sure it rounds down to the right value*/

    return SUCCESS;
}
int parse_solver(const mxArray *prhs[],struct optimizer_problem* problem){
    int number_of_fields_solver = mxGetNumberOfFields(prhs[2]);

    mwIndex  field_number;
    const mxArray* mx_struct_solver = prhs[2];
    for (field_number = 0; field_number < number_of_fields_solver; field_number++) {
        const char* name = mxGetFieldNameByNumber(mx_struct_solver, field_number); /* get name */
        const mxArray* data = mxGetField(mx_struct_solver, FIRST_ELEMENT_INDEX, name); /* get data */

        set_solver_parameter(name, data, problem);
    }

    return SUCCESS;
}

/*
 * some getters:
 */
unsigned char parser_get_constraint_mode(void){return constraint_mode;}