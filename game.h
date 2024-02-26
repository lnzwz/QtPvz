#ifndef GAME_H
#define GAME_H

#include "stdafx.h"

class Game //: public QObject
{
    //Q_OBJECT

public:
    Plant plants[400];//植物
    int plt_cnt;//植物数目
    Zombie zombies[800];//僵尸
    int zmb_cnt;//僵尸数目
    Bullet bullets[1000],bul_nw[200];//子弹
    int bul_cnt;//子弹数目
    ShiTi shiti[2000];//装饰物
    int shi_cnt;//装饰物数目
    int m_yg;//阳光
    bool is_day,is_fog,is_cycle;//场景
    int m_keng[MMM][MMN],m_hy[MMM][MMN];//是否为坑，荷叶血量
    bool over,started,real;//是否结束,是否开始
    bool car[MMN],car_driv[MMN];//有无车，是否正在用车
    bool SZZ,no_num,zk,endless,iswa[MMN];//是否正在双指针，AI种葵，是否无尽，是否为水池
    short fir_plt[MAXX][MMN];//向左第一个植物
    short plt_num[MAXX][MMN];//当前位置的植物
    short pd[MMN][1000],pj[MMN][1000];//排序用
    int row_zmb[MMN];//每行僵尸数目
    int XS,t_ai,lst[MMN];//AI用
    int le_ng[MMM][MMN],m_ice[MMM][MMN];//南瓜，吃南瓜时间，冰道
    int expad,ygad,lstcar,lstup,lstfan,js_nd[MMM][MMN];
    int jd,zjd,car_TM[MMN];std::vector<int> vdb;
    double lmw,spd_mul[MMM][MMN];
    int laygTM,iinum[100000],iis,fans,wz_fei[MMM][MMN],dps;
    double m_ng[MMM][MMN];
//用于优化
    bool eat_atk[100],is_tou[100],low_zw[100],yg_rtn[100];
    bool has_lev[100],cantwo[100]={0};
    double fire_mul[100],frez_spd[1000],frez_spd_q[1000],dun_p[100];//,hb_mul[100];
    double lev_mul[10]={1.0,1.3,1.8,2.0,2.4,2.6,3.0,3.3,3.5,4.0};
public:
    Game();
    virtual ~Game();
    void ClearCd();
    void MakeSI(int lx,int usermn,int usermm,int usersi);
    void CreateGame(int lx);
    void KillAll();
    int KillJsLine(int y,int kill,int ct=1,bool fire=0,int from=-1);
    int KillJs(int l,int r,int y,bool sky,const HURT&hu,bool jian=0,int from=-1);
    int KillJsLR(int x,int y,int ls,int rs,bool sky,const HURT&hu,bool jian=0,int from=-1);
    int KillJsPos(int x, int y, double fw,bool sky,const HURT&hu,int from=-1);
    bool KillJsLie(int x, int kill,int ct=1);
    bool CannotReach(int zx,int i)const;
    void SpecialPlt(int shu);
    void CalFirst();
    void GetZwTrans(QPainter &memdc,const Plant&zw,int&hi,int&wi)const;
    int GetJsDrawY(const Zombie&js,double&aa)const;
    void DrawZwPic(QPainter &memdc,const Plant&zw,int wi,int hi,const QPixmap*ic)const;
    void DrawZw(QPainter &memdc)const;
    void DrawZwLife(QPainter &memdc)const;
    void DrawShadow(QPainter &memdc,double tt)const;
    void DrawJs(QPainter &memdc,const Zombie&js)const;
    void DrawJs(QPainter &memdc)const;
    void DrawZd(QPainter &memdc,int hx,int hy,int si,int fr,bool isle,QColor co,bool du)const;
    void DrawZd(QPainter &memdc)const;
    void DrawBall(QPainter &memdc,int x,int y,QColor co,int r,int ty=0)const;
    void DrawMw(QPainter &memdc,double)const;
    void DrawSt(QPainter &memdc)const;
    void DrawRandomPoints(QPainter &memdc,QRect re,int n,QColor co,int st=0,double si=1.6)const;
    QColor GetJsColor(double x)const;
    double GetFogAlpha()const;
    void Draw(QPainter &memdc,bool an=0)const;
    void RemoveExtra();
    bool IsIcy(int x,int y)const;
    bool JsCai(int shu);
    void ZhangNan(int shu);
    bool JsInHome(int shu);
    bool MoveJs(int shu);
    void MoveAllZd();
    int GetFirstZw(int x,int y);
    int GetFirstJs(int x,int y);
    int GetLastJs(int x,int y);
    int GetZwShu(int x,int y)const;
    bool HurtZw(int x,int y,double et,bool upid);
    bool HurtZw(int i,double et,bool upid);
    bool HurtJs_Fast(int i,int ki,int rea=-1,int ct=0);
    bool HurtJs(int i,int ki,int rea=-1,short frez=0,int ct=1,int fire=0);
    void FreezeJs(int i,short frez,int stp=0);
    void FreezeStopJs(int i,int tm);
    void ShootZw(int x,int y,int et);
    bool Eat(int zshu,int jshu,double spd,double sq,bool snd=0);
    void DestroyGuan(int z);
    void DestroyZw(int x,int y,int dig);
    int DestroyZw(int w,int dig);
    void DestroyJs(int w,int rea=-1);
    void DestroyZd(int w);
    void DestroySt(int w);
    void Goj(int zshu);
    bool PlantZw(Plant zw);
    void NewZw(Plant zw);
    void NewJs(Zombie js);
    void NewZd(Bullet zd);
    void NewSt(ShiTi st);
    void Crepeer(int i);
    void Explode(int x,int y,int r,int hu,int de);
    void AI();
    int Valid(int x,int sy)const;
    int Find(int*,int)const;
    int FakeKill(int,int,int,int)const;
    int FakeKillL(int y,int kill)const;
    bool UnderGaint(int x,int y)const;
    bool UnderCar(int x,int y)const;
    bool BeforeShe(int x,int y)const;
    int Find(int zz[20],int n,int hg)const;
    bool NearGaint(int x,int y)const;
    bool NearCar(int x,int y)const;
    bool UnderMonth(int x,int y)const;
    bool NearQi(int x,int y)const;
    bool NearCre(int x,int y)const;
    bool NearCGT(int x,int y)const;
    void Load(QDataStream &);
    void Save(QDataStream &);
    bool MeiHuo(int i);
    void ChangeLev(Plant &zw,int f);
    void ClearLev(Plant&zw);
    void wsad_AI();
    double wsad_cal(int sz[5][9]);
    double GetNightAlpha();
    void NewRedPoint(int i,int si=1,bool show_num=1);
    void NewNumberPoint(int x,int y,int z,int co=1);
    void NewPlantPoint(int i,int si=1);
    void NewBluePoint(int i,int zz=0);
    int FindZw(int x,int y,int ty);
    void WanQuanTu();
    void FireTick();
    int FindZombie(QPoint pt);
    int LeftZombie(int w);
    int RightZombie(int w);
    void LengJing(int z,int su);
    void NewBaoZha(int x,int y,int z,int t,int sd=SI/14);
    bool NeedBullet(int y);
    void JieDong(int x,int y);
    void MoveZw(int z,int x=-1,int y=-1);
    void PlantOnWz(Plant&zw);
    void BulletOverZw(Bullet&zd);
    int CountFei(int x,int y);
    void NewPicSt(int x,int y,int id,int tm=80);
    void NewPicStL(int y,int id,int tm=80);
    void DuTick();
    void BulletOnJs(int bi,int zi,bool tou);
    void BaDou(int x,int y,double xu);
    int CalHurt(int zi,int ki,int frez,int fire);
    void MianYiCheck();
    bool ShiFei(int x,int y);
    int FakePlant(const Plant&zw);
    Bullet MhJsSkill(int bi);
};

#endif // GAME_H
