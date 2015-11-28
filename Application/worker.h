#ifndef WORKER_H
#define WORKER_H

#include <pthread.h>
#include "hyper.h"

pthread_mutex_t mutex_common;
pthread_cond_t cond_common;
sensor_point sen_1, sen_2, sen_3;
double sound_speed;

void* worker_fn(void* arg);

#endif //WORKER_H
