function R = Q_value(p,wy)
n = size(p,2);
ps = sum(p);
RS = 0;
for i = 1:n
    R(i) = fix(p(i)/ps*wy);
    RS = RS + R(i);
end
wy1 = wy - RS;
for i = 1:wy1
    for j = 1:n
        Q(j,i) = p(j).^2/(R(j)*(R(j)+1));
    end
    t = Q(1,i);
    a = 1;
    for m=2:n
        if Q(m,i)>t
            t = Q(m,i);
            a = m;
        end
    end
    R(a) = R(a)+1;
end


end

