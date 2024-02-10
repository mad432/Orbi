#include "saveload.h"
#include "ui_saveload.h"
#include <iostream>

SaveLoad::SaveLoad(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveLoad)
{
    ui->setupUi(this);

    QWidget::setWindowTitle("Save / Load");
}

SaveLoad::~SaveLoad()
{
    delete ui;
}


void SaveLoad::on_Save_clicked()
{
    emit Save_sig(ui->Text->toPlainText().toStdString());
}


void SaveLoad::on_Load_clicked()
{
    emit Load_sig(ui->Text->toPlainText().toStdString());
}


