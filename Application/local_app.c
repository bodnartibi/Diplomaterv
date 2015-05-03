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

int main(int argc, char* argv[])
{
  double* res_x_1;
  double* res_y_1;
  double* res_x_2;
  double* res_y_2;
  double* res_x_3;
  double* res_y_3;

	double sen_1_x, sen_1_y;
	double sen_2_x, sen_2_y;
	double sen_3_x, sen_3_y;

	long int t_1, t_2, t_3;

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

	cpu_set_t cpuset;
	pthread_t current_thread;

	if(argc < 5){
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
		printf("Size: %lu\n %s",sizeof(status_reg_value),buf);
		memcpy(&status_reg_value, buf, sizeof(status_reg_value));
		//snprintf((char*)(&status_reg_value), sizeof(status_reg_value), buf);
		printf("Read value: %x\n",status_reg_value);
		//sleep(1);

	}
  
/*
  fdix = 0;
  while(1)
  {
    len = read(fds[fdix], buf, sizeof(buf));
    if(len == 0)
    {
      printf("A pipe%d lezarult!\n", fdix + 1);
      break;
    }
    else if(len < 0)
    {
      perror("read");
      return -1;
    }
    else
    {
			//printf("Read: %s\n",buf);
      write(STDOUT_FILENO, buf, len);
    }
    fdix = (fdix + 1) % 2;
  }
  
  close(fds[0]);
  close(fds[1]);
*/
	
//TODO close files

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

	t_1 = 20;
	t_2 = 0;
	t_3 = 10;

   
   CPU_ZERO(&cpuset);
   CPU_SET(1, &cpuset);

   current_thread = pthread_self();    
   pthread_setaffinity_np(current_thread, sizeof(cpu_set_t), &cpuset);


	


	//while(1){

  calc_hyper(sen_1_x,sen_1_y,sen_2_x,sen_2_y,t_1,t_2,\
             res_x_1,res_y_1, size, 0.05,1.01);

  calc_hyper(sen_2_x,sen_2_y,sen_3_x,sen_3_y,t_2,t_3,\
             res_x_2,res_y_2, size, 0.05,1.01);

  calc_hyper(sen_3_x,sen_3_y,sen_1_x,sen_1_y,t_3,t_1,\
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

	//}


	return 0;
}
