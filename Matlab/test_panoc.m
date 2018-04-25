mex -g panoc.c PANOC_lib.lib

problem.cost_function = 'cost_f_df';
problem.dimension = 2;
problem.constraint_type = 'box';
problem.upper_bound=1;
problem.lower_bound=-1;

solver_params.tolerance = 1e-3;
solver_params.buffer_size = 50;

% panoc('init',problem,solver_params);
% panoc('solve',x0);
% panoc('cleanup');
