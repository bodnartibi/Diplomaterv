#include "worker.h"

void* worker_fn(void* arg){

  point* res_1;
  point* res_2;
  point* res_3;

  point* inter;
  point middle;
  point direction;

  int num_inter;
  int size = 1000;
  unsigned int times[3];
  int t_ready[3] = {0,0,0};
  int index = 0;
  int valid;

  double cross_threshold;

  calc_triangle_middle(sen_1, sen_2, sen_3, &middle);

  while(1){

    pthread_mutex_lock(&mutex_common);
    pthread_cond_wait (&cond_common, &mutex_common);
    pthread_mutex_unlock(&mutex_common);

    for (index = 0; index < 3; index++){
      if(list_is_valid(index) && !t_ready[index]){
        times[index] = list_get_time(index, &valid);
        t_ready[index] = 1;
      }
    }

    if(!(t_ready[0] && t_ready[1] && t_ready[2]))
      continue;

    sen_1.time = times[0];
    sen_2.time = times[1];
    sen_3.time = times[2];

    if( !is_timestamps_correct(sen_1, sen_2, sen_3, sound_speed))
    {
        printf("Bad datas, dropping timestamps\n");
        t_ready[0] = 0;
        t_ready[1] = 0;
        t_ready[2] = 0;
        clear_lists();
        continue;
    }

    printf("Start to calculate\n");
    printf("Times: %u %u %u\n", times[0], times[1], times[2]);
    res_1 = (point*)malloc(sizeof(point)*size);
    res_2 = (point*)malloc(sizeof(point)*size);
    res_3 = (point*)malloc(sizeof(point)*size);

    calc_hyper(sen_1, sen_2, \
               res_1, size, 0.5,1.005, sound_speed);

    calc_hyper(sen_2, sen_3, \
               res_2, size, 0.5,1.005, sound_speed);

// eleg ket hiperbolat kiszamitani
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
      calc_intersection(res_1, \
                        res_2, \
                        size, cross_threshold, \
                        inter, \
                        size, &num_inter);
      printf("%d points were found, radius: %f \n", num_inter, cross_threshold);

      if(num_inter > 0)
        break;
// eles ket hiperbola metszespontjat keresni
/*
      calc_intersection(res_2, \
                        res_3, \
                        size, cross_threshold, \
                        inter, \
                        size, &num_inter);

      printf("%d point find, thres: %f \n", num_inter, cross_threshold);

      if(num_inter > 0)
        break;

      calc_intersection(res_1, \
                        res_3, \
                        size, cross_threshold, \
                        inter, \
                        size, &num_inter);

      printf("%d point find, thres: %f \n", num_inter, cross_threshold);

      if(num_inter > 0)
        break;
*/
      cross_threshold = cross_threshold + 1;
    }

    printf("Intersections: \n");
    for(index = 0; index < num_inter; index++){
      printf(" %d %d \n", (int)((inter +index)->x+0.5), (int)((inter +index)->y+0.5));
    }

    calc_direction(inter, num_inter, middle, &direction);

    printf("Direction: %f %f\n", direction.x, direction.y);
    t_ready[0] = 0;
    t_ready[1] = 0;
    t_ready[2] = 0;
  }

  free(res_1);
  free(res_2);
  free(res_3);
  free(inter);
}
