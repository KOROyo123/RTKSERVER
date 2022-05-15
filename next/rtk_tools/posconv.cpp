#include "posconv.h"
#include "ui_posconv.h"

PosConv::PosConv(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PosConv)
{
    ui->setupUi(this);
}

PosConv::~PosConv()
{
    delete ui;
}
