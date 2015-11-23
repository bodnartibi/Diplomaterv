#include "drawWidget.h"

DrawWidget::DrawWidget()
{
    screen_size.setX(100);
    screen_size.setY(100);

    sensor_points.clear();
}

 void DrawWidget::set_size(QPoint size)
 {
     if(size.x() < 0 || size.y() < 0)
         return;

    screen_size.setX(size.x()/2);
    screen_size.setY(size.y()/2);
 }

void DrawWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    // tengelyek

    QPen myPen3(Qt::black, 1, Qt::SolidLine);
    painter.setPen(myPen3);

    for(int i=0; i < 2000 ; i++)
    {
        // x tengely
        painter.drawPoint(screen_size.x(), i);

        // y tengely
        painter.drawPoint(i, screen_size.y()+200);
    }

    QPen myPen(Qt::blue, 2, Qt::SolidLine);
    painter.setPen(myPen);

    foreach (QPoint p, points) {
        painter.drawPoint(p.x() + screen_size.x(), -p.y() + screen_size.y()+200);
    }

    QPen myPen2(Qt::red, 5, Qt::SolidLine);

    painter.setPen(myPen2);

    foreach (QPoint p, sensor_points) {
        painter.drawPoint(p.x()/PRINT_RATIO + screen_size.x(), -p.y()/PRINT_RATIO + screen_size.y()+200);
    }

    QPen myPen4(Qt::green, 5, Qt::SolidLine);

    painter.setPen(myPen4);

    foreach (QPoint p, inter_points) {
        painter.drawPoint(p.x() + screen_size.x(), -p.y() + screen_size.y()+200);
    }

    QPen myPen5(Qt::gray, 3, Qt::SolidLine);

    painter.setPen(myPen5);
    for(int i =0; i < 500; i++){
         painter.drawPoint(direction.x*i + screen_size.x() + middle.x, -direction.y*i + screen_size.y()+200 - middle.y);
    }

}


void DrawWidget::start_draw(QList<int> x, QList<int> y, QList<int> t, int size, double speed)
{


    sensor_point sen_1, sen_2, sen_3;
    point* inter;

    sensor_points.clear();
    inter_points.clear();

    QPoint* s0 = new QPoint(x.value(0),y.value(0));
    QPoint* s1 = new QPoint(x.value(1),y.value(1));
    QPoint* s2 = new QPoint(x.value(2),y.value(2));

    sen_1.p.x = x.value(0);
    sen_1.p.y = y.value(0);
    sen_2.p.x = x.value(1);
    sen_2.p.y = y.value(1);
    sen_3.p.x = x.value(2);
    sen_3.p.y = y.value(2);

    sen_1.time = t.value(0);
    sen_2.time = t.value(1);
    sen_3.time = t.value(2);

    sensor_points.append(*s0);
    int num_inter;
    sensor_points.append(*s1);
    sensor_points.append(*s2);

    point* res_1;
    point* res_2;
    point* res_3;
    res_1 = (point*)malloc(sizeof(point)*size);
    res_2 = (point*)malloc(sizeof(point)*size);
    res_3 = (point*)malloc(sizeof(point)*size);

    points.clear();

    calc_triangle_middle(sen_1,sen_2,sen_3,&middle);

    is_timestamps_correct(sen_1,sen_2,sen_3,speed);

    calc_hyper(sen_1, sen_2,\
               res_1, size, 0.5,1.005, speed);

    for(int i = 0; i < size; i++){
        QPoint* p = new QPoint((int)((res_1+i)->x/PRINT_RATIO+0.5),(int)((res_1+i)->y/PRINT_RATIO+0.5));
        points.append(*p);
    }

    calc_hyper(sen_2, sen_3,\
               res_2, size, 0.5,1.005, speed);
    for(int i = 0; i < size; i++){
        QPoint* p = new QPoint((int)((res_2+i)->x/PRINT_RATIO+0.5),(int)((res_2+i)->y/PRINT_RATIO+0.5));
        points.append(*p);
    }

    calc_hyper(sen_3, sen_1,\
               res_3, size, 0.5,1.005, speed);
    for(int i = 0; i < size; i++){
        QPoint* p = new QPoint((int)((res_3+i)->x/PRINT_RATIO+0.5),(int)((res_3+i)->y/PRINT_RATIO+0.5));
        points.append(*p);
    }

    inter = (point*)malloc(sizeof(point)*size);

    calc_intersection(res_1, res_2, \
                      size, 1.0, \
                      inter, \
                      size, &num_inter);

    if(num_inter)
        calc_direction(inter, num_inter, middle, &direction);

    for(int i = 0; i < num_inter; i++){
        QPoint* p = new QPoint((int)((inter+i)->x/PRINT_RATIO+0.5),(int)((inter+i)->y/PRINT_RATIO+0.5));
        inter_points.append(*p);
    }

    calc_intersection(res_2, res_3, \
                      size, 1.0, \
                      inter, \
                      size, &num_inter);
    if(num_inter)
        calc_direction(inter, num_inter, middle, &direction);

    for(int i = 0; i < num_inter; i++){
        QPoint* p = new QPoint((int)((inter+i)->x/PRINT_RATIO+0.5),(int)((inter+i)->y/PRINT_RATIO+0.5));
        inter_points.append(*p);
    }

    calc_intersection(res_1, res_3, \
                      size, 1.0, \
                      inter,
                      size, &num_inter);

    if(num_inter)
        calc_direction(inter, num_inter, middle, &direction);

    for(int i = 0; i < num_inter; i++){
        QPoint* p = new QPoint((int)((inter+i)->x/PRINT_RATIO+0.5),(int)((inter+i)->y/PRINT_RATIO+0.5));
        inter_points.append(*p);
    }



    repaint();
    return;
}
