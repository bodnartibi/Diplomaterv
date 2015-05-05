#define _GNU_SOURCE
#include <sched.h>

#include <stdio.h>
#include <math.h>
#include "hyper.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include <pthread.h>

#define MIC_1_READY_MASK 0x00ff0000
#define MIC_2_READY_MASK 0x0000ff00
#define MIC_3_READY_MASK 0x000000ff

#define ALL_MIC_READY (MIC_1_READY_MASK | MIC_2_READY_MASK | MIC_3_READY_MASK)

int main(int argc, char* argv[]){

  double* res_x_1;
  double* res_y_1;
  double* res_x_2;
  double* res_y_2;
  double* res_x_3;
  double* res_y_3;

	double sen_1_x, sen_1_y;
	double sen_2_x, sen_2_y;
	double sen_3_x, sen_3_y;

	unsigned int times[3];

  double* inter_x;
  double* inter_y;

  int num_inter;
	int index;
	int size = 1000;

	int print_points = 0;

	int fd_status;
	int fd_mics[3];
  char buf[256];
  int len;
  int fdix;

	int status_reg_value;

	//unsigned ez1 = 1, ez2 = 2;
	//int az;

	cpu_set_t cpuset;
	pthread_t current_thread;


	//az = ez1- ez2;
	//printf("Start %d\n", az);

	//return 0;

	if(argc < 6){
		printf("%s \nUsage: <status register path> <mic 1 register path> <mic 2 register path> <mic 3 register path> <print points (y/n)>\n", argv[0]);
		return 0;
	}
  printf("Start\n");

	if(*argv[5] == 'y')
		print_points = 1;

  printf("Opening status reg: %s\n",argv[1]);
  fd_status = open(argv[1], O_RDONLY | O_NONBLOCK);
  if(fd_status < 0)
  {
    perror("open status reg file");
    return -1;
  }
  
	for(index = 0; index < 3 ; index++){
		printf("Opening mic %d: %s\n",index,argv[index+2]);
	  fd_mics[index] = open(argv[index+2], O_RDONLY | O_NONBLOCK);
	  if(fd_mics[index] < 0){
	    perror("open a mic reg file");
 	   return -1;
 	 }
	}

	while(1){

		len = read(fd_status, buf, sizeof(buf));
		//printf("Read len: %d\n",len);
    if((len < 0) && (errno != EAGAIN)){
      perror("read");
      return EXIT_FAILURE;
    }
		if((len < 0) && (errno == EAGAIN)){
			//printf("Sleep\n");
			sleep(1);
			continue;
		}
    if(len == 0){
      printf("Status reg file has been closed\n");
      break;
    }

		buf[len] = 0;
		//printf("Size: %lu\n %s",sizeof(status_reg_value),buf);
		memcpy(&status_reg_value, buf, sizeof(status_reg_value));
		//snprintf((char*)(&status_reg_value), sizeof(status_reg_value), buf);
		printf("Read value status reg: %x\n",status_reg_value);
		//sleep(1);

		// elivleg a x86, x64 és az ARM is little endian

		// ----------------------------------------
		// Status Reg
		// [ reserved ] [ mic1 status] [ mic2 status] [ mic3 status ]
		// ha a mic ready akkor 0xff amugy 0x00
		//

		if(!(status_reg_value && ALL_MIC_READY)){
			continue;
		}

		for(index = 0; index < 3; index++){
			len = read(fd_mics[index], buf, sizeof(buf));
			// itt hibanak vesszuk ha nincs mit kiolvasni, hiszen ugy tudjuk h van
    	if(len < 0){
      	perror("read");
      	return EXIT_FAILURE;
    	}
			memcpy(&times[index], buf, sizeof(status_reg_value));
			printf("Read value mic %d reg: %x\n",index + 1, times[index]);

		}
	
//TODO close files

		size = 1000;

	  res_x_1 = (double*)malloc(sizeof(double)*size);
	  res_y_1 = (double*)malloc(sizeof(double)*size);
	  res_x_2 = (double*)malloc(sizeof(double)*size);
	  res_y_2 = (double*)malloc(sizeof(double)*size);
	  res_x_3 = (double*)malloc(sizeof(double)*size);
	  res_y_3 = (double*)malloc(sizeof(double)*size);
	
		sen_1_x = 0.0;
		sen_1_y = 0.0;
		sen_2_x = 150.0;
		sen_2_y = 0.0;
		sen_3_x = 0.0;
		sen_3_y = 150.0;

		//times[0] = 20;
		//times[1] = 0;
		//times[2] = 10;
	
   
   //CPU_ZERO(&cpuset);
   //CPU_SET(1, &cpuset);

   //current_thread = pthread_self();    
   //pthread_setaffinity_np(current_thread, sizeof(cpu_set_t), &cpuset);
	
		//while(1){
	
	  calc_hyper(sen_1_x,sen_1_y,sen_2_x,sen_2_y,times[0],times[1],\
	             res_x_1,res_y_1, size, 0.05,1.01);
	
	  calc_hyper(sen_2_x,sen_2_y,sen_3_x,sen_3_y,times[1],times[2],\
	             res_x_2,res_y_2, size, 0.05,1.01);
	
	  calc_hyper(sen_3_x,sen_3_y,sen_1_x,sen_1_y,times[2],times[0],\
	             res_x_3,res_y_3, size, 0.05,1.01);
	
		if(print_points) {
			printf("Pontok: %d \n", size);
		  for(index = 0; index < size; index++){
		    printf("%d --- %d\t%d\t%d\t%d\t%d\t%d\n", index,
							(int)(*(res_x_1 +index)+0.5), (int)(*(res_y_1 +index)+0.5),
							(int)(*(res_x_2 +index)+0.5), (int)(*(res_y_2 +index)+0.5),
							(int)(*(res_x_3 +index)+0.5), (int)(*(res_y_3 +index)+0.5));
		  }
		}

	  inter_x = (double*)malloc(sizeof(double)*size);
	  inter_y = (double*)malloc(sizeof(double)*size);
	
	
	  calc_intersection(res_x_1, res_y_1, \
	                    res_x_2, res_y_2, \
	                    size, 1.0, \
	                    inter_x, inter_y, \
	                    size, &num_inter);
	
	  calc_intersection(res_x_2, res_y_2, \
	                    res_x_3, res_y_3, \
	                    size, 1.0, \
	                    inter_x, inter_y, \
	                    size, &num_inter);
	
	  calc_intersection(res_x_1, res_y_1, \
	                    res_x_3, res_y_3, \
	                    size, 1.0, \
	                    inter_x, inter_y, \
	                    size, &num_inter);



		printf("Metszespontok: %d \n", num_inter);
	  for(index = 0; index < num_inter; index++){
	    printf(" %d %d \n", (int)(*(inter_x +index)+0.5), (int)(*(inter_y +index)+0.5));
	  }


	  free(res_x_1);
	  free(res_y_1);
	  free(res_x_2);
	  free(res_y_2);
	  free(res_x_3);
	  free(res_y_3);

		free(inter_x);
		free(inter_y);
	}

	return 0;
}
