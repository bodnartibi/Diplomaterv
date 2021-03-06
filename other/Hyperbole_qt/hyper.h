#ifndef HYPER_H
#define HYPER_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct point_ {
    double x;
    double y;
}point;

typedef struct sensor_point_ {
    point p;
    unsigned int time;
}sensor_point;

/*
 * A haromszog kozeppontjanak meghatarozasa
 */
void calc_triangle_middle(sensor_point s_1, \
                         sensor_point s_2, \
                         sensor_point s_3, \
                         point* res);

/*
 * A szenzorok altal kozrefogott haromszog
 * kozeppontjabol a metszespontok iranyaba
 * mutato egysegvektor meghatarozasa
 */
int calc_direction(point* inters, \
                   int length, \
                   point middle, \
                   point* res);

/*
 * Az idobelyegek ervenyessegenek ellenorzese
 */
int is_timestamps_correct(sensor_point s_1, \
                          sensor_point s_2, \
                          sensor_point s_3, \
                          double sound_speed);

/*
 * Hiperbolak pontjainak kiszamolasa
 */
void calc_hyper(sensor_point sensor_1, sensor_point sensor_2, \
                point* res, int res_length, \
                double step, double gain, \
                double sound_speed);

/*
 * Ket hiperbolaiv metszespontjainak meghatarozasa
 */
int calc_intersection(point* line_1, \
                      point* line_2, \
                      int length, double radius, \
                      point* res, \
                      int num_max_results, int* num_found_results);

#endif // HYPER_H
