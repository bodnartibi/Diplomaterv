clear

% bemeneti wav fajl eleresi utja
wav_file_path = 'csett.wav';
% kimeneti mat fajl eleresi utja
mat_file_path = 'csett.mat';
% simulinkes rendszer neve
model = 'sigmadelta_csett_to_PDM';
% Spektrumok szamolasanal az FFT fokszama
FFT_FOK = 10000;
% CIC szuro fokszama
D = 10;
% simulinkes rendszer kimeneti fajlja
simulink_file_path = 'PDM_sin.mat';

% wav fajl tartalmanak mentese mat fajlba
[csett] = wav_to_mat_function(wav_file_path,mat_file_path,10);

% wav fajlban talalhato hang hullamformaja
csett_t = [1:size(csett)];
figure(100)
plot(csett_t,csett,'b');
legend('A felvett jel idotartomanyban');
title('A felvett jel hullamformaja');

% simulinkes rendszer betoltese es futtatasa
load_system(model)
sim(model)

% PDM jel szuresenek elvegzese, eredmenyek megjelenitese
CIC_szuro_function(simulink_file_path, D, FFT_FOK);

% mat fajl beolvasasa
s = load(simulink_file_path);
% PDM idotartomany beli jel
PDM_signal = s.ans;
PDM_signal = transpose(PDM_signal(2,:));

figure(101)
fft_x = [0:1:FFT_FOK-1];
% modulalt jel spektruma
spek_signal = abs(fft(PDM_signal,FFT_FOK));
plot(fft_x,spek_signal,'b');
hold on
% eredeti jel spektruma
spek_orig=(abs(fft(csett,FFT_FOK)));
plot(fft_x,spek_orig,'r');
hold off
legend('A modulalt jel spektruma','A felvett, eredeti jel spektruma');
title('A felvett jel, es a modulalt jel spektruma');