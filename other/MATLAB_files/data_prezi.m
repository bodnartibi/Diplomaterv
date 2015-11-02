fileID = fopen('data1.txt','r');
formatSpec = '%x';
A = fscanf(fileID,formatSpec);
t = transpose([0:size(A)-1]);
plot(t,A)