#ifndef STDAFX_H
#define STDAFX_H

#define NOSOUND
#define TEST_MODE
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
#include <stack>
#include <bitset>

#define MAXSHU 103
#define MXUAN 12
#define MAXXUAN 24
#define MAXJSH 29
#define MAXND 1000
#define GQS 10
#define DBS 0.45
#define MANG 9
#define MAXGQS 60
#define MMN 22
#define MMM 39
#define ICCNT 282
#define eps 1e-9
#define ZGQS MAXGQS+25
#define MAXCZ 2
#define FW 0.51
#define MAXDZS 99
#define MAXBJ 6
#define MAXX 2000
#define MAXFREZ 2000
#define MAXXU 5.0
#define MAXDU 3000
#define G 0.15
#define BGCNT 5

#define RED QColor(255,0,0)
#define GREEN QColor(0,190,0)
#define YUMI QColor(175,175,0)
#define BUTTER QColor(170,170,0)
#define BLUE QColor(0,190,255)
#define YELLOW QColor(180,180,0)
#define DBLUE QColor (80,32,255)
#define MHCO QColor(240,0,150)
#define PURPLE QColor (255,0,255)
#define DUCO QColor(240,230,140)
#define DIANCO QColor(0,239,147)
#define PINK QColor(255,128,236)
#define LBLUE QColor(62,229,221)
#define LEFTCO Qt::transparent//QColor(128,42,42)

#define GL48 8
#define ull unsigned long long
#define inf 99999999

#define MAX_BUL_CNT 5000
#define MAX_ZMB_CNT 1000
#define MAX_ST_CNT 2000
#define MAX_ID_CNT 250

#define MAX_II_P 10000
#define MAX_II_Z 50000

#define HOME_X -115
#define TOUROTT 1600
#define MAX_TMR_TIME 1000000

#define RONG_P 2.5
#define FIRE_DU 5.0
#define DU_P 1.0
#define XU_HF 0.0015
#define DU_XUHF 0.003
#define DIAN_HURT 30000
#define DIAN_TRANS 0.01

extern int RXUAN,XUAN,TM,MM,MN,SI,FIRETM,ME,HI,W0,W1,W2,TOP;
extern double dan[100],zoom,sinra[10000],cosra[10000],PI,XW,YW,XT,YT;
extern QPixmap *ico,*ico_bl,*ico_red,*ico_blk,*ico_sha;
extern std::mt19937 RA;
extern QString path;
extern bool pub_bl[1000];
extern QSize ico_si[ICCNT+1];
class Zombie;
int ClearDun(Zombie&js);

struct HURT
{
    short du,frez,ct;
    int atk;double xu,fire,q;
    HURT(){}
    HURT(int Atk,short Frez=0,short Du=0,short Ct=0,double Fire=0,double Xu=0,double Q=0)
    {
        atk=Atk;xu=Xu;q=Q;
        frez=Frez;du=Du;
        ct=Ct;fire=Fire;
    }
    void mul(double p)
    {
        du*=p;frez*=p;q*=p;
        atk*=p;xu*=p;fire*=p;
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
    int ii;

    short num,def;//下标，防御
    short x,y;//位置
    short id;//图片id
    short type;//唯一标记
    short siz;//子弹大小
    short grow;//生长
    short fire,du,freeze;//火焰
    short frez_stp;

    bool anti_fire,ti;//是否寒冰
    bool mush,left,stop;//蘑菇,向左

    short move_spd;//移动速度(子弹)
    short jian,lev,bj,levod;//溅射，化肥

    double mul,tr;

    Plant(){nxt_tm=-1;left=fire=frez_stp=0;tr=0;mul=1;}
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
        freeze=(Hb?100:0);
        mush=Mg;
        grow=0;la_et=-1e9;
        tag=jian=lev=0;nxt_tm=-1;
        fire=ti=levod=0;
        anti_fire=stop=0;
        odtm=dzsy=def=0;bj=1;
        mul=1;num=-1;
    }
    void input(QDataStream &ar)
    {
        ar>>color>>attack;
        ar>>time>>dztm>>odtm>>dzsy;
        ar>>move_spd>>life;
        ar>>x>>y>>id>>frez_stp;
        ar>>cost>>type;
        ar>>siz>>grow>>def;
        ar>>freeze>>mush>>du;
        ar>>tag>>nxt_tm>>mul;
        ar>>jian>>lev>>left;
        ar>>fire>>mxlf>>ti;
        ar>>anti_fire>>stop>>bj;
        ar>>ii>>tr;
    }
    void output(QDataStream &ar)const
    {
        ar<<color<<attack;
        ar<<time<<dztm<<odtm<<dzsy;
        ar<<move_spd<<life;
        ar<<x<<y<<id<<frez_stp;
        ar<<cost<<type;
        ar<<siz<<grow<<def;
        ar<<freeze<<mush<<du;
        ar<<tag<<nxt_tm<<mul;
        ar<<jian<<lev<<left;
        ar<<fire<<mxlf<<ti;
        ar<<anti_fire<<stop<<bj;
        ar<<ii<<tr;
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
    int life,dun;//生命值，盾量
    int du,frez_stp;//中毒，冻结
    int num,stp_tm,ii;//下标，停止时间，标号
    int fss,fs_nd;//发射延迟
    int mxlf,mxdun;

    double x_flt,y_flt,vx,vy;//移动速度
    double xu,eat,q;//虚弱，吃速度，电量
    short x,y;//位置
    short id;//图片id
    short type;//编号
    short freeze;//寒冰
    short tag;//状态
    short eat_jl;//嘴的长度

    bool shoot;//是否发射
    bool sp_eat;//特殊吃法
    bool eating,fei;//正在吃，化肥有无

    Zombie(){q=vy=dun=0;x_flt=y_flt=-1;frez_stp=-1;}
    int Hurt(int hu,double ct)
    {
        int dh=int(hu*(1-ct)),rt=0;
        if(dh<0)dh=0;
        if(dh>dun)dh=dun;
        hu-=dh;dun-=dh,rt+=dh;
        if(dun<=0&&dh>0)
            id=ClearDun(*this);
        rt+=std::min(life,hu);
        life=std::min(life-hu,mxlf*2);
        return rt;
    }
    void Create(double Sd,int Shm,double Eat,int X,int Y,int Id,int Lx,bool Speat=false,bool Fs=false,int Fsn=0)
    {
        vx=Sd/2;
        mxlf=life=Shm*1000;
        x=X;y=Y;id=Id;
        eat=Eat;
        type=Lx;
        sp_eat=Speat;
        freeze=0;
        shoot=Fs;
        fs_nd=Fsn*1000;
        fss=tag=ii=du=0;
        eating=false;
        fei=false;
        stp_tm=-1e9;
        xu=1.0;num=-1;
        eat_jl=SI/2;
    }
    void setspd(int x,bool fu=0)
    {
        if(fu)vx=x;
        else vx=(vx>=0?x:-x)/2.0;
    }
    void input(QDataStream &ar)
    {
        ar>>life>>vx>>vy>>eat>>ii>>x_flt>>eat_jl;
        ar>>x>>y>>y_flt>>id>>tag>>stp_tm>>frez_stp;
        ar>>type>>sp_eat>>freeze>>du>>dun;
        ar>>shoot>>fss>>fei>>fs_nd>>xu>>mxlf;
    }
    void output(QDataStream &ar)const
    {
        ar<<life<<vx<<vy<<eat<<ii<<x_flt<<eat_jl;
        ar<<x<<y<<y_flt<<id<<tag<<stp_tm<<frez_stp;
        ar<<type<<sp_eat<<freeze<<du<<dun;
        ar<<shoot<<fss<<fei<<fs_nd<<xu<<mxlf;
    }
};
struct Bullet
{
    QColor color;//子弹颜色

    int attack,vis;//攻击力，访问的僵尸
    short du,freeze;//毒性，寒冰

    short x,y;//位置
    short tx,jian;//终点，溅射范围
    short siz,from;//大小
    short tag,fire;//标记，火焰(*10)

    bool mh,left,ct,xie;//魅惑，反向，穿透僵尸，斜线

    double vx,vy,x_flt,y_flt,xu,q;//位置，移动速度

    Bullet(){freeze=mh=false;jian=left=du=tx=fire=ct=xie=0;vis=tag=-1;from=1;x_flt=y_flt=-1;vy=xu=q=0;}
    void setspd(double spd,double theta)
    {
        left=0;
        vx=spd*cos(theta);
        vy=spd*sin(theta);
        if(vx<0)vx=-vx,left=1;
    }
    double gettheta()const
    {
        double dx=vx,dy=vy;
        if(left)dx=-vx;
        return atan2(dy,dx);
    }
    double getspd()const
    {
        return sqrt(vx*vx+vy*vy);
    }
    void mul(double p)
    {
        attack*=p;du*=p;
        fire*=p;q*=p;
        xu*=p;jian*=p;
        freeze*=p;
    }
    void move(double spd)
    {
        if(left)x_flt-=vx*spd;
        else x_flt+=vx*spd;
        y_flt+=vy*spd;
    }
    void input(QDataStream &ar)
    {
        ar>>color>>from>>xu;
        ar>>x>>y>>attack>>fire;
        ar>>siz>>freeze>>du;
        ar>>tx>>vis>>tag;
        ar>>jian>>mh>>left;
        ar>>ct>>xie;
        ar>>x_flt>>y_flt>>vx>>vy;
    }
    void output(QDataStream &ar)const
    {
        ar<<color<<from<<xu;
        ar<<x<<y<<attack<<fire;
        ar<<siz<<freeze<<du;
        ar<<tx<<vis<<tag;
        ar<<jian<<mh<<left;
        ar<<ct<<xie;
        ar<<x_flt<<y_flt<<vx<<vy;
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

struct Pool
{
    std::vector<int> st;
    int sl;
    Pool(){sl=0;}
    int New()
    {
        if(st.empty())
            return sl++;
        else
        {
            int rt=st.back();
            st.pop_back();return rt;
        }
    }
    void Del(int x)
    {
        st.push_back(x);
    }
    void clear()
    {
        st.clear();
        sl=0;
    }
    void input(QDataStream &ar)
    {
        ar>>sl;int s;
        ar>>s;st.resize(s);
        for(int i=0;i<s;i++)
            ar>>st[i];
    }
    void output(QDataStream &ar)
    {
        ar<<sl<<int(st.size());
        for(int i=0;i<st.size();i++)
            ar<<st[i];
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
int MyRand(int cnt[100],int p[100],int n);
int MyRand(double cz[100],int p[100],int n);
int GetMin(int a[100],int n);
int GetMax(int a[100],int n);
Bullet GetFanTan(int id);
std::pair<double,double> solve_ecfc(double a,double b,double c);

extern MainWindow *m_wnd;

#endif // STDAFX_H
