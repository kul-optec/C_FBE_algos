function [function_value,gradient] = rosen(x)
    %  x : initial point

    a=1;
    b=100;
    function_value =(a-x(1))^2 + b*(x(2)-x(1))^2;

    if nargout > 1
       gradient = [-2*(a-(b+1)*x(1)+b*x(2)); 2*b*(x(2)-x(1)) ];
    end
end
