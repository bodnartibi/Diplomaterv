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
#include <sys/select.h>

#define TRUE 1
#define FALSE 0

pthread_mutex_t mutex[3];
pthread_mutex_t mutex_common;
pthread_cond_t cond_common;
pthread_t worker_thread;

typedef struct time_elem_t {
  struct time_elem_t* next;
  struct time_elem_t* prev;
  unsigned int time;
} time_elem;

time_elem* time_list_heads[3] = {NULL,NULL,NULL};
time_elem* time_list_tails[3] = {NULL,NULL,NULL};

void add_time(int which, unsigned int time){
  pthread_mutex_lock(&mutex[which]);
  time_elem* new;

  printf("Add_time\n");
  new = malloc(sizeof(time_elem));
  new->next= NULL;
  new->time = time;
  if(!time_list_heads[which]){
    new->prev = NULL;
    time_list_heads[which] = new;
    time_list_tails[which] = time_list_heads[which];
  }
  else{
    time_list_tails[which]->next = new;
    new->prev = time_list_tails[which];
  }
  time_list_tails[which] = new;
  pthread_mutex_unlock(&mutex[which]);
}

unsigned int get_time(int which, int* valid){
  time_elem* act;
  unsigned int ret;

  printf("Get_time\n");
  pthread_mutex_lock(&mutex[which]);
  if(!time_list_heads[which]){
    *valid = FALSE;
    return 0;
  }
  act = time_list_heads[which];
  time_list_heads[which] = time_list_heads[which]->next;
  if(!time_list_heads[which])
    time_list_tails[which] = NULL;
  ret = act->time;
  free(act);
  pthread_mutex_unlock(&mutex[which]);
  *valid = TRUE;
  return ret;
}

int is_valid(int which){
  int ret;
  printf("Is_valid\n");
  pthread_mutex_lock(&mutex[which]);
  if (time_list_heads[which])
    ret = TRUE;
  else
    ret = FALSE;
  pthread_mutex_unlock(&mutex[which]);
  return ret;
}

void* worker_fn(void* arg){

  point* res_1;
  point* res_2;
  point* res_3;

  sensor_point sen_1, sen_2, sen_3;
  point* inter;

  int num_inter;
  int size = 1000;
  unsigned int times[3];
  int t_ready[3] = {0,0,0};
  int index = 0;
  int valid;

  double cross_threshold;

  while(1){

    pthread_mutex_lock(&mutex_common);
    pthread_cond_wait (&cond_common, &mutex_common);
    pthread_mutex_unlock(&mutex_common);

    for (index = 0; index < 3; index++){
      if(is_valid(index) && !t_ready[index]){
        times[index] = get_time(index, &valid);
        t_ready[index] = 1;
      }
    }

    if(!(t_ready[0] && t_ready[1] && t_ready[2]))
      continue;

    printf("Start to calculate\n");
    printf("Times: %u %u %u\n", times[0], times[1], times[2]);
    res_1 = (point*)malloc(sizeof(point)*size);
    res_2 = (point*)malloc(sizeof(point)*size);
    res_3 = (point*)malloc(sizeof(point)*size);

    sen_1.p.x = 0.0;
    sen_1.p.y = 0.0;
    sen_2.p.x = 150.0;
    sen_2.p.y = 0.0;
    sen_3.p.x = 0.0;
    sen_3.p.y = 150.0;

    sen_1.time = times[0];
    sen_2.time = times[1];
    sen_3.time = times[2];
/*
void calc_hyper(sensor_point sensor_1, sensor_point sensor_2, \
                point* res, int res_length, \
                double step, double gain);
*/
    calc_hyper(sen_1, sen_2, \
               res_1, size, 0.05,1.01);

    calc_hyper(sen_2, sen_3, \
               res_2, size, 0.05,1.01);

//    calc_hyper(sen_3, sen_1, \
//               res_3, size, 0.05,1.01);

    inter = (point*)malloc(sizeof(point)*size);
/*
      printf("Pontok: %d \n", size);
      for(index = 0; index < size; index++){
        printf("%d --- %d\t%d\t%d\t%d\n", index, //t%d\t%d\n", index,
              (int)((res_1 +index)->x+0.5), (int)((res_1 +index)->y+0.5),
              (int)((res_2 +index)->x+0.5), (int)((res_2 +index)->y+0.5),
//              (int)((res_3 +index)->x+0.5), (int)((res_3 +index)->y+0.5),
              );
      }
*/
    cross_threshold = 0.01;
    while(1) {
      cross_threshold = cross_threshold + 0.01;
      calc_intersection(res_1, \
                        res_2, \
                        size, cross_threshold, \
                        inter, \
                        size, &num_inter);
      printf("%d point find, thres: %f \n", num_inter, cross_threshold);

      if(num_inter == 0)
        continue;
/*
      calc_intersection(res_2, \
                        res_3, \
                        size, cross_threshold, \
                        inter, \
                        size, &num_inter);

      printf("%d point find, thres: %f \n", num_inter, cross_threshold);

      if(num_inter == 0)
        continue;

      calc_intersection(res_1, \
                        res_3, \
                        size, cross_threshold, \
                        inter, \
                        size, &num_inter);

      printf("%d point find, thres: %f \n", num_inter, cross_threshold);

      if(num_inter == 0)
        continue;
*/
      break;
    }

    printf("Metszespontok: %d \n", num_inter);
    for(index = 0; index < num_inter; index++){
      printf(" %d %d \n", (int)((inter +index)->x+0.5), (int)((inter +index)->y+0.5));
    }

    t_ready[0] = 0;
    t_ready[1] = 0;
    t_ready[2] = 0;
  }

  free(res_1);
  free(res_2);
  free(res_3);
  free(inter);
}

int main(int argc, char* argv[]){

  int index;
  int maxfd;
  int fd_mics[3];
  char buf[256];
  int len;
  int fdix;
  unsigned int times[3];

  fd_set watchset;
  fd_set inset;

  if(argc < 4){
    printf("%s \nUsage: <mic 1 register path> <mic 2 register path> <mic 3 register path>\n", argv[0]);
    return 0;
  }
  printf("Start\n");

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
        printf("Read value mic %d reg: hex %x dec %u\n",index + 1, times[index], times[index]);

        add_time(index, times[index]);

        pthread_mutex_lock(&mutex_common);
        pthread_cond_signal (&cond_common);
        pthread_mutex_unlock(&mutex_common);
      }
    }


    //TODO close files

  }

  return 0;
}
