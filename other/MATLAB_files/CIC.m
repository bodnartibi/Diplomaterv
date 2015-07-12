%%parameters
clear;

D = 25;

N = 3;

% CIC filter
cic_num = zeros(1,D+1);
cic_num(1) = 1;
cic_num(D+1) = -1;

cic_den = [1 -1];

cic_h = filt(cic_num,cic_den,0.01)
cic_h = cic_h^N;
[cic_imp_y,cic_imp_t] = impulse(cic_h);

% recursive average filter
ave_num = zeros(1,D+1);
ave_num(1) = 1;
ave_num(D+1) = -1;
ave_den = D*[1 -1];

ave_h = filt(ave_num,ave_den,0.01)
[ave_imp_y,ave_imp_t] = impulse(ave_h);

% plot
figure(1)
%subplot (2,1,1)
bode(cic_h); grid;
hold on
%subplot (2,1,2)
%figure(10)
bode(ave_h); grid;
hold off

figure(2)
plot(step(cic_h),'r+')
hold on 
plot(step(ave_h),'bo')
legend('CIC','Average')
hold off

figure(3)
plot(cic_imp_y, 'r+')
hold on
plot(ave_imp_y, 'bo')
legend('CIC','Average')
hold off
title('Impulse responses')
