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

	QApplication app(argc, argv);

    // Ablak letrehozasa
    QWidget *window = new QWidget();
    window->setWindowTitle("Hyperboles");

    // A vertikalis layout
    QVBoxLayout *layout_V = new QVBoxLayout();

    // Gorbekirajzolasert felelos widget letrehozasa
    DrawWidget* drawW = new DrawWidget();
    drawW->set_size(*size);

    // Inputokert felelos widget letrehozasa
    InputsWidget* input = new InputsWidget(drawW);
    // Hozzaadasa a layouthoz
    layout_V->addWidget(input);
    input->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    input->show();

    // Gorbekirajzolasert felelos widget hozzaadasa a layouthoz
    layout_V->addWidget(drawW);

    window->setLayout((layout_V));
    window->resize(size->x(),size->y()+500);
    window->show();

	return app.exec();
}
