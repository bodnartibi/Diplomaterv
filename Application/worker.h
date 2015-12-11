#ifndef WORKER_H
#define WORKER_H

#include <pthread.h>
#include <stdio.h>
#include "hyper.h"
#include "list.h"

#define SIZE 2000
#define STEP 0.5
#define GAIN 1.005

// Ha mindharom hiperbolat szeretnenk szamolni
//#define THREE_HYP

pthread_mutex_t mutex_common;
pthread_cond_t cond_common;
sensor_point sen_1, sen_2, sen_3;
double sound_speed;

/*
 * A hiperbolak szamolasaert felelos fuggveny.
 * A cond_common feltetelre varakozik,
 * ha arra jelzes erkezik a list.h fuggvenyeit hasznalja,
 * a listakbol idopontokat olvas ki.
 * A kozos valtozok ertekeit be kell allitani a hivas elott.
 */
void* worker_fn(void* arg);

#endif //WORKER_H
