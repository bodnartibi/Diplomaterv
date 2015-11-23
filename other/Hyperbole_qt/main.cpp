#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QDebug>
#include <QtGui>
#include <QLineEdit>

#include <drawWidget.h>
#include <hyper.h>
#include <inputsWidget.h>

int main(int argc, char*argv[]){

    QPoint* size = new QPoint(700,300);

    QList<QPoint> pp;
	QApplication app(argc, argv);

    QWidget *window = new QWidget();
    window->setWindowTitle("Hyperboles");


    QHBoxLayout *layout_H = new QHBoxLayout();
    //QPushButton *calc_button = new QPushButton();
    //layout_H->addWidget(calc_button);
    //QLineEdit *test_line = new QLineEdit();
    //layout_H->addWidget(test_line);

    DrawWidget* drawW = new DrawWidget();

    drawW->set_size(*size);

    InputsWidget* input = new InputsWidget(drawW);
    layout_H->addWidget(input);
    input->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    input->show();

    QVBoxLayout *layout_V = new QVBoxLayout();
    layout_V->addLayout(layout_H);
    window->setLayout((layout_V));

    //qDebug() << res;


/*
    int size = 10000;
    double* res_x;
    res_x = (double*)malloc(sizeof(double)*size);
    double* res_y;
    res_y = (double*)malloc(sizeof(double)*size);

    test();

    QPoint* one = new QPoint(100,100);
    QPoint* two = new QPoint(100,300);

    drawW->points.append(*one);
    drawW->points.append(*two);
    calc_hyper(one->x(),one->y(),two->x(),two->y(),20,1,res_x,res_y, size, 0.0001,1.005);

    for(int i = 0; i < size; i++){
        QPoint* p = new QPoint((int)*(res_x+i),(int)*(res_y+i));
        //ez[i] = *p;
        drawW->points.append(*p);
    }
*/
    //drawW.show();


    layout_V->addWidget(drawW);
    window->resize(size->x(),size->y()+500);
    window->show();

	return app.exec();
}
