#ifndef INPUTSWIDGET_H
#define INPUTSWIDGET_H

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QDebug>
#include <QtGui>
#include <QLineEdit>

class InputsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit InputsWidget(QWidget *parent = 0);
protected:

signals:

public slots:

    void start_calc();

private:
    QGridLayout* grid_layout;
    QPushButton* clac_button;

    QLineEdit* first_x_line;
    QLineEdit* first_y_line;
    QLineEdit* first_t_line;

    QLineEdit* second_x_line;
    QLineEdit* second_y_line;
    QLineEdit* second_t_line;

    QLineEdit* third_x_line;
    QLineEdit* third_y_line;
    QLineEdit* third_t_line;

    QLabel * error_label;

    void print_error(QString text);

};


#endif // INPUTSWIDGET_H
