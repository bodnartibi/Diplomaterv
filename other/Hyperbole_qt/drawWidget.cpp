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

    return;
}
