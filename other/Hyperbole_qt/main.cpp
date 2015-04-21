#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QApplication>
#include <QPushButton>
#include <QDebug>

#include <drawWidget.h>
#include <hyper.h>

int main(int argc, char*argv[]){

	int res;
    int index;

    QPoint ez[10000];

    QList<QPoint> pp;
	QApplication app(argc, argv);

    QWidget *window = new QWidget();
    window->setWindowTitle("Hyperboles");

    QVBoxLayout *layout = new QVBoxLayout();
    window->setLayout(layout);
    //qDebug() << res;

    DrawWidget* drawW = new DrawWidget();

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
    calc_hyper(one->x(),one->y(),two->x(),two->y(),30,1,res_x,res_y, size);

    for(int i = 0; i < size; i++){
        QPoint* p = new QPoint((int)*(res_x+i)+200,(int)*(res_y+i)+200);
        //ez[i] = *p;
        drawW->points.append(*p);
    }

    //drawW.show();


    layout->addWidget(drawW);
    window->resize(500,500);
    window->show();

	return app.exec();
}
