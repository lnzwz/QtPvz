#include "choogq.h"
#include "ui_choogq.h"
#include "stdafx.h"

Choogq::Choogq(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Choogq)
{
    ui->setupUi(this);
    QButtonGroup *gp1 = new QButtonGroup (this);
    gp1->addButton (ui->ty0);
    gp1->addButton (ui->ty1);
    gp2 = new QButtonGroup (this);
    gp2->addButton (ui->sce1);
    gp2->addButton (ui->sce2);
    gp2->addButton (ui->sce3);
    gp2->addButton (ui->sce4);
    gp2->addButton (ui->sce5);
    gp2->addButton (ui->sce6);
    gp3 = new QButtonGroup (this);
    gp3->addButton (ui->radioButton_1);
    gp3->addButton (ui->radioButton_2);
    gp3->addButton (ui->radioButton_3);
    gp3->addButton (ui->radioButton_4);
    gp3->addButton (ui->radioButton_5);
    gp3->addButton (ui->radioButton_6);
    gp3->addButton (ui->radioButton_7);
    gp3->addButton (ui->radioButton_8);
    gp3->addButton (ui->radioButton_9);
    gp3->addButton (ui->radioButton_10);
    gp3->addButton (ui->radioButton_11);
    gp3->addButton (ui->radioButton_12);
    gp3->addButton (ui->radioButton_13);
    gp3->addButton (ui->radioButton_14);
    gp4 = new QButtonGroup (this);
    const int N=27;int x=10,y=140;
    QString str[N]={"僵王（滚卡）","僵王（种植）","WSAD","无尽（水池）","大蒜图","我是僵尸","狂锤","双向僵尸","WSAD2","排山倒海","坚不可摧","砸罐","我是植物","可移动植物","背刺","2048",
"无限阳光","无限阳光+僵王","对战","反弹","植物僵尸","植物僵尸2","定向打击","收集僵尸","全面入侵","全面入侵2","大炮之力"};
    for(int i=0;i<N;i++)
    {
        bt[i].setParent(this);
        bt[i].setText(str[i]);
        int le=str[i].length()*10+40;
        if(x+le>640)x=10,y+=30;
        bt[i].move(x,y);bt[i].show();
        gp4->addButton(&bt[i]);x+=le;
    }
    ui->ty0->setChecked(1);
    ui->sce1->setChecked(1);
    ui->radioButton_1->setChecked(1);
    bt[0].setChecked(1);ext_dif=gq=0;
    for(auto bt:gp4->buttons())bt->setEnabled(0);
#ifdef Q_OS_IOS
    QFont f ("Arial", 10, false);
    setFont (f);
#endif
#ifdef Q_OS_ANDROID
    QFont f ("Arial", 10, false);
    setFont (f);
#endif
}

Choogq::~Choogq()
{
    delete ui;
}

void Choogq::on_buttonBox_accepted()
{
    ext_dif=gq=0;
    if(ui->ty0->isChecked())
    {
        int z1=(-1)-gp2->checkedId(),z2=(-1)-gp3->checkedId();
        if(z2>10)ext_dif=z2-10,z2=10;
        gq=(z1-1)*GQS+z2;
    }
    else if(ui->ty1->isChecked())
    {
        gq=MAXGQS+(-1)-gp4->checkedId();
        if(gq==75)
        {
            gq=8;
            ui->sp1->setChecked(1);
            ui->sp4->setChecked(1);
        }
        else if(gq==76)gq=2048;
        else if(gq>=77)gq-=2;
    }
    if(ui->sp1->isChecked())gq+=100;
    if(ui->sp2->isChecked())ext_dif+=1;
    if(ui->sp3->isChecked())gq+=200;
    if(ui->sp4->isChecked())gq+=400;
    if(gq>2048)gq=2048;
}


void Choogq::on_ty0_clicked()
{
    for(auto bt:gp2->buttons())bt->setEnabled(1);
    for(auto bt:gp3->buttons())bt->setEnabled(1);
    for(auto bt:gp4->buttons())bt->setEnabled(0);
}


void Choogq::on_ty1_clicked()
{
    for(auto bt:gp2->buttons())bt->setEnabled(0);
    for(auto bt:gp3->buttons())bt->setEnabled(0);
    for(auto bt:gp4->buttons())bt->setEnabled(1);
}


void Choogq::on_buttonBox_rejected()
{
    ext_dif=gq=0;
}

