function [csett] = wav_to_mat(in, out)
% in: input .wav file path
% out: output .mat file path
[csett,fs] = wavread(in);

csett = csett;
csett_t = [1:size(csett)];
csett = transpose(csett);
csett(1,:) = csett_t;
save(out,'csett')
csett = transpose(csett(2,:));

