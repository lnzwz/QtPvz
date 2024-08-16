#ifndef GAME_H
#define GAME_H

#include "stdafx.h"

class Game //: public QObject
{
    //Q_OBJECT

public:
    Plant plants[400];//植物
    int plt_cnt;//植物数目
    Zombie zombies[MAX_ZMB_CNT];//僵尸
    int zmb_cnt;//僵尸数目
    Bullet bullets[MAX_BUL_CNT];//子弹
    int bul_cnt;//子弹数目
    ShiTi shiti[MAX_ST_CNT];//装饰物
    int shi_cnt;//装饰物数目
    int m_yg;//阳光
    bool is_day,is_fog,is_cycle;//场景
    int gird_type[MMM][MMN],m_hy[MMM][MMN];//是否为坑，荷叶血量
    bool over,started,real;//是否结束,是否开始
    int cars[MMN],car_driv[MMN];//有无车，是否正在用车
    bool no_num,plt_kui,endless,iswa[MMN];//是否正在双指针，AI种葵，是否无尽，是否为水池
    short plt_num[MAXX][MMN];//当前位置的植物
    short pd[MMN][MAX_BUL_CNT],pj[MMN][MAX_ZMB_CNT];//排序用
    int row_zmb[MMN];//每行僵尸数目
    int XS,t_ai,lst[MMN];//AI用
    int le_ng[MMM][MMN],m_ice[MMM][MMN];//南瓜，吃南瓜时间，冰道
    int expad,ygad,lstcar,lstup,lstfan,js_nd[MMM][MMN];
    int jd,zjd,car_TM[MMN];std::vector<int> vdb;
    double lmw;
    int laygTM,fans,dps,la_mvzd;
    int zmb_ii[MAX_II_Z],iis_z,plt_ii[MAX_II_P],iis_p;
    double m_ng[MMM][MMN];
    std::vector<int> zmb_de,plt_de;
    Pool zd_pol;
    std::bitset<MAX_II_Z> zd_atk[500];
    bool nd_calfr;
    int car_num,car_ryg;
    bool lamw[MMM*2][MMN*2];QPixmap lamw_pic;
    int js_xsum,js_sum,dang_cnt,car_use,js_car,zw_eat;

    int wz_fei[MMM][MMN];
    double spd_mul[MMM][MMN],lf_mul[MMM][MMN];
//用于优化
    bool eat_atk[MAX_ID_CNT],is_tou[MAX_ID_CNT],low_zw[MAX_ID_CNT],yg_rtn[MAX_ID_CNT];
    bool has_lev[MAX_ID_CNT],cantwo[MAX_ID_CNT],frez_fsh[MAX_ID_CNT],has_mouth[MAX_ID_CNT];
    bool is_shear[MAX_ID_CNT],is_zha[MAX_ID_CNT],zw_wudi[MAX_ID_CNT],can_overzw[MAX_ID_CNT];
    bool jianguo[MAX_ID_CNT];
    double fire_mul[MAX_ID_CNT],frez_spd[1000],dun_p[MAX_ID_CNT],dian_mul[MAX_ID_CNT],dang_mul[MAX_ID_CNT];
    //double lev_mul[10]={1.0,1.3,1.8,2.0,2.4,2.6,3.0,3.3,3.5,4.0};
public:
    Game();
    virtual ~Game();
    void ClearGrid();
    void MakeSI(int lx,int usermn,int usermm,int usersi);
    void CreateGame(int lx);
    void KillAll();
    bool ZombieInide(int i,int xl,int xr,int yl,int yr,bool ck100=true);
    int KillJsLine(int y,const HURT&hu,int from=-1);
    int KillJs(int l,int r,int y,const HURT&hu,int from=-1,bool jian=0);
    int KillJsLR(int x,int y,double ls,double rs,const HURT&hu,int from=-1,bool jian=0);
    int KillJsLRUD(int x,int y,int ls,int rs,int us,int ds,const HURT&hu,int from=-1);
    int KillJsFW(int x,int y,int fw,const HURT&hu,int from=-1);
    int KillJsPos(int x, int y, double fw,const HURT&hu,int from=-1);
    bool KillJsLie(int x, int kill,int ct=0);
    bool CannotReach(const Bullet*zd,int zi)const;
    void SpecialPlt(int z_i);
    void CalFirst();
    void GetZwTrans(QPainter &memdc,const Plant&zw,int&hi,int&wi)const;
    int GetJsDrawY(const Zombie&js,double&aa);
    void DrawZwPic(QPainter &memdc,const Plant&zw,int wi,int hi,const QPixmap*ic)const;
    void DrawZw(QPainter &memdc);
    void DrawZwLife(QPainter &memdc);
    void DrawShadow(QPainter &memdc,double tt);
    void DrawJs(QPainter &memdc,const Zombie&js);
    void DrawJs(QPainter &memdc);
    void DrawZd(QPainter &memdc)const;
    int GetZdIcon(const Bullet&zd)const;
    void DrawZdPic(QPainter &memdc,double x,double y,double rot,double p, int id)const;
    void DrawBall(QPainter &memdc,int x,int y,QColor co,int r,int ty=0,int du=0,int hb=0)const;
    void DrawGradientLine(QPainter &memdc, double x, double y, double wi, double h1, double h2, double h3, QColor co1, QColor co2)const;
    void DrawXueHua(QPainter &memdc, QColor co, double ox, double oy, double r, int s, double ad=0)const;
    void DrawMw(QPainter &memdc,double);
    void DrawSt(QPainter &memdc)const;
    void DrawRandomPoints(QPainter &memdc,QRect re,int n,QColor co,int st=0,double si=1.6,int ty=0)const;
    QColor GetLifeColor(double x)const;
    double GetFogAlpha()const;
    void Draw(QPainter &memdc,bool an=0);
    void RemoveExtraSt();
    bool IsIcy(int x,int y)const;
    bool Js_On_Grid(int shu);
    void MagicZombie(int shu);
    bool JsInHome(int shu);
    bool MoveJs(int shu);
    void MoveAllZd(double spd=1.0);
    int GetFirstJs(int x,int y,const Bullet*zd=NULL,bool ck100=true);
    int GetLastJs(int x,int y,const Bullet*zd=NULL,bool ck100=true);
    int GetZwShu(int x,int y);
    bool HurtZw(int x,int y,double et,bool upid);
    bool HurtZw(int i,double et,bool upid);
    bool HurtJs_Fast(int i,int ki,int rea=-1,int co=1,int ct=10);
    bool HurtJs(int i,int ki,int rea=-1,int co=1,short frez=0,int ct=0,double fire=0,int du=0,double xu=0,double q=0);
    bool HurtJs(int i,int rea,int co,const HURT&hu);
    void FreezeJs(int i,short frez,int stp=0);
    void FreezeStopJs(int i,int tm);
    void HurtZw_Ty(int x,int y,double et,int ty=0,bool st=0);
    bool Eat(int wx,int wy,int jshu,double sq);
    void DestroyGuan(int z);
    void DestroyZw(int x,int y,int dig);
    void DestroyZw_Pre(int w,int dig);
    int DestroyZw(int w,int dig);
    void DestroyJs(int w,int rea=-1);
    void DestroyZd_Pre(const Bullet&zd);
    void DestroyZd(int w);
    void DestroySt(int w);
    void Goj(int zshu);
    bool PlantZw(Plant zw);
    bool PlantZw(Plant zw,int x,int y);
    void NewZw(Plant zw);
    void NewJs(Zombie js);
    void NewZd(Bullet zd);
    void NewSt(ShiTi st);
    void KillZw(int xl,int xr,int yl,int yr,double et,int ty=0,bool st=0,double kp=0);
    void KillZwPos(int x,int y,double et,int ty=0,bool st=0,double kp=0);
    void Explode(int x,int y,int r,double hu,double de);
    void AI();
    int Valid(int x,int sy)const;
    int Find(int*,int)const;
    int FakeKill(int,int,int,int);
    int FakeKillL(int y,int kill)const;
    bool UnderGaint(int x,int y)const;
    bool UnderCar(int x,int y)const;
    bool BeforeShe(int x,int y);
    int Find(int zz[20],int n,int hg)const;
    bool NearGaint(int x,int y)const;
    bool NearCar(int x,int y)const;
    bool UnderMonth(int x,int y)const;
    bool NearQi(int x,int y)const;
    bool NearCre(int x,int y)const;
    bool NearCGT(int x,int y)const;
    void input(QDataStream &);
    void output(QDataStream &);
    bool MeiHuo(int i);
    void ChangeLev(Plant &zw,int f);
    void ClearLev(Plant&zw);
    void wsad_AI();
    double wsad_cal(int sz[5][9]);
    double GetNightAlpha();
    void NewRedPoint(int i,int si=1,bool show_num=1,int co=1);
    void NewNumberPoint(int x,int y,int z,int co=1);
    void NewPlantPoint(int i,int si=1);
    void NewNgPoint(int x,int y,int si=1);
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
    void AddPlantWzEff(Plant&zw);
    void AddPlantWzEff(int x,int y);
    void ClearPlantWzEff(Plant&zw);
    void ClearPlantWzEff(int x,int y);
    bool BulletOverZw(int bi);
    int CountFei(int x,int y);
    void NewPicSt(int x,int y,int id,int tm=80);
    void NewPicStL(int y,int id,int tm=80);
    void DuTick();
    void DianTick();
    void BulletOnJs(int bi,int zi,bool tou);
    void LineXu(int x,int y,double xu);
    int CalHurt(int zi,int ki,int frez,double fire,int du,double xu,double q,int ct,int ft=0);
    void MianYiCheck();
    bool ShiFei(int x,int y);
    int FakePlant(const Plant&zw);
    void MhJsSkillTick();
    int SpecialEat(int z_i,int j_i,double sq);
    bool JsShoot(int j_i);
    bool DaZhao(int z_i,double p=1.0);
    void EndDaZhao(int z_i);
    void ResetZwTimer(int z_i);
    void DelExtraZmbPlt();
    bool Zd_At_Sky(const Bullet&zd)const;
    bool Js_At_Sky(int zi)const;
    bool Zd_Extra(const Bullet&zd);
    void Drive_Car(int y);
    bool ZdHitJs(const Bullet&zd,int ji);
    void DoZwAnimine(int tm,double spd=1.0);
    void FlagTick();
    bool ZombieEatPlant(int j_i,int x,int y);
    Bullet BulletFromZw(int zi);
    double CalScoreP();
    int GetMWCGoal();
    double CalJSQ(int x,int y);
    int ColorToFrom(const QColor&co)const;
    void RightKey(int w,QPoint pt);
    void ChuiTick();
};

#endif // GAME_H
