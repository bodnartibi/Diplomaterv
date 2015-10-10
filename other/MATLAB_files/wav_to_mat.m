[csett,fs] = wavread('csett.wav');
figure(100)
csett_t= [1:size(csett)];
plot(csett_t,csett,'b')
hold on
csett= transpose(csett);
csett(1,:)=csett_t;
plot(csett_t,csett(2,:),'r');
hold off
save('csett.mat','csett')
