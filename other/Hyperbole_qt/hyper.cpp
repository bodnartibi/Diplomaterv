#include "hyper.h"

// remove
#include <stdio.h>

/*
Koszinusz tetellel ki kell szamolnunk a haromszog egy pontjahoz tartozo szoget,
majd azzal kiszamolnunk a magassagat ahhoz a ponthoz.
A magassagnal nem lehet nagyobb a hang altal megtett ut.
C csucsra:
c2=a2+b2-2abcos(gamma)
A nal h = b*sinc = c*sinb
C nel h = a*sinb = b*sina
*/


// Az "a" hosszu oldalra huzott magassagot adja vissza
double high_of_triangle(double a, double b, double c)
{
    double gamma;
    double cosgamma;
    // cosG = (-c2+a2+b2)/2ab
    cosgamma = (-pow(c,2.0)+pow(b,2.0)+pow(a,2.0))/(2*a*b);
    gamma = acos(cosgamma);
    return b*sin(gamma);
}

int is_timestamps_correct(sensor_point s_1, \
                          sensor_point s_2, \
                          sensor_point s_3)
{
    int index;
    double sound_dist[3];
    double max_dist[3];


    //TEMP
    //
    //
    //
    return 1;

/*
    A1          A = sen. 1
    |\          B = sen. 2
  c | \  b      C = sen. 3
    |  \
    -----
  B2  a   C3
*/

    double a,b,c;

    // A point, sen. 1 --> a side
    a = sqrt(pow(s_2.p.x - s_3.p.x, 2.0) + pow(s_2.p.y - s_3.p.y, 2.0));

    b = sqrt(pow(s_1.p.x - s_3.p.x, 2.0) + pow(s_1.p.y - s_3.p.y, 2.0));
    c = sqrt(pow(s_1.p.x - s_2.p.x, 2.0) + pow(s_1.p.y - s_2.p.y, 2.0));

    // egy magassaghoz a hozzatartozo ponthoz tartozo
    // mindket szomszedos pont idejet ellenoriznunk kell
    max_dist[0] = high_of_triangle(a,b,c);
    max_dist[1] = high_of_triangle(b,c,a);
    max_dist[2] = high_of_triangle(c,a,b);

    // 0: a-b
    sound_dist[0] = (s_1.time - s_2.time)*SOUND_SPEED;
    // 1: a-c
    sound_dist[1] = (s_1.time - s_3.time)*SOUND_SPEED;
    // 2: b-c
    sound_dist[2] = (s_2.time - s_3.time)*SOUND_SPEED;

    printf("Distences: \n");
    for (index = 0; index < 3; index++)
    {
        printf("Max %f sound %f\n", max_dist[index],sound_dist[index]);
    }

    if(sound_dist[0] > max_dist[0] || sound_dist[1] > max_dist[0] || \
       sound_dist[0] > max_dist[1] || sound_dist[2] > max_dist[1] || \
       sound_dist[1] > max_dist[2] || sound_dist[2] > max_dist[2] )
    {
        return 0;
    }

    return 1;
}


int calc_intersection(point* line_1, \
                      point* line_2, \
                      int length, double radius, \
                      point* res, \
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
            distance = sqrt(pow((line_1 + i)->x - (line_2 + j)->x, 2.0) + \
                            pow((line_1 + i)->y - (line_2 + j)->y, 2.0));
            if(distance < radius)
            {
                // ket pont felezopontjat adjuk meg eredmenynek
                (res + res_num)->x = ((line_1 + i)->x + (line_2 + j)->x)/2;
                (res + res_num)->y = ((line_1 + i)->y + (line_2 + j)->y)/2;

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


void calc_hyper(sensor_point sensor_1, sensor_point sensor_2, \
                point* res, int res_length,\
                double step, double gain
                )
{
    // hangsebesseg
    const double speed = SOUND_SPEED;
    // ket szenzor tavolsaga
    double distance;
    // felezopont koordinatai
    point middle;
    // forgatas szoge
    double angle;

    // forgatasi szoghoz
    point vec;

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
    point rot_1;

    // hiperbola masik fele ugyanaz csak y koordinata az inverze
    point rot_2;
    // pontok eltolasa
    point fin_1;
    point fin_2;

    // ciklus index
    int index;

    // regiszter tulcsordulasa nem okoz problemat
    // 1 - UINT_MAX = 2
    sound_distance = (int)(sensor_1.time - sensor_2.time)*speed;

    //    sound_distance = (sensor_2.time - sensor_1.time)*speed;
    //TODO függnek a dolgok az idokvantumtol (ms, ns)

    // a ket szenzor tavolsagat
    distance = sqrt(pow(sensor_1.p.x - sensor_2.p.x, 2.0) + pow(sensor_1.p.y - sensor_2.p.y, 2.0));

    // a szenzopar felezopontja (az eltolashoz kell)
    middle.x = (sensor_1.p.x + sensor_2.p.x)/2;
    middle.y = (sensor_1.p.y + sensor_2.p.y)/2;

    // hiperbola forgatasa szogenek kiszamitasa

    vec.x = sensor_2.p.x - sensor_1.p.x;
    vec.y = sensor_2.p.y - sensor_1.p.y;

    angle = atan2(vec.y, vec.x) - atan2(0,1);
    // hiperbola kiszamitasa

    //TODO meddigmenjen a ciklus

    a = (distance)/2.0 - sound_distance;
    for(index = 0; index < res_length; ){

        // a kozelebbi szenzotrol "a" a tavolsag
        // a tavolabbitol a + dd
        b = a + 2*sound_distance;

        // coszinusztetellel a kozelebbinel levo szog
        coslamd = (1/(2*a*distance)) * (pow(a,2.0) + pow(distance,2.0) - pow(b,2.0));
        //coslamd = (1/(2*a*distance)) * (-pow(a,2.0) - pow(distance,2.0) + pow(b,2.0));

        // kozepponttol a tavolsag
        s = sqrt(pow(a,2.0) + pow(distance,2.0)/4.0 - a*distance*coslamd);

        // kozepponttol a szog
        cosfi = 1/(s*distance)*(pow(s,2.0) + pow(distance,2.0)/4.0 - pow(a,2));
        fi = acos(cosfi);

        // a pont koordinatai

        // ha cosfi not a number
        if(cosfi != cosfi)
        {
            x = middle.x + s;
            y = middle.y + s;
            (res + index)->x = x;
            (res + index)->y = y;
            index++;

            // biztonsag kedveert
            if(index >= res_length)
                break;

            (res + index)->x = x;
            (res + index)->y = y;
            index++;

            a = a*gain;
            continue;
        }
        x = s * cosfi;
        y = s * sin(fi);
        // pont forgatasa

        rot_1.x = x * cos(angle) - y * sin(angle);
        rot_1.y = x * sin(angle) + y * cos(angle);

        // hiperbola masik fele ugyanaz csak y koordinata az inverze
        rot_2.x = x * cos(angle) + y * sin(angle);
        rot_2.y = x * sin(angle) - y * cos(angle);


        // pontok eltolasa

        fin_1.x = rot_1.x + middle.x;
        fin_1.y = rot_1.y + middle.y;

        fin_2.x = rot_2.x + middle.x;
        fin_2.y = rot_2.y + middle.y;

        (res + index)->x = fin_1.x;
        (res + index)->y = fin_1.y;
        index++;

        // biztonsag kedveert
        if(index >= res_length)
            break;

        (res + index)->x = fin_2.x;
        (res + index)->y = fin_2.y;
        index++;

        // lepeskoz
        a += step;
        step = step * gain;

    }

}
