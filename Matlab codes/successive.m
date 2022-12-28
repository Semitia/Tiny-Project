function answer = successive(up,down,error)
%answer 积分估计值
%up 积分上限
%down 积分下限
%error 截断误差
%被积函数为1/x，懒得再开一个函数了
n=1;
answer = 0.5*(1/up + 1/down)*(up - down);
last_ans = answer;
n = 2*n;
answer = answer/2;
for i=1:2:n-1
    answer = answer + (1/(down + i*(up-down)/n))*((up - down)/n);
end

    disp(n);
    disp(answer);
    disp(abs(answer - last_ans));

while abs(answer - last_ans)>3*error
    last_ans = answer;
    n = 2*n;
    answer = answer/2;
    for i=1:2:n-1
        answer = answer + (1/(down + i*(up-down)/n))*((up - down)/n);
    end
    disp(n);
    disp(answer);
    disp(abs(answer - last_ans));
end

end

