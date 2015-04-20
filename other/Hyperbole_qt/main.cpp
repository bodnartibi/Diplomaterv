#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QApplication>
#include <QPushButton>
#include <QDebug>

#include <drawWidget.h>

int main(int argc, char*argv[]){

	int res;
    int index;

	QApplication app(argc, argv);

    QWidget *window = new QWidget();
    window->setWindowTitle("Hyperboles");

    QVBoxLayout *layout = new QVBoxLayout();
    window->setLayout(layout);
    //qDebug() << res;

    DrawWidget drawW;

    for(int i = 0; i < 50; i++)
        drawW.drawPoint(i,i);
    //drawW.show();

    layout->addWidget(drawW);
    window->show();

	return app.exec();
}
