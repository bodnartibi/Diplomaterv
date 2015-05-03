#ifndef HYPER_H
#define HYPER_H

#define SOUND_SPEED 3.430

#include <math.h>

void calc_hyper(double pos_1_x, double pos_1_y, double pos_2_x, double pos_2_y, \
                long int time_1, long int time_2, \
                double* res_x, double* res_y, int res_length, \
                double step, double gain);

int calc_intersection(double* line_1_x, double* line_1_y, \
                      double* line_2_x, double* line_2_y, \
                      int length, double radius, \
                      double* res_x, double* res_y, \
                      int num_max_results, int* num_found_results);

#endif // HYPER_H
