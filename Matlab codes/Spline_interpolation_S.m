function fx = Spline_interpolation_S(x,y,h,M,j,target)
%因为x,h,M都是从1开始算的，所以也不管什么加一不加一的了
fx = M(j)*((x(j+1)-target)^3)/(6*h(j)) + M(j+1)*((target-x(j))^3)/(6*h(j)) + (y(j)-M(j)*(h(j)^2)/6)*(x(j+1) - target)/h(j) + (y(j+1)-M(j+1)*(h(j)^2)/6)*(target-x(j))/h(j);
end

