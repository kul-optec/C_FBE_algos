function [ f,df ] = test_func( x )
    f = x(1)^2 + x(2)^2;
    df(1) =  2*x(1);
    df(2) =  2*x(2);
end
