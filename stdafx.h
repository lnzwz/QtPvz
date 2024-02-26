#ifndef STDAFX_H
#define STDAFX_H

#define NOSOUND
#define TEST
//#define NO_UI

#include <qglobal.h>
#ifdef Q_OS_WIN
#include <qt_windows.h>
#include <windows.h>
#endif
#include <QMainWindow>
#include <QMessageBox>
#include <QMouseEvent>
#include <QTimer>
#include <QTime>
#include <QPainter>
#include <QElapsedTimer>
#include <QFileDialog>
#include <QButtonGroup>
#include <QStyleFactory>
#include <QTcpSocket>
#include <QTcpServer>
#include <QColor>
#include <QLabel>
#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QDialog>
#include <QCheckBox>
#include <QFile>
#include <QThread>
#include <QSize>
#include <QPoint>
#include <QPixmap>

#ifndef NOSOUND
#include "QtConcurrent/qtconcurrentrun.h"
#include <QtMultimedia>
#include <QSoundEffect>
#endif

#include <algorithm>
#include <random>
#include <set>
#include <utility>
#include <cmath>
#include <queue>
#include <cmath>

#define MAXSHU 81
#define MXUAN 11
#define MAXJSH 29
#define MAXND 1000
#define GQS 10
#define DBS 0.45
#define MANG 9
#define MAXGQS 60
#define MMN 22
#define MMM 39
#define ICCNT 265
#define eps 1e-9
#define ZGQS MAXGQS+25
#define MAXCZ 2
#define AXS 7
#define FW 0.51
#define MAXDZS 99
#define MAXBJ 6
#define MAXX 2000
#define MAXFREZ 2000
#define MAXXU 5.0
#define MAXDU 250

#define RED QColor(255,0,0)
#define GREEN QColor(0,190,0)
#define LGREEN QColor(150,255,150)
#define YUMI QColor(175,175,0)
#define BUTTER QColor(170,170,0)
#define BLUE QColor(0,100,230)
#define YELLOW QColor(180,180,0)
#define LYELLOW QColor(230,230,0)
#define FYELLOW QColor(200,80,250)
#define DBLUE QColor (80,32,255)
#define MHCO QColor(240,0,150)
#define PURPLE QColor (255,0,255)

#define GL48 8
#define ull unsigned long long

extern int XUAN,TM,MM,MN,SI,FIRETM,ME,HI,W0,W1,W2;
extern double dan[100],zoom,sinra[10000],cosra[10000];
extern QPixmap *ico,*ico_bl,*ico_rd,*ico_blk,*ico_sha;
extern std::mt19937 RA;
extern QString path;
extern bool pub_bl[1000];
class Zombie;
int ClearDun(Zombie&js);

struct HURT
{
    short du,frez,ct;int atk;
    bool fire;HURT(){}
    HURT(int Atk,short Frez=0,short Du=0,short Ct=0,bool Fire=0)
    {
        atk=Atk;
        frez=Frez;du=Du;
        ct=Ct;fire=Fire;
    }
};

struct Plant
{
    QColor color;//子弹颜色

    int time,odtm;//攻击时间
    int dztm,dzsy;//能量豆次数，能量豆剩余
    double life,mxlf;//生命值
    int cost,la_et;//阳光值
    int tag,nxt_tm;//特殊变量，下次时间
    int attack;//攻击力

    short num,def;//下标，防御
    short x,y;//位置
    short id;//图片id
    short type;//唯一标记
    short siz;//子弹大小
    short grow;//生长
    short fire;//火焰
    short frez_stp;

    bool freeze,anti_fire,ti;//是否寒冰
    bool mush,left,stop;//蘑菇,向左

    short move_spd;//移动速度(子弹)
    short jian,lev,bj,levod;//溅射，化肥

    double sp_mul;

    Plant(){nxt_tm=-1;left=fire=frez_stp=0;}
    void Create(QColor Color,int Goj,int Time,int Dztm,int Sd,int Shm,int X,int Y,int Id,int Yg,int Lx,int Dx=10,bool Hb=false,bool Mg=false)
    {
        color=Color;
        attack=Goj;
        if(attack>0)attack*=1000;
        time=odtm=Time;
        dztm=Dztm;
        move_spd=Sd;
        mxlf=life=Shm;
        x=X;y=Y;
        id=Id;
        cost=Yg;
        type=Lx;
        siz=Dx;
        freeze=Hb;
        mush=Mg;
        grow=0;la_et=-1e9;
        tag=jian=lev=0;nxt_tm=-1;
        fire=ti=levod=0;
        anti_fire=stop;
        odtm=dzsy=def=0;bj=1;
        sp_mul=1;
    }
    void input(QDataStream &ar)
    {
        ar>>color>>attack;
        ar>>time>>dztm>>odtm>>dzsy;
        ar>>move_spd>>life;
        ar>>x>>y>>id>>frez_stp;
        ar>>cost>>type;
        ar>>siz>>grow>>def;
        ar>>freeze>>mush;
        ar>>tag>>nxt_tm>>sp_mul;
        ar>>jian>>lev>>left;
        ar>>fire>>mxlf>>ti;
        ar>>anti_fire>>stop>>bj;
    }
    void output(QDataStream &ar)const
    {
        ar<<color<<attack;
        ar<<time<<dztm<<odtm<<dzsy;
        ar<<move_spd<<life;
        ar<<x<<y<<id<<frez_stp;
        ar<<cost<<type;
        ar<<siz<<grow<<def;
        ar<<freeze<<mush;
        ar<<tag<<nxt_tm<<sp_mul;
        ar<<jian<<lev<<left;
        ar<<fire<<mxlf<<ti;
        ar<<anti_fire<<stop<<bj;
    }
    bool Burning()const{return fire&&fire<=FIRETM;}
    bool Burn()
    {
        frez_stp=0;
        if(fire==0&&!anti_fire)
            {fire=1;return 1;}
        return 0;
    }
};
struct Zombie
{
    int life,dun;//生命值
    int eat,du,frez_stp;//吃速度
    int num,stp_tm,ii;//下标
    int fss,fs_nd;//发射延迟
    int mxlf,mxdun;

    double x_flt,spd,spdy,ry;//移动速度
    double xu;
    short x,y;//位置
    short id;//图片id
    short type;//编号
    short freeze;//寒冰
    short sta;//状态

    bool shoot;//是否发射
    bool sp_eat;//特殊吃法
    bool eating,fei;//正在吃，化肥有无

    Zombie(){spdy=dun=0;ry=-1;frez_stp=-1;}
    int Hurt(int hu,double ct)
    {
        int dh=int(hu*(1-ct)),rt=0;
        hu-=dh;dun-=dh,rt+=dh;
        if(dun<0)
        {
            if(dun+dh>0)id=ClearDun(*this);
            hu-=dun;rt+=dun;dun=0;
        }
        rt+=std::min(life,hu);
        life-=hu;return rt;
    }
    void Create(double Sd,int Shm,int Eat,int X,int Y,int Id,int Lx,bool Beat=false,bool Fs=false,int Fsn=0)
    {
        spd=Sd/2;
        life=Shm*1000;
        x=X;y=Y;id=Id;
        eat=Eat;
        type=Lx;
        sp_eat=Beat;
        freeze=0;
        shoot=Fs;
        fs_nd=Fsn*1000;
        fss=sta=ii=du=0;
        eating=false;
        fei=false;
        stp_tm=-1e9;
        xu=1.0;
    }
    void input(QDataStream &ar)
    {
        ar>>life>>spd>>spdy>>eat>>ii>>x_flt;
        ar>>x>>y>>ry>>id>>sta>>stp_tm>>frez_stp;
        ar>>type>>sp_eat>>freeze>>du>>dun;
        ar>>shoot>>fss>>fei>>fs_nd>>xu;
    }
    void output(QDataStream &ar)const
    {
        ar<<life<<spd<<spdy<<eat<<ii<<x_flt;
        ar<<x<<y<<ry<<id<<sta<<stp_tm<<frez_stp;
        ar<<type<<sp_eat<<freeze<<du<<dun;
        ar<<shoot<<fss<<fei<<fs_nd<<xu;
    }
};
struct Bullet
{
    QColor color;//子弹颜色

    int attack;//攻击力
    short du,freeze;//寒冰

    short spd;//移动速度
    short x,y;//位置
    short sx,tx,jian;//起点，终点，溅射范围
    short siz,from;//大小
    short tag;//标记

    bool mh;//魅惑
    bool left;int sy;

    Bullet(){freeze=mh=false;jian=left=du=sx=tx=0;from=tag=-1;}
    void input(QDataStream &ar)
    {
        ar>>color>>spd>>from;
        ar>>x>>y>>attack;
        ar>>siz>>freeze>>du;
        ar>>sx>>tx>>sy>>tag;
        ar>>mh>>jian>>left;
    }
    void output(QDataStream &ar)const
    {
        ar<<color<<spd<<from;
        ar<<x<<y<<attack;
        ar<<siz<<freeze<<du;
        ar<<sx<<tx<<sy<<tag;
        ar<<mh<<jian<<left;
    }
};

struct MyTimer
{
    int id,nxt,time;
    MyTimer(){}
    MyTimer(int Id,int Ne,int Jg)
    {
        id=Id;nxt=Ne;time=Jg;
    }
    void input(QDataStream &ar)
    {
        ar>>id>>nxt>>time;
    }
    void output(QDataStream &ar)const
    {
        ar<<id<<nxt<<time;
    }
};

struct ShiTi
{
    int tm,id;
    short x,y,ty;

    void input(QDataStream &ar)
    {
        ar>>id>>x>>y>>ty>>tm;
    }
    void output(QDataStream &ar)
    {
        ar<<id<<x<<y<<ty<<tm;
    }
};

struct ARC
{
    int a,b,c;ARC(){}
    ARC(int A,int B,int C):a(A),b(B),c(C){}
};

bool operator<(const ARC&a,const ARC&b);

class Game;
class MainWindow;

int GetZwIcon(const Plant &zw);
int GetJsIcon(Zombie &js);
int GetDunIcon(const Zombie &js);
int SpecialEat(Plant &zw,Zombie &js,Game &ga,double spd,double sq);
bool JsShoot(Zombie &js,Game &ga);
bool DaZhao(Plant &zw,MainWindow* wnd);
void QxDaZhao(Plant &zw,MainWindow* wnd,Game &ga);
int MyRand(int cnt[100],int p[100],int n);
int MyRand(double cz[100],int p[100],int n);
int GetMin(int a[100],int n);
int GetMax(int a[100],int n);
Bullet GetFanTan(int id);

extern MainWindow *m_wnd;

#endif // STDAFX_H
