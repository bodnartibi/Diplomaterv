#define _GNU_SOURCE
#include <sched.h>

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include <sys/select.h>

#define TRUE 1
#define FALSE 0


int main(int argc, char* argv[]){

  int index;
  int maxfd;
  double sound_dist[3];
  char buf[256];
  int len;
  int fdix;
  unsigned int times[3];
  int fd_mics[3];
  int t_ready[3];
  double speed;

  fd_set watchset;
  fd_set inset;

  if(argc < 5){
    printf("%s \nUsage: \n<mic 1 register path> <mic 2 register path> <mic 3 register path> <speed>\n", argv[0]);
    return 0;
  }
  printf("Start\n");

  speed = atof (argv[4]);

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
    if(select(maxfd + 1, &inset, NULL, NULL, NULL) < 0){
      perror("select");
      return EXIT_FAILURE;
    }
    // TODO fajl lezarast le kell kezelni!!

    for (index = 0; index < 3; index++){
      if(FD_ISSET(fd_mics[index], &inset)){
        len = read(fd_mics[index], buf, sizeof(buf));
        // itt hibanak vesszuk ha nincs mit kiolvasni, hiszen ugy tudjuk h van
        if(len < 0){
          perror("read");
          return EXIT_FAILURE;
        }
        memcpy(&times[index], buf, sizeof(times[0]));
        t_ready[index] = 1;
        printf("Read value mic %d reg: hex %x dec %u\n",index + 1, times[index], times[index]);
      }
    }

    if(!(t_ready[0] && t_ready[1] && t_ready[2]))
        continue;

    // 0: a-b
    sound_dist[0] = abs(times[0] - times[1])*speed;
    // 1: a-c
    sound_dist[1] = abs(times[1] - times[2])*speed;
    // 2: b-c
    sound_dist[2] = abs(times[0] - times[2])*speed;

    printf("1.- 2. : %f\n",sound_dist[0]);
    printf("2.- 3. : %f\n",sound_dist[1]);
    printf("1.- 3. : %f\n",sound_dist[2]);

    t_ready[0] = t_ready[1] = t_ready[2] = 0;
  }

  return 0;
}
