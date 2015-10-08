[signal,fs] = wavread('csett.wav');
signal= transpose(signal)
sig_s= size(signal)
sig_s= sig_s(2)
signal(1,:)=[1:1:sig_s];
save('csett.mat','signal')
