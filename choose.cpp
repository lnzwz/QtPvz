#include "choose.h"
#include "ui_choose.h"
#include "mainwindow.h"

Choose::Choose(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Choose)
{
    ui->setupUi(this);
    timer.setTimerType(Qt::PreciseTimer);
    timer.start(20);tim.start();
    connect(&timer,&QTimer::timeout,this,&Choose::OnTimer);
    cho_cnt=0;gq=-1;
}

void Choose::Init()
{
    int si=40;
    resize((SHU+CN-1)/CN*(si*2+20),si*CN+130);
#ifdef Q_OS_ANDROID
    ui->buttonBox->move(130,280);
    ui->lastc->move(380,280);
    ui->label->move(20,270);si=30;
    resize(800,320);
#else
    ui->buttonBox->move(230,si*CN+95);
    ui->lastc->move(380,si*CN+95);
    ui->label->move(20,si*CN+85);
#endif
    for(int i=0,x=0,y=0;i<SHU;i++)
    {
        ck[i]=new QCheckBox(this);
        ck[i]->setIconSize(QSize(si,si));
        ck[i]->setIcon(ico[i]);
        if(yg[i]==99999)
        {
            yg[i]=0;
            ck[i]->setEnabled(0);
        }
        ck[i]->setText(QString::asprintf("%d",yg[i]));
        ck[i]->move(x,y);
        y+=si;if(y>=si*CN)y=0,x+=si*2+20;
    }
    si=35;
#ifdef Q_OS_ANDROID
    si=20;
#endif
    for(int i=0;i<JSH;i++)
        if(!icj[i].isNull())icj[i]=icj[i].scaled(si*m_wnd->b_ret,si*m_wnd->b_ret,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    for(int i=0;i<MAXSHU;i++)
        if(!ico[i].isNull())ico[i]=ico[i].scaled(si*m_wnd->b_ret,si*m_wnd->b_ret,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    QFile file (path+QString::asprintf ("lastc/%d.in", gq));
    if(file.open (QIODevice::ReadOnly))ui->lastc->setEnabled(1);
    else ui->lastc->setEnabled(0);
    ui->buttonBox->raise();
}

Choose::~Choose()
{
    for(int i=0;i<SHU;i++)
        delete ck[i];
    delete ui;
}

void Choose::Sort()
{
    int m=0,sa[MXUAN*2]={0},sb[MXUAN*2]={0};
    for(int i=0;i<SHU;i++)
    {
        if(ck[i]->isChecked())
        {
            sa[m]=i;
            sb[m++]=dfn[i];
        }
    }
    for(int i=0;i<m;i++)
    {
        for(int j=i+1;j<m;j++)
        {
            if(sb[i]>sb[j])
            {
                int t=sa[i];sa[i]=sa[j];sa[j]=t;
                t=sb[i];sb[i]=sb[j];sb[j]=t;
            }
        }
    }
    for(int i=0;i<m;i++)
        xuan[i]=sa[i];
    cho_cnt=m;
}

void Choose::on_buttonBox_accepted()
{
    Sort();
    if(gq!=-1)
    {
        QFile file (path+QString::asprintf ("lastc/%d.in", gq));
        file.open (QIODevice::WriteOnly);
        QDataStream ar (&file);
        for(int i=0;i<XUAN;i++)
            ar<<xuan[i];
        file.close();
    }
}

void Choose::OnTimer()
{
    int m=0;
    for(int n=0;n<SHU;n++)
    {
        if(ck[n]->isChecked())
        {
            m+=1;
            if(dfn[n]==-1)dfn[n]=tim.elapsed();
        }
        else dfn[n]=-1;
    }
    ui->label->setText(QString::asprintf("还能选%d个",XUAN-m));
    ui->buttonBox->buttons()[0]->setEnabled(m==XUAN);
    if(m<=XUAN)Sort();
    update();
}

void Choose::paintEvent(QPaintEvent *event)
{
    QPainter dc(this);
#ifdef Q_OS_ANDROID
    for(int i=0;i<cho_cnt;i++)
        dc.drawPixmap(QRect(i*25,250,25,25),ico[xuan[i]]);
    for(int i=0;i<JSH;i++)
        dc.drawPixmap(QRect(i*20,220,20,20),icj[i]);
#else
    for(int i=0;i<cho_cnt;i++)
        dc.drawPixmap(QRect(i*40,40*CN+50,35,35),ico[xuan[i]]);
    for(int i=0;i<JSH;i++)
        dc.drawPixmap(QRect(i*32,40*CN+10,35,35),icj[i]);
#endif
}

void Choose::on_lastc_clicked()
{
    QFile file (path+QString::asprintf ("lastc/%d.in", gq));
    file.open (QIODevice::ReadOnly);
    QDataStream ar (&file);
    for(int i=0;i<SHU;i++)
        ck[i]->setChecked(0);
    for(int i=0;i<XUAN;i++)
    {
        int t;ar>>t;
        if(t>=SHU)continue;
        ck[t]->setChecked(1);
        dfn[t]=i;xuan[i]=t;
    }
    file.close();
}
