#ifndef HYPER_H
#define HYPER_H

// 331.5 + 0.6*T  m/s
// 343.00 mm/ms
// 0.3430 mm/us
// 50 MHz counter -> 20 ns
#define SOUND_SPEED 0.00686 // mm/20ns
//#define TIME_QUANTUM 0.02 // us
// position must be in mm

#include <math.h>

typedef struct point_ {
    double x;
    double y;
}point;

typedef struct sensor_point_ {
    point p;
    unsigned int time;
}sensor_point;


int is_timestamps_correct(sensor_point s_1, \
                          sensor_point s_2);

void calc_hyper(sensor_point sensor_1, sensor_point sensor_2, \
                point* res, int res_length, \
                double step, double gain);

int calc_intersection(point* line_1, \
                      point* line_2, \
                      int length, double radius, \
                      point* res, \
                      int num_max_results, int* num_found_results);

#endif // HYPER_H
