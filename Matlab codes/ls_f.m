function fx = ls_f(x,number)
%least square's functions
%每个模型的这个函数都需要专门定制
switch number
    case 1
        fx = 1;
        %break,为了防止写c语言的时候又忘记break，即使在MATLAB我也要写一下
    case 2
        fx = x;
end

end

