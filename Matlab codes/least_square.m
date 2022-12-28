function a = least_square(x,y,w,n)
%a��ϵ������
%x������������
%y����������ֵ
%w������Ȩ�طֲ�
%n�������Ժ�������
m = size(x,2);
FAI = zeros(n);
B = zeros(n,1);

for i = 1:n
    for j = 1:n
        for k = 1:m
            FAI(i,j) = FAI(i,j) + w(k)*ls_f(x(k),i)*ls_f(x(k),j);
        end
    end
end

for i=1:n
    for k=1:m
        B(i,1) = B(i,1) + w(k)*y(k)*ls_f(x(k),i);
    end
end

a = FAI\B;
% disp(m);
% disp(FAI);
% disp(B);
end

