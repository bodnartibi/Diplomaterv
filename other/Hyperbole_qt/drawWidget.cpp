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

    for(int i; i < 2000 ; i++)
    {
        // x tengely
        painter.drawPoint(screen_size.x(), i);

        // y tengely
        painter.drawPoint(i , screen_size.y());
    }

    QPen myPen(Qt::blue, 2, Qt::SolidLine);
    painter.setPen(myPen);

    foreach (QPoint p, points) {
        painter.drawPoint(p.x() + screen_size.x(), p.y() + screen_size.y());
    }

    QPen myPen2(Qt::red, 5, Qt::SolidLine);

    painter.setPen(myPen2);

    foreach (QPoint p, sensor_points) {
        painter.drawPoint(p.x() + screen_size.x(), p.y() + screen_size.y());
    }



}

void DrawWidget::start_draw(QList<int> x, QList<int> y, QList<int> t)
{

    sensor_points.clear();

    QPoint* s0 = new QPoint(x.value(0),y.value(0));
    QPoint* s1 = new QPoint(x.value(1),y.value(1));
    QPoint* s2 = new QPoint(x.value(2),y.value(2));

    sensor_points.append(*s0);
    sensor_points.append(*s1);
    sensor_points.append(*s2);

    int size = 10000;
    double* res_x;
    res_x = (double*)malloc(sizeof(double)*size);
    double* res_y;
    res_y = (double*)malloc(sizeof(double)*size);

    points.clear();

    calc_hyper(x.value(0),y.value(0),x.value(1),y.value(1),t.value(0),t.value(1),res_x,res_y, size, 0.0001,1.005);

    for(int i = 0; i < size; i++){
        QPoint* p = new QPoint((int)(*(res_x+i)+0.5),(int)(*(res_y+i)+0.5));
        points.append(*p);
    }

    calc_hyper(x.value(1),y.value(1),x.value(2),y.value(2),t.value(1),t.value(2),res_x,res_y, size, 0.0001,1.005);

    for(int i = 0; i < size; i++){
        QPoint* p = new QPoint((int)(*(res_x+i)+0.5),(int)(*(res_y+i)+0.5));
        points.append(*p);
    }

    calc_hyper(x.value(2),y.value(2),x.value(0),y.value(0),t.value(2),t.value(0),res_x,res_y, size, 0.0001,1.005);

    for(int i = 0; i < size; i++){
        QPoint* p = new QPoint((int)(*(res_x+i)+0.5),(int)(*(res_y+i)+0.5));
        points.append(*p);
    }

    repaint();
    return;
}
