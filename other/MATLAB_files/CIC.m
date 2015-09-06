%%parameters
clear;

D = 50;
Ts = 308*10^(-9) %308 ns, STMICROELECTRONICS
%Ts = 308;
N = 1;

% CIC filter
cic_num = zeros(1,D+1);
cic_num(1) = 1;
cic_num(D+1) = -1;

cic_den = [1 -1];

cic_h = filt(cic_num,cic_den,Ts)
cic_h = cic_h^N;
[cic_imp_y,cic_imp_t] = impulse(cic_h);

% recursive average filter
ave_num = zeros(1,D+1);
ave_num(1) = 1;
ave_num(D+1) = -1;
ave_den = D*[1 -1];

ave_h = filt(ave_num,ave_den,Ts)
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
title('Step responses')

figure(3)
plot(cic_imp_y, 'r+')
hold on
plot(ave_imp_y, 'bo')
legend('CIC','Average')
hold off
title('Impulse responses')

figure(4)

[h,w] = freqz(cic_num,cic_den,'half',2001);
plot(w/pi,20*log10(abs(h)),'r')
grid;
ax = gca;
%ax.YLim = [-100 20];
ax.XTick = 0:.25:1;
xlabel('Normalized Frequency (\times\pi rad/sample)')
ylabel('Magnitude (dB)')

hold on;
[h,w] = freqz(ave_num,ave_den,'half',2001);
plot(w/pi,20*log10(abs(h)),'b')
hold off;
legend('CIC','Average')
