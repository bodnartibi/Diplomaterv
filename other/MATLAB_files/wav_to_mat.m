function [csett] = wav_to_mat(in, out)
% in: input .wav file path
% out: output .mat file path
[csett,fs] = wavread(in);
figure(100)
csett_t= [1:size(csett)];
plot(csett_t,csett,'b')
hold on
csett= transpose(csett);
csett(1,:)=csett_t;
plot(csett_t,csett(2,:),'r');
hold off
save(out,'csett')
csett = transpose(csett(2,:));

