#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H

#include <QtGui>
#include <QWidget>
#include <QList>
#include <QPoint>

#include <hyper.h>


class DrawWidget : public QWidget
{
    Q_OBJECT

public:
    QList<QPoint> points;
    QList<QPoint> sensor_points;

    void set_size(QPoint size);
    DrawWidget();


protected:
    void paintEvent(QPaintEvent *event);

private:

    QPoint screen_size;
signals:

public slots:
    void start_draw(QList<int> x, QList<int> y, QList<int> t);
    //void start_draw();
};

#endif // DRAWWIDGET_H
