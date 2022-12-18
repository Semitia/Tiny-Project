function [x] = Gauss_Seidal(A,b, error)
%A:系数矩阵
%b:等式右侧矩阵（叫啥来着想不起来了）
%error:误差值
%x:解

n = size(A);
L = zeros(n);
U = zeros(n);
D = zeros(n);
for i=1:n
    D(i,i) = A(i,i);
end

for i=2:n
    for j=1:i-1
        L(i,j) = -A(i,j);
        U(j,i) = -A(j,i);
    end
end

G = (D-L)\U;
f = (D-L)\b;
x1 = [0; 0; 0];
err = inf;
while err>error
    x2 = G*x1 + f;
    err = norm(x2-x1,inf);
    x1 = x2;
end

x = x2;

end

