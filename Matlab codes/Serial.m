function Serial()   %��������
delete(instrfindall);   %�ȹرմ��ڣ�������ܵ��³���

global x            %ȫ�ֱ������������жϺ���ʹ��

global t;			%ȫ�ֱ��������������Ҫ����ͼ�εĸ�����Ҫ����
global m;
global i;

t = [0];      %ʱ����
m = [0];      %������
i = 0;        %���ڼ���

 p = plot(t,m);
 axis([i-50 i+50 -100 100]);
 grid on;

x = serial('com3');
set(x,'BaudRate',115200);
set(x,'BytesAvailableFcnMode','Terminator') %ASCII�������ַ�����
set(x,'Terminator','CR/LF')         %���յ�\r\n�󴥷��ж�
x.BytesAvailableFcn = @Callback  %�����ж���Ӧ���������������жϺ�����
fopen(x);

pause

fclose(x);
end

