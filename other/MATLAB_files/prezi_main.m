clear
[csett] = wav_to_mat('csett.wav','csett.mat');

figure(100)
csett_t= [1:size(csett)];
plot(csett_t,csett,'b')

model = 'sigmadelta_csett_to_PDM';
load_system(model)
sim(model)

CIC_szures_prezi