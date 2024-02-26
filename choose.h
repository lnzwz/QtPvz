#ifndef CHOOSE_H
#define CHOOSE_H

#define CN 8
#include "stdafx.h"

namespace Ui {
class Choose;
}

class Choose : public QDialog
{
    Q_OBJECT

public:
    explicit Choose(QWidget *parent = nullptr);
    ~Choose();
    void Init();
    void Sort();
    void paintEvent(QPaintEvent *event)override;

public:
    QPixmap ico[MAXSHU],icj[MAXJSH];
    int xuan[MXUAN*2],SHU,XUAN,yg[MAXSHU],dfn[MAXSHU],JSH;
    QCheckBox *ck[MAXSHU];
    QTimer timer;
    QElapsedTimer tim;
    int cho_cnt,gq,usermn;


private slots:
    void on_buttonBox_accepted();
    void OnTimer();

    void on_lastc_clicked();

private:
    Ui::Choose *ui;
};

#endif // CHOOSE_H
