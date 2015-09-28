#include "hyper.h"

// remove
#include <stdio.h>

int is_timestamps_correct(double pos_1_x, double pos_1_y, \
                          double pos_2_x, double pos_2_y, \
                          unsigned int time_1, unsigned int time_2)
{
    int sound_dist;
    int sensor_dist;

    sensor_dist = distance = sqrt(pow(pos_1_x - pos_2_x, 2.0) + pow(pos_1_y - pos_2_y, 2.0));
    sound_dist = abs(time_1 - time_2)*SOUND_SPEED;

    if(sound_dist > sensor_dist)
        return 0;

    return 1;
}

int calc_intersection(double* line_1_x, double* line_1_y, \
                      double* line_2_x, double* line_2_y, \
                      int length, double radius, \
                      double* res_x, double* res_y, \
                      int num_max_results, int* num_found_results)
{
    int i,j;
    double distance;
    int res_num;

    res_num = 0;
    for(i = 0; i < length; i++)
    {
        for(j = 0; j < length; j++)
        {
            distance = sqrt(pow(*(line_1_x + i) - *(line_2_x + j), 2.0) + \
                            pow(*(line_1_y + i) - *(line_2_y + j), 2.0));
            if(distance < radius)
            {
                // ket pont felezopontjat adjuk meg eredmenynek
                *(res_x + res_num) = (*(line_1_x + i) + *(line_2_x + j))/2;
                *(res_y + res_num) = (*(line_1_y + i) + *(line_2_y + j))/2;

                res_num ++;

                if(res_num >= num_max_results)
                {
                    break;
                }

            }
        }

        if(res_num >= num_max_results)
        {
            break;
        }
    }

    *num_found_results = res_num;

    return 0;
}


void calc_hyper(double pos_1_x, double pos_1_y, double pos_2_x, double pos_2_y, \
                unsigned int time_1,unsigned int time_2, \
                double* res_x, double* res_y, int res_length,\
                double step, double gain
                )
{
    // hangsebesseg
    const double speed = SOUND_SPEED;
    // ket szenzor tavolsaga
    double distance;
    // felezopont koordinatai
    double middle_x, middle_y;
    // forgatas szoge
    double angle;

    // forgatasi szoghoz
    double vec_x, vec_y;

    // amekkora utat a hang megtett ennyi ido alatt
    double sound_distance;

    // nyers hiperbolak szamitasahoz
    double x;
    double y;
    double a;
    double b;
    double coslamd;
    double s;
    double cosfi;
    double fi;

    // forgatashoz es eltolashoz
    // pont forgatasa
    double rot_x_1;
    double rot_y_1;

    // hiperbola masik fele ugyanaz csak y koordinata az inverze
    double rot_x_2;
    double rot_y_2;
    // pontok eltolasa
    double fin_x_1;
    double fin_y_1;
    double fin_x_2;
    double fin_y_2;

    // ciklus index
    int index;

    // regiszter tulcsordulasa nem okoz problemat
    // 1 - UINT_MAX = 2
    sound_distance = (int)(time_1 - time_2)*speed;

    //    sound_distance = (time_2 - time_1)*speed;
    //TODO függnek a dolgok az idokvantumtol (ms, ns)

    // a ket szenzor tavolsagat
    distance = sqrt(pow(pos_1_x - pos_2_x, 2.0) + pow(pos_1_y - pos_2_y, 2.0));

    // a szenzopar felezopontja (az eltolashoz kell)
    middle_x = (pos_1_x + pos_2_x)/2;
    middle_y = (pos_1_y + pos_2_y)/2;

    // hiperbola forgatasa szogenek kiszamitasa

    vec_x = pos_1_x - pos_2_x;
    vec_y = pos_1_y - pos_2_y;

    if((pos_1_x - pos_2_x) == 0)
    {
        angle = M_PI/2;
    }
    else
    {
        angle = atan2(vec_y, vec_x) - atan2(0,1);
    }

    // hiperbola kiszamitasa

    //TODO meddigmenjen a ciklus

    a = (distance + sound_distance)/2.0;
    for(index = 0; index < res_length; ){

        // a kozelebbi szenzotrol "a" a tavolsag
        // a tavolabbitol a + dd
        b = a - sound_distance;

        // coszinusztetellel a kozelebbinel levo szog
        coslamd = (1/(2*a*distance)) * (pow(a,2.0) + pow(distance,2.0) - pow(b,2.0));

        // kozepponttol a tavolsag
        s = sqrt(pow(a,2.0) + pow(distance,2.0)/4.0 - a*distance*coslamd);

        // kozepponttol a szog
        cosfi = 1/(s*distance)*(pow(s,2.0) + pow(distance,2.0)/4.0 - pow(a,2));
        fi = acos(cosfi);

        // a pont koordinatai

        // ha cosfi not a number
        if(cosfi != cosfi)
        {
            x = middle_x + s;
            y = middle_y + s;
            *(res_x + index) = x;
            *(res_y + index) = y;
            index++;

            // biztonsag kedveert
            if(index >= res_length)
                break;

            *(res_x + index) = x;
            *(res_y + index) = -y;
            index++;

            a +=0.1;
            continue;
        }
        x = s * cosfi;
        y = s * sin(fi);
        // pont forgatasa

        rot_x_1 = x * cos(angle) - y * sin(angle);
        rot_y_1 = x * sin(angle) + y * cos(angle);

        // hiperbola masik fele ugyanaz csak y koordinata az inverze
        rot_x_2 = x * cos(angle) + y * sin(angle);
        rot_y_2 = x * sin(angle) - y * cos(angle);


        // pontok eltolasa

        fin_x_1 = rot_x_1 + middle_x;
        fin_y_1 = rot_y_1 + middle_y;

        fin_x_2 = rot_x_2 + middle_x;
        fin_y_2 = rot_y_2 + middle_y;

        *(res_x + index) = fin_x_1;
        *(res_y + index) = fin_y_1;
        index++;

        // biztonsag kedveert
        if(index >= res_length)
            break;

        *(res_x + index) = fin_x_2;
        *(res_y + index) = fin_y_2;
        index++;

        // lepeskoz
        a += step;
        step = step * gain;

    }

}
