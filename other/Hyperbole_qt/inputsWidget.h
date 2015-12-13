#ifndef INPUTSWIDGET_H
#define INPUTSWIDGET_H

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QDebug>
#include <QtGui>
#include <QLineEdit>
#include <drawWidget.h>

/*
 * A kezelofeluletert felelos osztaly
 */

class InputsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit InputsWidget(QWidget *parent = 0);
protected:

signals:

    /*
     * Szamitashoz szukseges adatok kuldeseert felelos signal
     */
    void send_start_draw(QList<int> x, QList<int> y, QList<int> t, int size, double speed);

public slots:

    /*
     * A bemeneti adatok ervenyessegenek ellenorzese,
     * majd a szamolasert felelos objektumnak ertesites
     * illetve a bemeneti ertekek kuldese.
     */
    void start_calc();

private:

    /*
     * A feluleten helyetfoglalo elemek
     */

    QGridLayout* grid_layout;
    QPushButton* calc_button;

    QLineEdit* first_x_line;
    QLineEdit* first_y_line;
    QLineEdit* first_t_line;

    QLineEdit* second_x_line;
    QLineEdit* second_y_line;
    QLineEdit* second_t_line;

    QLineEdit* third_x_line;
    QLineEdit* third_y_line;
    QLineEdit* third_t_line;

    QLineEdit* num_line;
    QLineEdit* speed_line;

    QLabel * error_label;

    QWidget * parent;

    /*
     * Hibauzenet naplozasa a feluletre
     */
    void print_error(QString text);

};


#endif // INPUTSWIDGET_H
