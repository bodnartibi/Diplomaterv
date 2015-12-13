clear
M0=textread('logsys_3MIC_002_MIC0_128_6_16.txt','%8c');
M1=textread('logsys_3MIC_002_MIC1_128_6_16.txt','%8c');
M2=textread('logsys_3MIC_002_MIC2_128_6_16.txt','%8c');

M0=hex2dec(char(M0));
M1=hex2dec(char(M1));
M2=hex2dec(char(M2));

t0 = transpose([1:size(M0)]);
t1 = transpose([1:size(M1)]);
t2 = transpose([1:size(M2)]);

figure(6)

plot(t0,M0,'r')
hold on
plot(t1,M1,'g')
plot(t2,M2,'b')
hold off

