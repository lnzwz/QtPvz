#include "./ui_mainwindow.h"
#include "mainwindow.h"
#include "choose.h"

MainWindow::MainWindow (QWidget *parent) :
    QMainWindow (parent),
    ui (new Ui::MainWindow) {
    ::m_wnd=this;
    ui->setupUi (this);
    beans=0;
    low_eff=auto_fire=0;b_kjc=1;
    b_ret=devicePixelRatioF();
    ME=ui->menubar->height();
#ifdef Q_OS_MACOS
    ME=0;
#endif
    zoom=1.0;
    lsZM=usermn=-1;tim.start ();

    ::ico=ico;::ico_bl=ico_bl;
    ::ico_blk=ico_blk;::ico_red=ico_red;::ico_sha=ico_sha;

    connect (&icotmr, &QTimer::timeout, this, &MainWindow::UpdateIcon);//图标刷新（仅在游戏未开始时使用该计时器）
    icotmr.setTimerType (Qt::CoarseTimer);
    icotmr.setSingleShot (false);

    connect (&uptmr, &QTimer::timeout, this, &MainWindow::Repaint);//界面刷新
    uptmr.setTimerType (Qt::CoarseTimer);
    uptmr.setSingleShot (false);uptmr.start(500);

    connect(&serv,&QTcpServer::newConnection,this,&MainWindow::OnConnect);

    drty=1;bgid=4;Init ();//默认不使用多线程绘图

    dr_trd=new QThread();dr_trd->start();
    draw=new MyDraw();
    draw->moveToThread(dr_trd);
    draw->init();
    gl.move(0,ME);
    gl.ty=1;gl.setParent(this);gl.lower();

    drtmr.setTimerType(Qt::PreciseTimer);
    drtmr.setSingleShot(false);
    connect (&drtmr, &QTimer::timeout, draw, &MyDraw::OnTimer);//多线程绘图

    srand (time (NULL));
    m_spd=1;low_mw=crea=b_two=xie=beans=0;

    for(int i=0;i<10000;i++)
    {
        double a=double(RA())/RA.max()*PI*2;
        sinra[i]=sin(a);cosra[i]=cos(a);
    }
    QButtonGroup *gp=new QButtonGroup (this);
    gp->addButton (ui->easy);
    gp->addButton (ui->normal);
    gp->addButton (ui->hard);
    gp->addButton (ui->tui);
    gp->addButton (ui->water);
    gp->addButton (ui->xin);//六种难度模式
    font_fam=font().family();
    m_game.real=1;an_game.real=0;
    QDir dir(path+"lastc");
    if(!dir.exists())dir.mkpath(path+"lastc");
    lstcz=sdcnt=0;
    for(int i=0;i<MAXJSH;i++)del_js[i]=0;
    xmul=1.0;b_snd=0;sender=0;
    tcpmap.setParent(this);rdmap.setParent(this);
    connect(&tcpmap,SIGNAL(mappedInt(int)),this,SLOT(OnReadData(int)));
    connect(&rdmap,SIGNAL(mappedInt(int)),this,SLOT(DoReadData(int)));
    m_game.is_day=1;m_game.is_cycle=0;
}

void MainWindow::showEvent(QShowEvent *event)
{
    resize (W0+W1+W2, HI+ME+TOP);
    QPixmap pix;pix.load("://icons/202.png");
    QCursor cu(pix,20,15);
    setCursor(cu);cur=cu;
    pix.load("://icons/10.png");
    setWindowIcon(pix);
#ifndef Q_OS_ANDROID //菜单栏
    connect (ui->a_open, &QAction::triggered, this, &MainWindow::on_open_clicked);
    connect (ui->a_save, &QAction::triggered, this, &MainWindow::on_save_clicked);
    connect (ui->a_quick, &QAction::triggered, this, &MainWindow::on_quick_clicked);
    connect (ui->a_start, &QAction::triggered, this, &MainWindow::on_btc_clicked);
    connect (ui->a_add, &QAction::triggered, this, &MainWindow::jiasu);
    connect (ui->a_sub, &QAction::triggered, this, &MainWindow::jiansu);
    connect (ui->a_sta, &QAction::triggered, this, &MainWindow::ViewStatus);
    connect (ui->a_quit, &QAction::triggered, this, &MainWindow::Quit);
    connect (ui->a_over, &QAction::triggered, this, &MainWindow::GameOver);
    connect (ui->a_sun, &QAction::triggered, this, &MainWindow::SwitchSun);
    connect (ui->a_xue, &QAction::triggered, this, &MainWindow::ShowLife);
    connect (ui->a_swi, &QAction::triggered, this, &MainWindow::ChangeZw);
    connect (ui->a_mw, &QAction::triggered, this, &MainWindow::SwiMw);
    connect (ui->a_oth, &QAction::triggered, this, &MainWindow::OtherPre);
    connect (ui->a_swdr, &QAction::triggered, this, &MainWindow::SwiDraw);
    connect (ui->a_kjc, &QAction::triggered, this, &MainWindow::SwiKJC);
    connect (ui->a_res, &QAction::triggered, this, &MainWindow::RestartGame);
    connect (ui->a_tx, &QAction::triggered, this, &MainWindow::SwiEff);
    connect (ui->a_snd, &QAction::triggered, this, &MainWindow::SwiSound);
    connect (ui->a_last, &QAction::triggered, this, &MainWindow::LoadLast);
    connect (ui->a_full, &QAction::triggered, this, &MainWindow::FullScreen);
    connect (ui->a_swbg, &QAction::triggered, this, &MainWindow::SwiBG);
    connect (ui->a_infsn, &QAction::triggered, this, &MainWindow::MJ_InfSn);
    connect (ui->a_clrcd, &QAction::triggered, this, &MainWindow::MJ_ClrCD);
    connect (ui->a_kill, &QAction::triggered, this, &MainWindow::MJ_Kill);
    connect (ui->a_adsn, &QAction::triggered, this, &MainWindow::MJ_AdSn);
    connect (ui->a_car, &QAction::triggered, this, &MainWindow::MJ_Car);
    ui->menubar->raise();
#else
    ui->menubar->hide();
    low_mw=1;low_eff=1;tmr_delay=1;ME=0;
#endif
    QFile fil (path+"lasthost");
    if(fil.exists())
    {
        fil.open (QIODevice::ReadOnly);
        QDataStream ar (&fil);QString host;
        ar>>host;fil.close ();ui->luj->setText(host);
    }
    ui->a_snd->setDisabled(1);
#ifdef Q_OS_IOS
    ui->open->hide();ui->save->hide();
    low_mw=1;
#endif
    EnableButton();
#ifdef NO_UI
    resize (100, 340+ME);
    ui->btc->setFlat(0);ui->bt_oth->setFlat(0);
    ui->btc->setIconSize(QSize(0,0));ui->btc->setText("开始");
    ui->bt_oth->setIconSize(QSize(0,0));ui->bt_oth->setText("设置");
    return;
#else
#ifndef TEST_MODE
    LoadLast(1);
#endif
#endif
}

void MainWindow::closeEvent(QCloseEvent *ev)
{
    if(m_game.started&&tcp.size()==0)
    {
        if (!isstop)
            on_stop_clicked ();
        QFile file (path+"lastplay");
        file.open (QIODevice::WriteOnly);
        SaveToFile(file);
    }
}

MainWindow::~MainWindow () {
    delete draw;
    delete ui;
}

void MainWindow::ResetIcons()
{
    upd_ico=1;
    if(!m_game.started&&!icotmr.isActive())
        icotmr.start(10);
}

void MainWindow::resizeEvent(QResizeEvent *ev)
{
    QSize s=size ();
    double od=zoom;TOP=50;
    zoom=std::fmin(double(s.width()-W2)/(W0+W1),double(s.height()-ME)/(HI+TOP));
    if(isFullScreen())
    {
        int ys=(HI-60)/(1-YW-YT),yl=60-ys*YW,hi=s.height()-ME;
        if((yl+ys+TOP)*zoom<hi)
            TOP=ceil(hi/zoom-(yl+ys));
    }
    if (fabs(od - zoom)>eps)
    {
        lszm=tim.elapsed();
        ResetIcons();
#ifdef Q_OS_ANDROID
        QFont f ("Arial", floor (20 * zoom), false);
        setFont (f);
#endif
#ifdef Q_OS_IOS
        QFont f ("Arial", floor (20 * zoom), false);
        setFont (f);
#endif
    }
    int wi=width(),hi=height();
    draw->gl->resize (wi,hi-ME);
    draw->angl->resize (wi,hi-ME);
    draw->gl->move (0, ME);
    draw->angl->move (0, ME);
    if(drty)gl.resize(draw->gl->size()),gl.move(draw->gl->pos());
}

void MainWindow::paintEvent (QPaintEvent *event) {
    if(fir_init)return;
#ifdef NO_UI
    return;
#endif
    if(drty==0)
    {
        QPainter dc(this);
        dc.drawPixmap(QRect(draw->gl->pos(),pil.size()/b_ret),plajs?anpil:pil);
        dc.end();
    }
}

void MainWindow::InitZw()
{
    int si=22*SI/110,ts=12*SI/110;
    int dza=80,dzb=150,dzc=50,dzj=20;
    QColor nul=Qt::white;//创建植物：子弹颜色，攻击力(负数表示特殊植物)，间隔时间，大招时间,移动速度(子弹),生命值,0,0,图片ID,阳光,类型,子弹大小,是否寒冰,是否蘑菇,是否灰烬类
    plants[0].Create (Qt::white, -1, 18000, 20, 0, 80, 0, 0, 2, 50, 1); //向日葵
    plants[1].Create (GREEN, 1, 1600, dza, 4, 80, 0, 0, 1, 80, 0, si); //豌豆射手
    plants[2].Create (Qt::black, 0, 1e8, dzj, 2, 750, 0, 0, 4, 60, 2); //小坚果
    plants[3].Create (RED, -2, 0, 0, 100, 50, 0, 0, 7, 150, 3); //炸弹
    plants[56].Create(YUMI,1,1600,dza,4,80,0,0,210,120,56,ts);//玉米投手
    plants[53].Create(QColor(186,49,36),0,1e8,1e8,0,1,0,0,196,40,53);//巴豆
    plants[63].Create(nul,-28,1000,1e8,0,1,0,0,225,10,63);//冰豆
    plants[4].Create (RED, 1, 1600, dza, 4, 80, 0, 0, 8, 150, 4, si); //火球射手
    plants[5].Create (RED, -3, 0, 0, 90, 50, 0, 0, 9, 125, 5); //火爆辣椒
    plants[6].Create (BLUE, 1, 1900, dza, 4, 80, 0, 0, 48, 100, 9, si, true); //寒冰射手
    plants[64].Create(GREEN,1,1600,60,4,80,0,0,226,175,64,si);//双发射手
    plants[7].Create (Qt::black, 0, 1e8, dzj, 2, 1500, 0, 0, 11, 130, 6); //高坚果
    plants[8].Create (QColor(234,235,112), -4, 0, 0, 80, 1e8, 0, 0, 12, 75, 7); //倭瓜
    plants[87].Create (Qt::green, 1, 2000, dza, 4, 80, 0, 0, 83, 100, 87,ts); //卷心菜投手
    plants[9].Create (Qt::green, 2, 2000, dzc, 4, 80, 0, 0, 43, 250, 8,ts); //西瓜投手
    plants[16].Create (QColor(222,190,134), -20, 2000, 0, 10, 120, 0, 0, 71, 30, 17); //土豆地雷
    plants[41].Create(GREEN,1,1200,dzb,4,80,0,0,168,350,41,si);//三线射手
    plants[77].Create(GREEN,1,1600,dzb,4,80,0,0,243,300,77,si);//普通机枪射手
    plants[89].Create(Qt::white,-18,1600,dza,2,80,0,0,256,100,89);//激光豆
    plants[61].Create(DBLUE,1,1900,dza,4,80,0,0,223,270,61,si,1);//强力寒冰射手
    plants[74].Create(nul,-29,0,0,25,80,0,0,240,80,74);//虚弱炸弹
    plants[75].Create(nul,0,1e8,0,0,1,0,0,241,30,75);//虚弱豆
    plants[76].Create(Qt::gray,0,1000,dza,4,80,0,0,242,100,76,si);//虚弱射手
    plants[10].Create (nul, -8, 1000, dzj, 60, 700, 0, 0, 82, 100, 19); //全息坚果
    plants[12].Create (Qt::white, -9, 8000, 20, 4, 80, 0, 0, 94, 120, 20, 1); //双子向日葵
    plants[67].Create(GREEN,0,1600,dza,4,80,0,0,229,100,67, si);//有毒射手
    plants[68].Create(GREEN,0,1600,dzb,4,80,0,0,230,200,68, si);//双发有毒射手
    plants[70].Create(nul,-24,0,0,50,50,0,0,234,100,70);//毒气弹
    plants[14].Create (QColor(169,72,166), -21, 1000, 0, 10, 80, 0, 0, 68, 150, 16); //大嘴花
    plants[102].Create(QColor(173,100,67),1,2000,dzc,2,80,0,0,281,500,102,si);//橡木弓手
    plants[90].Create(DIANCO,1,1600,dza,4,80,0,0,254,125,90,si);//闪电射手
    plants[84].Create(nul,-33,1000,0,0,80,0,0,259,0,84);//模仿者
    plants[79].Create(RED,-30,2500,dza,3,80,0,0,42,250,79);//火龙草
    plants[95].Create(Qt::black, 0, 1e8, dzj, 0, 1000, 0, 0, 273, 120, 95);//导电坚果
    plants[92].Create(QColor(253,171,49),1,1600,dza,4,80,0,0,257,200,92,si);//辣椒投手
    plants[57].Create (nul, -23, 0, 0, 500, 80, 0, 0, 215, 120, 57); //冰冻炸弹
    plants[36].Create (nul, 0, 1e8, 0, 0, 160, 0, 0, 149, 150, 36); //火炬树桩
    plants[58].Create (nul, 0, 1e8, 0, 0, 160, 0, 0, 220, 100, 58,10,true); //冰火炬树桩
    plants[82].Create(BLUE,-32,3000,dza,1,80,0,0,258,225,82);//冰龙草
    plants[72].Create(RED,-26,0,0,10,80,0,0,238,50,72);//小炸弹
    plants[98].Create(QColor(99,47,9),-35,1500,0,4,80,0,0,88,250,98,si*2);//椰子小炮
    plants[44].Create(YELLOW,1,3200,dzb,4,80,0,0,175,500,44, si);//彩色机枪射手
    plants[78].Create(nul,0,1e8,0,0,80,0,0,248,150,78);//火药桶
    plants[93].Create(DIANCO,-12,0,0,30,80,0,0,272,100,93);//闪电炸弹
    plants[11].Create (Qt::blue, 3, 2000, dzc, 4, 80, 0, 0, 55, 480, 10, ts, true); //冰西瓜投手
    plants[69].Create(Qt::yellow,0,2000,dzc,4,80,0,0,231,275,69,ts);//有毒投手
    plants[101].Create(nul, 0, 1e8, dzj, 0, 500, 0, 0, 279, 75, 101);//中毒坚果
    plants[73].Create(nul,-27,0,0,70,80,0,0,239,120,73);//水炸弹
    plants[55].Create(PINK,-22,1000,75,-1,80,00,0,203,125,55);//激光射手
    plants[22].Create (Qt::yellow, 1, 1600, dzb, 4, 80, 0, 0, 98, 120, 22, si); //杨桃
    plants[83].Create (BLUE, 1, 1900, dzb, 4, 80, 0, 0, 266, 150, 83, si, true); //寒冰杨桃
    plants[80].Create(GREEN,-31,1600,60,0,1000,0,0,253,250,80);//菜问
    plants[88].Create(GREEN,1,1000,dzb,4,80,0,0,269,200,88,si);//旋转射手
    plants[81].Create(nul,0,1e8,0,0,80,0,0,252,90,81);//毒火炬
    plants[97].Create(nul,0,1e8,dzj,0,750,0,0,275,30,97);//红薯
    plants[26].Create(PURPLE, 1, 300, dza, 2, 80, 0, 0, 106, 225, 26); //双头大喷菇
    plants[60].Create(GREEN,1,1600,dza,3,80,0,0,222,300,60,13 * SI / 110);//回旋镖射手
    plants[91].Create(DIANCO,-34,6000,dza,5,80,0,0,271,300,91);//闪电芦苇
    plants[96].Create(QColor(232,148,62),2,2000,dza,4,80,0,0,274,375,96,si*3);//橙子射手
    plants[65].Create(GREEN,10,1600,dzc,4,80,0,0,227,700,65,48*SI/110);//巨型射手

    plants[15].Create (nul, 0, 1e8, 0, 0, 120, 0, 0, 56, 20, 11); //路灯
    plants[54].Create (GREEN,-15,500,0,5,80,0,0,199,100,54);//三叶草
    plants[13].Create (nul, -10, 1200, 25, 0, 80, 0, 0, 95, 120, 21); //仙桃
    plants[66].Create (nul,0,0,0,0,0,0,0,228,25,66);//血瓶
    plants[25].Create (nul, 0, 1e8, dzj, 0, 1000, 0, 0, 104, 150, 25); //反弹
    plants[86].Create (GREEN, 1, 1600, dza, 4, 80, 0, 0, 16, 100, 86, si); //双头豌豆射手

    plants[17].Create (nul, -5, 12000, 17, 8, 80, 0, 0, 57, 25, 12, 0, false, true); //阳光菇
    plants[18].Create (PURPLE, 1, 1600, dza, 4, 40, 0, 0, 58, 0, 13, ts, false, true); //小喷菇
    plants[59].Create (PURPLE, 1, 150, dza, 2, 80, 0, 0, 221, 350, 59, 8 * SI / 110, false, true);//大喷菇
    plants[19].Create (Qt::black, -6, 0, 0, 150, 40, 0, 0, 63, 175, 15, 10, false, true); //毁灭菇
    plants[20].Create (nul, -7, 0, 0, 200, 40, 0, 0, 72, 125, 18, 0, false, true); //寒冰菇
    plants[32].Create (MHCO, 0, 1e8, 0, 0, 40, 0, 0, 141, 50, 32, 0, false, true); //魅惑菇
    plants[40].Create (nul,-16,500,0,25,80,0,0,158,70,40,0,0,1);//磁力菇

    plants[21].Create (nul, 0, 0, 0, 0, 800, 0, 0, 62, 10, 14); //荷叶
    plants[28].Create (nul, 0, 0, 0, 0, 0, 0, 0, 130, 50, 28); //咖啡豆
    plants[23].Create (PURPLE, -11, 700, 10, 1, 160, 0, 0, 101, 300, 23, 0, false, true); //忧郁蘑菇
    plants[94].Create (QColor(185,27,39), 2, 600, dza, 2, 80, 0, 0, 270, 350, 94);//猫尾草
    plants[24].Create (nul, 0, 0, 0, 0, 600, 0, 0, 268, 80, 24); //南瓜
    plants[27].Create (nul, -13, 2000, 0, 30, 40, 0, 0, 110, 20, 27); //大葱
    plants[33].Create (QColor (255, 140, 0), 0, 1e8, 0, 0, 80, 0, 0, 142, 25, 33); //大蒜
    plants[29].Create (Qt::gray, 0, 1e8, 1e8, 0, 120, 0, 0, 132, 125, 29); //地刺
    plants[31].Create (nul, -26, 1e8, 0, 0, 160, 0, 0, 135, 275, 31); //大炮1
    plants[30].Create (nul, -14, 3600, 0, 10, 160, 0, 0, 134, 275, 30); //大炮2
    plants[37].Create (GREEN, 1, 1600, dzb, 4, 100, 0, 0, 155, 80, 37, si); //豌豆荚
    plants[85].Create (nul,0,1e8,0,0,80,0,0,15,175,85);//分裂泡泡
    plants[100].Create(GREEN, 1, 1600, dza, 4, 80, 0, 0, 278, 150, 100, 25);//分裂射手


    plants[39].Create(QColor(185,27,39),1,800,dza,4,80,0,0,159,175,39,0);//仙人掌
    plants[71].Create(Qt::white,-25,500,0,10,80,0,0,236,300,71);//棱镜
    plants[34].Create (nul, 0, 1e8, 0, 0, 80, 0, 0, 143, 160, 34); //加速器
    plants[35].Create (YELLOW, 0, 1e8, dzj, 2, 750, 0, 0, 144, 100, 35); //有毒坚果
    plants[42].Create (nul, -17, 0, 0, 0, 50, 0, 0, 169, 200, 42); //魅惑炸弹

    plants[45].Create (nul, 0, 1e8, 1e8, 0, 200, 0, 0, 115, 800, 45); //血量升级
    plants[46].Create (nul, 0, 1e8, 1e8, 0, 200, 0, 0, 176, 1600, 46); //速度升级
    plants[47].Create (nul, 0, 1e8, 1e8, 0, 200, 0, 0, 177, 1000, 47); //攻击升级
    plants[48].Create (nul, -19, 1000, 0, 0, 80, 0, 0, 179, 120, 48); //金盏花

    plants[43].Create(nul,0,0,0,0,0,0,0,170,5,43);//翻转工具
    plants[38].Create(nul, 0, 0, 0, 0, 0, 0, 0, 157, 0, 38);//化肥
    plants[99].Create(nul, -36, 0, 0, 0, 80, 0, 0, 276, 200, 99);//能量花
    plants[49].Create(nul,0,0,0,0,0,0,0,186,50,49);//点火器
    plants[50].Create(nul,0,0,0,0,0,0,0,187,50,50);//灭火器
    plants[51].Create(nul,0,1e8,1e8,0,1,0,0,194,10,51);//僵尸加速器
    plants[52].Create(nul,0,0,0,0,0,0,0,195,5,52);//植物开关
    plants[62].Create(nul,0,1e8,1e8,0,80,0,0,224,50,62);//培养皿


    for(int i=0;i<MAXSHU;i++)plants[i].dztm=plants[i].dztm/5*2;
    for(int i=0;i<MAXSHU;i++)plt_p[i]=10,plt_sum[i]=0,sp_plt[i]=0;
    int era[30]={0,12,15,17,18,19,21,27,28,30,31,33,40,43,45,46,47,48,49,51,52,59,62,71,84,85,86,55,42,38};
    for(int i=0;i<30;i++)plt_p[era[i]]=0;
    plt_p[64]=plt_p[56]=plt_p[4]=plt_p[6]=plt_p[87]=plt_p[9]=plt_p[41]=plt_p[67]=15;
    plt_p[37]=15;plt_p[25]=plt_p[29]=plt_p[55]=plt_p[88]=plt_p[90]=plt_p[100]=7;
    plt_p[2]=plt_p[10]=plt_p[35]=plt_p[72]=plt_p[77]=plt_p[11]=plt_p[79]=plt_p[89]=plt_p[94]=plt_p[96]=5;
    plt_p[20]=plt_p[34]=plt_p[44]=plt_cnt[13]=plt_p[54]=plt_p[70]=plt_p[73]=plt_p[74]=plt_p[75]=plt_p[80]=plt_p[91]=plt_p[98]=plt_p[102]=4;
    plt_p[7]=plt_p[8]=plt_p[16]=plt_p[14]=plt_p[5]=plt_p[54]=plt_p[57]=plt_p[65]=plt_p[78]=plt_p[93]=plt_p[101]=3;
    plt_p[3]=plt_p[53]=plt_p[50]=plt_p[99]=2;
    plants[98].jian=40;
    plants[96].jian=35;//溅射伤害(橙子)
    plants[9].jian=plants[11].jian=plants[69].jian=20; //溅射伤害(瓜)
    plants[87].jian=plants[92].jian=15;//卷心菜，辣椒
    plants[56].attack=600;
    plants[67].attack=plants[68].attack=200;
    plants[90].attack=300;
    plants[69].attack=500;
    plants[18].attack=800;
    plants[76].attack=100;
    plants[59].attack=250;
    plants[26].attack=250;
    plants[102].attack=200;
    plants[67].du=plants[68].du=25;
    plants[69].du=70;
    plants[55].tag=plants[62].tag=-1;//定向，存储
    plants[71].tag=-1e9;//棱镜
    plants[61].freeze=160;//强力寒冰
    plants[82].freeze=200;//冰龙草
    int shun[MAXSHU]={0,1,2,3,56,53,63,4,5,6,64,7,8,87,9,16,41,77,89,61,74,75,76,10,12,67,68,70,14,102,90,84,79,95,92,57,36,58,82,72,98,44,78,93,11,69,101,73,55,22,83,80,88,81,97,26,60,91,96,65,
                        15,54,13,66,25,86,
                        17,18,59,19,20,32,40,
                        21,28,23,94,24,27,33,29,31,30,37,85,100,
                        39,71,34,35,42,
                        45,46,47,48,43,38,99,49,50,51,52,62};//顺序
    for(int i=0;i<MAXSHU;i++)
        p_shun[i]=shun[i],q_shun[shun[i]]=i;

    int zws[ZGQS]={5,8,7,6,5,4,5,6,7,7,    2,2,2},sp[22]={9,41,44,55,11,22,23,26,59,60,61,13,14,65,69,77,79,80,88,94,96,102};
    zws[20]=3;zws[21]=2;zws[22]=2;
    zws[40]=zws[41]=zws[42]=zws[43]=zws[44]=zws[45]=2;zws[46]=1;
    zws[50]=zws[51]=zws[52]=zws[53]=zws[54]=1;
    for(int i=0;i<22;i++)sp_plt[sp[i]]=1;
    //每关的增量
    for (int n=1; n<ZGQS; n++)
        zws[n] += zws[n - 1];
    for (int n=0; n<ZGQS; n++)
        plt_cnt[n]=zws[n];
    //wsad:63关, wsad2:69关
    for (int i=0; i<MAXSHU; i++)
        wsad[i]=wsad2[i]=0;
    int _wsad2[10]={7,11,14,88,59,29,56,44,39,40};
    //int _wsad2[10]={59};
    int _wsad[30]={4,7,9,10,10,11,61,69,13,14,23,23,59,27,34,35,36,34,39,41,25,44,58,56,56,60,60,76,76,83};
    for(int i=0;i<10;i++)wsad2[_wsad2[i]]+=1;
    for(int i=0;i<30;i++)wsad[_wsad[i]]+=1;
    //冷却速度
    cd_spd[0]=10;cd_spd[1]=15;cd_spd[2]=3.8;cd_spd[3]=0.75;cd_spd[4]=12;
    cd_spd[5]=1;cd_spd[6]=12;cd_spd[7]=2;cd_spd[8]=1.4;cd_spd[9]=10;
    cd_spd[10]=2.6;cd_spd[11]=3.4;cd_spd[12]=7;cd_spd[13]=3;cd_spd[14]=1.9;
    cd_spd[15]=12;cd_spd[16]=1.8;cd_spd[17]=9;cd_spd[18]=10;cd_spd[19]=0.6;
    cd_spd[20]=1.8;cd_spd[21]=25;cd_spd[22]=6;cd_spd[23]=3.5;cd_spd[24]=3;
    cd_spd[25]=2;cd_spd[26]=3;cd_spd[27]=5;cd_spd[28]=4;cd_spd[29]=4;
    cd_spd[30]=0.9;cd_spd[31]=0.9;cd_spd[32]=2.5;cd_spd[33]=2.2;cd_spd[34]=2;
    cd_spd[35]=2.2;cd_spd[36]=3;cd_spd[37]=9;cd_spd[38]=0;cd_spd[39]=10;
    cd_spd[40]=10;cd_spd[41]=5;cd_spd[42]=0.6;cd_spd[43]=2.8;cd_spd[44]=1.9;
    cd_spd[45]=0.35;cd_spd[46]=0.25;cd_spd[47]=0.3;cd_spd[48]=1;cd_spd[49]=3;
    cd_spd[50]=5;cd_spd[51]=4;cd_spd[52]=8;cd_spd[53]=1.4;cd_spd[54]=3.2;
    cd_spd[55]=8;cd_spd[56]=5;cd_spd[57]=1.3; cd_spd[58]=3;cd_spd[59]=1.5;
    cd_spd[60]=2;cd_spd[61]=5;cd_spd[62]=3;cd_spd[63]=4.1;cd_spd[64]=8;
    cd_spd[65]=5;cd_spd[66]=6;cd_spd[67]=7;cd_spd[68]=4;cd_spd[69]=5;
    cd_spd[70]=2;cd_spd[71]=4;cd_spd[72]=3.8;cd_spd[73]=2.7;cd_spd[74]=3;
    cd_spd[75]=3.7;cd_spd[76]=10;cd_spd[77]=6;cd_spd[78]=0.8;cd_spd[79]=5;
    cd_spd[80]=4;cd_spd[81]=2.8;cd_spd[82]=5;cd_spd[83]=5;cd_spd[84]=0;
    cd_spd[85]=1.5;cd_spd[86]=10;cd_spd[87]=10;cd_spd[88]=7;cd_spd[89]=8;
    cd_spd[90]=4;cd_spd[91]=3;cd_spd[92]=10;cd_spd[93]=2;cd_spd[94]=5;
    cd_spd[95]=2;cd_spd[96]=5;cd_spd[97]=4.5;cd_spd[98]=4;cd_spd[99]=4;
    cd_spd[100]=6;cd_spd[101]=3;cd_spd[102]=5;
    for(int i=0;i<MAXSHU;i++)tyToNumP[plants[i].type]=i;
    for(int i=0;i<MAXSHU;i++)expnd[i]=del_x[i]=not_imjsz[i]=0;
    expnd[45]=15000;expnd[46]=30000;expnd[47]=20000;
    plants[4].anti_fire=plants[8].anti_fire=plants[36].anti_fire=plants[61].anti_fire=1;
    int a_x[8]={43,38,99,49,50,51,52,62};a_xs=8;
    for(int i=0;i<8;i++)del_x[def_xuan[i]=a_x[i]]=1;
    int ntimjs[45]={3,5,12,16,15,21,24,27,28,29,30,31,33,57,38,42,43,45,46,47,48,49,50,51,52,54,55,62,66,70,17,19,20,40,71,72,73,74,78,84,86,89,93,98,99};
    for(int i=0;i<45;i++)not_imjsz[ntimjs[i]]=true;
}

void MainWindow::InitJs()
{
    //创建僵尸：速度，生命，吃的速度，0，0，图片ID，编号，是否能跳跃，是否发射
    zombies[0].Create (2, 8, 1, 0, 0, 3, 0); //普通僵尸
    zombies[1].Create (2, 22, 1, 0, 0, 5, 1); //塑料僵尸
    zombies[2].Create (3.9, 14, 1, 0, 0, 29, 5, true); //撑杆跳僵尸
    zombies[23].Create(2, 15, 1,0,0,197,25);//向日葵僵尸
    zombies[3].Create (2, 62, 1, 0, 0, 6, 2); //铁桶僵尸
    zombies[4].Create (2, 21, 3, 0, 0, 32, 6); //大嘴僵尸
    zombies[6].Create (3, 26, 1, 0, 0, 90, 4); //报纸僵尸
    zombies[8].Create (2, 17, 1, 0, 0, 73, 13, false, true, 60); //豌豆射手僵尸
    zombies[5].Create (4.5, 40, 1, 0, 0, 10, 3); //跑步僵尸
    zombies[22].Create(2, 90, 1.5, 0,0,192,24);//小坚果僵尸
    zombies[7].Create (1.8, 90, 1, 0, 0, 39, 7); //铁门僵尸
    zombies[10].Create (2, 23, 1, 0, 0, 79, 14, false, true,60); //西瓜僵尸
    zombies[27].Create(2.5, 15, 1,0,0,244,32,false,true);//辣椒僵尸
    zombies[21].Create(1.4, 300, 0.7,0,0,188,23);//高坚果僵尸
    zombies[9].Create (2, 19, 1, 0, 0, 44, 8, true); //跳跳僵尸
    zombies[16].Create (2.2, 34, 1, 0, 0, 107, 17,true); //魔法僵尸
    zombies[13].Create (2, 31, 1, 0, 0, 181, 22, false, true,100); //冰球僵尸
    zombies[19].Create(2.8,39,0,0,0,163,20,false,true);//喷火僵尸
    zombies[11].Create (2.6, 70, 50, 0, 0, 65, 12); //小车僵尸
    zombies[28].Create(5.5,50,0,0,0,246,33,true);//窝瓜僵尸
    zombies[12].Create (1.7, 180, 3000, 0, 0, 49, 9,true,true); //巨人僵尸
    zombies[18].Create(3, 15, 1,0,0,160,19);//气球僵尸
    zombies[14].Create (2, 33, 0, 0, 0, 92, 15,true); //炸弹僵尸
    zombies[15].Create (5, 22, 1.5, 0, 0, 96, 16, false, true,60); //末影人
    zombies[25].Create(3, 41, 1,0,0,208,27,1);//扶梯僵尸
    zombies[17].Create (3, 29, 1, 0, 0, 147, 18, true); //气功僵尸
    zombies[20].Create(0,16,2,0,0,178,21,1);//飞贼僵尸
    zombies[24].Create(2,34,1,0,0,205,26);//火药桶僵尸
    zombies[26].Create(1.5,290,5000,0,0,216,28,true,true,160);//超级巨人

    int shun[MAXJSH]={0,1,2,23,3,4,6,8,5,22,7,10,27,21,9,16,13,19,11,28,12,18,14,15,25,17,20,24,26};
    int js_ty[MAXJSH]={0,0,0,1,2,1,3,1,2,3,2,2,2,2,3,3,3,3,3,2,0,1,1,0,3,3,3,2,2},js_hd[9]={5,7,10,11,12,13,16,17,21};
    //0：菜；1：防御；2：攻击；3：其它
    for (int i=0; i<MAXJSH; i++) {
        zmb_sum[i]=can_wat[i]=0;
        jsty[i]=js_ty[i];jshd[i]=0;
        j_yg[i]=zombies[i].life/1000 * 5; //僵尸的阳光
        if (i==17)j_yg[i] *= 4.5;
        else if(i==8)j_yg[i]*=2.5;
        else if (i==10)j_yg[i] *= 3.2;
        else if (i==14)j_yg[i] *= 2;
        else if(i==16)j_yg[i] *= 3.5;
        else if(i==22)j_yg[i]*=1.1;
        if (zombies[i].eat>3&&zombies[i].eat<5)j_yg[i] += 100;
        else if(zombies[i].eat>1)j_yg[i] += 50;
        else if(zombies[i].eat<0.5)j_yg[i] *= 2;
        if(i==19)j_yg[i]*=1.2;
        if(i==23)j_yg[i]+=25;
        if (zombies[i].vx>2||i==6)j_yg[i] += 50;
        if (i==2)j_yg[i] += 50;
        if(i==1)j_yg[i]*=5;
        if(i==20)j_yg[i]*=6;
        if(i==25)j_yg[i]*=3;
        j_yg[i] /= 1.1;
        j_yg[i]=((j_yg[i]+3) / 5) * 5;
        js_cz[i]=0;
    }
    for (int i=0; i<9;i++)jshd[js_hd[i]]=1;
    int canwat[8]={0,1,3,4,8,9,19,24};//能下水
    for(int i=0;i<8;i++)can_wat[zombies[canwat[i]].type]=true;
    //can_wat[0]=can_wat[1]=can_wat[2]=can_wat[5]=can_wat[6]=can_wat[8]=can_wat[13]=can_wat[21]=can_wat[25]=true;
    //相对概率
    zmb_p[0]=2;zmb_p[1]=6;zmb_p[2]=3;zmb_p[3]=4;zmb_p[4]=3;
    zmb_p[5]=4;zmb_p[6]=5;zmb_p[7]=4;zmb_p[8]=3;zmb_p[9]=3;
    zmb_p[10]=1;zmb_p[11]=2;zmb_p[12]=1;zmb_p[13]=1;zmb_p[14]=4;
    zmb_p[15]=2;zmb_p[16]=3;zmb_p[17]=3;zmb_p[18]=3;zmb_p[19]=4;
    zmb_p[20]=0;zmb_p[21]=0;zmb_p[22]=2;zmb_p[23]=0;zmb_p[24]=0;
    zmb_p[25]=4;zmb_p[26]=0;zmb_p[27]=3;zmb_p[28]=2;
    for(int i=0;i<MAXJSH;i++)tyToNumZ[zombies[i].type]=i,to_an[i]=-1;
    to_an[4]=3;to_an[8]=4;to_an[9]=3;to_an[10]=5;to_an[11]=12;to_an[14]=5;to_an[15]=4;to_an[16]=5;
    to_an[17]=7;to_an[13]=12;to_an[18]=3;to_an[19]=7;to_an[23]=0;to_an[25]=17;to_an[28]=5;
    for(int i=0;i<MAXJSH;i++)
    {
        od_ico[i]=zombies[i].id;z_shun[i]=shun[i];
        if(del_js[i])zombies[i]=zombies[to_an[i]];
    }
    int jad[GQS]={2,3,2,2,2,3,3,4,5,3};z_cn[0]=jad[0];
    for(int i=1;i<GQS;i++)z_cn[i]=z_cn[i-1]+jad[i];
}

void MainWindow::MakeTuJian()
{
    InitIcon();
    QString str="## 植物图鉴\n";
    str+="| 名称 | 图片 | 功能 | 阳光 | 冷却时间 | 生命值 | 攻击力 | 攻击间隔 | 溅射范围 |\n";
    str+="| ---- | ---- | ---- | ---- | -------- | ------ | ------ | -------- | -------- |\n";
    char zw_na[][20]={"向日葵","豌豆射手","小坚果","炸弹","玉米投手","巴豆","冰豆","火球射手","火爆辣椒","寒冰射手","双发射手","高坚果","倭瓜","卷心菜投手","西瓜投手","土豆地雷","三线射手",
                    "普通机枪射手","激光豆","强力寒冰射手","虚弱炸弹","虚弱豆","虚弱射手","全息坚果","双子向日葵","有毒射手","双发有毒射手","毒气弹","大嘴花","橡木弓手","闪电射手","模仿者",
                    "火龙草","导电坚果","辣椒投手","冰冻炸弹","火炬树桩","冰火炬树桩","冰龙草","小炸弹","椰子小炮","彩色机枪射手","火药桶","闪电炸弹","冰西瓜投手","有毒投手","中毒坚果",
                    "水炸弹","激光射手","杨桃","寒冰杨桃","菜问","旋转射手","毒火炬","红薯","双头大喷菇","回旋镖射手","闪电芦苇","橙子射手","巨型射手","路灯","三叶草","仙桃","血瓶","反弹",
                    "双头豌豆射手","阳光菇","小喷菇","大喷菇","毁灭菇","寒冰菇","魅惑菇","磁力菇","荷叶","咖啡豆","忧郁蘑菇","猫尾草","南瓜","大葱","大蒜","地刺","玉米加农炮1","玉米加农炮2",
                    "豌豆荚","分裂泡泡","分裂射手","仙人掌","棱镜","加速器","有毒坚果","魅惑炸弹","血量升级","速度升级","攻击升级","金盏花","翻转工具","化肥","能量花","点火器","灭火器",
                    "僵尸加速器","植物开关","培养皿"};
    for(int s=0;s<MAXSHU;s++)
    {
        int i=p_shun[s],id=plants[i].id;
        ico[id].toImage().scaled(50,50,Qt::KeepAspectRatio,Qt::SmoothTransformation).save(QString::asprintf("tujian/%d.png",id));
        str+="| ";str+=zw_na[s];
        str+=QString::asprintf(" | ![%s](./tujian/%d.png) | ",zw_na[s],id);
        const Plant&zw=plants[i];
        if(zw.mush)str+="蘑菇 | ";
        else str+="| ";
        str+=QString::asprintf("%d | ",zw.cost);
        str+=QString::asprintf("%.1lf秒 | ",40.0/cd_spd[i]);
        str+=QString::asprintf("%.1lf | ",zw.life/20.0);
        if(zw.attack>=0)
        {
            if(zw.attack>0)
                str+=QString::asprintf("%d",zw.attack/100);
        }
        else
        {
            if(zw.move_spd>0)
                str+=QString::asprintf("%d",zw.move_spd*10);
        }
        if(zw.freeze>0)str+=QString::asprintf("（附加%.1lf秒寒冰效果）",zw.freeze/20.0);
        if(zw.du>0)str+=QString::asprintf("（附加%.1lf点中毒效果）",zw.du/50.0);
        if(zw.time>0&&zw.time<1e8)str+=QString::asprintf(" | %.1lf秒 | ",zw.time/1000.0);
        else str+=QString::asprintf(" | | ");
        if(zw.jian>0)str+=QString::asprintf("%d |",zw.jian);
        else str+=QString::asprintf("|");
        str+='\n';
    }
    QFile file("tujian.md");
    file.open (QIODevice::WriteOnly);
    QByteArray da;
    QDataStream st(&da,QIODevice::WriteOnly);
    st<<str;
    file.write(da);
    file.close();
}

void MainWindow::Init () {
    InitZw();InitJs();
    if (SI<=55) //双倍
    {
        for (int i=0; i<MAXSHU; i++)
            cd_spd[i] *= 1.1, wsad[i] *= 2,wsad2[i]*=2;
    }
    for (int n=0; n<MXUAN; n++)
        m_xuan[n]=-1;
    l48=TM=ZWSHU=JSH=ND=zf_len=0;
    if(b_sun)ui->a_sun->setText("关闭更多阳光");
    else ui->a_sun->setText("开启更多阳光");
    m_b48=isstop=false;
    beans=tmr_cnt=stop_cnt=zuobi=0;
    select=sele_x=sele_y=-1;
    if(tmid!=-1)killTimer(tmid);tmid=-1;
    if (fir_init)
        InitIcon ();
    fir_init=0;
    for (int i=0; i<MN; i++)
        m_lstJ[i]=m_lstC[i]=m_lstD[i]=m_lstT[i]=m_lstB[i]=m_lstZ[i]=0;
    int ne[7]={1, 6, 4, 26, 22, 9, 11};//2048顺序
    for (int i=0; i<6; i++)
        n_2048[plants[ne[i]].type]=plants[ne[i+1]].type;
    n_2048[plants[ne[6]].type]=-1;

    //MakeTuJian();
}

void MainWindow::InitOneIcon(int i)
{
    double zm=zoom*b_ret;laSI=SI;
    QString str=QString::asprintf ("://icons/%d.png", i);
    if(i==111)str=QString::asprintf("://icons/bg/%d/mi.png",bgid);
    else if(i==112||i==113)str=QString::asprintf("://icons/bg/%d/%d.png",bgid,i-112);
    else if(i==125)str=QString::asprintf("://icons/bg/%d/ri.png",bgid);
    QImage ic(str);
    if(ic.isNull())return;
    ic=ic.convertToFormat(QImage::Format_RGBA8888);
    if ((i>=116&&i<=124)||i==91||i==185) {//按钮图片
        if(fabs(zm-lsZM)<eps)return;//避免不必要的刷新
        ico[i]=QPixmap::fromImage (ic);
    }
    else if(i==139||i==140||i==233||i==211||i==114||i==267||i==150||i==204)
    {
        if(fabs(zm-lsZM)<eps)return;//避免不必要的刷新
        ico[i]=QPixmap::fromImage (ic.scaled (ic.size()*zm,Qt::KeepAspectRatio,Qt::SmoothTransformation));
        ico_si[i]=ic.size();
    }
    else if(i==146||i==13||i==115)
    {
        if(fabs(zm-lsZM)<eps)return;//避免不必要的刷新
        ico[i]=QPixmap::fromImage (ic.scaled (ic.size()*zm/2, Qt::KeepAspectRatio,Qt::SmoothTransformation));
        ico_si[i]=ic.size()/2;
    }
    else if (i==112||i==113||i==14||i==174)//子弹图像
        ico[i]=QPixmap::fromImage (ic.scaled (ic.size()*zm*SI/220, Qt::KeepAspectRatio,Qt::SmoothTransformation)),ico_si[i]=ic.size()*SI/220;
    else if(i==161)//大型子弹图像
        ico[i]=QPixmap::fromImage (ic.scaled (ic.size()*zm*SI/440, Qt::KeepAspectRatio,Qt::SmoothTransformation)),ico_si[i]=ic.size()*SI/440;
    else if(i==111||i==125)//背景图像
    {
        int wi=(W1-50)/(1-XW-XT),hi=(HI-60)/(1-YW-YT);
        if(i==125)wi=hi*1536/1345;
        ico_si[i]=QSize(wi,hi);
        ico[i]=QPixmap::fromImage (ic.scaled (ceil (wi * zm), ceil (hi * zm), Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
    }
    else {
        int dx=SI;
        if (i==40||i==41||i==264||i==265)dx*=1.3;
        else if(i==49||i==51||i==52||i==54)dx*=1.5;
        else if (i>=216&&i<=219)dx*=1.7;
        else if(i==104||i==105)dx*=2;
        ico_si[i]=QSize(dx,dx);
        ico[i]=QPixmap::fromImage (ic.scaled (ceil (dx*zm), ceil (dx*zm), Qt::KeepAspectRatio,Qt::SmoothTransformation));
        if (i==28) {ico_red[i]=QPixmap::fromImage(ico[i].toImage().mirrored(1,0));return;}
        auto rgb=ic.bits();int si=ic.width()*ic.height();
        long long sr=0,sg=0,sb=0;int cn=0;
        for(int s=0,t=0;s<si;s++,t+=4)//变蓝
        {
            int r=rgb[t],g=rgb[t+1],b=rgb[t+2],a=rgb[t+3];
            if (a<200)continue;
            sr+=r;sg+=g;sb+=b;cn+=1;
            r >>= 1; g >>= 1; b <<= 1;
            if (b>255)b=255;
            rgb[t]=r;rgb[t+1]=g;rgb[t+2]=b;
        }
        cn*=1.2;
        ico_bl[i]=QPixmap::fromImage (ic);
        ic=ico[i].toImage().convertToFormat(QImage::Format_RGBA8888);
        rgb=ic.bits();si=ic.width()*ic.height();
        for(int s=0,t=0;s<si;s++,t+=4)//变红
        {
            int r=rgb[t],g=rgb[t+1],b=rgb[t+2],a=rgb[t+3];
            if (a<200)continue;
            r <<= 1; g >>= 1; b >>= 1;
            if (r>255)r=255;
            rgb[t]=r;rgb[t+1]=g;rgb[t+2]=b;
        }
        ico_red[i]=QPixmap::fromImage (ic);
        ic=ico[i].toImage().convertToFormat(QImage::Format_RGBA8888);
        rgb=ic.bits();si=ic.width()*ic.height();
        for(int s=0,t=0;s<si;s++,t+=4)//变黑
            rgb[t]=rgb[t+1]=rgb[t+2]=0;
        ico_blk[i]=QPixmap::fromImage (ic);
        for(int s=0,t=0;s<si;s++,t+=4)//变浅
            rgb[t+3]/=2;
        ico_sha[i]=QPixmap::fromImage (ic);
        mco[i]=QColor(sr/cn,sg/cn,sb/cn);
    }
    if(i==1)ico_blk[0]=ico_blk[1];
}

void MainWindow::InitIcon () //预处理图标
{
    if(fir_init)
    {
        InitOneIcon(112);
        for (int i=0; i<=ICCNT; i++)
            ico[i]=ico_bl[i]=ico_red[i]=ico_blk[i]=ico_sha[i]=ico[112];
        InitOneIcon(111);
        InitOneIcon(113);
        InitOneIcon(125);
        InitOneIcon(117);
        InitOneIcon(124);
        InitOneIcon(116);
        InitOneIcon(91);
        InitOneIcon(185);
        InitOneIcon(184);
        InitOneIcon(78);
        InitOneIcon(131);
        upd_ico=1;lszm=inf;
        icotmr.start(10);//首次刷新图标
        return;
    }
    for (int i=1; i<=ICCNT; i++)
        InitOneIcon(i);
    FinishInitIcon();
}

bool MainWindow::AddPlant (int x, int y) {
    bool rt=false;
    if (select==-3) //大炮
    {
        Plant zw=plants[3];
        zw.x=x-SI/2;zw.y=y;
        zw.cost=0;zw.tag=1;
        m_game.PlantZw (zw);
        SendDeltaPlantZw(zw);
        select=-1;return true;
    }
    Plant zw;zw.type=-1;
    if (x>=0&&x<MM*SI&&y>=0&&y<MN&&select!=-1&&exp>=expnd[select]) {
        if ((!is_boss||is12)&&!b_guan) //非滚卡
        {
            if (cd[select]>=800) {
                if(b_zhu)//排山倒海
                {
                    zw=plants[select];
                    for(int s=0;s<MN;s++)
                    {
                        if (m_game.PlantZw (zw, x/SI, s))
                            cd[select]=0,zw.cost=0;
                    }
                }
                else
                {
                    if (m_game.PlantZw (plants[select], x/SI, y))
                        cd[select]=0,zw=plants[select],exp-=expnd[select];
                }
            }
        }
        else //滚卡
        {
            if (m_game.PlantZw (plants[select], x/SI, y)) {
                zw=plants[select];
                int n=0;
                for (n=0; n<XUAN; n++)
                    if (m_xuan[n]==select)
                        break;
                for (n += 1; n<XUAN; n++)
                    m_xuan[n - 1]=m_xuan[n];
                XUAN -= 1;
                m_xuan[XUAN]=-1;
                m_game.m_yg=9999;
            }
        }
        rt=true;
    }
    if (rt&&select!=-3)
        select=-1;
    SendDeltaPlantZw(zw);
    return rt;
}
bool MainWindow::AddPlant (QPoint pt) {
    return AddPlant (pt.x () - (W0+50), (pt.y () - 60) / SI);
}
bool MainWindow::AddJs (QPoint pt) //手动添加僵尸
{
    if (select<0)
        return false;
    int x=((pt.x () - (W0+50)) / SI) * SI, y=(pt.y () - 60) / SI;
    if ((plajs||x>=MN * SI)&&x<=MM * SI&&y>=0&&y<MN&&m_game.m_yg>=j_yg[select]&&(!m_game.iswa[y]||can_wat[zombies[select].type])) {
        m_game.m_yg -= j_yg[select];
        zombies[select].x=x;
        zombies[select].y=y;
        if(plajs)
        {
            Zombie js=zombies[select];
            js.x=MM*SI-SI/2;int xcn=0,mcn=0,ecn=0;
            for(int i=0;i<m_game.plt_cnt;i++)
            {
                if(m_game.plants[i].type==45)xcn+=1;
                else if(m_game.plants[i].type==46)mcn+=1;
                else if(m_game.plants[i].type==47)ecn+=1;
            }
            if(xcn)js.life+=pow(xcn,0.8)*std::max(10.0,pow(js.life,2.0/3.0));
            if(js.vx)js.vx+=std::min(mcn,5);
            js.eat+=std::min(ecn,5)+1;
            if(select==1)js.x=-SI,js.vx=-js.vx;
            if(select==19)js.x=x+10;
            q_js.push(std::make_pair(tim.elapsed(),js));
        }
        else m_game.NewJs (zombies[select]);
        select=-1;
        return true;
    }
    return false;
}

void MainWindow::GenerateJs()
{
    int n=z_sx[js_nw[js_nwi++]];
    if(js_nwi>=1000)js_nwi=0;
    if(gq_nd>=6&&m_n==3&&!is_boss&&m_nd<MAXJSH&&m_game.jd%(gq_nd<=7?3:2)==0)n=2;
    zombies[n].y=-1;
    if (n==8||n==4||n==10||n==17||n==19||n==24) //大嘴，豌豆射手等：寻找掩护
    {
        int maxs=400, wz=-1,*ww,*sz[4]={m_lstC,m_lstJ,m_lstD,m_lstT}; //10秒
        for(int i=0;i<4&&wz==-1;i++)
        {
            for (int j=0; j<MN; j++) {
                int shu=TM - sz[i][j];
                if (shu<maxs)
                    maxs=shu,wz=j,ww=&sz[i][j];
            }
        }
        if(wz!=-1)
        {
            zombies[n].y=wz;
            if(RA()%3!=0)(*ww)=TM-400;
        }
    }
    else if (n==12||n==21||n==26) //巨人：防止生成在一行
        m_lstJ[zombies[n].y=GetMin(m_lstJ,MN)]=TM;
    else if (n==11) //小车：防止生成在一行
        m_lstC[zombies[n].y=GetMin(m_lstC,MN)]=TM;
    else if (n==7) //盾牌：防止生成在一行
        m_lstD[zombies[n].y=GetMin(m_lstD,MN)]=TM;
    else if (n==3||n==5||n==22) //铁桶：防止生成在一行
        m_lstT[zombies[n].y=GetMin(m_lstT,MN)]=TM;
    if(zombies[n].y==-1)
    {
        if (can_wat[zombies[n].type]&&RA () % 3==0&&m_game.jd>m_game.zjd/3) //后期增大水池概率
        {
            int s = 0,w[MMN];
            for (int i = 0; i < MN; i++)
                if (m_game.iswa[i])w[s++] = i;
            if(s)zombies[n].y=w[RA () % s];
        }
        if(zombies[n].y==-1)zombies[n].y=GenerateY();
    }
    Zombie js=zombies[n];js.x=(MM - (fr_mid?4:1)) * SI+SI / 2;
    if(xie&&m_game.jd%4==1)js.vy=(m_game.jd&4?1:-1)*js.vx;
    if(xie&&m_game.jd%4==2)js.vy=(m_game.jd&4?0.6:-0.6)*js.vx;
    if(xie&&m_game.jd%4==3)js.vy=(m_game.jd&4?1.6:-1.6)*js.vx;
    while(TM-m_game.car_TM[js.y]<=800&&RA()%8>0)js.y=RA()%MN;//20s内丢车保护
    if (is_boss&&m_jwbs[js.y]<=0)return;
    if(is_boss)
    {
        int t=m_game.zmb_ii[js.y];
        if(m_game.zombies[t].vx!=0)return;
        js.x=m_game.zombies[t].x;
    }
    m_lstZ[js.y]=TM;js.life*=xmul;m_SumZ[js.y]+=1;
    if(m_game.is_cycle&&m_game.is_day)
        js.life*=0.8;
    if (f_shu&&(js.type==2||js.type==3||js.type==7)) //化肥僵尸
    {
        js.fei=true;
        f_shu -= 1;
        js.vx *=1.5;
        js.life *= 1.5;
        js.eat *= 1.5;
    }
    else
    {
        if(zmb_sum[n]%4==1)
        {
            double p=1.1+((zmb_sum[n]/4)%4)*0.1;
            js.vx*=p;js.life*=p;js.fs_nd/=p;js.eat*=p;
        }
        zmb_sum[n]+=1;
    }
    js.vx*=(double(RA())/RA.max()*0.2+0.9);
    if(js.shoot&&js.fs_nd>0)js.fss=RA()%js.fs_nd;
    while (js.type >= 0&&!can_wat[js.type]&&m_game.iswa[js.y])
        js.y = RA () % MN;
    js.eat_jl=SI/3+RA()%(SI/3);
    if(b_zhu)//排山倒海
    {
        for(int s=0;s<MN;s++)
        {
            js.y=s;
            m_game.NewJs(js);
        }
    }
    else m_game.NewJs (js);
    if (mirr) //镜面关卡
    {
        js.x=-100;
        js.vx=-js.vx;
        js.life*=0.8;
        m_game.NewJs (js);
    }
    if (!is_boss) //普通关卡
    {
        m_game.jd+=1;
        if (m_game.jd % 50==0 && gqe!=9)
            f_shu += 1; //增加化肥
    }
}

int MainWindow::GenerateY()
{
    int y;const int CNT=5;
    int p[CNT]={1,4,3,2,2},cn[CNT]={0};
    int ty=MyRand(cn,p,CNT);
    if(!is_boss&&m_n<=3)ty=2;
    if(ty==0)y=GetMax(m_SumZ,MN);
    else if(ty==1)y=GetMin(m_SumZ,MN);
    else if(ty==2)
    {
        int m_kx[MMN],ss=0;
        for(int i=0;i<MN;i++)
        {
            if(TM-m_lstZ[i]>100)
                m_kx[ss++]=i;
        }
        if(ss==0)y=RA () % MN;
        else y=m_kx[RA()%ss];//尽量均匀分布
    }
    else if(ty==3)y =RA () % MN;
    else
    {
        int su[MMN]={0};
        for(int i=0;i<m_game.plt_cnt;i++)
            su[m_game.plants[i].y]+=1;
        y=GetMin(su,MN);
    }
    return y;
}

void MainWindow::ChangeZw()
{
    if(!can_swi)return;
    XUAN=RXUAN;
    bool bk=false;
    if (!isstop)
        on_stop_clicked (), bk=true;
    Choose *ch=new Choose (this);
    ch->setWindowTitle ("更换植物");
    ch->SHU=0;
    ch->XUAN=XUAN;
    ch->JSH=JSH;
    for (int i=0; i<ZWSHU; i++) {
        int t=p_shun[i];
        if(del_x[t])continue;
        ch->yg[ch->SHU]=plants[t].cost;
        ch->ico[ch->SHU++].load (QString::asprintf ("://icons/%d.png", plants[t].id));
    }
    for (int i=0; i<JSH; i++)
        ch->icj[i].load (QString::asprintf ("://icons/%d.png", zombies[z_sx[i]].id));
    ch->setWindowModality (Qt::ApplicationModal);
    ch->Init ();
    for(int i=0;i<XUAN;i++)
    {
        int t=m_xuan[i];
        if(del_x[t])continue;
        ch->ck[q_shun[t]]->setChecked(1);
        ch->dfn[q_shun[t]]=i;
    }
    ch->Sort();
    ch->show ();
    if (ch->exec ())
    {
        for (int i=0; i<XUAN; i++)
        {
            if(ch->xuan[i]==-1)m_xuan[i]=-1;
            else m_xuan[i]=p_shun[ch->xuan[i]];
        }
        while(XUAN>0&&m_xuan[XUAN-1]==-1)
            XUAN-=1;
        for(int i=0;i<a_xs;i++)
            m_xuan[i+XUAN]=def_xuan[i];
        XUAN+=a_xs;
        CheckMFZ(m_xuan,XUAN);
    }
    if (bk)
        on_stop_clicked ();
}

void MainWindow::AdjustZMBP(int ty)
{
    int mul[MAXJSH];
    for(int i=0;i<MAXJSH;i++)mul[i]=1;
    for(int i=0;i<MAXJSH;i++)
        zmb_p[i]/=lamul[i];
    if(b_sho&&gqe<=0&&ty!=0)ty=1;
    if(ty==0)
    {
        for(int i=0;i<MAXJSH;i++)
            if(jsty[i]==0)mul[i]=2;
    }
    else if(ty==2)
    {
        for(int i=0;i<MAXJSH;i++)
            if(jsty[i]==1&&!jshd[i])mul[i]=2;
    }
    else if(ty==3)
    {
        for(int i=0;i<MAXJSH;i++)
            if(jsty[i]==2&&!jshd[i])mul[i]=2;
    }
    else if(ty==4)
    {
        for(int i=0;i<MAXJSH;i++)
        {
            if(jsty[i]==0)mul[i]=2;
            else if(jshd[i])mul[i]=4;
            else mul[i]=3;
        }
    }
    else if(ty==5)
    {
        for(int i=0;i<MAXJSH;i++)
        {
            if(jsty[i]!=0)mul[i]=2;
            if(jshd[i])mul[i]=3;
        }
    }
    for(int i=0;i<MAXJSH;i++)
        lamul[i]=mul[i],zmb_p[i]*=mul[i];
}

void MainWindow::MakeBoJs()
{
    if(!is_boss&&m_n>=ND)return;
    int m=(is_boss?500:tm_lst[m_n]/tm_js[m_n]);
    if(m>500)m=500;
    int t_p[MAXJSH],cn=(is_boss?JSH:m_nd);
    for(int i=0;i<JSH;i++)t_p[i]=zmb_p[z_sx[i]];
    for(int i=0;i<m;i++)js_nw[i]=MyRand(js_cz,t_p,cn);
    if(!is_boss)std::shuffle(js_nw,js_nw+m,RA);
    for(int i=0;i<m;i++)js_nw[i+m]=js_nw[i];
    js_nwi=0;
}

void MainWindow::GetNxtDbTm()
{
    nxt_dbtm=inf;
    int t=v_tm+tm_lst[m_n],i;
    for(i=m_n+1;!isdb[i]&&i<=ND;i++)
        t+=tm_lst[i];
    if(i<=ND)nxt_dbtm=t;
}


void MainWindow::EndLessNd()
{
    for(int i=0;i<MAXJSH;i++)
        zmb_p[i]/=lamul[i];
    if(m_n==15)zmb_p[26]+=1;
    if (m_n>5&&m_n%5==0) //增加难度(等差等比)
    {
        if(m_n%10==0)
        {
            for (int i=0; i<MAXJSH; i++) //增加血量
                zombies[i].life=int(zombies[i].life*1.1+0.5);
            if(m_n%20==0)zmb_p[12]+=1;
            else zmb_p[22]+=1;
        }
        else
        {
            for (int i=0; i<MAXJSH; i++) //增加血量
                zombies[i].life += 4000;
        }
        if (m_n % 15==0)
        {
            zmb_p[11]+=1;
            zmb_p[5]+=1;
        }
        if (m_n % 25==0)
        {
            for (int i=0; i<MAXJSH; i++)
                zombies[i].vx += 0.4;//加速
            zmb_p[26]+=1;
        }
        if (m_n % 20==0) //减慢恢复
        {
            for (int i=0; i<MAXSHU; i++)
                cd_spd[i] *= 0.9;
        }
        if (m_n % 15==0)
        {
            for (int i=0; i<MAXJSH; i++)
                zombies[i].eat += 0.3;
        }
    }
    for(int i=0;i<MAXJSH;i++)
        zmb_p[i]*=lamul[i];
}

void MainWindow::NextBo()
{
    m_n=std::min(m_n+1,ND); //下一波
    KillTimer (1002);KillTimer (1004);
    SetTimer (1002, tm_js[m_n]); //重置生成速度
    SetTimer (1004, tm_lst[m_n]); //重置修改速度
    m_nd=jsh[m_n];//增加僵尸种类
    if(m_game.endless)EndLessNd();
    if(zdbs>=2&&zdbs<5)
    {
        int t=dbn;bool b=isdb[m_n];
        if(b_sho)t+=1;
        if(m_n==ND-1)t+=1,b=1;
        if(b)AdjustZMBP(t);
    }
    if (isdb[m_n]) //一大波僵尸
    {
        if(zdbs>=5)
        {
            if(dbn<=5)AdjustZMBP(dbn);
            else AdjustZMBP((dbn-6)%5+1);
        }
        Zombie js;
        js.Create (4, gq_nd*3.5*xmul, 1, (MM - (fr_mid?4:1)) * SI+5, MN / 2, 264, 31); //旗帜僵尸
        m_game.NewJs (js);
        if (mirr) {
            js.x=-100;
            js.vx=-js.vx;
            m_game.NewJs (js);
        }
        else if(MN%2==0)
        {
            js.y-=1;
            m_game.NewJs (js);
        }
        if(b_two)
        {
            js.y=0;m_game.NewJs (js);
            js.y=MN-1;m_game.NewJs (js);
        }
        dbn+=1;m_game.jd+=1;
        GetNxtDbTm();
        //if(!m_ai)activateWindow();
    }
    MakeBoJs();
}

QString MainWindow::GetGqStr(int gg)
{
    if(gg==2048)return QString::asprintf("关卡：2048");
    int z1,z2,z3;
    z3=gg/100;gg%=100;
    if(gg<=60)z1=(gg-1)/10;
    else z1=6;
    z2=gg-z1*10;
    QString rt=QString::asprintf("关卡：%d - %d",z1+1,z2);
    if((z3>>1)&1)rt+=".5";
    if(ext_dif>=-2&&ext_dif<=2)
    {
        for(int s=0;s<ext_dif;s++)rt+='+';
        for(int s=0;s<-ext_dif;s++)rt+='-';
    }
    else if(ext_dif>0)rt+=QString::asprintf("+%d",ext_dif);
    else rt+=QString::asprintf("-%d",-ext_dif);
    if(z3)
    {
        rt+=QString::asprintf("  特性：");
        for(int s=0;s<3;s++)
        {
            if((z3>>s)&1)rt+='*';
            else rt+='.';
        }
    }
    return rt;
}

int MainWindow::CalScore(QString &str)
{
    if (chui)return chui_sc;
    double p=m_game.CalScoreP();
    if(m_ai&&b_fast)ai_1+=p;
    double z[5]={0,1,0.8,0.5,0.3},nd=z[m_ea];
    if(m_hd)nd=1.5;
    if(m_imp)nd=1.8;
    if(sun_doued)nd/=4;
    int dcn=0;
    for(int i=0;i<MAXJSH;i++)
        if(del_js[i])dcn+=1;
    nd*=pow(double(MAXJSH-dcn)/MAXJSH,0.5);
    nd*=xmul*xmul;
    str=GetGqStr(real_gq)+QString::asprintf("%s%s\n得分率：%.6lf%%\n难度加成%.2lf",m_ai?"  自动模式":"",crea?"  创造模式":"",p*100,nd);
    if(dcn)str+=QString::asprintf("\n关闭了%d种僵尸",dcn);
    return p*m_game.jd*nd;
}

void MainWindow::FinishInitIcon()
{
    icotmr.stop();lsZM=zoom*b_ret;
    for(int i=0;i<MAXSHU;i++)
        main_col[plants[i].type]=mco[plants[i].id];
#ifdef NO_UI
    return;
#endif
    ui->btc->setIcon(ico[m_game.started?91:116]);
    ui->btc->setIconSize(ico[m_game.started?91:116].size());
    ui->stop->setIcon(ico[!isstop?117:124]);ui->stop->setIconSize(ico[!isstop?117:124].size());
    //ui->buy->setIcon(ico[118]);ui->buy->setIconSize(ico[118].size());
    ui->quick->setIcon(ico[119]);ui->quick->setIconSize(ico[119].size());
    ui->save->setIcon(ico[120]);ui->save->setIconSize(ico[120].size());
    ui->open->setIcon(ico[121]);ui->open->setIconSize(ico[121].size());
    ui->bt_oth->setIcon(ico[185]);ui->bt_oth->setIconSize(ico[185].size());
}

void MainWindow::UpdateIcon()
{
    if(m_game.started&&icotmr.isActive())
        icotmr.stop();//游戏开始时，停止计时器
    if(upd_ico<=ICCNT||tim.elapsed()-lszm>300)
    {
        if(lszm<inf)//更新期间，缩放比例发生改变
            lszm=inf,upd_ico=1;
        InitOneIcon(upd_ico);upd_ico+=1;
        if(upd_ico>ICCNT)FinishInitIcon();
    }
}

void MainWindow::DoTimer (int tm_id)
{
    m_game.DelExtraZmbPlt();
    if (m_game.over) {
        GameOver ();
        return;
    }
    else if (tm_id<=0) //处理植物的计数器
    {
        int t=m_game.plt_ii[-tm_id];
        if(t>=0)
        {
            if(!b_ycl)
                m_game.Goj (t);
        }
        else KillTimer(tm_id);
    }
    else {
        if (tm_id==1000) //移动僵尸
        {
            if(duiz&&an_game.zmb_cnt>=0)
                for (int i=0; i<an_game.zmb_cnt; i++)
                    an_game.MoveJs (i);
            for (int i=0; i<m_game.zmb_cnt; i++)
                m_game.MoveJs (i);
            for (int i=0; i<m_game.zmb_cnt; i++)
                m_game.zombies[i].x=int(m_game.zombies[i].x_flt+0.5);
        }
        else if (tm_id==1001) //移动子弹
        {
            double t=std::min(m_spd,1.0);
            m_game.MoveAllZd(t);
            if(duiz&&an_game.zmb_cnt>=0)
                an_game.MoveAllZd(t);
            m_game.DoZwAnimine(v_tm,t);
            for (int i=0; i<m_game.plt_cnt; i++)
                m_game.plants[i].id=GetZwIcon (m_game.plants[i]);
        }
        else if (tm_id==1002) //产生僵尸
        {
            if ((b_rd||(tcp.size()>0&&!sender))&&!duiz)return;
            if (is_boss) //Boss关卡
            {
                GenerateJs();KillTimer(1002);
                double p=double(m_game.jd)/m_game.zjd;
                p+=TM/100000.0;if(p>1.5)p=1.5;
                SetTimer(1002,(4000-ext_dif*200)*SI/110.0/pow(10.0,p));
                return;
            }
            if (m_n==ND) //结束
            {
                if (m_game.zmb_cnt==0||(m_ai&&TM>=40000)) {
                    QString str;
                    int sc=CalScore(str);
                    KillTimer (1002);
                    GameOver ();
                    str=QString::asprintf ("胜利！ 得分:%d\n", sc)+str;
                    if(!b_fast||!m_ai)
                        QMessageBox::information (this, tr ("提示"), str);
                    else ai_0+=1,ai_2+=1;
                    activateWindow();
                }
                return;
            }
            else GenerateJs();
        }
        else if (tm_id==1003) //白天阳光
        {
            if(!m_game.is_day)return;
            m_game.m_yg += (sun_dou?37:25);m_game.laygTM=TM;
            sun_tm+=250;KillTimer(1003);SetTimer(1003,sun_tm);
        }
        else if (tm_id==1004) //生成下一波僵尸
            NextBo();
        else if (tm_id==1005) //产生植物(滚卡)
        {
            if (XUAN<MXUAN) {
                int r;
                if(plt11s<20)
                {
                    int pre[20]={87,56,67,2,4,6,7,9,23,10,41,64,35,68,25,76,29,77,8,3};
                    r=pre[plt11s];
                }
                else r=MyRand(plt_sum,plt_p,MAXSHU);
                m_xuan[XUAN++]=r;
                plt11s+=1;
                if(plt11s>20&&plt11s<=50)
                {
                    KillTimer(1005);
                    SetTimer(1005,3600+ext_dif*200+(plt11s-20)*100);
                }
            }
        }
        else if (tm_id==1006) //绘图
        {
            if(drty)/*gl.resize(draw->gl->size()),gl.move(draw->gl->pos()),*/gl.update();
            else update();
        }
        else if (tm_id==1007) //踩
        {
            int sc[MMN]={0}, mx=-1, wz=-1;
            for (int i=0; i<m_game.plt_cnt; i++)
                if (m_game.plants[i].y!=-1)
                    sc[m_game.plants[i].y] += m_game.plants[i].cost;
            for (int n=0; n<MN; n++)
                m_jwbs[n]=0;
            int bos_ps[MMN];
            for (int n=0; n<MN; n++)bos_ps[n]=-1;
            for (int i=0; i<m_game.zmb_cnt; i++) {
                if (m_game.zombies[i].y!=-1&&m_game.zombies[i].type==11) {
                    m_jwbs[m_game.zombies[i].y]=m_game.zombies[i].life;
                    bos_ps[m_game.zombies[i].y]=i;
                }
            }
            for (int y=0; y<MN; y++)
                sc[y]*=(0.5+double(RA())/RA.max());
            for (int y=0; y<MN; y++)
                if (bos_ps[y]!=-1&&sc[y]>mx) //贪心
                    mx=sc[y],wz=y;
            if (wz!=-1) {
                m_game.zombies[bos_ps[wz]].vx=20;
                for(int i=0;i<m_game.zmb_cnt;i++)
                    if(m_game.zombies[i].y==wz&&m_game.zombies[i].type<100&&m_game.zombies[i].type!=11)
                        m_game.DestroyJs(i,0);//把僵尸踩死
            }
        }
        else if (tm_id==1008) //AI
            m_game.AI ();
        else if (tm_id==1009) //Tick (25ms)
        {
            m_game.RemoveExtraSt();
            if(an_game.zmb_cnt>=0)
                an_game.RemoveExtraSt();
            TM += 1;
            if(TM%10==0)
            {
                if(is_boss)
                {
                    for (int n=0; n<MN; n++)
                    {
                        int z=m_game.zmb_ii[n];m_jwbs[n]=0;
                        if(z==-1)continue;
                        m_jwbs[n]=m_game.zombies[z].life/1000;
                        if (m_game.zombies[z].x>(MM - 1) * SI+5) //走出场地
                            m_game.zombies[z].x=m_game.zombies[z].x_flt=(MM - 1) * SI+5,m_game.zombies[z].vx=0;
                    }
                    int jd=0,mx=m_jwbs[MN];
                    for (int n=0; n<MN; n++)
                        jd += mx - m_jwbs[n]; //计算BOSS血量
                    m_game.jd=jd;
                    if (jd==MN * mx) {
                        KillTimer (1002);
                        m_game.over=true;
                        m_game.KillAll ();
                        GameOver ();
                        QMessageBox::information (this, tr ("提示"), tr ("胜利"));
                        activateWindow();
                        return;
                    }
                }
                if(TM%40==0&&duiz)m_game.jd+=1;
                if(TM%40==0&&!b_fast)
                {
                    int t=TM/40;
                    dps_sum[t]=m_game.dps/1000.0;m_game.dps=0;
                    if(t)dps_sum[t]+=dps_sum[t-1];
                    dps_now=dps_sum[t];
                    if(t>=3)dps_now-=dps_sum[t-5];
                    dps_now/=3.0;
                }
                if(TM%20==0)
                {
                    for (int i=0; i<m_game.plt_cnt; i++)
                        if(m_game.plants[i].type==36)
                        {
                            for(int x=-1;x<=1;x++)
                                for(int y=-1;y<=1;y++)
                                {
                                    int w=m_game.GetZwShu(m_game.plants[i].x+x*SI,m_game.plants[i].y+y);
                                    if(w!=-1&&m_game.plants[w].frez_stp)m_game.JieDong(w,4);
                                }
                        }
                    m_game.DuTick();
                    m_game.DianTick();
                    m_game.WanQuanTu();//完全图伤害
                }
                if (TM % 60==0) //每1.5秒一次
                {
                    if(b_guan&&m_game.zmb_cnt==0)//砸罐胜利
                    {
                        int gs=0,cn=0;
                        for(int i=0;i<m_game.plt_cnt;i++)
                            if(m_game.plants[i].type==200)
                            {gs=1;break;}
                        if(!gs)
                        {
                            if(!wait_guan)
                            {
                                wait_guan=1;m_n+=1;
                                for(int i=0;i<MN;i++)kp_noguan[1][i]=1;
                            }
                            for(int i=0;i<MN;i++)
                                for(int j=0;j<MM;j++)cn+=kp_noguan[j][i];
                            if(cn==MN*2)MakeGuan();
                        }
                    }
                    if(m_game.is_cycle)
                    {
                        int tt=TM/40;
                        if(tt%60==30)
                        {
                            m_game.is_day=0;
                            for (int i=0; i<m_game.plt_cnt; i++)
                                if(m_game.plants[i].mush&&m_game.plants[i].odtm<=5)
                                    m_game.Goj(i);
                        }
                        else if(tt%60==0)
                            m_game.is_day=1;
                        update();
                    }
                    if(is_coll)
                    {
                        bool zd=0;
                        for(int i=0;i<MM;i++)
                            for(int j=0;j<MN;j++)
                                if(m_game.js_nd[i][j]>=0)zd=1;
                        if(!zd)
                        {
                            for(int i=0;i<MM;i++)
                                for(int j=0;j<MN;j++)
                                    if(m_game.js_nd[i][j]==-3)m_game.js_nd[i][j]=js_od[i][j],zd=1,ercn+=1;
                            if(!zd)
                            {
                                QMessageBox::information(this,"",QString::asprintf("胜利！用时%.2lf秒，错误次数%d次",TM/40.0,ercn));
                                GameOver();activateWindow();
                            }
                        }
                    }
                    for (int i=0; i<m_game.plt_cnt; i++)
                        if(m_game.plants[i].frez_stp)m_game.JieDong(i,1);
                }
            }
            if(TM%std::max(100-gq_nd*5,10)==0)//每2秒一次
                m_game.FlagTick();
            if(m_game.fans&&TM%2==0)
            {
                int s=m_game.fans;
                for(int i=0;i<m_game.zmb_cnt;i++)
                    if(m_game.zombies[i].type<100)m_game.zombies[i].x_flt+=s;
            }
            if(!b_fast)
            {
                QPoint cur=mapFromGlobal(cursor ().pos ());
                mi_x=std::min(mi_x,cur.x());ma_x=std::max(ma_x,cur.x());
                mi_y=std::min(mi_y,cur.y());ma_y=std::max(ma_y,cur.y());
            }
            if(TM%2==0)
            {
                if(m_ai)
                    for (int n=0; n<MAXSHU; n++) //冷却
                        cd[n]=std::min(cd[n]+cd_spd[n],800.0);
                else
                {
                    for (int i=0; i<XUAN; i++) //冷却
                    {
                        int n=m_xuan[i];
                        cd[n]=std::min(cd[n]+cd_spd[n],800.0);
                    }
                }
                if(!ena_fei)z_fs=0;
                if (z_fs>0 && cd[38]<eps) //化肥数目
                    z_fs -= 1,cd[38]=800;
            }
            m_game.FireTick();
            m_game.MhJsSkillTick();
            if(gqe==7)m_game.ChuiTick();
            if(inf_sun)m_game.m_yg=999999;
            if(nxt_dbtm-v_tm<=2500)
            {
                nxt_dbtm=inf;
                ShiTi st;st.ty=12;
                st.id=0;st.tm=TM;
                st.x=(MM*SI+50)/2;
                st.y=(MN*SI+60)/2;
                if(dbn+1==zdbs)
                {
                    st.y-=60;
                    m_game.NewSt(st);
                    st.id=1;st.y+=60;
                    m_game.NewSt(st);
                }
                else m_game.NewSt(st);
            }
        }
        else if (tm_id==1010) //加速回复倒计时
        {
            for (int i=0; i<XUAN; i++) {
                int t=plants[m_xuan[i]].type;
                if (t!=3&&t!=5&&t!=7&&t!=16&&t!=30&&t!=31)
                    cd_spd[m_xuan[i]] /= 10000;
            }
            KillTimer (1010);
        }
        else if (tm_id==1011) //回放/存档
        {
            if (!b_hui)return;
            if (b_rd) //回放
            {
                HuiFang (0);
                if (w_tm==1)InitIcon ();
                w_tm += (abs (h_sd)==4 ? h_sd / 4 : h_sd);
            }
            else //存档
            {
                QString na=ui->luj->text ();
                na += QString::asprintf ("/%d.in", w_tm);
                QFile file (na);
                on_stop_clicked ();
                file.open (QIODevice::WriteOnly);
                SaveToFile(file);
                on_stop_clicked ();
                stop_cnt-=1;
                w_tm += 1;
            }
            ui->dfn->setText (QString::asprintf ("%d", w_tm));
        }
    }
}

void MainWindow::RestoreFromFile(const QString&na)
{
    if(tmid!=-1)killTimer(tmid);
    QFile file (na);
    file.open (QIODevice::ReadOnly);
    QByteArray da=file.readAll();
    da=qUncompress(da);
    QDataStream ar (&da,QIODevice::ReadOnly);
    if(b_two)
    {int ty;ar>>ty;ar>>ty;}
    Load (ar);file.close ();
    tim.restart();tm_det=v_tm;
    for (int i=0; i<MN; i++)
        m_game.row_zmb[i]=0;
    for (int i=0; i<m_game.zmb_cnt; i++) {
        m_game.row_zmb[m_game.zombies[i].y] += 1;
        m_game.zombies[i].num=i;
    }
    for (int i=0; i<m_game.plt_cnt; i++)
        m_game.plants[i].num=i;
    m_game.nd_calfr=true;
    m_game.over=false;
    on_stop_clicked ();
    ui->Auto->setChecked (m_ai);
    ui->label_2->setFocus ();
    m_game.started=true;
    DisableButton();
    ShowGameButton ();
    InitIcon ();
    tmid=startTimer(20,Qt::PreciseTimer);
}

void MainWindow::HuiFang (bool hs) {
    QString na=ui->luj->text ();
    na += QString::asprintf ("/%d.in", w_tm);
    QFile file (na);
    if(!file.open (QIODevice::ReadOnly))return;
    m_game.started=false;
    QDataStream ar (&file);
    Load (ar);file.close ();
    for (int i=0; i<MN; i++)
        m_game.row_zmb[i]=0;
    for (int i=0; i<m_game.zmb_cnt; i++) {
        m_game.row_zmb[m_game.zombies[i].y] += 1;
        m_game.zombies[i].num=i;
    }
    for (int i=0; i<m_game.plt_cnt; i++)
        m_game.plants[i].num=i;
    m_game.nd_calfr=true;
    m_game.over =false;
    int tt=-ui->lineEdit->text ().toInt ();
    if (tt!=0)h_sd=tt;
    else tt=h_sd;
    if (tt<0)tt=-tt;
    if (hs) tt=1;
    m_spd*=tt;
//    double sd=m_spd * tt;
//
    ui->Auto->setChecked (m_ai);
    //ui->buy->show();
    DisableButton();
    m_game.started=true;
    ShowGameButton ();
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    if(event->timerId()!=tmid)return;
    if(!isstop)OnTick();
}

void MainWindow::SendTick()
{
    if(tcp.size()==0)return;
    if(!duiz&&sender&&sdcnt%10==0)
        SendGameData();
    else if(duiz)
    {
        if(sdcnt%15==0)SendGameData();
        else if(sdcnt%15==5)
        {
            if(q_js.size()>0)
            {
                auto t=q_js.front();q_js.pop();
                Zombie js=t.second;
                js.x-=(tim.elapsed()-t.first)/50*js.vx/2;
                SendDeltaNewJs(js);
            }
        }
        else if(sdcnt%15==10)
        {
            SendDeltaExpYg(m_game.expad,m_game.ygad);
            m_game.expad=m_game.ygad=0;
        }
    }
    sdcnt+=1;
}

void MainWindow::OnTick()
{
    if(!b_fast)
    {
        v_tm=tim.elapsed();
        v_tm=v_tm*m_spd+tm_det;
    }
    if(!b_fast)DoTimer(1006);//绘图
    //if(tim.elapsed()-dbg_tm>30)dbg+=1,dbg_str+=QString::asprintf("%d ",int(tim.elapsed()-dbg_tm));
    //dbg_tm=tim.elapsed();
    //int lt=tim.elapsed();
    while(1)
    {
        int wz=-1,mi=1e9;
        for (int i=0; i<tmr_cnt; i++)
            if(tmr[i].nxt<mi)mi=tmr[i].nxt,wz=i;
        if(b_fast)v_tm=mi;
        if(wz==-1||mi>v_tm)break;
        tmr[wz].nxt+=CalTmrTime(tmr[wz].id,tmr[wz].time);
        DoTimer(tmr[wz].id);
        if(b_fast)break;
    }
    if(!b_fast||!m_ai)UpdateIcon();
    SendTick();
    //if(tim.elapsed()-lt>10)dbg+=1;
}

int MainWindow::CalTmrTime(int id,int tt)
{
    if (id<0)
    {
        int w=m_game.plt_ii[-id];
        if(w==-1)return tt;
        double t=m_game.CalJSQ(m_game.plants[w].x,m_game.plants[w].y);
        if(m_game.plants[w].dzsy==0)
        {
            tt/=t; //加速器
            if(m_game.plants[w].Burning())tt*=1.2;
            if(m_game.plants[w].attack>0&&!m_game.is_tou[m_game.plants[w].type]&&m_game.plants[w].type!=26&&m_game.plants[w].type!=59)//防止同步
            {
                if(RA()%5==0)tt*=(0.9+0.2*double(RA())/RA.max());
                else if(RA()%20==0)tt/=1.5;
            }
            if(tt<20)tt=20;
        }
    }
    else if(id==1001&&m_game.is_cycle&&(!m_game.is_day))
        tt/=1.2;
    return tt;
}

int MainWindow::GetNdTy()
{
    int ty=3;
    if (ui->xin->isChecked ()) //新手
        ty=0;
    else if (ui->water->isChecked ()) //简单
        ty=1;
    else if (ui->easy->isChecked ()) //普通
        ty=2;
    else if (ui->normal->isChecked ()) //困难
        ty=3;
    else if (ui->hard->isChecked ()) //疯狂
        ty=4;
    else if (ui->tui->isChecked ()) //不可能
        ty=5;
    return ty;
}

void MainWindow::SolveZwNd(bool rev)
{
    m_ea=1;int ty=GetNdTy();
    if(rev)ty=std::min(5,6-ty);
    if (ty==0) //新手
    {
        m_ea=4;FIRETM=4;
        for (int i=0; i<MAXSHU; i++) {
            plants[i].life *= 1.3;
            plants[i].time /= (i==0||i==12||i==17?1.1:1.2);
            cd_spd[i] *= 1.3;
            plants[i].cost -= 5;
            if (plants[i].cost<0)
                plants[i].cost=0;
        }
    }
    else if (ty==1) //简单
    {
        m_ea=3;FIRETM=15;
        for (int i=0; i<MAXSHU; i++) {
            plants[i].life *= 1.2;
            plants[i].time /= (i==0||i==12||i==17?1.05:1.1);
            cd_spd[i] *= 1.2;
            plants[i].cost -= 2;
            if (plants[i].cost<0)
                plants[i].cost=0;
        }
    }
    else if (ty==2) //普通
    {
        m_ea=2;FIRETM=25;
        for (int i=0; i<MAXSHU; i++) {
            plants[i].life *= 1.1;
            plants[i].time /= (i==0||i==12||i==17?1.01:1.05);
            cd_spd[i] *= 1.1;
        }
    }
    else if (ty==3) //困难
        m_ea=1,FIRETM=30;
    else if (ty==4) //疯狂
    {
        m_hd=true;FIRETM=35;
        for (int i=0; i<MAXSHU; i++) {
            plants[i].life /= 1.1;
            if (plants[i].life<1)
                plants[i].life=1;
            plants[i].time *= (i==0||i==12||i==17?1.02:1.1);
            cd_spd[i] *= 0.95;
            if (i>0)
                plants[i].cost += 2;
        }
    }
    else if (ty==5) //不可能
    {
        m_hd=true;
        m_imp=true;FIRETM=45;
        for (int i=0; i<MAXSHU; i++) {
            plants[i].life /= 1.2;
            if (plants[i].life<1)
                plants[i].life=1;
            plants[i].time *= (i==0||i==12||i==17?1.05:1.25);
            cd_spd[i] *= 0.9;
            if (i>0)
                plants[i].cost += 5;
        }
    }
    plants[18].cost=0;
}

void MainWindow::SolveJsNd(bool rev)
{
    int ty=GetNdTy();
    if(rev)ty=std::min(5,6-ty);
    if (ty==0) //新手
    {
        for (int i=0; i<MAXJSH; i++)
            zombies[i].life /= 1.25,zombies[i].fs_nd*=1.5;
        zmb_p[21] -= 1;
        zmb_p[22] -= 1;
        zmb_p[11] -= 1;
    }
    else if (ty==1) //简单
    {
        for (int i=0; i<MAXJSH; i++)
            zombies[i].life /= 1.15,zombies[i].fs_nd*=1.3;
        zmb_p[21] -= 1;
    }
    else if (ty==2) //普通
    {
        for (int i=0; i<MAXJSH; i++)
            zombies[i].life /= 1.1,zombies[i].fs_nd*=1.1;
    }
    else if (ty==3); //困难
    else if (ty==4) //疯狂
    {
        for (int i=0; i<MAXJSH; i++) {
            zombies[i].life *= 1.15,zombies[i].fs_nd*=0.9;
            if(i>=3)zombies[i].eat += 0.2;
            zombies[i].vx += 0.2;
        }
        zmb_p[8] += 1;
        zmb_p[11] += 1;
        zmb_p[22] += 1;
    }
    else if (ty==5) //不可能
    {
        for (int i=0; i<MAXJSH; i++) {
            zombies[i].life *= 1.25,zombies[i].fs_nd*=0.75;
            if(i>=2)zombies[i].eat += 0.5;
            zombies[i].vx+=0.5;
        }
        zmb_p[8] += 1;
        zmb_p[10] += 1;
        zmb_p[11] += 1;
        zmb_p[12] += 1;
        zmb_p[21] += 1;
        zmb_p[22] += 1;
    }
}

void MainWindow::SolveNd()
{
    bool rv0=0,rv1=0;
    if(gqe==17)rv0=1;
    else if(gqe==6)rv0=rv1=1;
    SolveZwNd(rv0);SolveJsNd(rv1);
    if(b_two)
    {
        for (int i=0; i<MAXSHU; i++)
        {
            if(i>0)plants[i].cost=int(ceil((plants[i].cost/5)*1.4)+0.5)*5;
            cd_spd[i]/=1.5;
        }
        for(int i=0;i<MAXJSH;i++)zombies[i].life*=1.3;
    }
}

void MainWindow::MakeBo(int gq,int gq_r)
{
    if(gq_r==MAXGQS+20)gq_r=MAXGQS+19;
    for(int i=0;i<MAXND;i++)jsh[i]=0;
    double AD=0.5;bool half=0;
    gq=std::max(1,gq*2+ext_dif);
    if(gq%2!=0)half=1;gq_nd=gq=gq/2;
    double gqd=gq+(half?AD:0);
    ND=5+int (DBS * (std::fmin(gqd,11.0)- 1) * 6+0.5)+6;
    for(int s=0;s<=4;s++)tm_lst[s]=1,tm_js[s]=1e9;
    int fs=(gq<=7?1:gq-6)+b_sho;jsh[0]=1;
    bool no_sta=(gqe==3||gqe==5||gqe==7||gqe==9||gqe==15||gqe==18||gqe==21),slow=(gqe==18);
    if(!no_sta)
    {
        tm_js[0]=10000+std::min(5,fs)*1000;
        tm_lst[0]=tm_js[0]+1000;
        if(fs>1)tm_js[1]=200,tm_lst[1]=200*(fs-1)+100;
        tm_js[2]=7000;tm_lst[2]=8000;
        tm_js[3]=std::fmax(3000,7000-gqd*300);
        tm_lst[3]=20000+std::min(gq,10)*1000;
    }
    jsh[3]=2;jsh[4]=2;
    if(slow||gq_r==MAXGQS+19)jsh[3]=jsh[4]=1;
    tm_js[4]=std::fmax(4000,10000-500*gqd);
    tm_lst[4]=tm_js[4]*2;
    if(b_sho)
    {
        for(int i=0;i<=5;i++)
            if(tm_lst[i]>50)tm_lst[i]/=1.2,tm_js[i]/=1.2;
    }
    if(no_sta)tm_js[3]=500,tm_lst[3]=7600;
    if(gq_r==MAXGQS+15)tm_lst[0]=10000,tm_lst[4]=2600;
    double dad=1000.0 / (GQS-1.0); //每关差值
    int dbs=2000 - int (dad * (std::min(gqd,10.0)- 1)+0.5);zdbs=0;
    if(!b_sho)dbs+=500;
    int dbt=2500+(gqd-1) * 400;double pt=1.0;
    if(gq>=10)dbs*=std::max(1-0.1*(gqd-10),0.2);
    if(b_two)
    {
        dbt*=1.2;
        for(int i=0;i<5;i++)if(tm_lst[i]>50)tm_lst[i]/=1.2;
    }
    if(m_game.endless) //无尽
    {
        tm_js[0]=5000, tm_lst[0]=50000;
        tm_lst[1]=tm_lst[2]=tm_lst[3]=1;
        tm_js[1]=tm_js[2]=tm_js[3]=1e9;
        ND=90;for(int i=0;i<=ND;i++)isdb[i]=false;
        int tm=1500-ext_dif*100;
        dbt=5000+ext_dif*50;
        int i=0, j=0;
        for (int n=5,s=jsh[0]; n<ND; n += 4) {
            if(n+4>=ND)
                s=JSH,dbt*=4,ND=n+1;//最后一波
            else if(n+8>=ND)dbt*=1.5;
            else if(n+12>=ND)dbt*=1.2;
            isdb[n]=true;
            s+=2;jsh[n]=s;
            for (i=n, j=0; i<ND&&j<3; i++, j++)
                tm_js[i]=tm+j*300, tm_lst[i]=dbt;
            tm_js[i]=5000, tm_lst[i]=10000+n*500;
            jsh[i]=s+1;
            dbt += 1500;tm -= 100;
            if (tm<300)tm=300;
        }
    }
    else if(b_ycl||gqe==25)//预处理
    {
        ND=40;
        for(int i=0;i<=ND;i++)isdb[i]=false,jsh[i]=JSH;
        int tm=600-ext_dif*100;
        dbt=2000+ext_dif*200;
        int i=0, j=0;
        for (int n=0; n<ND; n += 4) {
            isdb[n]=true;
            for (i=n, j=0; i<ND&&j<4; i++, j++)
                tm_js[i]=tm+j * 200, tm_lst[i]=dbt;
            dbt += 1000;tm -= 50;
            if (tm<200)tm=200;
        }
    }
    else if(gqe==17)//对战
    {
        ND=3;tm_lst[0]=tm_lst[1]=tm_lst[2]=tm_lst[3]=1e9;
        tm_js[0]=tm_js[1]=tm_js[2]=tm_js[3]=1.5e9;
    }
    else if(is_coll)//拼图
    {
        ND=900;for(int i=0;i<=ND;i++)isdb[i]=false;
        dbt=5000;int tm=1000;
        int i=0, j=0;
        for (int n=5,s=jsh[4]; n<ND; n += 4) {
            isdb[n]=true;s+=2;jsh[n]=s;
            for (i=n, j=0; i<ND&&j<3; i++, j++)
                tm_js[i]=tm+j * 400, tm_lst[i]=dbt;
            tm_js[i]=3000, tm_lst[i]=15000;
            jsh[i]=s+1;dbt += 1000;
        }
    }
    else
    {
        for(int i=0;i<=ND;i++)isdb[i]=false;
        int ad=2-int(slow);
        if(b_sho||no_sta)ad+=1;
        for (int n=5,s=jsh[4]+1; n<ND; n += 6) {
            if(n+6>=ND)//最后一波
            {
                jsh[n]=jsh[n+1]=JSH;
                tm_lst[n]=dbt;tm_lst[n+1]=dbt*1.5;
                tm_js[n]=dbs*pt;tm_js[n+1]=dbs*pt*0.9;
                ND=n+2;isdb[n]=true;break;
            }
            s+=ad;jsh[n]=s;ad+=int(b_sho);
            if(!b_sho&&n==5)ad+=1;
            int i=0, j=0;isdb[n]=true;
            for (i=n, j=0; i<ND&& j<5; i++, j++)
                tm_js[i]=dbs*pt+j*300, tm_lst[i]=dbt*(1.0-j*0.1);
            if(ad==1)jsh[i]=s+1;
            else jsh[i-1]=s+1,jsh[i]=s+2;
            if (b_sho)tm_js[i]=2700, tm_lst[i]=11000;
            else tm_js[i]=3300, tm_lst[i]=13500;
            //间歇
            dbt += 1000;
            if(n==5)pt-=0.2;
            else pt-=0.05;
            pt-=gq/300.0;
            if(pt<0.1)pt=0.1;
        }
    }
    for(int i=0;i<ND;i++)
    {
        if(tm_js[i]*1.25<tm_lst[i])tm_js[i]*=1.25;
        if(i>0&&jsh[i]<jsh[i-1])
            jsh[i]=jsh[i-1];
        jsh[i]=std::min(jsh[i],JSH);
        if(isdb[i])zdbs+=1;
    }
    if(b_sho&&gqe!=9)
    {
        for(int i=0;i<ND;i++)
            if(tm_lst[i]>50)tm_lst[i]/=1.2,tm_js[i]/=1.2;
    }
    if(b_two)
    {
        for(int i=0;i<ND;i++)
            if(tm_lst[i]>50)tm_js[i]/=1.5;
    }
    if(gq_r==1&&crea)ND=1,tm_js[0]=tm_lst[0]=1e8;
    else if(gqe==18||gqe==23||gqe==24)
    {
        for(int i=0;i<ND;i++)
            jsh[i]=JSH;
    }
    else if(gqe==25)
    {
        for(int i=0;i<MAXJSH;i++)zombies[i].vx+=zombies[i].vx/2;
        for(int i=0;i<ND;i++)
            if(tm_lst[i]>50)tm_lst[i]/=1.5,tm_js[i]/=5;
    }
    else if(gqe==7)
    {
        for(int i=0;i<ND;i++)tm_js[i]/=3;
        for(int i=0;i<MAXSHU;i++)plants[i].life*=2.5;
    }
}

void MainWindow::MakeZMBP(int gq,int gq_nd,int lx)
{
    if(gq_nd*2+ext_dif>20)
        zmb_p[19]+=1,zmb_p[26]=(gq_nd*2+ext_dif-20)/2;
    switch(gq_nd)
    {
        case 10:zmb_p[16]+=1;zmb_p[21]+=1;
        case 9:zmb_p[17]+=1;zmb_p[21]+=1;
        case 8:zmb_p[10]+=1;zmb_p[16]+=1;
        case 7:zmb_p[7]+=1;zmb_p[8]+=1;
        case 6:zmb_p[5]+=1;
        case 5:zmb_p[3]+=1;
        default:break;
    }
    if(!b_sho&&m_gq<=MAXGQS)
    {
        zmb_p[7]=zmb_p[9]=zmb_p[15]=zmb_p[22]=zmb_p[25]=zmb_p[27]=zmb_p[28]=0;
        if(gq_nd<=9)zmb_p[13]=0;
        if(gq_nd<=7)zmb_p[10]=0;
    }
    if(m_gq<=50&&gq_nd<=8)zmb_p[13]=0;
    if(gqe==7)zmb_p[12]=zmb_p[26]=0;
    if (gqe==3||gqe==5||gqe==9||gqe==21) //WSAD类,降低难度
    {
        zmb_p[14]=1;zmb_p[15]=0;
        plants[23].mush=plants[32].mush=plants[40].mush=false;
        zmb_p[10] -= 1;zmb_p[16]=3 - m_ea;zmb_p[17]=1;zmb_p[28]=1;
        if (gqe==9)
        {
            zmb_p[8]=zmb_p[10]=zmb_p[14]=zmb_p[15]=zmb_p[15]=0;
            zmb_p[3]-=1;zmb_p[5]-=2;zmb_p[16]=std::min(zmb_p[16],2);
            zmb_p[21]-=1;zmb_p[13]=0;zmb_p[11] -= 1;zmb_p[12] -= 1;zmb_p[27]=1;
        }
    }
    if(gqe==18)
    {
        for (int i=0; i<MAXJSH; i++)
        {
            if(i==0||i==3||i==8||i==10||i==13)continue;
            zmb_p[i]=0;
        }
        zmb_p[3]-=3;
        zmb_p[0]-=2;
    }
    if(gqe==19)
    {
        for (int i=0; i<MAXJSH; i++)
        {
            if(i==23||i==8||i==10||i==14||i==16||i==19||i==22||i==27)continue;
            zmb_p[i]=0;
        }
        zmb_p[8]+=2;zmb_p[16]+=1;zmb_p[22]+=2;zmb_p[24]=2;
        zmb_p[14]+=1;zmb_p[19]+=1;zmb_p[23]=4;zmb_p[27]+=1;
        zombies[8].fs_nd/=1.3;zombies[10].fs_nd/=1.3;zombies[13].fs_nd/=1.3;
    }
    if(gqe==20)
    {
        for (int i=0; i<MAXJSH; i++)
        {
            if(i==23||i==8||i==10||i==13||i==14||i==16||i==19||i==21||i==22||i==27||i==28)continue;
            zmb_p[i]=0;
        }
        zmb_p[8]+=1;zmb_p[16]+=2;zmb_p[19]+=2;zmb_p[21]+=3;
        zmb_p[10]+=1;zmb_p[13]+=2;zmb_p[22]+=1;zmb_p[23]=2;zmb_p[24]=3;
        zmb_p[27]+=1;zmb_p[28]+=2;
        zombies[8].fs_nd/=1.5;zombies[10].fs_nd/=1.5;zombies[13].fs_nd/=1.5;
    }
    if(gqe==22)zmb_p[15]=0;
    if(gqe==23||gqe==24)
    {
        for(int i=0;i<MAXJSH;i++)
        {
            if(i!=20)zmb_p[i]=int(10*sqrt(zmb_p[i]+1.0)+0.5);
            else zmb_p[i]=0;
        }
        plants[0].time/=1.2;
        plants[12].time/=1.2;
        plants[17].time/=1.2;
    }
    if(b_ycl)
        zmb_p[8]-=2,zmb_p[10]-=2,zmb_p[11]-=1,zmb_p[24]=5;
    if(m_ai||m_gq<=5*GQS)zmb_p[18]=0;//气球僵尸
    if(gqe==1)zmb_p[18]=0,zmb_p[8]-=2,zmb_p[10]-=2;
    if(gqe==25)
    {
        for (int n=0; n<MAXJSH; n++)zmb_p[n]=0;
        zmb_p[0]=25;zmb_p[21]=3;zmb_p[8]=2;zmb_p[24]=1;
    }
    for (int n=0; n<MAXJSH; n++)
        if (zmb_p[n]<0)zmb_p[n]=0;
}

int MainWindow::MakeGq (int gq) //制作关卡
{
    bool all=0,b_wsad=0;
    fr_mid=noztm=0;
    if(gq>400&&gq!=2048)gq-=400,fr_mid=1,m_gq=gq;
    if(gq>200&&gq!=2048)gq-=200,b_wsad=1,m_gq=gq;
    if(gq>100&&gq!=2048)gq-=100,all=1,m_gq=gq;
    gqe=gq-MAXGQS;
    int lx=(gq==2048 ? 0 : (gq - 1) / GQS); //类型
    if (lx>=6)lx=0; //特色关卡
    if(gqe==11)lx=4;
    else if(gqe==24)lx=5;
    else if(gqe==4)lx=4;
    if(userlx!=-1)lx=userlx;
    m_game.endless=(gqe==4);
    m_game.MakeSI(lx,usermn,usermm,usersi);Init ();
    can_swi=(all||(gq-1)/10==5||gqe==2||gqe==4||gqe==14||crea||gqe==17||gqe==22)&& !m_ai;
    ws_le=(gqe!=6&&gqe!=12&&gqe!=13);
    b_sho=ui->Short->isChecked();//短时
    chui=imjs=m_imp=m_hd=mirr=b_guan=false;
    m_b48=(gq==2048);ena_fei=true;
    SolveNd();ZWSHU=JSH=0;m_game.zjd=m_game.jd=0;inf_sun=crea;
    is_boss=is12=is_coll=b_zhu=b_ycl=can_wsad=b_fast=false;
    if(gqe==3||gqe==5||gqe==7||gqe==9||gqe==15||gqe==21||gqe==11)b_sho=1;
    if(gqe==4)b_sho=0;
    if (gqe==6) //我是僵尸
    {
        imjs=chui=true;
        XUAN=MXUAN;a_xs=0;
        m_ztm=1e9;noztm=1;
        ND=JSH=0;
        if (plants[0].life<80)
            plants[0].life=80;
        else
            plants[0].life=int (80 * pow (plants[0].life / 80.0, 0.1)+0.5);
        a_xs=0;
        for (int i=0, j=0; i<MAXJSH; i++)
        {
            if(i==1)j_yg[i]=80;
            if(i==0||i==11||i==13||i==18||i==20||i==23||i==15||i==24)continue;
            else x_js[j++]=i;
        }
        zombies[8].fs_nd/=2;zombies[10].fs_nd/=2;zombies[13].fs_nd/=2;
        return 0;
    }
    if(gqe==12)//砸罐子
    {
        XUAN=0;a_xs=0;
        m_ztm=1e9;noztm=1;
        ND=JSH=0;
        b_guan=true;
        return 0;
    }
    gq=(gq - 1) % GQS+1;
    if (gqe==4||m_gq==2048||gqe==7||gqe==11||gqe==10||gqe==15)gq=10;
    if (gqe==5 ||gqe==9||gqe==21||gqe==20)gq=8;
    if(gqe==19||gqe==22)gq=7;
    if (gqe==8)gq=6;
    if (gqe==14||gqe==23||gqe==24||gqe==3)gq=11;
    if(gqe==18)gq=8;
    //特色关卡的难度
    if (gqe==8)mirr=true,cd_spd[43]*=4; //镜面
    if (gqe==7)chui=true, chui_sc=0;
    b_ycl=(gqe==11||gqe==13);
    can_wsad=(b_wsad||gqe==3||gqe==5||gqe==9||gqe==14||gqe==22||gqe==25);
    is_coll=(gqe==22);
    b_zhu=(gqe==10);
    if(b_zhu)plants[38].cost=99999;
    JSH=z_cn[std::min(gq-1,9)];
    if(lx!=0)JSH+=2;
    if(JSH>MAXJSH||gqe==5||gqe==9||gqe==3||gqe==4||gqe==13||gqe==19||gqe==20||gqe==21||gqe==23||gqe==24||gqe==25||is_coll)
        JSH=MAXJSH;
    ZWSHU=plt_cnt[m_gq-1];
    if (ZWSHU>MAXSHU)ZWSHU=MAXSHU;
    MakeZMBP(m_gq,gq,lx);
    if(b_sho)//快速模式
    {
        plants[0].cost+=10;plants[12].cost+=25;
        plants[0].time/=1.2;plants[12].time/=1.2;
        zmb_p[12] += 1;
        if(m_gq!=69)
        {
            for(int i=0;i<MAXJSH;i++)
            {
                int s=z_shun[i];
                zombies[s].life=int(std::max(pow(pow(zombies[s].life,1.5)+pow(i*1000,1.5),2.0/3.0),zombies[s].life*1.2)+0.5);
            }
            for(int i=0;i<MAXSHU;i++)
            {
                if(plants[i].life>=400)
                {
                    plants[i].life*=1.2;
                    plants[i].cost=5*int(plants[i].cost*1.1/5+0.5);
                }
                cd_spd[i]*=1.05;
            }
        }
    }
    else if(!m_game.endless)
    {
        for(int i=0;i<MAXSHU;i++)
            if(sp_plt[i])plants[i].time*=1.2,plants[i].cost=5*int(plants[i].cost/1.25/5+0.5);
    }
    if((m_gq<=60||m_gq==69)&&!(m_gq==1&&crea))
    {
        for(int i=0;i<MAXJSH;i++)
            zombies[i].life=int(pow(zombies[i].life/1000.0,0.85+gq*0.02)*1000.0+0.5);
    }
    bool prex=0;XUAN=7;//卡槽数目
    if (gq>=4)XUAN=8;
    if (gq>=6)XUAN=9;
    if (gq>=8)XUAN=10;
    if (gq>=10)XUAN=11;
    if(lx!=0&&XUAN<MXUAN)XUAN+=1;
    if(lx==5||gqe==14||gqe==19||gqe==20)XUAN=13;
    if (gqe==7||gqe==1)XUAN=0;
    if (gqe==4||gqe==2||gqe==16||b_ycl||b_zhu||is_coll)
        XUAN=MAXXUAN-a_xs;
    if (gqe==1) {
        ZWSHU=0;inf_sun=1;
        a_xs=0;is_boss=true;
        plants[20].mush=plants[32].mush=plants[23].mush=false;
        int jss=0;
        for(int i=0;i<MAXJSH;i++)
        {
            int t=z_shun[i];
            if(zmb_p[t])z_sx[jss++]=t;
        }
        JSH=jss;
        return 0;
    }
    else if (gqe==2||gqe==16) {
        is_boss=is12=true;
        JSH=MAXJSH;
        ZWSHU=MAXSHU;
    }
    else if (gqe==3||gqe==5||gqe==9||gqe==21||gqe==25) {
        def_xuan[a_xs++]=54;
        plants[54].cost=plants[50].cost=10;
        XUAN=0;SwiXuan();prex=1;
    }
    else if (m_gq==2048) {
        ZWSHU=MAXSHU;
        XUAN=8;prex=1;
        int xu[8]={11, 3, 5, 7, 8, 10, 25, 18};
        for (int s=0; s<8; s++)
            m_xuan[s]=xu[s];
    }
    else if(gqe==18)
    {
        ZWSHU=MAXSHU;
        XUAN=9;plants[32].mush=0;prex=1;
        int xu[9]={0,3,5,7,8,12,25,32,35};
        for (int s=0; s<8; s++)
            m_xuan[s]=xu[s];
        plants[25].cost=130;
        cd_spd[32]*=2;plants[32].cost=80;
    }
    else if(gqe==7)
    {
        XUAN=3;prex=1;a_xs=0;
        m_xuan[0]=0;m_xuan[1]=2;m_xuan[2]=7;
    }
    else if(gqe==15||gqe==16)
    {
        JSH=MAXJSH;ZWSHU=MAXSHU;XUAN=11;
        for(int i=0;i<MAXSHU;i++)
        {
            cd_spd[i]/=std::max(2.5,plants[i].cost/50.0);
            plants[i].cost=0;
        }
        plants[0].cost=plants[12].cost=plants[17].cost=99999;
    }
    else if(b_zhu)
    {
        for(int i=0;i<MAXSHU;i++)
            plants[i].cost*=2,cd_spd[i]*=0.8;
    }
    else if(b_ycl)
    {
        plants[0].cost=plants[12].cost=plants[17].cost=99999;
        plants[30].cost+=200;plants[31].cost+=200;
        if(gqe==13)
            plants[3].cost=plants[8].cost=plants[16].cost=plants[19].cost=plants[20].cost=plants[5].cost=plants[42].cost=99999;
    }
    else if(crea&&m_gq==1)//测试用关卡
        zombies[0].life=1000000,zombies[0].eat=0.1;//木桩
    int jss=0;
    for(int i=0;i<JSH;i++)
    {
        int t=z_shun[i];
        if(zmb_p[t])z_sx[jss++]=t;
    }
    JSH=jss;
    MakeBo(gq,m_gq);
    for (int n=0; n<ND; n++) {
        if (n>=5)
            tm_js[n]=tm_js[n] * SI / 110; //僵尸双倍
        if (tm_lst[n] % tm_js[n]==0) //防止生成变少
            tm_lst[n] += std::min(tm_js[n]/5,1000);
    }
    int ms=0;m_game.vdb.clear(); //总个数
    for (int n=0; n<ND; n++)
    {
        ms += int (tm_lst[n] / tm_js[n]);
        if(isdb[n])ms+=1;
    }
    for (int i=0, h=0; i<ND; i++) {
        if (isdb[i]&&!is_boss)m_game.vdb.push_back(h),h+=1;
        h += tm_lst[i] / tm_js[i];
    }
    tm_js[ND]=1000, tm_lst[ND]=100000000;
    if(!b_fast&&!is_boss)m_game.zjd=ms;
    if(gqe==17)
    {
        duiz=1;lx=4;
        ZWSHU=MAXSHU;XUAN=16;
        if(!m_ai)
        {
            for (int i=1; i<MAXSHU; i++)
                plants[i].cost=int(ceil((plants[i].cost/5)*1.2)+0.5)*5;
        }
        else
        {
            plants[0].time*=1.2;plants[12].time*=1.2;
            for (int i=0;i<MAXJSH;i++)
                j_yg[i]= int(ceil((j_yg[i]/5)/1.2)+0.5)*5;
            for (int i=0; i<MAXSHU; i++)
                expnd[i]*=1.5;
        }
        for (int i=0;i<MAXJSH;i++)
            j_yg[i]= int(ceil((j_yg[i]/5)*0.75)+0.5)*5;
        int sl[7]={2,7,10,25,35,44,44};
        for(int i=0;i<7;i++)cd_spd[sl[i]]*=0.7;
        for (int i=0, j=0; i<MAXJSH; i++)
        {
            if (i==0||i==6||i==8||i==11||i==15||i==16||i==9||i==14||i==18)
                continue;
            x_js[j++]=i;
        }
    }
    if (!is12) {
        int tim=0;
        for (int i=0; i<ND; i++)
            tim += tm_lst[i];
        m_ztm=tim;
        zom_cnt=ms;
        if(!all||!m_ai)
            QMessageBox::information (this, tr ("提示"), QString::asprintf ("游戏时间约为%.2lf分钟，约生成僵尸%d个。%s%s", (tim / 60000.0) / m_spd, (mirr ? ms * 2 : ms),crea?"\n创造模式已开启":"",b_two?"\n双人模式已开启":""));
        activateWindow();
    }
    if(m_gq<=5&&!(all||crea))a_xs=0;
    if(all||crea)
        XUAN=MAXXUAN-a_xs,ZWSHU=MAXSHU;
    if(XUAN>ZWSHU)XUAN=ZWSHU;
    RXUAN=XUAN;
    if (m_ai) {
        XUAN=11;
        int xu[11]={0,3,5,8,12,11,26,35,1,6,18};
        for (int i=0; i<11; i++)
            m_xuan[i]=xu[i];
    }
    else if (ZWSHU<=MXUAN) {
        for (int n=0; n<MXUAN; n++)
            m_xuan[n]=-1;
        for (int n=0; n<XUAN; n++)
            m_xuan[n]=p_shun[n];
        prex=true;
    }
    else if (!prex) //选择植物
    {
        Choose *ch=new Choose (this);
        QString bt[6]={"普通", "迷雾", "黑夜", "黑夜+迷雾", "水池", "混合"};
        QString ts[30]={"","僵王","","长时间","","","","双向僵尸","","排山倒海","坚不可摧","","我是植物","WSAD","无限阳光","无限阳光+僵王","对战","反弹","植物僵尸","植物僵尸2","","收集僵尸","全面入侵","全面入侵2"};
        if(m_gq==1&&crea)bt[0]="测试模式";
        if (m_gq>MAXGQS)
        {
            QString t="选择植物："+ts[m_gq-MAXGQS-1];
            if(lx>0)t+="（"+bt[lx]+"）";
            ch->setWindowTitle (t);
        }
        else
            ch->setWindowTitle ("选择植物："+bt[lx]);
        if(gqe!=17)del_x[45]=del_x[46]=del_x[47]=del_x[48]=1;
        ch->SHU=0;
        ch->XUAN=XUAN;
        ch->JSH=JSH;
        ch->gq=real_gq;
        for (int i=0; i<ZWSHU; i++) {
            int t=p_shun[i];
            if(del_x[t])continue;
            ch->yg[ch->SHU]=plants[t].cost;
            ch->ico[ch->SHU++].load (QString::asprintf ("://icons/%d.png", plants[t].id));
        }
        for (int i=0; i<JSH; i++)
            ch->icj[i].load (QString::asprintf ("://icons/%d.png", zombies[z_sx[i]].id));
        ch->setWindowModality (Qt::ApplicationModal);
        ch->Init ();ch->show ();
        if (!ch->exec ())
            throw (1); //未选择
        for (int i=0; i<XUAN; i++)
        {
            if(ch->xuan[i]==-1)m_xuan[i]=-1;
            else m_xuan[i]=p_shun[ch->xuan[i]];
        }
        while(XUAN>0&&m_xuan[XUAN-1]==-1)
            XUAN-=1;
        for(int i=0;i<a_xs;i++)
            m_xuan[i+XUAN]=def_xuan[i];
        XUAN+=a_xs;
        CheckMFZ(m_xuan,XUAN);
    }
    if(prex)
    {
        for(int i=0;i<a_xs;i++)
            m_xuan[i+XUAN]=def_xuan[i];
        XUAN+=a_xs;
    }
    return lx;
}

void MainWindow::CheckMFZ(int xz[MAXXUAN],int n)
{
    int wz=-1;
    for(int i=0;i<n;i++)
        if(plants[xz[i]].type==84)
        {
            wz=i;
            break;
        }
    if(wz==-1)return;
    int t=xz[wz==0?1:wz-1];
    plants[84].cost=plants[t].cost;
    plants[84].tag=t;
    cd_spd[84]=cd_spd[t];
}

void MainWindow::GameOver (int ty) {
    QFile fi(path+"lastplay");
    fi.remove();
    m_game.over=true;
    if(tmid!=-1)killTimer(tmid);tmid=-1;
    if(!b_fast)EnableButton();
    if (m_game.started)
        m_game.KillAll ();
    ZWSHU=JSH=ND=tmr_cnt=0;
    m_game.started=false;
#ifdef Q_OS_WIN
    ui->centralwidget->show(),hiui=0;
#else
#ifdef Q_OS_IOS
    ui->centralwidget->show(),hiui=0;
    ui->menubar->show();
#else
#ifdef Q_OS_ANDROID
    ui->centralwidget->show(),hiui=0;
    ui->menubar->show();
#endif
#endif
#endif
    if(ty==0&&!b_fast)
        on_lineEdit_textEdited(ui->lineEdit->text());
}

void MainWindow::MakeGuan()//砸罐
{
    plants[37].tag=4;
    for(int i=0;i<MAXSHU;i++)plants[i].mush=false;
    for(int i=0;i<MAXJSH;i++)zombies[i].life*=1.2;//难度增加
    Plant zw;zw.Create(Qt::black,0,1e8,1e8,0,80,0,0,165,0,200);//罐子
    int gj[MMN][MMM],mx=MM-4,gs=MN,zs=MN*(mx+1);
    int wx[MMN*MMM],wy[MMN*MMM],ws;bool gre[MMN][MMM];
    static int lakw=0;
    if(m_n==0)lakw=0;
    for(int ss=0;ss<1000;ss++)
    {
        for(int i=0;i<MN;i++)
            for(int j=0;j<=mx;j++)
                gj[i][j]=-1,gre[i][j]=0;
        int ps=zs/(2.2+m_n*0.45)+0.1;
        ps=std::max(ps-ext_dif,1);
        ps=std::min(ps,zs*2/3);
        int kx[36]={3,4,5,35,6,7,8,9,11,14,16,23,25,26,29,60,32,37,41,15,36,58,61,68,69,70,74,76,77,78,79,80,82,83,86,90},kn=36;
        for(int i=0;i<ps;i++)//放置植物
        {
            int zz=kx[lakw];
            lakw=(lakw+1)%kn;
            while(1)
            {
                int y=RA()%MN,x=RA()%(mx+1);
                if(gj[y][x]<0)
                {
                    gj[y][x]=zz+1;
                    break;
                }
            }
        }
        for(int s=0,cn=0;s<1000&&cn<gs;s++)//绿色罐子
        {
            int y=RA()%MN,x=RA()%(mx+1);
            if(gj[y][x]>0&&!gre[y][x])
                gre[y][x]=1,cn+=1;
        }
        std::pair<int,int> jw[MMN*MMM];int js=0;
        for(int i=0;i<MN;i++)
            for(int j=0;j<=mx;j++)
                if(gj[i][j]==-1)
                    jw[js++]=std::make_pair(i,j);
        std::shuffle(jw,jw+js,RA);//打乱僵尸位置
        for(int i=0;i<js;i++)
        {
            int t=i%MAXJSH;
            if(t==18)t+=1;
            if(t==20||(t==21&&m_n<=1))t=5;
            if(t==23||t==26)t-=1;
            /*if(m_n>=4)
            {
                if(i<m_n/2)t=11;
                else if(i<m_n)t=12;
            }*/
            gj[jw[i].first][jw[i].second]=-t-1;
        }
        ws=0;
        for(int i=0;i<MN;i++)
            for(int j=1;j<MM;j++)
                if(!kp_noguan[j][i])wx[ws]=j,wy[ws]=i,ws+=1;
        bool err=0;
        for(int i=0,s=0;i<MN;i++)
            for(int j=0;j<=mx;j++,s++)
                if(wx[s]<=3&&(gj[i][j]==-12||gj[i][j]==-13))//小车，巨人太近
                    err=true;
        if(!err)break;
    }
    for(int i=0;i<MN;i++)
        for(int j=0;j<MM;j++)
            kp_noguan[j][i]=0;
    for(int i=0,s=0;i<MN;i++)
    {
        for(int j=0;j<=mx;j++,s++)
        {
            int x=wx[s],y=wy[s];
            zw.attack=gj[i][j];
            m_game.DestroyZw(x*SI,y,0);
            m_game.m_ice[x][y]=-1;
            if(gre[i][j])zw.id=171;//绿色
            else zw.id=165;
            m_game.PlantZw(zw,x,y);
            zw.move_spd=0;
        }
        m_game.cars[i]=0;
    }
    m_game.m_yg=9999;wait_guan=0;
}

void MainWindow::MakeImjs()
{
    bool rnd=ui->Short->isChecked ();//是否随机
    plants[37].tag=4;
    m_game.m_yg=inf;
    isstop=false;
    for(int i=0;i<MAXSHU;i++)
        plants[i].mush=false;
    int sed=0;
    bool ok;sed=ui->dfnz->text().toInt(&ok);
    if(!ok)sed=(rnd?time(NULL):0);
    if(rnd)srand(sed);
    for (int i=0; i<MN; i++) {
        for (int j=0,r; j<MN; j++) {
            for (int s=0; s<MAXSHU; s++)
                plants[s].nxt_tm=rand () % (plants[s].time/2+1)+plants[s].time / 2; //随机开始时间
            if (i==0) {//最左面生成攻击类植物
                int gj[10]={56, 4, 68, 9, 11, 59, 60, 41,44,61};
                r=gj[rnd ? rand () % 10 : (j+sed) % 10];
            }
            else {
                if (rnd)//随机
                    r=rand () % MAXSHU;
                else
                    r=((i+j)*(i*j+1)+(j+1)%(i+1)+sed) % MAXSHU;
                if(!rnd&&(i+j+j+sed)%5==0)r=0;
                if(rnd&&rand()%5==0)r=0;
                if(rnd&&rand()%20<-ext_dif)r=0;
                if (not_imjsz[r]) {
                    if ((rnd ? (rand () % std::max(8-ext_dif,1)>2) : ((i+i+j+sed) % 4== 0)))
                        r=0; //增大向日葵概率
                    else {
                        r=(i+1) % (j+1);
                        r=(r * r * (i * MN+j)+sed) % MAXSHU;
                        if (rnd)
                            r=rand () % MAXSHU;
                        if (not_imjsz[r])
                            r=0;
                    }
                }
            }
            m_game.PlantZw (plants[r],i,j);
        }
    }
    m_game.m_yg=300;
}

void MainWindow::MakePreset()
{
    m_game.m_yg=inf;
    for (int i=0; i<MM/2; i++)
        for (int j=0; j<MN; j++)
            if (m_game.grid_type[i][j]==1)
                m_game.m_hy[i][j]=800;
    for (int i=0; i<MN; i++)
    {
        m_game.PlantZw (plants[12],0,i);
        m_game.PlantZw (plants[56],1,i);
        m_game.PlantZw (plants[6],2,i);
        m_game.PlantZw (plants[7],MM*3/4,i);
    }
    m_game.m_yg=50;
}

void MainWindow::StartGame (int lx) {
#ifdef Q_OS_IOS
    ui->centralwidget->hide(),hiui=1;
    ui->menubar->hide();
#endif
#ifdef Q_OS_ANDROID
    ui->centralwidget->hide(),hiui=1;
    ui->menubar->hide();
#endif
    if(!b_fast)activateWindow();
    select=(gqe==7?-2:-1);
    sdcnt=exp=ercn=sdcnt=0;
    sun_dou=sun_doued=b_sun;
    f_shu=z_fs=swizw=plt11s=m_n=dbn=0;
    labeanTM=-inf;xuan_st=0;
    tim.restart();v_tm=tm_det=0;
    m_nd=(is_boss ||b_ycl? JSH : jsh[0]);
    if(!b_fast)tmid=startTimer(20,Qt::PreciseTimer);
    if(gqe!=13)SetTimer (1000, (gqe==7?20:50));//僵尸移动
    SetTimer(1001,20*(m_spd<1?m_spd:1));//子弹
    if (!imjs&&!b_ycl&&!b_guan)
        SetTimer (1002, is_boss ? (4000-ext_dif*200) * SI / 110.0 : tm_js[0]);//第一个僵尸
    if (!imjs&&!b_ycl&&!b_guan&&!is_boss)
        SetTimer (1004, tm_lst[0]);//第一波
    if (m_ai)
        SetTimer (1008, 300);//自动
    if (is_boss) //BOSS
    {
        if(!is12)SetTimer (1005, 3600+ext_dif*200);//生成
        SetTimer (1007, is12?120000:80000);//踩
    }
    SetTimer (1009, 25);
    if ((b_hui=ui->hui->isChecked ())) //生成回放
        SetTimer (1011, 100);
    for (int n=0; n<MAXSHU; n++)
        cd[n]=(n==0||n==17||n==12||b_guan?800.0:0);
    m_game.CreateGame (lx);
    if ((m_game.is_cycle||m_game.is_day)&&!imjs&&!b_ycl&&!b_guan&&!(gqe==3||gqe==5||gqe==9))
    {
        sun_tm=(gqe==15?16000:5000);
        SetTimer (1003, sun_tm);//阳光
    }
    if (!m_game.is_day) //黑夜,降低阳光速率
        plants[0].time *= 1.5, plants[12].time *= 1.5;
    if(SI!=laSI)upd_ico=1;
    for (int i=0; i<MM; i++)
        for (int j=0; j<MN; j++)
            wsad_col[i][j]=(can_wsad?0:-1);
    if(!can_wsad&&ws_le)
    {
        for (int j=0; j<MN; j++)
            wsad_col[0][j]=1;
    }
    if (is_boss)//BOSS关卡
    {
        for (int n=0; n<MN; n++)
            m_jwbs[n]=m_jwbs[MN];
        if (is12)
            m_game.m_yg=165000/SI;
        else
            m_game.m_yg=10000;
    }
    for (int n=0; n<MN; n++)
    {
        m_lstB[n] =m_lstC[n] =-99999999;
        m_lstD[n] =m_lstJ[n] =-99999999;
        m_lstT[n] =m_lstZ[n] =-99999999;
        m_SumZ[n]=0;//清除僵尸历史记录
    }
    if(m_game.endless)beans=5;
    else if((is_boss&&!is12)||b_guan)beans=3;
    else beans=0;
    if(m_hd)m_game.m_yg+=25;
    if(m_imp)m_game.m_yg+=25;
    if(b_zhu)m_game.m_yg=250;
    if(b_sho||m_game.endless)m_game.m_yg+=900;
    if((m_game.endless||b_sho)&&!can_wsad&&!imjs&&!b_guan)
    {
        int mi=MN/2;
        for(int i=0;i<MN;i++)
            m_game.spd_mul[abs(i-mi)][i]=m_game.spd_mul[abs(i-mi+(MN%2==0))+MM-2-mi][i]=1.5;
        for(int i=0;i<MM;i++)
            for(int j=0;j<MN;j++)
                m_game.lf_mul[i][j]=m_game.spd_mul[i][j];
    }
    if(gqe==23||gqe==24)
        m_nd=JSH,m_game.m_yg+=2000;
    if(gqe==3||gqe==5||gqe==9||gqe==11||gqe==13||gqe==15||gqe==16)beans=0;
    if (gqe==3||gqe==9)//WSAD
    {
        int zx=0,zy=0;
        m_game.m_yg=inf;
        plants[37].tag=4;
        for (int i=0; i<MAXSHU; i++)
            for (int s=0; s<(gqe==3?wsad[i]:wsad2[i]); s++)
            {
                plants[i].mush=0;
                m_game.PlantZw (plants[i],zx,zy);
                zy += 1;
                if (zy>=MN)
                    zx += 1, zy=0;
            }
        m_game.m_yg=25;
    }
    else if(gqe==25)
    {
        m_game.m_yg=inf;
        plants[30].time=1500;
        for(int i=0;i<MN;i++)
        {
            m_game.PlantZw (plants[31],0,i);
            m_game.PlantZw (plants[30],1,i);
        }
        for (int s=0; s<3; s++)
            m_game.PlantZw (plants[7],3,s);
        m_game.PlantZw (plants[13],3,3);
        m_game.PlantZw (plants[25],3,4);
        m_game.PlantZw (plants[13],2,0);
        for(int y=1;y<5;y++)
            m_game.PlantZw (plants[55],2,y);
        plants[50].cost=20;cd_spd[50]=5;
        zombies[24].life=10;
        m_game.m_yg=100;
    }
    else if(gqe==5)//完全图生成器
    {
        int zx=0,zy=0;
        m_game.m_yg=inf;
        for (int s=0; s<(9+m_ea - m_hd) * 110 / SI; s++)
        {
            m_game.PlantZw (plants[33],zx,zy);
            zy += 1;
            if (zy>=MN)
                zx += 1, zy=0;
        }
        for (int s=0; s<330 / SI; s++)
        {
            m_game.PlantZw (plants[10],zx,zy);
            zy += 1;
            if (zy>=MN)
                zx += 1, zy=0;
        }
        m_game.PlantZw (plants[13],zx,zy);
        zy += 1;
        if (zy>=MN)
            zx += 1, zy=0;
        m_game.PlantZw (plants[27],zx,zy);
        m_game.m_yg=25;
    }
    else if(gqe==22)
    {
        for(int x=3,s=0;x>=0;x--)
            for(int y=0;y<MN;y++,s++)
                m_game.js_nd[x][y]=js_od[x][y]=zombies[z_sx[s%JSH]].type;
    }
    else if(gqe==21)
    {
        int zz[5]={4,55,55,7};
        for(int s=0;s<4;s++)
        {
            for(int y=0;y<MN;y++)
            {
                Plant zw=plants[zz[s]];
                zw.cost=0;zw.x=s*SI;zw.y=y;
                m_game.PlantZw(zw);
            }
        }
    }
    else if(gqe==18)
    {
        for(int s=0;s<3;s++)
        {
            Plant zw=plants[25];
            zw.cost=0;zw.x=0;zw.y=s;
            m_game.PlantZw(zw);
        }
        Plant zw=plants[9];
        zw.cost=0;zw.x=0;zw.y=3;
        m_game.PlantZw(zw);
    }
    else if(gqe==13)//我是植物
    {
        int wz[MMN],sx[MAXJSH];
        for(int i=0;i<MAXJSH;i++)sx[i]=i;
        for(int i=0;i<MN;i++)wz[i]=(MM-1)*SI;
        bool ok;int sed=ui->dfnz->text().toInt(&ok);
        if(!ok)sed=rand();
        std::shuffle(sx,sx+MAXJSH,std::mt19937(sed));
        for(int i=0;i<MAXJSH;i++)
        {
            int w=i%MN,z=sx[i];
            if(z==14||z==15)z=3;
            zombies[z].x=wz[w];zombies[z].y=w;
            wz[w]-=SI/2;
            m_game.NewJs(zombies[z]);
        }
        for(int i=0;i<MN;i++)m_game.cars[i]=0;
    }
    else if(imjs)
        MakeImjs();
    else if(b_guan)
    {
        for(int i=0;i<MN;i++)kp_noguan[1][i]=kp_noguan[2][i]=1;
        MakeGuan();
    }
    else if (m_game.endless&&ui->Short->isChecked()) //预设
        MakePreset();
    if (m_b48)
        m_game.m_yg=500;
    if (crea)
    {
        for (int i=0; i<MAXSHU; i++)
            cd_spd[i]=800;
        m_game.m_yg=exp=inf;
        plants[37].tag=4;
    }
    if(b_ycl)
    {
        for(int i=0;i<MAXSHU;i++)
            if(i!=38)cd_spd[i]+=800;
        if(gqe==13)
            m_game.m_yg=5000,z_fs=3;
        else
            m_game.m_yg=9000*91/SI,z_fs=550/SI;
        if(m_ai)m_game.m_yg*=3;
    }
    else
    {
        ui->huisu->show();
        ui->huisu->setText("回溯");
    }
    draw->gl->update ();
    ui->a_swi->setEnabled(can_swi);
    for(int i=0;i<MAXJSH;i++)lamul[i]=1;
    if(!is_boss&&!b_sho)AdjustZMBP(0);
    if(m_ai)
    {
        b_fast=(real_gq/100)%2;
        if(b_fast)//瞬间模式
        {
            while(!m_game.over)
                OnTick();
        }
    }
    SendInt(3,m_game.m_yg);
    MakeBoJs();
    GetNxtDbTm();
}
void MainWindow::SetTimer (int id, int jg) {
    if(jg>MAX_TMR_TIME)return;
    tmr[tmr_cnt++]=MyTimer (id, v_tm+jg, jg);
}
void MainWindow::NewTimer (int id, int jg, int ne) //添加临时计时器
{
    if(jg>MAX_TMR_TIME)return;
    tmr[tmr_cnt++]=MyTimer (id, v_tm+ne, jg);
}
int MainWindow::KillTimer (int id) {
    int z=FindTimer(id);
    if (z==-1)return -1;
    int rt=std::max(0,tmr[z].nxt-v_tm);
    tmr_cnt -= 1;
    if (z<tmr_cnt)
        tmr[z]=tmr[tmr_cnt];
    return rt;
}
void MainWindow::Save (QDataStream &ar) {
    if(tcp.size()==0)
    {
        ar<<MAXSHU<<MAXJSH;
        for (int i=0; i<MAXSHU; i++) {
            plants[i].output (ar);
            ar << cd_spd[i] << cd[i];
        }
        for (int i=0; i<MAXJSH; i++) {
            zombies[i].output (ar);
            ar << zmb_p[i] << j_yg[i]<<lamul[i];
        }
    }
    else ar<<0<<0;
    ar << MN << MM << SI << TM<<W0<<W1<<HI;
    m_game.output (ar);
    ar << tmr_cnt;
    for (int i=0; i<tmr_cnt; i++)
        tmr[i].output (ar);
    ar << ND << m_nd << m_n << ZWSHU << JSH << zom_cnt << m_ztm << imjs << chui << mirr << f_shu << z_fs<<b_ycl<<b_fast<<l48<<b_guan<<sun_dou;
    if(tcp.size()==0)
    {
        for (int i=0; i<JSH; i++)
            ar<<z_sx[i];
        for (int i=0; i<=ND; i++)
            ar << tm_js[i] << tm_lst[i] <<isdb[i]<<jsh[i];
    }
    ar << select << sele_x << sele_y << beans<<xuan_st;
    if(tcp.size()==0)
    {
        ar<<RXUAN<<XUAN<<a_xs;
        for (int i=0; i<MAXXUAN; i++)
            ar << m_xuan[i]<<x_js[i];
        for (int i=0; i<a_xs; i++)
            ar << def_xuan[i];
    }
    for (int i=0; i<=MN; i++)
        ar << m_jwbs[i];
    ar << is_boss << is12 << m_ai << m_gq << m_ea << m_hd << m_imp << m_b48<<real_gq<<gq_nd<<ext_dif;
    for (int i=0; i<MN; i++)
        ar << m_lstJ[i] << m_lstC[i] << m_lstD[i] << m_lstT[i] << m_lstB[i]<<m_lstZ[i]<<m_SumZ[i];
    ar << ui->speed->text () << ui->Short->isChecked () << crea<<b_sho;
    ar <<sun_doued<<stop_cnt;
    for(int i=0;i<MAXJSH;i++)ar<<zmb_sum[i]<<del_js[i]<<lamul[i];
    for(int i=0;i<MAXSHU;i++)ar<<plt_sum[i];
    ar<<wait_guan<<can_swi<<zuobi<<can_wsad<<xmul<<nxt_dbtm;
    if(tcp.size()==0)
    {
        for(int i=0;i<MM;i++)
            for(int j=0;j<MN;j++)ar<<wsad_col[i][j]<<js_od[i][j]<<kp_noguan[i][j];
    }
    ar<<ws_le<<fr_mid<<ui->lineEdit->text()<<auto_fire<<b_two<<xie<<is_coll<<ercn<<noztm<<dbn<<zdbs;
    ar<<sun_tm<<swizw<<dps_now<<inf_sun<<plt11s;
    if(tcp.size()>0)ar<<sdcnt;
    //v_tm=m_spd*tim.elapsed()+tm_det;
    int m=(tm_js[m_n]==0||is_boss?500:tm_lst[m_n]/tm_js[m_n]);
    if(m>500)m=500;
    ar<<v_tm<<m<<js_nwi<<bgid;
    for(int i=0;i<m;i++)ar<<js_nw[i];
}
void MainWindow::Load (QDataStream &ar) {
    int s1,s2;ar>>s1>>s2;
    for (int i=0; i<s1; i++) {
        plants[i].input (ar);
        ar >> cd_spd[i] >> cd[i];
    }
    for (int i=0; i<s2; i++) {
        zombies[i].input (ar);
        ar >> zmb_p[i] >> j_yg[i]>>lamul[i];
    }
    ar >> MN >> MM >> SI >> TM>>W0>>W1>>HI;
    m_game.input (ar);
    ar >> tmr_cnt;
    for (int i=0; i<tmr_cnt; i++)
        tmr[i].input (ar);
    ar >> ND >> m_nd >> m_n >> ZWSHU >> JSH >> zom_cnt >> m_ztm >> imjs >> chui >> mirr >> f_shu >> z_fs>>b_ycl>>b_fast>>l48>>b_guan>>sun_dou;
    if(b_ycl)
    {
        ui->huisu->setText("进攻");
        ui->huisu->show();
    }
    else
    {
        ui->huisu->setText("回溯");
        ui->huisu->show();
        ui->dfnz->show();
    }
    if(tcp.size()==0)
    {
        for (int i=0; i<JSH; i++)
            ar>>z_sx[i];
        for (int i=0; i<=ND; i++)
            ar >> tm_js[i] >> tm_lst[i]>>isdb[i]>>jsh[i];
    }
    ar >> select >> sele_x >> sele_y >> beans>>xuan_st;
    if(tcp.size()==0)
    {
        ar>>RXUAN>>XUAN>>a_xs;
        for (int i=0; i<MAXXUAN; i++)
            ar >> m_xuan[i]>>x_js[i];
        for (int i=0; i<a_xs; i++)
            ar >> def_xuan[i];
    }
    for (int i=0; i<=MN; i++)
        ar >> m_jwbs[i];
    ar >> is_boss >> is12 >> m_ai >> m_gq >> m_ea >> m_hd >> m_imp >> m_b48>>real_gq>>gq_nd>>ext_dif;
    for (int i=0; i<MN; i++)
        ar >> m_lstJ[i] >> m_lstC[i] >> m_lstD[i] >> m_lstT[i] >> m_lstB[i]>>m_lstZ[i]>>m_SumZ[i];
    QString sd;ar >> sd;
    ui->speed->setValue (m_spd=sd.toDouble ());
    if (m_ea==4)
        ui->xin->setChecked (1);
    else if (m_ea==3)
        ui->water->setChecked (1);
    else if (m_ea==2)
        ui->easy->setChecked (1);
    else if (m_imp)
        ui->tui->setChecked (1);
    else if (m_hd)
        ui->hard->setChecked (1);
    else
        ui->normal->setChecked (1);
    bool sho;ar >> sho;
    ui->Short->setChecked (sho);
    ar >> crea>>b_sho>>sun_doued>>stop_cnt;
    for(int i=0;i<MAXJSH;i++)ar>>zmb_sum[i]>>del_js[i]>>lamul[i];
    for(int i=0;i<MAXSHU;i++)ar>>plt_sum[i];
    ar>>wait_guan>>can_swi>>zuobi>>can_wsad>>xmul>>nxt_dbtm;
    if(tcp.size()==0)
    {
        for(int i=0;i<MM;i++)
            for(int j=0;j<MN;j++)ar>>wsad_col[i][j]>>js_od[i][j]>>kp_noguan[i][j];
    }
    ar>>ws_le>>fr_mid>>sd>>auto_fire>>b_two>>xie>>is_coll>>ercn>>noztm>>dbn>>zdbs;
    ar>>sun_tm>>swizw>>dps_now>>inf_sun>>plt11s;
    if(!b_hui)ui->lineEdit->setText(sd);
    int m;ar>>v_tm>>m>>js_nwi>>bgid;
    for(int i=0;i<m;i++)ar>>js_nw[i],js_nw[i+m]=js_nw[i];
}

int MainWindow::FindTimer(int id)
{
    for (int i=0; i<tmr_cnt; i++)
        if(tmr[i].id==id)return i;
    return -1;
}

void MainWindow::left2048 () {
    if (TM - l48<30)//不可用
        return;
    l48=TM;
    Plant lx[MMM][MMN];
    int dy[MAXSHU];
    for (int i=0; i<MAXSHU; i++)
        dy[plants[i].type]=i;
    for (int i=0; i<MN; i++) {//获取植物信息
        for (int j=0; j<MN; j++) {
            int z=m_game.GetZwShu (j * SI, i);
            if (z==-1)
                lx[j][i].type=-1;
            else
                lx[j][i]=m_game.plants[z];
        }
    }
    for (int i=0; i<m_game.plt_cnt; i++) {
        if (m_game.plants[i].x / SI<MN) {
            int x=m_game.plants[i].x / SI, y=m_game.plants[i].y;
            lx[x][y].nxt_tm=m_game.DestroyZw (i,0);//记录（距离下次）时间
            i -= 1;
        }
    }
    for (int i=0; i<MN; i++) {
        Plant st[MMM];
        int tp=0;
        bool ko=false, bk=false;
        for (int j=0; j<MN; j++) {
            Plant z=lx[j][i];
            lx[j][i].type=-1;
            if (z.type==-1)
                ko=true;
            else if (ko)
                bk=true;
            while (z.type!=-1) { //压栈
                if (tp==0||z.type!=st[tp - 1].type||n_2048[z.type]<=0) {
                    st[tp++]=z;
                    z.type=-1;
                }
                else {
                    int t=n_2048[st[--tp].type];
                    if (n_2048[t]==-1) {//最终
                        m_game.m_yg += 1500;
                        if (beans<5)
                            beans += 1;
                    }
                    z=plants[dy[t]];//新的
                }
            }
        }
        if (tp<MN&&rand () %  GL48==0&&(tp==0||bk)) {//随机生成
            if (tp==0)
                st[tp++]=plants[1];
            else {
                Plant t=st[tp - 1];
                st[tp++]=(t.type==0 ? plants[6] : plants[1]);
            }
        }
        for (int j=0; j<tp; j++)
            lx[j][i]=st[j];
    }
    for (int i=0; i<MN; i++) {//恢复
        for (int j=0; j<MN; j++) {
            Plant z=lx[j][i];
            if (z.type==-1)
                continue;
            m_game.m_yg += z.cost;
            z.x=j * SI;
            z.y=i;
            m_game.PlantZw (z);
        }
    }
    m_game.nd_calfr=true;
}
void MainWindow::up2048 () { //同上
    if (TM - l48<30)
        return;
    l48=TM;
    Plant lx[MMM][MMN];
    int dy[MAXSHU];
    for (int i=0; i<MAXSHU; i++)
        dy[plants[i].type]=i;
    for (int i=0; i<MN; i++) {
        for (int j=0; j<MN; j++) {
            int z=m_game.GetZwShu (j * SI, i);
            if (z==-1)
                lx[j][i].type=-1;
            else
                lx[j][i]=m_game.plants[z];
        }
    }
    for (int i=0; i<m_game.plt_cnt; i++) {
        if (m_game.plants[i].x / SI<MN) {
            int x=m_game.plants[i].x / SI, y=m_game.plants[i].y;
            lx[x][y].nxt_tm=m_game.DestroyZw (i,0);
            i -= 1;
        }
    }
    for (int j=0; j<MN; j++) {
        Plant st[MMN];
        int tp=0;
        bool ko=false, bk=false;
        for (int i=0; i<MN; i++) {
            Plant z=lx[j][i];
            lx[j][i].type=-1;
            if (z.type==-1)
                ko=true;
            else if (ko)
                bk=true;
            while (z.type!=-1) {
                if (tp==0||z.type!=st[tp - 1].type||n_2048[z.type]<=0) {
                    st[tp++]=z;
                    z.type=-1;
                }
                else {
                    int t=n_2048[st[--tp].type];
                    if (n_2048[t]==-1) {
                        m_game.m_yg += 1500;
                        if (beans<5)
                            beans += 1;
                    }
                    z=plants[dy[t]];
                }
            }
        }
        if (tp<MN&&rand () % GL48==0&&(tp==0||bk)) {
            if (tp==0)
                st[tp++]=plants[1];
            else {
                Plant t=st[tp - 1];
                st[tp++]=(t.type==0 ? plants[6] : plants[1]);
            }
        }
        for (int i=0; i<tp; i++)
            lx[j][i]=st[i];
    }
    for (int i=0; i<MN; i++) {
        for (int j=0; j<MN; j++) {
            Plant z=lx[j][i];
            if (z.type==-1)
                continue;
            m_game.m_yg += z.cost;
            z.x=j * SI;
            z.y=i;
            m_game.PlantZw (z);
        }
    }
    m_game.nd_calfr=true;
}
void MainWindow::right2048 () { //同上
    if (TM - l48<30)
        return;
    l48=TM;
    Plant lx[MMM][MMN];
    int dy[MAXSHU];
    for (int i=0; i<MAXSHU; i++)
        dy[plants[i].type]=i;
    for (int i=0; i<MN; i++) {
        for (int j=0; j<MN; j++) {
            int z=m_game.GetZwShu (j * SI, i);
            if (z==-1)
                lx[j][i].type=-1;
            else
                lx[j][i]=m_game.plants[z];
        }
    }
    for (int i=0; i<m_game.plt_cnt; i++) {
        if (m_game.plants[i].x / SI<MN) {
            int x=m_game.plants[i].x / SI, y=m_game.plants[i].y;
            lx[x][y].nxt_tm=m_game.DestroyZw (i,0);
            i -= 1;
        }
    }
    for (int i=0; i<MN; i++) {
        Plant st[MMM];
        int tp=0;
        bool bk=false, ko=false;
        for (int j=MN - 1; j>=0; j--) {
            Plant z=lx[j][i];
            lx[j][i].type=-1;
            if (z.type==-1)
                ko=true;
            else if (ko)
                bk=true;
            while (z.type!=-1) {
                if (tp==0||z.type!=st[tp - 1].type||n_2048[z.type]<=0) {
                    st[tp++]=z;
                    z.type=-1;
                }
                else {
                    int t=n_2048[st[--tp].type];
                    if (n_2048[t]==-1) {
                        m_game.m_yg += 1500;
                        if (beans<5)
                            beans += 1;
                    }
                    z=plants[dy[t]];
                    bk=true;
                }
            }
        }
        if (tp<MN&&rand () % GL48==0&&(tp==0||bk)) {
            if (tp==0)
                st[tp++]=plants[1];
            else {
                Plant t=st[tp - 1];
                st[tp++]=(t.type==0 ? plants[6] : plants[1]);
            }
        }
        for (int j=0; j<tp; j++)
            lx[MN - 1 - j][i]=st[j];
    }
    for (int i=0; i<MN; i++) {
        for (int j=0; j<MN; j++) {
            Plant z=lx[j][i];
            if (z.type==-1)
                continue;
            m_game.m_yg += z.cost;
            z.x=j * SI;
            z.y=i;
            m_game.PlantZw (z);
        }
    }
    m_game.nd_calfr=true;
}
void MainWindow::down2048 () { //同上
    if (TM - l48<30)
        return;
    l48=TM;
    Plant lx[MMM][MMN];
    int dy[MAXSHU];
    for (int i=0; i<MAXSHU; i++)
        dy[plants[i].type]=i;
    for (int i=0; i<MN; i++) {
        for (int j=0; j<MN; j++) {
            int z=m_game.GetZwShu (j * SI, i);
            if (z==-1)
                lx[j][i].type=-1;
            else
                lx[j][i]=m_game.plants[z];
        }
    }
    for (int i=0; i<m_game.plt_cnt; i++) {
        if (m_game.plants[i].x / SI<MN) {
            int x=m_game.plants[i].x / SI, y=m_game.plants[i].y;
            lx[x][y].nxt_tm=m_game.DestroyZw (i,0);
            i -= 1;
        }
    }
    for (int j=0; j<MN; j++) {
        Plant st[MMN];
        int tp=0;
        bool ko=false, bk=false;
        for (int i=MN - 1; i>=0; i--) {
            Plant z=lx[j][i];
            lx[j][i].type=-1;
            if (z.type==-1)
                ko=true;
            else if (ko)
                bk=true;
            while (z.type!=-1) {
                if (tp==0||z.type!=st[tp - 1].type||n_2048[z.type]<=0) {
                    st[tp++]=z;
                    z.type=-1;
                }
                else {
                    int t=n_2048[st[--tp].type];
                    if (n_2048[t]==-1) {
                        m_game.m_yg += 1500;
                        if (beans<5)
                            beans += 1;
                    }
                    z=plants[dy[t]];
                }
            }
        }
        if (tp<MN&&rand () % GL48==0&&(tp==0||bk)) {
            if (tp==0)
                st[tp++]=plants[1];
            else {
                Plant t=st[tp - 1];
                st[tp++]=(t.type==0 ? plants[6] : plants[1]);
            }
        }
        for (int i=0; i<tp; i++)
            lx[j][MN - 1 - i]=st[i];
    }
    for (int i=0; i<MN; i++) {
        for (int j=0; j<MN; j++) {
            Plant z=lx[j][i];
            if (z.type==-1)
                continue;
            m_game.m_yg += z.cost;
            z.x=j * SI;
            z.y=i;
            m_game.PlantZw (z);
        }
    }
    m_game.nd_calfr=true;
}
void MainWindow::WakeUp (int y) { //唤醒
    if(isstop)return;
    for (int i=0; i<tmr_cnt; i++) {
        int d=tmr[i].id;
        if(d>0)continue;
        d=m_game.plt_ii[-d];
        if (m_game.plants[d].y==y&&m_game.plants[d].dzsy==0) {
            if(m_game.plants[d].attack>0)
                tmr[i].nxt =v_tm+RA () % (tmr[i].time / 3+1);//防止同时开始造成卡顿
        }
    }
}

void MainWindow::Paint(QPixmap _pil,QPixmap _anpil)
{
    pil=_pil;
    anpil=_anpil;
}

void MainWindow::Repaint()
{
#ifdef NO_UI
    return;
#endif
    bool fu=(isMaximized()||isFullScreen());
    if(m_game.started&&tim.elapsed()<1000)
    {
        if(!fu&&!b_fast)
            resize(QSize(W0+W1+W2,TOP+ME+HI));
    }
    else if(!fu&&!b_fast)
        resize(QSize(zoom*(W0+W1)+W2+0.5,ME+zoom*(HI+TOP)+0.5));
    if(m_game.started&&!isstop)return;
    gl.update();
    update();
}

void MainWindow::OnConnect()
{
    auto tc=serv.nextPendingConnection();
    if(!tc->isReadable())exit(0);
    connect(tc,SIGNAL(readyRead()),&tcpmap,SLOT(map()));
    tcpmap.setMapping(tc,int(tcp.size()));
    QTimer *tm=new QTimer(this);
    tm->setSingleShot(1);tm->setTimerType(Qt::PreciseTimer);
    connect(tm,SIGNAL(timeout()),&rdmap,SLOT(map()));
    rdmap.setMapping(tm,int(tcp.size()));
    tcp.push_back(tc);rdtmr.push_back(tm);
    QMessageBox::information(this,"连接",QString::asprintf("目前已连接至%d人",int(tcp.size())));
}

void MainWindow::AddHash(QByteArray&a)
{
    QByteArray b;ull le=Hash(a);
    b.resize(10);
    for(int s=0;s<10;s++)
    {
        b[9-s]=le%100+1;
        le/=100;
    }
    a.push_front(b);
}

ull MainWindow::Hash(QByteArray&a,ull ck)
{
    const ull se=1313131;ull jg=0;
    int le=a.length();bool fd=0;
    for(int i=0;i<le;i++)
    {
        jg=jg*se+a[i]+1;
        if(jg*se+le==ck)fd=1;
    }
    if(ck!=(ull)(-1))return fd;
    jg=jg*se+le;
    return jg;
}

bool MainWindow::CheckHash(QByteArray&a)
{
    int sa=a.length();ull ha=0;
    if(sa<10)return 0;
    for(int i=0;i<10;i++)
    {
        int t=a[i];
        if(t<1||t>100)return 0;
        ha=ha*100+(t-1);
    }
    a.remove(0,10);
    if(!Hash(a,ha))return 0;
    return 1;
}

void MainWindow::DoReadData(int fr)
{
    QByteArray tt=tcp[fr]->readAll();
    if(!m_game.started)return;
    tt=qUncompress(tt);
    if(!CheckHash(tt))
    {
        qDebug()<<"dirty "<<TM;
        return;
    }
    QDataStream da(tt);
    int ty;da>>ty;
    if(ty==0)//游戏数据
    {
        if(lstcz){lstcz-=1;return;}
        int a=sele_x,b=sele_y,c=select,d=m_game.m_yg;
        Load(da);int nsdcnt;da>>nsdcnt;
        sele_x=a;sele_y=b;select=c;m_game.m_yg=d;
        for (int i=0; i<MN; i++)
            m_game.row_zmb[i]=0;
        for (int i=0; i<m_game.zmb_cnt; i++) {
            m_game.row_zmb[m_game.zombies[i].y] += 1;
            m_game.zombies[i].num=i;
        }
        for (int i=0; i<m_game.plt_cnt; i++)
            m_game.plants[i].num=i;
        m_game.nd_calfr=true;
        int tt=tim.elapsed();
        for(int i=0;i<tmr_cnt;i++)
            tmr[i].nxt+=tt;
        //for(int i=0;i<sdcnt-nsdcnt;i++)//防止回弹
        //    m_game.MoveAllZd();
        sdcnt=nsdcnt;
    }
    else if(ty==1)lstcz=0;
    else if(ty==2)
    {
        ReadDelta(da);
        if(sender&&!duiz)SendInt(1);
    }
    else if(ty==3)//游戏开始
    {
        da>>m_game.m_yg;
        cd[0]=cd[12]=cd[17]=800;
        sdcnt=0;
        for(int i=1;i<MAXSHU;i++)
            if(i!=12&&i!=17)cd[i]=0;
    }
    else if(ty==4)
    {
        da>>TM;
        an_game.input(da);
        an_game.nd_calfr=true;
    }
    else if(ty==5)
    {
        da>>beans;
        SwitchStop();
    }
    update();
}

void MainWindow::OnReadData(int fr)
{
    rdtmr[fr]->start(3);
}

void MainWindow::ReadDelta(QDataStream&da)
{
    int ty;da>>ty;
    if(ty==0)//WSAD
    {
        int sx,sy,tx,ty,a=sele_x,b=sele_y;
        da>>sx>>sy>>tx>>ty;DoWSAD(sx,sy,tx,ty);
        sele_x=a;sele_y=b;
    }
    else if(ty==1)//PlantZw
    {
        Plant zw;zw.input(da);
        m_game.m_yg+=zw.cost;
        m_game.PlantZw(zw);
    }
    else if(ty==2)//Dig
    {
        int x,y;da>>x>>y;
        int w=m_game.GetZwShu(x,y);
        if(w==-1)
        {
            m_game.m_ng[x / SI][y]=0;
            m_game.nd_calfr=true;
        }
        else
        {
            if(m_game.plants[w].type==24)
                m_game.m_ng[x/SI][y]=0;
            m_game.DestroyZw (w,1);
        }
    }
    else if(ty==3)//Dz
    {
        int w;da>>w;
        bool rt=m_game.DaZhao(w);
        if(rt)beans-=1;
    }
    else if(ty==4)
    {
        Zombie js;js.input(da);
        m_game.NewJs(js);
    }
    else if(ty==5)
    {
        int a,b;da>>a>>b;
        exp+=a;m_game.m_yg+=b;
    }
    else if(ty==6)
    {
        int z;da>>z;
        Plant&zw=m_game.plants[z];
        if(zw.tag!=-1&&zw.grow>0)
        {
            zw.grow-=1;
            if(zw.grow==0)zw.tag=-1;
        }
    }
}
