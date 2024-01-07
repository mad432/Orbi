#include "c_slider.h"
#include "ui_c_slider.h"

C_slider::C_slider(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::C_slider)
{
    ui->setupUi(this);
    QWidget::setWindowTitle("Speed of Light :");
}

C_slider::~C_slider()
{
    delete ui;
}

void C_slider::on_horizontalSlider_valueChanged(int value)
{
    emit valueChanged(value);
}
