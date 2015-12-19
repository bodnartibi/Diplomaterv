#include "worker.h"

void* worker_fn(void* arg){

  point res_1[SIZE];
  point res_2[SIZE];
#ifdef THREE_HYP
  point res_3[SIZE];
#endif

  point inter[SIZE];
  point middle;
  point direction;

  int num_inter;
  unsigned int times[3];
  int t_ready[3] = {0,0,0};
  int index = 0;
  int valid;

  double cross_threshold;

  clock_t t_h_1, t_h_2, t_i_1, t_i_2;

  calc_triangle_middle(sen_1, sen_2, sen_3, &middle);

  while(1) {

    // Varakozas a masik szal jelzesere
    pthread_mutex_lock(&mutex_common);
    pthread_cond_wait (&cond_common, &mutex_common);
    pthread_mutex_unlock(&mutex_common);

    // Megnezzuk a listakat
    for (index = 0; index < 3; index++) {
      // Ha van uj elem a listaban,
      // es meg nem kertunk elemet ehhez a szamolashoz
      if(list_is_valid(index) && !t_ready[index]) {
        times[index] = list_get_time(index, &valid);
        t_ready[index] = 1;
      }
    }

    // Ha meg nem all mind a 3 adat rendelkezesre
    if(!(t_ready[0] && t_ready[1] && t_ready[2]))
      continue;

    // A poziciokat mar a masik szal kitoltotte, mi csak az idot adjuk hozza
    sen_1.time = times[0];
    sen_2.time = times[1];
    sen_3.time = times[2];

    // Ha nem tudunk hiperbolakat szamolni, uritjuk a listakat
    if( !is_timestamps_correct(sen_1, sen_2, sen_3, sound_speed)) {
        printf("Bad datas, dropping timestamps\n");
        t_ready[0] = 0;
        t_ready[1] = 0;
        t_ready[2] = 0;
        clear_lists();
        continue;
    }

    printf("Start to calculate\n");
    printf("Times: %u %u %u\n", times[0], times[1], times[2]);
    t_h_1 = clock();

    calc_hyper(sen_1, sen_2, \
               res_1, SIZE, STEP, GAIN, sound_speed);

    calc_hyper(sen_2, sen_3, \
               res_2, SIZE, STEP, GAIN, sound_speed);

#ifdef THREE_HYP
    calc_hyper(sen_3, sen_1, \
               res_3, SIZE, STEP, GAIN, sound_speed);
#endif
    t_h_2 = clock();
    printf("Hyperboles has been calculated, time: %f sec\n", (float)(t_h_2 - t_h_1)/CLOCKS_PER_SEC);
    t_i_1 = clock();
    cross_threshold = 1.01;
    while(1) {
      calc_intersection(res_1, \
                        res_2, \
                        SIZE, cross_threshold, \
                        inter, \
                        SIZE, &num_inter);
      printf("%d points were found, radius: %f \n", num_inter, cross_threshold);

      if(num_inter > 0)
        break;

#ifdef THREE_HYP
      calc_intersection(res_2, \
                        res_3, \
                        SIZE, cross_threshold, \
                        inter, \
                        SIZE, &num_inter);

      printf("%d points were found, radius: %f \n", num_inter, cross_threshold);

      if(num_inter > 0)
        break;

      calc_intersection(res_1, \
                        res_3, \
                        SIZE, cross_threshold, \
                        inter, \
                        SIZE, &num_inter);

      printf("%d points were found, radius: %f \n", num_inter, cross_threshold);

      if(num_inter > 0)
        break;
#endif
      t_i_2 = clock();
      printf("Duration of intersection calulation: %f sec\n", (float)(t_i_2 - t_i_1)/CLOCKS_PER_SEC);
      // Ha nem talaltunk metszespontot,
      // akkor noveljuk a megengedheto tavolsagot
      cross_threshold = cross_threshold + 1;
    }

    printf("Intersections: \n");
    for(index = 0; index < num_inter; index++) {
      printf(" %d %d \n", (int)(inter[index].x+0.5), (int)(inter[index].y+0.5));
    }

    calc_direction(inter, num_inter, middle, &direction);

    printf("Direction: %f %f\n", direction.x, direction.y);
    t_ready[0] = 0;
    t_ready[1] = 0;
    t_ready[2] = 0;
  }
}
