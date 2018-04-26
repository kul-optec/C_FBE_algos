function [f,g] = rosen(x)
%  x : initial point
%  f : function value
%  g : gradient
%
a=1;
b=100;
f =(a-x(1))^2 + b*(x(2)-x(1))^2;

if nargout > 1
   g = [-2*(a-(b+1)*x(1)+b*x(2)); 2*b*(x(2)-x(1)) ];
end
