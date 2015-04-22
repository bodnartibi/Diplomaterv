#include "drawWidget.h"

void DrawWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QPen myPen(Qt::black, 2, Qt::SolidLine);
    painter.setPen(myPen);
    myPen.setColor(Qt::black);

    foreach (QPoint p, points) {
        painter.drawPoint(p.x(),p.y());
    }


}

void DrawWidget::start_draw(QList<int> x, QList<int> y, QList<int> t)
{
    int size = 10000;
    double* res_x;
    res_x = (double*)malloc(sizeof(double)*size);
    double* res_y;
    res_y = (double*)malloc(sizeof(double)*size);

    points.clear();

    calc_hyper(x.value(0),y.value(0),x.value(1),y.value(1),t.value(0),t.value(1),res_x,res_y, size, 0.0001,1.005);

    for(int i = 0; i < size; i++){
        QPoint* p = new QPoint((int)*(res_x+i),(int)*(res_y+i));
        points.append(*p);
    }

    calc_hyper(x.value(1),y.value(1),x.value(2),y.value(2),t.value(1),t.value(2),res_x,res_y, size, 0.0001,1.005);

    for(int i = 0; i < size; i++){
        QPoint* p = new QPoint((int)*(res_x+i),(int)*(res_y+i));
        points.append(*p);
    }

    calc_hyper(x.value(2),y.value(2),x.value(0),y.value(0),t.value(2),t.value(0),res_x,res_y, size, 0.0001,1.005);

    for(int i = 0; i < size; i++){
        QPoint* p = new QPoint((int)*(res_x+i),(int)*(res_y+i));
        points.append(*p);
    }

    repaint();
    return;
}
