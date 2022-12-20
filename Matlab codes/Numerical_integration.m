function [integration] = Numerical_integration(n,mode,up,low)
% n:����������
% mode:���ַ�ʽѡ��
% up:��������
% low:��������

I  =zeros(1,n);
h = (up - low)/n;
integration = 0;
switch mode
    case 1 %���λ���
        for i=1:1:n
           I(i) =  (Numerical_integration_f(low+(i-1)*h) + Numerical_integration_f(low+i*h))*h/2;
           integration = integration + I(i);
        end
    case 2 %Simpson
        for i=1:1:n
           I(i) =  (Numerical_integration_f(low+(i-1)*h) + 4*Numerical_integration_f(low+(i-0.5)*h)+ Numerical_integration_f(low+i*h))*h/6;
           integration = integration + I(i);
        end
end

end

