#ifndef PANOC_ARGUMENTS_PARSE_H
#define PANOC_ARGUMENTS_PARSE_H

#include "mex.h"
#include"../include/optimizer.h"

/* Parse parameters related to all types of problems. */
int parse_problem(const mxArray *prhs[],struct optimizer_problem* problem);
int parse_solver(const mxArray *prhs[],struct optimizer_problem* problem);

char* parser_get_name_cost_function(void);

#endif
