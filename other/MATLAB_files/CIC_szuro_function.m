function [] = CIC_szures_prezi(input_path, D, FFT_FOK)
% input_path: a PDM jelet tartalmaza mat fajl eleresi utja
% D: a CIC szuro fokszama
% FFT_FOK: az FFT fokszama

% input mat fajl beolvasasa
s = load(input_path);
% PDM idotartomany beli jel
PDM_signal = s.ans;
PDM_signal = transpose(PDM_signal(2,:));
t = [0:1:size(PDM_signal(:,1))-1];
% spektrum kiszamitasa
PDM_spektrum = abs(fft(PDM_signal,FFT_FOK));
fft_x = [0:1:FFT_FOK-1];

% CIC szuro atviteli fuggvenyenek osszeallitasa
cic_num = zeros(1,D+1);
cic_num(1) = 1;
cic_num(D+1) = -1;
cic_den = [1 -1];

% szures elvegzese
out = filter(cic_num,cic_den,PDM_signal);
% szurt jel spektruma
out_spektrum = abs(fft(out,FFT_FOK));

% impulzus bemeno jel eloallitasa
imp = zeros(size(PDM_signal));
imp(1) = 1;
% impulzusvalasz eloallitasa
imp_valasz = filter(cic_num,cic_den,imp);
% szuro impulzusvalaszanak atviteli fuggvenye
imp_valasz_spektrum = abs(fft(imp_valasz,FFT_FOK));

figure(1)
plot(t, out, 'r');
legend('Szurt PDM jel hullamformaja');
title('Szurt PDM jel idotartomanyban');

figure(2)
plot(fft_x, PDM_spektrum,'b');
hold on
plot(fft_x, out_spektrum,'r');
plot(fft_x, imp_valasz_spektrum,'g');
hold off
legend('PDM jel spektruma','Szurt PDM jel spektruma','CIC impulzusvalasz spektruma');
title('Spektrumok');




