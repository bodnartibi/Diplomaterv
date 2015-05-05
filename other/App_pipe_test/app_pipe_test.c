#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char* argv[]){

	int fd_status;
	int fd_mics[3];
	int index;

	unsigned int status = 0x00ffffff;
	unsigned int t_1 = 0x0000000A;
	unsigned int t_2 = 0x00000000;
	unsigned int t_3 = 0x00000014;

	if(argc < 5){
		printf("%s \nUsage: <status register path> <mic 1 register path> <mic 2 register path> <mic 3 register path> <)>\n", argv[0]);
		return 0;
	}
  printf("Start\n");

  printf("Opening status reg: %s\n",argv[1]);
  fd_status = open(argv[1], O_CREAT | O_WRONLY);
  if(fd_status < 0)
  {
    perror("open status reg file");
    return -1;
  }
  
	for(index = 0; index < 3 ; index++){
		printf("Opening mic %d: %s\n",index,argv[index+2]);
	  fd_mics[index] = open(argv[index+2], O_CREAT | O_WRONLY);
	  if(fd_mics[index] < 0){
	    perror("open a mic reg file");
 	   return -1;
 	 }
	}

	write(fd_status,&status,sizeof(status));
	write(fd_mics[0],&t_1,sizeof(t_1));
	write(fd_mics[1],&t_2,sizeof(t_2));
	write(fd_mics[2],&t_3,sizeof(t_3));
	
	printf("Sleep...");
	sleep(10);
	

}
