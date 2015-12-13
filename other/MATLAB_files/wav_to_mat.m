function [csett] = wav_to_mat(in, out, gain)
% in: input .wav file path
% out: output .mat file path
% gain: amplitude gain
[csett,fs] = wavread(in);

csett = csett*gain;
csett_t = [1:size(csett)];
csett = transpose(csett);
csett(1,:) = csett_t;
save(out,'csett')
csett = transpose(csett(2,:));

