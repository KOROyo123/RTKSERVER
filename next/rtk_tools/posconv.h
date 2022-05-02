#ifndef POSCONV_H
#define POSCONV_H

#include <QDialog>

namespace Ui {
class PosConv;
}

class PosConv : public QDialog
{
    Q_OBJECT

public:
    explicit PosConv(QWidget *parent = nullptr);
    ~PosConv();

private:
    Ui::PosConv *ui;
};

#endif // POSCONV_H
