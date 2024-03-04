#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "stdafx.h"
#include "game.h"
#include "mythread.h"
#include "myglwidget.h"
#include <QSignalMapper>
//#include "mytimer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *event)override;
    void mouseReleaseEvent(QMouseEvent *event)override;
    void mouseDoubleClickEvent(QMouseEvent *event)override;
    void keyPressEvent(QKeyEvent *event)override;
    void paintEvent(QPaintEvent *event)override;
    void timerEvent(QTimerEvent *event)override;
    void showEvent(QShowEvent *event)override;
    void closeEvent(QCloseEvent *ev)override;
    void Drag(QPoint pt);
    void KeyDown(int z);
    void DoDraw();
    void DrawLeft(QPainter&memdc)const;
    void StartGame(int lx);
    int GetNdTy();
    void SolveJsNd(bool rev=0);
    void SolveZwNd(bool rev=0);
    void SolveNd();
    void MakeBo(int,int);
    void MakeZMBP(int gq,int gq_nd,int lx,int Lx);
    int MakeGq(int gq);
    bool AddPlant(int x,int y);
    bool AddPlant(QPoint pt);
    bool AddJs(QPoint pt);
    void SetTimer(int id,int jg);
    void KillTimer(int);
    void NewTimer(int id,int jg,int ne);
    void DoTimer(int);
    void GenerateJs();
    int GenerateY();
    void NextBo();
    void MakeBoJs();
    void Init();
    void InitZw();
    void InitJs();
    void InitIcon();
    void InitOneIcon(int i);
    void Load(QDataStream &ar);
    void Save(QDataStream &ar);
    void ShowGameButton();
    void EnableButton();
    void DisableButton();
    QPoint SPoint(int x,int y);
    void left2048();
    void right2048();
    void up2048();
    void down2048();
    void SolveDz(QPoint pt);
    void WakeUp(int y);
    void HuiFang(bool hs);
    bool DoWSAD(int,int,int,int);
    void SolveWSAD(int,int);
    void ChangeZw();
    void Retina();
    void SwitchSun();
    void ShowLife();
    int CalScore(QString &);
    void MakeImjs();
    void MakePreset();
    void CheckPao(QPoint pt);
    void SolveDig(QPoint pt);
    void SolveSel(QPoint pt);
    void OnStart();
    QString GetGqStr(int gg);
    void RestoreFromFile(const QString&na);
    void SendInt(int x);
    void SendInt(int x,int y);
    void OnSevChange();
    void SendGameData();
    ull Hash(QByteArray&a,ull ck=-1);
    void AddHash(QByteArray&a);
    bool CheckHash(QByteArray&a);
    void ReadDelta(QDataStream&da);
    void SendDeltaWSAD(int,int,int,int);
    void SendDeltaPlantZw(const Plant &zw);
    void SendDeltaDig(int,int);
    void SendDeltaDz(int);
    void SendDeltaNewJs(const Zombie&js);
    void SendDeltaExpYg(int expad,int ygad);
    void SendDeltaUse(int x);
    void SwiXuan();
    void AdjustZMBP(int ty);
    bool TypeStr(const char*s);
    void SaveToFile(QFile&file);
    void ReadFile(QFile&file);
    void SwitchStop();
    int FindTimer(int id);
    int CalTmrTime(int id,int tm);
    void OnTick();
    void FinishInitIcon();
    void SendTick();
    ~MainWindow();

public:
    QTimer icotmr,drtmr,uptmr;//图标，绘图计时器
    QElapsedTimer tim;//主计时器
    int nxt_tmr,m_gq,real_gq,gq_nd,gqe,ext_dif;//关卡，下一计时器，实际关卡，？，附加难度
    Game m_game,an_game;
    Plant plants[MAXSHU];
    Zombie zombies[MAXJSH];
    int tmr_cnt=0,tm_det=0;//计时器数量
    MyTimer tmr[1010];
    int m_nd,m_n;//当前波数，僵尸种类数，上次绘制时间
    int tm_js[MAXND];//僵尸生成间隔
    int tm_lst[MAXND];//阶段持续时间
    int select;//选中的植物
    int sele_x,sele_y;//选中的位置
    int beans;//能量豆数目
    int plt_cnt[ZGQS];//每关植物数
    double cd[MAXSHU];//恢复进度
    int m_xuan[MXUAN],x_js[MXUAN];//选择的植物，僵尸
    int a_xuan[MXUAN],a_xs;//附加卡槽
    bool del_x[MAXSHU];//无法选择的特殊植物
    int zmb_p[MAXJSH];//僵尸概率
    int plt_p[MAXSHU];//植物生成概率
    bool can_wat[30];//（僵尸）是否能进水
    double cd_spd[MAXSHU];//恢复速度
    int m_jwbs[MMN+1];//僵王血量
    int SHU,JSH,ND;//？，僵尸种类总数，波数
    bool is11,is12,is82;//是否为两种BOSS关卡和82
    bool isstop=false;//是否暂停
    int m_lstJ[MMN];
    int m_lstC[MMN];
    int m_lstD[MMN];
    int m_lstT[MMN];
    int m_lstB[MMN];
    int m_lstZ[MMN];
    int m_SumZ[MMN];//以上为僵尸历史记录
    int p_shun[MAXSHU],q_shun[MAXSHU],z_shun[MAXJSH],z_sx[MAXJSH],z_cn[GQS],jsh[MAXND];//植物顺序，僵尸顺序，实际僵尸顺序，每关僵尸数目，每阶段僵尸数目
    int wsad[MAXSHU],wsad2[MAXSHU],j_yg[MAXJSH];//2种wsad，僵尸阳光
    int m_ztm,zom_cnt,m_ea,l48;//总时间，总僵尸数，难度，上次2048时间
    int y_sum,f_shu,z_fs,w_tm,h_sd,v_tm;//狂锤分数，待生成的化肥数目，剩余化肥数目，，回放速度，加速模式虚拟时间
    int lmsx=0,lmsy=0,mi_x,mi_y,ma_x,ma_y;//鼠标x，鼠标y（用于拖动）
    bool m_ai,m_hd,m_imp,m_b48,imjs,chui,mirr,b_rd,b_hui,b_ycl,b_fast,b_zhu,b_guan,can_swi;
    //双倍，AI，困难，不可能，2048，我是僵尸，狂锤，镜面，回放，回放，预处理，短时，柱子，罐子
    bool isdb[MAXND];//是否为旗帜
    double lsZM,m_spd,b_ret;//缩放比例，速度，像素密度
    QPixmap ico[ICCNT+1],ico_bl[ICCNT+1],ico_red[ICCNT+1],ico_blk[ICCNT+1],ico_sha[ICCNT+1];
    //图标，蓝色图标，镜面图标，魅惑图标，？，镜面魅惑，黑色，影子
    QColor main_col[ICCNT+1],mco[ICCNT+1];//主要颜色
    int n_2048[MAXSHU]={0};//2048合成表
    QString font_fam;//字体名称
    int m_zf[5010],zf_len,sun_dou,sun_doued;//输入字符，长度，是否阳光双倍
    int tyToNumP[100],tyToNumZ[100];bool show_life=false;//类型到下标，显示血量
    int zmb_sum[MAXJSH],plt_sum[MAXSHU],stop_cnt,plt11s;
    long long cnt_sum,dang_sum;bool arc;
    int arc_tm[1000],arc_wz[1000],arc_ty[1000],arc_i,arc_cnt;
    bool kp_noguan[MMM][MMN],wait_guan,fir_init=1,zuobi;
    int lszm,fin_TM;
    int wsad_col[MMM][MMN],bean_cst;bool can_wsad,ws_le,fr_mid;
    int ai_0;double ai_1,ai_2;int low_mw,tmid=-1;double xmul;
    MyDraw *draw;QPixmap pil,anpil;QThread *dr_trd;
    std::vector<QTcpSocket*> tcp;QTcpServer serv;bool sender;
    int lstcz,sdcnt;std::vector<QTimer*> rdtmr;bool plajs,duiz;
    std::queue<std::pair<int,Zombie> > q_js;int exp,expnd[MAXSHU],to_an[MAXJSH],od_ico[MAXJSH];
    bool del_js[MAXJSH],auto_fire,crea,b_two;int RXUAN;bool tmr_delay,hiui=0;
    MyGLWidget gl;int drty,js_od[22][12],ercn;bool xie,noztm;QCursor cur;
    int laSI,jsty[MAXJSH],lamul[MAXJSH],dbn,zdbs,sun_tm,usermn,usermm,usersi;
    bool jshd[MAXJSH],b_kjc,b_sun,low_eff,b_snd,sd_et,swizw,sp_plt[MAXSHU],inf_sun;
    double dps_sum[10000],dps_now;bool b_sho;
    QSignalMapper tcpmap,rdmap;

    double js_cz[MAXJSH];int js_nw[1000],js_nwi;

public slots:
    void on_btc_clicked();
    void UpdateIcon();
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
    void GameOver(int ty=0);
    void MakeGuan();
    void SwiMw();
    void Paint(QPixmap _pil,QPixmap _anpil);
    void Repaint();
    void OnConnect();
    void OnReadData(int fr);
    void DoReadData(int fr);
    void OtherPre();
    void SwiDraw();
    void SwiTmrDelay();
    void SwiEff();
    void SwiKJC();
    void SwiSound();
    void RestartGame();
    void LoadLast(bool st=0);
public:
    Ui::MainWindow *ui;
private slots:
    void on_huisu_clicked();
    void on_speed_textChanged(const QString &arg1);
    void on_kehu_triggered();
    void on_fuwu_triggered();
    void on_bt_oth_clicked();
    void on_Short_clicked(bool checked);
};

#endif // MAINWINDOW_H
