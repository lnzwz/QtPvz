#include "other.h"
#include "ui_other.h"

Other::Other(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Other)
{
    inited=0;
    ui->setupUi(this);
}

void Other::Init()
{
    for(int i=0,x=0,y=0;i<MAXJSH;i++)
    {
        ck[i]=new QCheckBox(this);
        ck[i]->setIconSize(QSize(40,40));
        ck[i]->setIcon(ico[i]);
        ck[i]->move(x,y);
        y+=40;if(y>=200)y=0,x+=100;
        if(!can_del[i])ck[i]->setDisabled(1);
    }
    QFile file (path+QString::asprintf ("lastc/oth.in"));
    if(file.open (QIODevice::ReadOnly))ui->lastc->setEnabled(1);
    else ui->lastc->setEnabled(0);
    Sync();inited=1;
}

Other::~Other()
{
    for(int i=0;i<MAXJSH;i++)
        delete ck[i];
    delete ui;
}

void Other::on_lastc_clicked()
{
    QFile file (path+QString::asprintf ("lastc/oth.in"));
    file.open (QIODevice::ReadOnly);
    QDataStream ar (&file);
    for(int i=0;i<MAXJSH;i++)
        ar>>del_js[i];
    ar>>auto_fire>>crea>>b_two>>b_xie>>b_sun;
    ar>>usermn>>usermm>>usersi;
    file.close();Sync();
}


void Other::on_buttonBox_accepted()
{
    for(int i=0;i<MAXJSH;i++)
        del_js[i]=(!ck[i]->isChecked());
    auto_fire=ui->Auto_fire->isChecked();
    crea=ui->creative->isChecked();
    b_two=ui->two->isChecked();
    b_xie=ui->xie->isChecked();
    b_sun=ui->DbSun->isChecked();
    if(ui->zdy->isChecked())
    {
        usermn=ui->Usermn->text().toInt();
        usermm=ui->Usermm->text().toInt();
        usersi=ui->Usersi->text().toInt();
    }
    else usermn=-1;
    bool ok;xmul=ui->xm->text().toDouble(&ok)/100.0;
    if(!ok)xmul=1.0;
    QFile file (path+QString::asprintf ("lastc/oth.in"));
    file.open (QIODevice::WriteOnly);
    QDataStream ar (&file);
    for(int i=0;i<MAXJSH;i++)
        ar<<del_js[i];
    ar<<auto_fire<<crea<<b_two<<b_xie<<b_sun;
    ar<<usermn<<usermm<<usersi;
    file.close();
}


void Other::on_creative_stateChanged(int arg1)
{
    if(arg1&&inited)
        QMessageBox::warning(this,tr("警告"),tr("开启此选项可能会影响游戏体验。"));
}


void Other::on_two_stateChanged(int arg1)
{
    if(arg1&&inited)
        QMessageBox::warning(this,tr("警告"),tr("双人模式将显著提高难度。"));
}

void Other::Sync()
{
    for(int i=0;i<MAXJSH;i++)
        ck[i]->setChecked(!del_js[i]);
    ui->Auto_fire->setChecked(auto_fire);
    ui->creative->setChecked(crea);
    ui->two->setChecked(b_two);
    ui->xie->setChecked(b_xie);
    ui->DbSun->setChecked(b_sun);
    if(usermn!=-1)
    {
        ui->Usermn->setText(QString::asprintf("%d",usermn));
        ui->Usermm->setText(QString::asprintf("%d",usermm));
        ui->Usersi->setText(QString::asprintf("%d",usersi));
        ui->zdy->setChecked(1);
        ui->Usermn->setEnabled(1);
        ui->Usermm->setEnabled(1);
        ui->Usersi->setEnabled(1);
    }
    else
    {
        ui->zdy->setChecked(0);
        ui->Usermn->setEnabled(0);
        ui->Usermm->setEnabled(0);
        ui->Usersi->setEnabled(0);
    }
}

void Other::on_zdy_stateChanged(int arg1)
{
    ui->Usermn->setEnabled(arg1);
    ui->Usermm->setEnabled(arg1);
    ui->Usersi->setEnabled(arg1);
}


void Other::on_Usermn_textEdited(const QString &arg1)
{
    usermn=ui->Usermn->text().toInt();
    if(usermn)
    {
        usersi=550/usermn;
        usermm=1001/usersi;
        ui->Usermm->setText(QString::asprintf("%d",usermm));
        ui->Usersi->setText(QString::asprintf("%d",usersi));
    }
}

