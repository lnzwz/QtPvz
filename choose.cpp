#include "choose.h"
#include "ui_choose.h"

Choose::Choose(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Choose)
{
    ui->setupUi(this);
    timer.setTimerType(Qt::PreciseTimer);
    timer.start(20);tim.start();
    connect(&timer,&QTimer::timeout,this,&Choose::OnTimer);
}

void Choose::Init()
{
#ifndef Q_OS_ANDROID
    resize(800,295);
    ui->buttonBox->move(530,240);
    ui->label->move(20,250);
#endif
    for(int i=0,x=0,y=0;i<SHU;i++)
    {
        ck[i]=new QCheckBox(this);
#ifdef Q_OS_ANDROID
        ck[i]->setIconSize(QSize(80,80));
#else
        ck[i]->setIconSize(QSize(40,40));
#endif
        ck[i]->setIcon(ico[i]);
        ck[i]->setText(QString::asprintf("%d",yg[i]));
        ck[i]->move(x,y);
#ifdef Q_OS_ANDROID
        y+=80;if(y>=400)y=0,x+=220;
#else
        y+=40;if(y>=200)y=0,x+=100;
#endif
    }
    for(int i=0;i<JSH;i++)
    {
        js[i]=new QLabel(this);
#ifdef Q_OS_ANDROID
        icj[i]=icj[i].scaled(70,70,Qt::KeepAspectRatio);
        js[i]->resize(QSize(70,70));
        js[i]->setPixmap(icj[i]);
        js[i]->move(i*80,420);
#else
        icj[i]=icj[i].scaled(35,35,Qt::KeepAspectRatio);
        js[i]->resize(QSize(35,35));
        js[i]->setPixmap(icj[i]);
        js[i]->move(i*40,210);
#endif
    }
}

Choose::~Choose()
{
    delete ui;
}

void Choose::on_buttonBox_accepted()
{
    int m=0,sa[MXUAN]={0},sb[MXUAN]={0};
    for(int i=0;i<SHU;i++)
    {
        if(ck[i]->isChecked())
        {
            sa[m]=i;
            sb[m++]=dfn[i];
        }
    }
    for(int i=0;i<XUAN;i++)
    {
        for(int j=i+1;j<XUAN;j++)
        {
            if(sb[i]>sb[j])
            {
                int t=sa[i];sa[i]=sa[j];sa[j]=t;
                t=sb[i];sb[i]=sb[j];sb[j]=t;
            }
        }
    }
    for(int i=0;i<XUAN;i++)
        xuan[i]=sa[i];
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
    ui->buttonBox->setEnabled(m==XUAN);
}
