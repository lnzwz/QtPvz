#ifndef CHOOGQ_H
#define CHOOGQ_H

#include <QRadioButton>
#include <QDialog>
#include <QButtonGroup>

namespace Ui {
class Choogq;
}

class Choogq : public QDialog
{
    Q_OBJECT

public:
    explicit Choogq(QWidget *parent = nullptr);
    ~Choogq();
    int gq,ext_dif;
    QButtonGroup *gp2,*gp3,*gp4;
    QRadioButton bt[50];

private slots:
    void on_buttonBox_accepted();

    void on_ty0_clicked();

    void on_ty1_clicked();

    void on_buttonBox_rejected();

private:
    Ui::Choogq *ui;
};

#endif // CHOOGQ_H
