#include <inputsWidget.h>

InputsWidget::InputsWidget(QWidget *drawW): QWidget(drawW)
{
    this->parent = parent;
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

    calc_button = new QPushButton("Calculate");
    grid_layout->addWidget(calc_button,0,4);
    connect(calc_button,SIGNAL(clicked()), this, SLOT(start_calc()));
    connect(this,SIGNAL(send_start_draw(QList<int>, QList<int>, QList<int>, int)), \
            drawW, SLOT(start_draw(QList<int>, QList<int>, QList<int>, int)));

    grid_layout->addWidget(new QLabel("Number of points"),1,4);
    num_line = new QLineEdit();
    grid_layout->addWidget(num_line,2,4);


}

void InputsWidget::start_calc()
{
    bool OK;
    QList<int> x;
    QList<int> y;
    QList<int> t;
    int size;

    x.append(first_x_line->text().toInt(&OK));
    if(!OK)
        goto error;
    x.append(second_x_line->text().toInt(&OK));
    if(!OK)
        goto error;
    x.append(third_x_line->text().toInt(&OK));
    if(!OK)
        goto error;
    y.append(first_y_line->text().toInt(&OK));
    if(!OK)
        goto error;
    y.append(second_y_line->text().toInt(&OK));
    if(!OK)
        goto error;
    y.append(third_y_line->text().toInt(&OK));
    if(!OK)
        goto error;
    t.append(first_t_line->text().toInt(&OK));
    if(!OK)
        goto error;
    t.append(second_t_line->text().toInt(&OK));
    if(!OK)
        goto error;
    t.append(third_t_line->text().toInt(&OK));
    if(!OK)
        goto error;

    size = num_line->text().toInt(&OK);
    if(!OK)
        goto error;

    print_error("");
    emit send_start_draw(x,y,t,size);
    return;

    error:
    x.clear();
    y.clear();
    t.clear();
    print_error("ERROR: Invalid input(s)");
    return;
}

void InputsWidget::print_error(QString text)
{
    error_label->setText(text);
    return;
}

