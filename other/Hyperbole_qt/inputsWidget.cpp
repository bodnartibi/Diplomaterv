#include <inputsWidget.h>

InputsWidget::InputsWidget(QWidget *parent): QWidget(parent)
{
    grid_layout = new QGridLayout();
    this->setLayout(grid_layout);

    // nevek
    grid_layout->addWidget(new QLabel(""),0,0);
    grid_layout->addWidget(new QLabel("X koordinate"),0,1);
    grid_layout->addWidget(new QLabel("Y koordinate"),0,2);
    grid_layout->addWidget(new QLabel("Time"),0,3);


    // elso oszlop
    grid_layout->addWidget(new QLabel("First point:"),1,0);
    grid_layout->addWidget(new QLabel("Second point:"),2,0);
    grid_layout->addWidget(new QLabel("Third point:"),3,0);

    first_x_line = new QLineEdit();
    first_y_line = new QLineEdit();
    first_t_line = new QLineEdit();

    second_x_line = new QLineEdit();
    second_y_line = new QLineEdit();
    second_t_line = new QLineEdit();

    third_x_line = new QLineEdit();
    third_y_line = new QLineEdit();
    third_t_line = new QLineEdit();

    grid_layout->addWidget(first_x_line,1,1);
    grid_layout->addWidget(first_y_line,1,2);
    grid_layout->addWidget(first_t_line,1,3);
    grid_layout->addWidget(second_x_line,2,1);
    grid_layout->addWidget(second_y_line,2,2);
    grid_layout->addWidget(second_t_line,2,3);
    grid_layout->addWidget(third_x_line,3,1);
    grid_layout->addWidget(third_y_line,3,2);
    grid_layout->addWidget(third_t_line,3,3);

    // errorok kiirasahoz
    error_label = new QLabel();
    grid_layout->addWidget(error_label,4,0);

}

void InputsWidget::start_calc()
{
    return;
}

void InputsWidget::print_error(QString text)
{
    return;
}
