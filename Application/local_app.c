#define _GNU_SOURCE
#include <sched.h>

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include <pthread.h>
#include <sys/select.h>

#include "hyper.h"
#include "list.h"
#include "worker.h"


pthread_t worker_thread;

int main(int argc, char* argv[]){

  int index;
  int maxfd;
  int fd_mics[3];
  char buf[256];
  int len;
  int fdix;
  unsigned int times[3];
  int ret;

  fd_set watchset;
  fd_set inset;

  if(argc < 11){
    printf("%s \nUsage: \n<mic 1 register path> <mic 2 register path> <mic 3 register path>\n", argv[0]);
    printf("<mic 1 X> <mic 1 Y> <mic 2 X> <mic 2 Y> <mic 3 X> <mic 3 Y>\n <sound speed (mm/timequantum)>\n");
    return 0;
  }
  printf("Start\n");

  sen_1.p.x = atof (argv[4]);
  sen_1.p.y = atof (argv[5]);
  sen_2.p.x = atof (argv[6]);
  sen_2.p.y = atof (argv[7]);
  sen_3.p.x = atof (argv[8]);
  sen_3.p.y = atof (argv[9]);
  sound_speed = atof (argv[10]);

  printf("Coordinates in order:\n%f %f\n%f %f\n%f %f\n", \
          sen_1.p.x, sen_1.p.y, \
          sen_2.p.x, sen_2.p.y, \
          sen_3.p.x, sen_3.p.y);

  pthread_mutex_init(&mutex[0], NULL);
  pthread_mutex_init(&mutex[1], NULL);
  pthread_mutex_init(&mutex[2], NULL);
  pthread_mutex_init(&mutex_common, NULL);
  pthread_cond_init(&cond_common, NULL);

  if(pthread_create(&worker_thread, NULL, worker_fn, NULL)){
    fprintf(stderr,"Hiba a szal letrehozasaban.\n");
    exit(EXIT_FAILURE);
  }

  for(index = 0; index < 3 ; index++){
    printf("Opening mic %d: %s\n",index,argv[index+1]);
    fd_mics[index] = open(argv[index+1], O_RDONLY | O_NONBLOCK);
    if(fd_mics[index] < 0){
      perror("open a mic reg file");
      return -1;
    }
  }

  FD_ZERO(&watchset);
  maxfd = 0;
  for (index =0; index < 3; index++){
    FD_SET(fd_mics[index], &watchset);
    if(fd_mics[index] > maxfd)
      maxfd = fd_mics[index];
  }

  while(1){

    inset = watchset;
    ret = select(maxfd + 1, &inset, NULL, NULL, NULL);
    if (ret < 0){
      perror("select");
      return EXIT_FAILURE;
    }

    for (index = 0; index < 3; index++){
      if(FD_ISSET(fd_mics[index], &inset)){
        len = read(fd_mics[index], buf, sizeof(buf));
        // Itt hibanak vesszuk ha nincs mit kiolvasni, hiszen ugy tudjuk h van
        if(len < 0){
          perror("read");
          return EXIT_FAILURE;
        }
        if(len == 0){
          printf("File was closed, exiting\n");
          return 0;
        }
        memcpy((void*)&times[index], (void*)buf, sizeof(times[index]));
        printf("Read value mic %d reg: hex %x dec %u\n",index + 1, times[index], times[index]);

        list_add_time(index, times[index]);

        pthread_mutex_lock(&mutex_common);
        pthread_cond_signal (&cond_common);
        pthread_mutex_unlock(&mutex_common);
      }
    }

  }

  return 0;
}
