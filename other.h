#ifndef OTHER_H
#define OTHER_H

#include <QDialog>
#include <QCheckBox>
#include <QFile>
#include "stdafx.h"

namespace Ui {
class Other;
}

class Other : public QDialog
{
    Q_OBJECT

public:
    bool del_js[MAXJSH],can_del[MAXJSH],auto_fire,crea,b_two,b_xie,inited,b_sun;
    double xmul;int usermn,usermm,usersi;
    explicit Other(QWidget *parent = nullptr);
    ~Other();
    void Init();
    void Sync();
    QPixmap ico[MAXJSH];
    QCheckBox *ck[MAXSHU];

private slots:
    void on_lastc_clicked();

    void on_buttonBox_accepted();

    void on_creative_stateChanged(int arg1);

    void on_two_stateChanged(int arg1);

    void on_zdy_stateChanged(int arg1);

    void on_Usermn_textEdited(const QString &arg1);

private:
    Ui::Other *ui;
};

#endif // OTHER_H
