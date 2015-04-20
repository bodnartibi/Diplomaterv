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

void DrawWidget::drawPoint(int x, int y)
{
    /*
    QPainter painter(this);
    QPen myPen(Qt::black, 12, Qt::SolidLine);
    painter.setPen(myPen);
    painter.drawPoint(x,y);
    update();
    */
}
