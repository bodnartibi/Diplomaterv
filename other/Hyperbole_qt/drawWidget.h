#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H

#include <QtGui>
#include <QWidget>
#include <QList>
#include <QPoint>

#include <hyper.h>

#define PRINT_RATIO 1

class DrawWidget : public QWidget
{
    Q_OBJECT

public:
    QList<QPoint> points;
    QList<QPoint> sensor_points;
    QList<QPoint> inter_points;

    void set_size(QPoint size);
    DrawWidget();


protected:
    void paintEvent(QPaintEvent *event);

private:

    QPoint screen_size;
signals:

public slots:
    void start_draw(QList<int> x, QList<int> y, QList<int> t, int size, double speed);
    //void start_draw();
};

#endif // DRAWWIDGET_H
