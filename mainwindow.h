#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QMouseEvent>
#include <QTimer>
#include <QTime>
#include <QPainter>
#include <QElapsedTimer>
#include <QFileDialog>
#include <QOpenGLWidget>
#include <QButtonGroup>
#include <cmath>
#include <QOpenGLFunctions>

#include "stdafx.h"
#include "choose.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct SZhw
{
    QColor color;//子弹颜色
    int goj;//攻击力
    int time;//攻击时间(毫秒)
    int dztm;//能量豆时间(毫秒)
    int sd;//移动速度(子弹)
    int shm;//生命值
    int x,y;//位置
    int id;//图片id
    int yg;//阳光值
    int lx;
    int dx;
    int fu;
    int shu;
    bool hb;//是否寒冰
    bool mg;
    bool zha;
    int nfs,te;
    SZhw(){te=-1;}
    void Create(QColor Color,int Goj,int Time,int Dztm,int Sd,int Shm,int X,int Y,int Id,int Yg,int Lx,int Dx=10,bool Hb=false,bool Mg=false,bool Zha=false)
    {
        color=Color;
        goj=Goj;
        time=Time;
        dztm=Dztm;
        sd=Sd;
        shm=Shm;
        x=X;
        y=Y;
        id=Id;
        yg=Yg;
        lx=Lx;
        dx=Dx;
        hb=Hb;
        mg=Mg;
        fu=0;
        if(Zha)
            fu=Dx;
        zha=Zha;
        nfs=0;te=-1;
    }
    void input(QDataStream &ar)
    {
        ar>>color>>goj;
        ar>>time>>dztm;
        ar>>sd>>shm;
        ar>>x>>y>>id;
        ar>>yg>>lx;
        ar>>dx>>fu;
        ar>>hb>>mg;
        ar>>zha>>nfs>>te;
    }
    void output(QDataStream &ar)
    {
        ar<<color<<goj;
        ar<<time<<dztm;
        ar<<sd<<shm;
        ar<<x<<y<<id;
        ar<<yg<<lx;
        ar<<dx<<fu;
        ar<<hb<<mg;
        ar<<zha<<nfs<<te;
    }
};
struct SJsh
{
    int shm;//生命值
    int sd;//移动速度
    int eat;//吃速度
    int x,y;//位置
    int id;//图片id
    int lx;//编号
    bool beat;//是否跳
    int hb;//寒冰
    bool fu;
    bool fs;//是否发射
    int fss;//发射延迟
    int shu;
    bool b_et;
    void Create(int Sd,int Shm,int Eat,int X,int Y,int Id,int Lx,bool Beat=false,bool Fs=false)
    {
        sd=Sd;
        shm=Shm;
        x=X;
        y=Y;
        id=Id;
        eat=Eat;
        lx=Lx;
        beat=Beat;
        hb=100;
        fu=false;
        fs=Fs;
        fss=0;
        b_et=false;
    }
    void input(QDataStream &ar)
    {
        ar>>shm>>sd>>eat;
        ar>>x>>y>>id;
        ar>>lx>>beat>>hb;
        ar>>fu>>fs>>fss;
    }
    void output(QDataStream &ar)
    {
        ar<<shm<<sd<<eat;
        ar<<x<<y<<id;
        ar<<lx<<beat<<hb;
        ar<<fu<<fs<<fss;
    }
};
struct SZid
{
    QColor color;//子弹颜色
    int sd;//移动速度
    int x,y;//位置
    int goj;//攻击力
    int dx;//大小
    bool hb,tes,mh;
    int sx,tx,zx,zy;
    SZid(){tes=mh=hb=false;}
    void input(QDataStream &ar)
    {
        ar>>color>>sd;
        ar>>x>>y>>goj;
        ar>>dx>>hb;
        ar>>sx>>tx>>zx>>zy>>mh;
    }
    void output(QDataStream &ar)
    {
        ar<<color<<sd;
        ar<<x<<y<<goj;
        ar<<dx<<hb;
        ar<<sx<<tx<<zx<<zy<<mh;
    }
};

struct STm
{
    int id,ne,jg;bool cf;double yu;
    STm(){}
    STm(int Id,int Ne,int Jg,double Yu)
    {
        id=Id;ne=Ne;jg=Jg;cf=false;yu=Yu;
    }
    void input(QDataStream &ar)
    {
        ar>>id>>ne>>jg;
    }
    void output(QDataStream &ar)
    {
        ar<<id<<ne<<jg;
    }
};

class CGame : public QObject
{
    Q_OBJECT

public:
    MainWindow* m_wnd;
    SZhw m_zhw[300];//植物
    int m_zshu;//植物数目
    SJsh m_jsh[1000],tmj[1000];//僵尸
    int m_jshu;//僵尸数目
    SZid m_zid[1000],tmz[1000];//子弹
    int m_dshu;//子弹数目
    int m_yg,m_cc;//阳光
    int m_lx;
    int m_keng[MMM][MMN];//是否为坑
    bool m_over;
    bool m_ks;
    bool m_car[MMN],m_cing[MMN];
    bool m_hb;
    int fr[MMM*MSI+100][MMN];
    int pd[MMN][1010],pj[MMN][1010];
    int yd[1010],yj[1010];
    int hjs[MMN];
    int XS,bh[MMM*MSI+100][MMN];
    int t_ai,lst[MMN];
    bool SZZ,zk,b64,iswa[MMN];
    int m_ng[MMM][MMN],m_dc[MMM][MMN],m_ice[MMM][MMN];
    int lalj[MMN],lazd[MMM][MMN],lawg[MMM][MMN];
public:
    CGame();
    virtual ~CGame();
    void CreateGame(int lx);
    void KillAll();
    bool KillJsLine(int y,int kill);
    bool KillJs(int l,int r,int y,int kill);
    bool KillJs(int x, int y, int kill);
    bool NotZw(int shu);
    void CalZw();
    void Draw(QPainter &memdc);
    bool MoveJs(int shu);
    bool MoveZd(int shu);
    int GetFirstZw(int x,int y);
    int GetFirstJs(int x,int y);
    int GetZwShu(int x,int y)const;
    bool Eat(int zshu,int jshu);
    void DestroyZw(int x,int y);
    int DestroyZw(int shu);
    void DestroyJs(int shu);
    void DestroyZd(int shu);
    bool Goj(int zshu);
    bool NewZw(SZhw zw);
    void NewJs(SJsh js);
    void NewZd(SZid zd);
    void Crepeer(int i);
    void MoveAllZd();
    void AI();
    int Valid(int x,int sy);
    int Find(int*,int);
    int FakeKill(int,int,int,int);
    int FakeKillL(int y,int kill);
    bool UnderGaint(int x,int y);
    bool UnderCar(int x,int y);
    bool BeforeShe(int x,int y);
    int Find(int zz[20],int n,int hg);
    bool NearGaint(int x,int y);
    bool NearCar(int x,int y);
    bool UnderMonth(int x,int y);
    void input(QDataStream &);
    void output(QDataStream &);
    bool meihuo(int i);
    QSize MSize(int x,int y);
    QPoint MPoint(int x,int y);
};

class MyGLWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *event);
    void DoDraw();
    void StartGame(int lx);
    int MakeGq(int gq);
    bool Plant(int x,int y);
    bool Plant(QPoint pt);
    void SetTimer(int id,int jg,int *nul);
    void KillTimer(int);
    void NewTimer(int id,int jg,int ne);
    bool DoTimer(int);
    void Init();
    void ResetTimer();
    void Load(QDataStream &ar);
    void Save(QDataStream &ar);
    void ShowButton();
    void Initicon();
    QSize MSize(int x,int y);
    QPoint MPoint(int x,int y);
    QPoint SPoint(int x,int y);
    void left2048();
    void right2048();
    void up2048();
    void down2048();
    void SolveDz(QPoint pt);
    ~MainWindow();

public:
    QTimer timer;
    QElapsedTimer tim;
    int cnt=0,m_gq,nid,r_gq;
    CGame m_game;
    SZhw m_zw[MAXSHU];
    SJsh m_js[MAXJSH];
    int m_tms=0;
    STm tmr[1010];
    int m_nd,m_n,lad=0;
    int m_time[MAXND];
    int m_jg[MAXND];
    int m_nzw;
    int m_xx,m_xy;
    int m_dzs;
    int m_zws[MAXGQS+4];
    double m_en[MAXSHU];
    int m_xuan[MXUAN];
    int m_p[MAXJSH];
    bool b_wa[20];
    double m_hf[MAXSHU];
    int m_jwbs[MMN];
    int SHU,JSH,ND;
    bool m_b11,m_b12;
    bool m_hbzw,b_zt=false;
    int jd,tm;
    int m_lstJ[MMN];
    int m_lstC[MMN];
    int m_lstD[MMN];
    int m_lstT[MMN];
    int m_lstB[MMN];
    int wsad[MAXSHU];
    int m_ztm,zj,m_ea,l48;
    int l_x=0,l_y=0;
    bool DOU,m_ai,m_hd,m_tui,m_b48;
    double m_sf=1;
    QPixmap ico[150],icd[150],icm[150];
    MyGLWidget *gl,*gx;
    int n_2048[MAXSHU]={0};
    int sum=0,sums=0;

public slots:
    void on_btc_clicked();
    void OnTimer();
    void on_stop_clicked();
    void on_lineEdit_textEdited(const QString &arg1);
    void on_save_clicked();
    void on_open_clicked();
    void on_Auto_stateChanged(int arg1);
    void on_buy_clicked();
    void on_quick_clicked();
    void jiasu();
    void jiansu();
    void ViewStatus();
    void Quit();
    void GameOver();

public:
    Ui::MainWindow *ui;
};

class MyGLWidget : public QOpenGLWidget
{
public:
    MainWindow *m_wnd;
public:
  MyGLWidget(QWidget *parent) : QOpenGLWidget(parent) { }
  void paintEvent(QPaintEvent *e);
  QSize MSize(int x,int y);
  QPoint MPoint(int x,int y);
  int ty;

protected:
  void initializeGL()
  {
  }

  void resizeGL(int w, int h)
  {
  }

  void paintGL()
  {
      //QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
      //f->glClear(GL_COLOR_BUFFER_BIT);
  }

};

#endif // MAINWINDOW_H
