#ifndef CHOOSE_H
#define CHOOSE_H

#include <QDialog>
#include "stdafx.h"
#include <QCheckBox>
#include <QTimer>
#include <QElapsedTimer>
#include <QLabel>

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

public:
    QPixmap ico[MAXSHU],icj[MAXJSH];
    int xuan[MXUAN],SHU,XUAN,yg[MAXSHU],dfn[MAXSHU],JSH;
    QCheckBox *ck[MAXSHU];QLabel *js[MAXJSH];
    QTimer timer;
    QElapsedTimer tim;


private slots:
    void on_buttonBox_accepted();
    void OnTimer();

private:
    Ui::Choose *ui;
};

#endif // CHOOSE_H
