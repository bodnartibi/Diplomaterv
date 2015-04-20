#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H

#include <QtGui>
#include <QWidget>

class DrawWidget : public QWidget
{
    Q_OBJECT
public:
    void drawPoint(int x, int y);

protected:
    void paintEvent(QPaintEvent *event);
signals:

public slots:

};

#endif // DRAWWIDGET_H
