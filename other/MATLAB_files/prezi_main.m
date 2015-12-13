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
[csett] = wav_to_mat(wav_file_path,mat_file_path,10);

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
[spek_y, spek_signal] = CIC_szures_prezi(simulink_file_path, D, FFT_FOK);

figure(101)
%szorzo csak a latvany miatt
plot(spek_y,spek_signal,'b');
hold on
spek_orig=(abs(fft(csett,FFT_FOK)));
plot(spek_y,spek_orig,'r');
hold off
legend('Szurt PDM jel spektruma','A felvett, eredeti jel spektruma');
title('A felvett jel, es a szurt PDM jel spektruma');