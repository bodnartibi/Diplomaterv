clear;
D = 50;
size_sq = 5000;
time=[0:1:size_sq-1];
sq = (square(time/100)) + 0.2 * rand(1, length(time));

%[signal,fs] = wavread('csett.wav');
FFT_FOK = 5000; %44200;

signal = transpose(sq);


spek_signal=abs(fft(signal,FFT_FOK))*100;
y=[0:1:FFT_FOK-1];
out_t = [0:1:size(signal(:,1))-1];

% CIC filter
cic_num = zeros(1,D+1);
cic_num(1) = 1;
cic_num(D+1) = -1;

cic_den = [1 -1];

out = filter(cic_num,cic_den,signal);
spek_out = abs(fft(out,FFT_FOK));

figure(1)
%osztas a latvany miatt
plot(out_t,out/D,'r')
hold on
plot(out_t,signal,'b')
hold off

figure(2)
%szorzo csak a latvany miatt
plot(y,spek_signal,'b');
hold on
plot(y,spek_out,'r');
hold off


