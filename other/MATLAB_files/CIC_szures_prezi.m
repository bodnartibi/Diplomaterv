D = 50;
%size_input = 10000;
FFT_FOK = 100000; %44200;

%time=[0:1:size_input-1];
%signal = (square(time/500)); %+ 0.5 * rand(1, length(time));
%signal = sawtooth(time/250,1);

s= load('PDM_sin.mat');
signal = s.ans;
signal = transpose(signal(2,:));
spek_signal=abs(fft(signal,FFT_FOK));
spek_y=[0:1:FFT_FOK-1];
out_t = [0:1:size(signal(:,1))-1];

% CIC filter
cic_num = zeros(1,D+1);
cic_num(1) = 1;
cic_num(D+1) = -1;
cic_den = [1 -1];

out = filter(cic_num,cic_den,signal);
spek_out = abs(fft(out,FFT_FOK));

imp_x = zeros(size(signal));
imp_x(1) = 1;

imp_out = filter(cic_num,cic_den,imp_x);
frek_resp = abs(fft(imp_out,FFT_FOK));

figure(1)
%osztas a latvany miatt
plot(out_t,out,'r');
hold on
%plot(out_t,signal,'b');
hold off
%%
figure(2)
%szorzo csak a latvany miatt
plot(spek_y,spek_signal*15,'b');
hold on
plot(spek_y,spek_out,'r');
plot(spek_y,frek_resp*15,'g');
hold off




