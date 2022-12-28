function answer = Romberg(up,down,error)
T = zeros(1000,1000);
n = 1;
T(1,1) = (up - down)*(Rom_f(up) + Rom_f(down))/2;
disp(T(1,1));
for k = 1:1:10
    n = 2*n;
    a = 1;
    b = k;
    T(1,b+1) = T(1,b)/2;
    for j=1:2:n-1
        T(1,b+1) = T(1,b+1) + Rom_f(down + j*(up-down)/n)*((up - down)/n);
    end
    for j = b:-1:1
        a = a+1;
        T(a,j) = T(a-1,j+1)*4^(a-1)/(4^(a-1)-1) - T(a-1,j)*1/(4^(a-1)-1);
        %matlab数组从1开始真的恶心到我了，a在第八行加过一，T里的下标不用再加了，但是别忘了4的指数要减掉1
    end
    
    disp(T(1,k+1));
    disp(T(k+1,1));
    if abs(T(k+1,1) - T(k,1)) < error
        final = k+1;
        break;
    end
    
end
%disp(T(1,1));
%disp(T(1,2));
%disp(T(1,3));
%disp(T(2,1));
%disp(T(2,2));
%disp(T(3,1));
answer = T(final,1);

end

