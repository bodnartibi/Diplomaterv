#include <hyper.h>
#include <math.h>




void calc_hyper(int pos_1_x, int pos_1_y, int pos_2_x, int pos_2_y, \
                int time_1, int time_2, \
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

    //if(time_1 >= time_2)
        sound_distance = (time_1 - time_2)*speed;
    //else
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

    a = (distance + sound_distance)/2;
    for(index = 0; index < res_length; ){

        // a kozelebbi szenzotrol "a" a tavolsag
        // a tavolabbitol a + dd
        b = a - sound_distance;

        // coszinusztetellel a kozelebbinel levo szog
        coslamd = (1/(2*a*distance)) * (pow(a,2.0) + pow(distance,2.0) - pow(b,2.0));

        // kozepponttol a tavolsag
        s = sqrt(pow(a,2.0) + pow(distance,2.0)/4 - a*distance*coslamd);

        // kozepponttol a szog
        cosfi = 1/(s*distance)*(pow(s,2.0) + pow(distance,2.0)/4 - pow(a,2));
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


void test()
{
    return;
}
