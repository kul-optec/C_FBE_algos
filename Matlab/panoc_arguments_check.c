#include "panoc_arguments_check.h"
#include "panoc.h"
#include "mex.h"

/*
 * checks the validity of the input arguments and returns the mode
 */
int check_input_arguments(int nrhs, const mxArray* prhs[]){
    const char * error_message_invalid_mode = "The first input argument must be either 'init','solve' or 'cleanup'";
    if (!mxIsChar(prhs[0])) {
        mexErrMsgTxt(error_message_invalid_mode);
    }
    const int mode = validate_mode(prhs);
    switch (mode)
    {
        case INVALID_MODE:
            mexErrMsgTxt(error_message_invalid_mode);
            break;
        case INIT_MODE:
            check_input_arguments_init_mode(nrhs, prhs);
            break;
    }
    return mode;
}

/*
 * check if the mode is either: init,solve or cleanup
 * -> return the mode
 */
int validate_mode(const mxArray *prhs[]){
    const char* mode_in_char = mxGetChars(prhs[0]);

    if (mode_in_char[0] == 'i') return INIT_MODE;
    if (mode_in_char[0] == 'c') return CLEANUP_MODE;
    if (mode_in_char[0] == 's') return SOLVE_MODE;
    return INVALID_MODE;
}

/* 
 * panoc(mode,name_const_function,type_constraints,data_constraint)
 * 
 * mode : char array (already checked)
 * name_const_function : char array
 * type_constraints : char array
 */
int check_input_arguments_init_mode(int nrhs, const mxArray *prhs[]){
    const char* error_message_invalid_init_args = "invalid options with init, use panoc(mode,problem,solver_params)";

    if(nrhs != 3) mexErrMsgTxt(error_message_invalid_init_args);
    if (!mxIsStruct(prhs[1]) || !mxIsStruct(prhs[2])) mexErrMsgTxt(error_message_invalid_init_args);

    return SUCCESS;
}