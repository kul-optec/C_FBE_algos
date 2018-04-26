#ifndef PANOC_ARGUMENTS_CHECK_H
#define PANOC_ARGUMENTS_CHECK_H
#include "mex.h"

/* check type of input parameters */
int check_input_arguments(int nrhs, const mxArray* prhs[]);
int check_input_arguments_init_mode(int nrhs, const mxArray *prhs[]);

/* get the mode from the input parameter */
int validate_mode(const mxArray *prhs[]);

#endif