function answer = Spline_interpolation(x,f,target)
%只是自然条件下的三次样条插值函数
%x:函数点
%f:函数值
%ff:首末点一阶导数
%target:想求f(target)
n = size(x,2); %列数，即样本数
h = zeros(1,n);
v = zeros(1,n);
u = zeros(1,n);
d = zeros(n,1);
F = zeros(n);
A = zeros(n,n);
del2x = zeros(1,n-2);
del3x = zeros(1,n-3);
del4x = zeros(1,n-4);
h=diff(x);
del_y = diff(f);
first = del_y./h;
for i=1:n-2
    del2x(i) = x(i+2) - x(i);
end
del2y = diff(first);
second = del2y./del2x;
for i=1:n-3
    del3x(i) = x(i+3) - x(i);
end
del3y = diff(second);
third = del3y./del3x;
for i=1:n-4
    del4x(i) = x(i+4) - x(i);
end
del4y = diff(third);
forth = del4y./del4x;
for i=1:n-2
    u(i) = h(i-1+1)/(h(i-1+1)+h(i+1));
    %这里减一又加一呢，是因为MATLAB数组是从1开始的，书上是从零开始的；
    %减一是与书上格式保持一致，加一为了适应MATLAB
    v(i) = 1-u(i);
end
d(0+1) = 0;
d(n-1+1) = 0;%假设有N组数据，实际上书上的n=N-1,那我这里的n=N。
for i=1+1:n-1
    d(i) = 6*second(i-1);
end
A(1,1) = 2; A(1,2) =0;
A(n,n-1) = 0; A(n,n) = 2;
for i=2:n-1
    A(i,i) = 2;
    A(i,i-1) = u(i-1);
    A(i,i+1) = v(i-1);
end

M = A\d;

%disp(second);
%disp(A);
%disp(d);
%disp(M);

if target<=x(1)
    answer = Spline_interpolation_S(x,f,h,M,1,target);
end

for i=1:n-1
    if (target>x(i) & target<=x(i+1))
        answer = Spline_interpolation_S(x,f,h,M,i,target);
    end
end
if target>x(n)
    answer = Spline_interpolation_S(x,f,h,M,n-1,target);
end
end