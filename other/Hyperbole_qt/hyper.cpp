#include <hyper.h>
#include <math.h>

void calc_hyper(int pos_1_x, int pos_1_y, int pos_2_x, int pos_2_y, int time_1, int time_2, double* res_x, double* res_y, int res_length)
{
    // hangsebesseg
    const double speed = SOUND_SPEED;
    // ket szenzor tavolsaga
    double distance;
    // felezopont koordinatai
    double middle_x, middle_y;
    // forgatas szoge
    double angle;

    // temporalis
    double vec_x, vec_y;

    // amekkora utat a hang megtett ennyi ido alatt
    double sound_distance;

    sound_distance = (time_1 - time_2)*speed;

    //TODO függnek a dolgok az idokvantumtol (ms, ns)

    // a ket szenzor tavolsagat
    distance = sqrt(pow(pos_1_x-pos_2_x,2.0)+pow(pos_1_y-pos_2_y,2.0));

    // a szenzopar felezopontja (az eltolashoz kell)
    middle_x = (pos_1_x + pos_2_x)/2;
    middle_y = (pos_1_y + pos_2_y)/2;

    // hiperbola forgatasa szogenek kiszamitasa

    vec_x = pos_1_x - pos_2_x;
    vec_y = pos_1_y + pos_2_y;

    if((pos_1_x - pos_2_x) == 0)
    {
        angle = M_PI/2;
    }
    else
    {
        angle = atan2(vec_y, vec_x) - atan2(0,1);
    }

    // hiperbola kiszamitasa
    int index = 0;
    int x;
    int y;
    int a;
    for(a = (distance - sound_distance)/2; 500000 > a; a++ ){
        /**
        * a kozelebbi szenzotrol "a" a tavolsag
        * a tavolabbitol a + dd
        */
        double b = a + sound_distance;
        /**
        * coszinusztetellel a kozelebbinel levo szog
        */
        double coslamd = (1/(2*a*distance)) * (pow(a,2.0) + pow(distance,2.0) - pow(b,2.0));
        /**
        * kozepponttol a tavolsag
        */
         double s = sqrt(pow(a,2.0) + pow(distance,2.0)/4 - a*distance*coslamd);
         /**
          * kozepponttol a szog
          */
           double cosfi = 1/(s*distance)*(pow(s,2.0) + pow(distance,2.0)/4 - pow(a,2));
           double fi = acos(cosfi);
          /**
            * a pont koordinatai
            */
           x = s * cosfi;
           y = s * sin(fi);

            *(res_x + index) = x;
            *(res_y + index) = y;


            index++;
            if(index >= res_length)
                break;
        }

    // hiperbola eltolasa es forgatasa


}


void test()
{
    return;
}
