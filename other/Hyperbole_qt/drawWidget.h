#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H

#include <QtGui>
#include <QWidget>
#include <QList>
#include <QPoint>

#include <hyper.h>

#define PRINT_RATIO 1
#define Y_AXIS_OFFSET 200
#define X_AXIS_OFFSET 200

/*
 * A pontok kirajzolasaert felelos osztaly
 */

class DrawWidget : public QWidget
{
    Q_OBJECT

public:
    QList<QPoint> points;
    QList<QPoint> sensor_points;
    QList<QPoint> inter_points;
    point direction;
    point middle;

    void set_size(QPoint size);
    DrawWidget();

protected:
    /*
     * Az ujrarajzolaskor meghivando metodus
     */
    void paintEvent(QPaintEvent *event);

private:
    QPoint screen_size;

signals:

public slots:
    /*
     * A hiperbolaivek szamitasahoz szukseges adatok fogadasa,
     * majd azok felhasznalasaval az ivek szamolasat vegzo fuggvenyek hivasa
     */
    void start_draw(QList<int> x, QList<int> y, QList<int> t, int size, double speed);
};

#endif // DRAWWIDGET_H
