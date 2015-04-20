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
    calc_hyper(0,0,150,0,20,1,res_x,res_y, size);

    for(int i = 0; i < size; i++){
        QPoint* p = new QPoint(*(res_x+i)+100,*(res_x+i)+100);
        drawW->points.append(*p);
    }
    //drawW.show();

    layout->addWidget(drawW);
    window->resize(200,200);
    window->show();

	return app.exec();
}
