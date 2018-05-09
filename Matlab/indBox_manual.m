function [proxg,g] = indBox_manual(x,upper_bound,lower_bound,gamma)
    % indicator box fuction
    proxg = min(upper_bound, max(lower_bound, x));
    g = 0;
end

