function Callback(obj,event)   %�����жϷ�����������ͼ��

global t; %ʱ��
global m; %������
global i; %ʱ��仯ֵ


    % disp('I receive!')
    out = fscanf(obj);
    data = str2num(out)	%�����յ����ַ�ת��Ϊ��ֵ
     
     %%----------------------���¸�����Ҫ���б�д-------------------------------
    t = [t i];							
    m = [m data];
    plot(t,m)
    xlabel('t');
    ylabel('data');
    axis([i-80 i+80 50 120]);
    grid on;
    
    i=i+1;

end
