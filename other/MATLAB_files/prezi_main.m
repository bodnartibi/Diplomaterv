clear
[csett] = wav_to_mat('csett.wav','csett.mat');

figure(99)
csett_t= [1:size(csett)];

plot(csett_t,csett,'b')

model = 'sigmadelta_csett_to_PDM';
load_system(model)
sim(model)

CIC_szures_prezi

figure(100)
%szorzo csak a latvany miatt
plot(spek_y,spek_signal,'b');
hold on
spek_orig=(abs(fft(csett,FFT_FOK)));
plot(spek_y,spek_orig,'r');
%plot(spek_y,spek_signal-spek_orig,'g');
hold off