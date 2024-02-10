#include "saveload.h"
#include "ui_saveload.h"

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


void SaveLoad::on_Text_objectNameChanged(const QString &objectName)
{
    F_name = objectName.toStdString();
}


void SaveLoad::on_Save_clicked()
{
    emit Save_sig(F_name);
}


void SaveLoad::on_Load_clicked()
{
    emit Load_sig(F_name);
}

