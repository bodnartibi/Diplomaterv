#ifndef HYPER_H
#define HYPER_H

// 331.5 + 0.6*T  m/s
#define SOUND_SPEED 34300  // mm/ms
// 50 MHz counter -> 20 ns
#define TIME_QUANTUM 0.0002 // ms
// position must be in mm

#include <math.h>

int is_timestamps_correct(double pos_1_x, double pos_1_y, \
                          double pos_2_x, double pos_2_y, \
                          unsigned int time_1, unsigned int time_2);

void calc_hyper(double pos_1_x, double pos_1_y, double pos_2_x, double pos_2_y, \
                unsigned int time_1, unsigned int time_2, \
                double* res_x, double* res_y, int res_length, \
                double step, double gain);

int calc_intersection(double* line_1_x, double* line_1_y, \
                      double* line_2_x, double* line_2_y, \
                      int length, double radius, \
                      double* res_x, double* res_y, \
                      int num_max_results, int* num_found_results);

#endif // HYPER_H
