%%parameters
clear;

D = 10 ;

% CIC filter
cic_num = zeros(1,D+1);
cic_num(1) = 1;
cic_num(D+1) = -1;

cic_den = [1 -1];

cic_h = filt(cic_num,cic_den)
[cic_imp_y,cic_imp_t] = impulse(cic_h)

% recursive average filter
ave_num = zeros(1,D+1);
ave_num(1) = 1;
ave_num(D+1) = -1;
ave_den = D*[1 -1];

ave_h = filt(ave_num,ave_den)
[ave_imp_y,ave_imp_t] = impulse(ave_h);

% plot
figure(1)
bode(cic_h), grid

figure(2)
plot(step(cic_h),'r')
hold on 
plot(step(ave_h),'g')
legend('CIC','Average')
hold off

figure(4)
bode(ave_h), grid

figure(3)
plot(cic_imp_y, 'r')
hold on
plot(ave_imp_y, 'g')
legend('CIC','Average')
hold off
title('Impulse responses')
