clear all;
mex -g panoc.c panoc_arguments_check.c panoc_arguments_parse.c PANOC_lib.lib
%%
problem.dimension = 2;
problem.constraint_type = 'box';
problem.upper_bound=4;
problem.lower_bound=-4;
problem.cost_function='rosen';

solver_params.tolerance = 1e-10;
solver_params.buffer_size = 50;
solver_params.max_iterations = 200;

solution = [-1;1];

panoc('init',problem,solver_params);
number_of_terations = panoc('solve',solution);
panoc('cleanup');
%%