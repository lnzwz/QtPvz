#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);tim.start();
    connect(&timer,&QTimer::timeout,this,&MainWindow::OnTimer);
    timer.setTimerType(Qt::PreciseTimer);
    timer.setSingleShot(true);
    DOU=false;m_game.m_wnd=this;Init();
    gl=new MyGLWidget(this);
    gl->m_wnd=this;gl->ty=1;
    gl->resize(1040,610);
    gl->move(100,ME);
    gl->lower();gl->show();
    gx=new MyGLWidget(this);
    gx->m_wnd=this;gx->ty=0;
    gx->resize(100,610);
    gx->move(0,ME);gx->show();
    srand(time(NULL));
    /*connect(ui->a_open,&QAction::triggered,this,&MainWindow::on_open_clicked);
    connect(ui->a_save,&QAction::triggered,this,&MainWindow::on_save_clicked);
    connect(ui->a_buy,&QAction::triggered,this,&MainWindow::on_buy_clicked);
    connect(ui->a_quick,&QAction::triggered,this,&MainWindow::on_quick_clicked);
    connect(ui->a_start,&QAction::triggered,this,&MainWindow::on_btc_clicked);
    connect(ui->a_add,&QAction::triggered,this,&MainWindow::jiasu);
    connect(ui->a_sub,&QAction::triggered,this,&MainWindow::jiansu);
    connect(ui->a_sta,&QAction::triggered,this,&MainWindow::ViewStatus);
    connect(ui->a_quit,&QAction::triggered,this,&MainWindow::Quit);
    connect(ui->a_over,&QAction::triggered,this,&MainWindow::GameOver);*/
    QButtonGroup *gp = new QButtonGroup(this);
    gp->addButton(ui->easy);
    gp->addButton(ui->normal);
    gp->addButton(ui->hard);
    gp->addButton(ui->tui);
    gp->addButton(ui->water);
    gp->addButton(ui->xin);
    ui->buy->setVisible(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(!m_game.m_ks)return;
    int z=event->key();bool b=false;
    if(z==' ')
    {
        on_stop_clicked();
        return;
    }
    if(b_zt)return;
    if(z>=Qt::Key_1&&z<=Qt::Key_9)
        m_nzw=z-Qt::Key_1,b=1;
    else if(z==Qt::Key_0)
        m_nzw=9,b=1;
    else if(z==Qt::Key_Minus)
        m_nzw=10,b=1;
    else if(z=='`')
        m_nzw=-2;
    else if(m_xx!=-1)
    {
        int tx=m_xx,ty=m_xy;
        if(z==Qt::Key_W||z==Qt::Key_Up)ty-=1;
        else if(z==Qt::Key_S||z==Qt::Key_Down)ty+=1;
        else if(z==Qt::Key_A||z==Qt::Key_Left)tx-=SI;
        else if(z==Qt::Key_D||z==Qt::Key_Right)tx+=SI;
        if(tx>=0&&tx<=(MM-1)*SI&&ty>=0&&ty<MN&&m_game.GetZwShu(tx,ty)==-1&&TM-m_game.m_ice[tx/SI][ty]>=(m_hd?800:400))
        {
            int z=m_game.GetZwShu(m_xx,m_xy);
            if(z!=-1)
            {
                m_game.m_zhw[z].x=tx,m_game.m_zhw[z].y=ty;
                m_game.CalZw();
            }
            m_xx=tx;m_xy=ty;
        }
    }
    else if(m_b48)
    {
        if(z==Qt::Key_W||z==Qt::Key_Up)up2048();
        else if(z==Qt::Key_S||z==Qt::Key_Down)down2048();
        else if(z==Qt::Key_A||z==Qt::Key_Left)left2048();
        else if(z==Qt::Key_D||z==Qt::Key_Right)right2048();
    }
    if(b)
    {
        if(m_nzw>=XUAN||m_nzw>=SHU-(m_b11))
            m_nzw=-1;
        else
        {
            if(m_game.m_yg>=m_zw[m_xuan[m_nzw]].yg)
                m_nzw=m_xuan[m_nzw];
            else
                m_nzw=-1;
        }
    }
}

void MainWindow::SolveDz(QPoint pt)
{
    if(m_dzs>0)
    {
        int x=int((pt.x()-150)/SI)*SI;
        int y=int((pt.y()-60)/SI);
        if(x>=0&&x<=(MM-1)*SI&&y>=0&&y<MN)
        {
            int rt=m_game.GetZwShu(x,y);
            if(rt!=-1)
            {
                bool rtn=DaZhao(m_game.m_zhw[rt],this);
                if(rtn)
                    m_dzs-=1;
            }
        }
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(!m_game.m_ks)return;
    if(b_zt)return;
    if(event->button()==Qt::LeftButton)
    {
        bool t=false;
        QPoint pt=SPoint(event->pos().x(),event->pos().y());
        if(m_xx!=-1)
        {
            int x=((pt.x()-150)/SI)*SI;
            int y=((pt.y()-60)/SI);
            if(x>=0&&x<=(MM-1)*SI&&y>=0&&y<MN&&m_game.GetZwShu(x,y)==-1&&TM-m_game.m_ice[x/SI][y]>=(m_hd?800:400))
            {
                int z=m_game.GetZwShu(m_xx,m_xy);
                if(z!=-1)
                {
                    m_game.m_zhw[z].x=x,m_game.m_zhw[z].y=y;
                    m_game.CalZw();
                }
                m_xx=x;m_xy=y;
            }
        }
        else if(pt.x()>=150&&pt.y()>=60)
        {
            t=(m_nzw!=-1);
            if(m_nzw==-2)
            {
                int x=int((pt.x()-150)/SI)*SI;
                int y=int((pt.y()-60)/SI);
                m_game.m_ng[x/SI][y]=m_game.m_dc[x/SI][y]=0;
                m_game.DestroyZw(x,y);
                m_nzw=-1;
            }
            else if(m_nzw==-4)
            {
                SolveDz(pt);
                m_nzw=-1;
            }
            else Plant(pt);
        }
        if(m_b48&&!t)
        {
            int x=event->pos().x(),y=event->pos().y();
            x-=l_x;y-=l_y;
            if(x*x+y*y>=5000)
            {
                double jj=atan2(y,x),p4=acos(-1.0)/4.0,eps=1e-9;
                if(jj>-p4-eps&&jj<p4+eps)right2048();
                else if(jj>p4-eps&&jj<3*p4+eps)down2048();
                else if(jj>-3*p4-eps&&jj<-p4+eps)up2048();
                else left2048();
            }
        }
    }
    else if(event->button()==Qt::RightButton)
    {
        QPoint pt=SPoint(event->pos().x(),event->pos().y());
        SolveDz(pt);
    }
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(!m_game.m_ks)return;
    if(b_zt)return;
    if(event->button()==Qt::MiddleButton)//挖
    {
        QPoint pt=SPoint(event->pos().x(),event->pos().y());
        int x=((pt.x()-150)/SI)*SI;
        int y=((pt.y()-60)/SI);
        if(x>=0&&x<=(MM-1)*SI&&y>=0&&y<MN&&XUAN)
        {
            m_game.m_ng[x/SI][y]=0;
            m_game.DestroyZw(x,y);
        }
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(!m_game.m_ks)return;
    if(b_zt)return;
    if(event->button()==Qt::LeftButton)
    {
        bool t=(m_nzw==-1);
        QPoint pt=SPoint(event->pos().x(),event->pos().y());
        if(m_xx!=-1)
        {
            int x=((pt.x()-150)/SI)*SI;
            int y=((pt.y()-60)/SI);
            if(x>=0&&x<=(MM-1)*SI&&y>=0&&y<MN&&m_game.GetZwShu(x,y)==-1&&TM-m_game.m_ice[x/SI][y]>=(m_hd?800:400))
            {
                int z=m_game.GetZwShu(m_xx,m_xy);
                if(z!=-1)
                {
                    m_game.m_zhw[z].x=x,m_game.m_zhw[z].y=y;
                    m_game.CalZw();
                }
                m_xx=x;m_xy=y;
            }
        }
        else if(m_nzw!=-1)
        {
            if(m_nzw==-2)
            {
                int x=int((pt.x()-150)/SI)*SI;
                int y=int((pt.y()-60)/SI);
                m_game.m_ng[x/SI][y]=m_game.m_dc[x/SI][y]=0;
                m_game.DestroyZw(x,y);
                m_nzw=-1;
            }
            else if(m_nzw==-4)
            {
                SolveDz(pt);
                m_nzw=-1;
            }
            else if(!Plant(pt))
                t=true;
        }
        if(t)
        {
            int y=int(pt.y()/50.0);
            if(pt.y()>550&&pt.y()<=610&&pt.x()>=50&&pt.x()<=100&&m_dzs)
                m_nzw=-4;
            else if(pt.y()>550&&pt.y()<=610&&pt.x()<50)
                m_nzw=-2;
            else if(pt.x()<=100&&y<XUAN&&y<SHU-(m_b11)&&m_game.m_yg>=m_zw[m_xuan[y]].yg)
                m_nzw=m_xuan[y];
            int x=int((pt.x()-150)/SI);
            y=int((pt.y()-60)/SI);
            if(m_gq==MAXGQS+3&&y<MN&&x>=0&&x<MM&&m_game.GetZwShu(x*SI,y)!=-1)
            {
                m_xx=x*SI;
                m_xy=y;
            }
        }
        l_x=event->pos().x();l_y=event->pos().y();
    }
    else if(event->button()==Qt::MiddleButton)m_nzw=-1;
}

void MainWindow::DoDraw()
{
    QPainter memdc(this);
    memdc.drawPixmap(QRect(QPoint(floor(1140*m_sf)-1,ME),MSize(330,610)),ico[125]);
    if(!m_b11)
    {
        QPen p(QColor(255,0,0));p.setStyle(Qt::SolidLine);
        memdc.setPen(p);
        int x=ui->prog->pos().x(),y=ui->prog->pos().y()+ME,z=ui->prog->size().width(),s=0;
        for(int i=0;i<ND-1;i++)
            s+=m_jg[i]/m_time[i];
        if(s!=0)
        {
            for(int i=1,h=0;i<ND;i++)
            {
                if(m_time[i]<3000*SI/110&&m_time[i]>1&&m_time[i-1]>=2000*SI/110&&m_time[i]!=100*SI/110)
                    memdc.drawLine(x+z*h/s,y,x+z*h/s,y-3);
                h+=m_jg[i]/m_time[i];
            }
        }
    }
    memdc.end();
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QSize s=size();
    double a=s.width()/1234.0;
    double b=s.height()/610.0;
    double od=m_sf;
    m_sf=(a<b?a:b);
    if(fabs(od-m_sf)>1e-10)
        Initicon();
    //QFont f("",floor(5*m_sf),false);setFont(f);
    gl->resize(floor(1040*m_sf+1e-10),floor(610*m_sf+1e-10));
    gx->resize(floor(100*m_sf+1e-10),floor(610*m_sf+1e-10));
    gl->move(floor(100*m_sf+1e-10),ME);
    DoDraw();
}

void MainWindow::Init()
{
    //return;
    //创建植物：子弹颜色，攻击力，攻击时间，移动速度(子弹)
    MN=5;MM=9;SI=110;QColor nul;
    if(DOU)MN*=2,MM*=2,SI/=2;
    m_zw[0].Create(nul,-1,9000*pow(SI/110.0,0.25),1500,0,60,0,0,2,50,1);//向日葵
    m_zw[1].Create(QColor(0,255,0),1,2400,2000,4,60,0,0,1,100,0,18*SI/110);//豌豆射手
    m_zw[2].Create(nul,0,100000000,1000000000,0,600,0,0,4,50,2);//坚果
    m_zw[3].Create(nul,-2,1,0,0,50,0,0,7,150,3);//炸弹
    m_zw[4].Create(QColor(255,0,0),2,2300,2000,4,60,0,0,8,200,4,18*SI/110);//火球射手
    m_zw[5].Create(nul,-3,1,0,0,50,0,0,9,125,5);//火爆辣椒
    m_zw[6].Create(QColor(0,255,255),1,2400,2000,4,60,0,0,48,175,9,18*SI/110,true);//寒冰射手
    m_zw[7].Create(nul,0,100000000,1000000000,0,1600,0,0,11,125,6);//高坚果
    m_zw[8].Create(nul,-4,200,0,0,100000000,0,0,12,50,7);//倭瓜
    m_zw[9].Create(QColor(0,200,0),4,2400,2500,5,60,0,0,43,375,8,30*SI/110);//西瓜
    m_zw[10].Create(nul,-8,1000,1000000000,0,600,0,0,82,75,19);//恢复坚果
    m_zw[11].Create(QColor(10,170,255),5,2500,1800,4,60,0,0,55,500,10,30*SI/110,true);//冰瓜
    m_zw[12].Create(nul,-9,5000*pow(SI/110.0,0.25),1500,0,60,0,0,94,125,20);//双子
    m_zw[13].Create(nul,-10,1000,1500,0,80,0,0,95,125,21);//医疗
    m_zw[14].Create(nul,90,500,0,40,60,0,0,68,150,16,0,false,false,true);//大嘴花

    m_zw[15].Create(nul,0,1000000000,0,0,60,0,0,56,25,11);//灯
    m_zw[16].Create(nul,90,500,0,0,60,0,0,71,25,17,40,true,false,true);//土豆地雷

    m_zw[17].Create(nul,-5,8900,0,0,60,0,0,57,25,12,0,false,true);//阳光菇
    m_zw[18].Create(QColor(255,0,255),1,2400,1600,4,60,0,0,58,0,13,5,false,true);//小喷菇
    m_zw[19].Create(nul,-6,500,0,0,1,0,0,63,125,15,10,false,true);//毁灭菇
    m_zw[20].Create(nul,-7,500,0,0,1,0,0,72,125,18,0,false,true);//寒冰菇

    m_zw[21].Create(nul,0,1000000000,0,0,0,0,0,62,25,14);//荷叶
    m_zw[22].Create(QColor(255,255,0),1,1500,3000,4,60,0,0,98,250,22,10*SI/110);//星星
    m_zw[23].Create(nul,-11,1100,2000,0,60,0,0,101,200,23);//忧郁蘑菇
    m_zw[24].Create(nul,0,0,0,0,0,0,0,102,125,24);//南瓜

    m_zw[25].Create(nul,0,1000000000,0,0,50,0,0,104,250,25);//反弹
    m_zw[26].Create(nul,-12,2500/3,5000,0,60,0,0,106,450,26);//双头大喷菇
    m_zw[27].Create(nul,-13,80000,0,0,1,0,0,110,2,27);//大葱
    m_zw[28].Create(nul,0,0,0,0,0,0,0,130,50,28);//咖啡豆
    m_zw[29].Create(nul,0,0,0,0,0,0,0,132,200,29);//地刺
    m_zw[30].Create(nul,-14,5000,0,0,60,0,0,134,400,30);
    m_zw[31].Create(nul,-15,1000000000,0,0,60,0,0,135,400,31);
    m_zw[32].Create(nul,0,100000000,0,0,3,0,0,141,175,32,0,0,true);
    m_zw[33].Create(nul,0,100000000,0,0,60,0,0,142,200,33);


    int zws[MAXGQS+4]={3,2,2,2,1,2,1,1,1,0,2};
    zws[20]=4;zws[40]=1;zws[41]=1;zws[42]=1;zws[43]=1;zws[58]=3;zws[59]=6;
    for(int n=1;n<MAXGQS+4;n++)
        zws[n]+=zws[n-1];
    for(int n=0;n<=MAXGQS+3;n++)
        m_zws[n]=zws[n];

    for(int i=0;i<MAXSHU;i++)wsad[i]=0;
    wsad[4]=1;wsad[6]=2;wsad[7]=2;wsad[9]=2;wsad[10]=3;wsad[11]=2;wsad[13]=2;wsad[14]=2;wsad[22]=2;wsad[23]=2;wsad[26]=1;wsad[27]=1;
    //创建僵尸：速度，生命，吃的速度，0，0，图片ID，编号，是否能跳跃，是否发射
    m_js[0].Create(1,6,1,0,0,3,0);//普通僵尸
    m_js[1].Create(1,12,1,0,0,5,1);//塑料僵尸
    m_js[2].Create(2,15,1,0,0,29,5,true);//撑杆跳僵尸
    m_js[3].Create(1,30,1,0,0,6,2);//铁桶僵尸
    m_js[4].Create(1,20,4,0,0,32,6);//大嘴僵尸
    m_js[5].Create(3,16,1,0,0,10,3);//跑步僵尸
    m_js[6].Create(1,15,1,0,0,24,4);//报纸僵尸
    m_js[7].Create(1,75,2,0,0,36,7);//盾牌僵尸
    m_js[8].Create(1,18,1,0,0,73,13,false,true);//豌豆射手僵尸
    m_js[9].Create(2,20,1,0,0,44,8,true);//跳跳僵尸
    m_js[10].Create(1,20,1,0,0,79,14,false,true);//冰瓜僵尸
    m_js[11].Create(2,90,40,0,0,65,12);//小车僵尸
    m_js[12].Create(1,200,10000,0,0,49,9);//巨人僵尸
    m_js[13].Create(1,20,0,0,0,92,15);//苦力怕
    m_js[14].Create(2,25,2,0,0,96,16,false,true);//末影人
    m_js[15].Create(1,35,1,0,0,107,17);//zhannan
    //冷却速度
    m_hf[0]=12;
    m_hf[1]=15;
    m_hf[2]=6;
    m_hf[3]=0.9;
    m_hf[4]=12;
    m_hf[5]=1.1;
    m_hf[6]=12;
    m_hf[7]=5;
    m_hf[8]=4;
    m_hf[9]=10;
    m_hf[10]=7.5;
    m_hf[11]=3.8;
    m_hf[12]=8;
    m_hf[13]=3;
    m_hf[14]=2.5;
    m_hf[15]=12;
    m_hf[16]=5;
    m_hf[17]=15;
    m_hf[18]=12;
    m_hf[19]=2;
    m_hf[20]=1.8;
    m_hf[21]=12;
    m_hf[22]=7;
    m_hf[23]=7;
    m_hf[24]=6;
    m_hf[25]=6;
    m_hf[26]=7.5;
    m_hf[27]=5;
    m_hf[28]=8;
    m_hf[29]=4;
    m_hf[30]=0.5;
    m_hf[31]=0.5;
    m_hf[32]=2;
    m_hf[33]=2.5;
    //相对概率
    m_p[0]=3;
    m_p[1]=4;
    m_p[2]=4;
    m_p[3]=5;
    m_p[4]=6;
    m_p[5]=6;
    m_p[6]=5;
    m_p[7]=8;
    m_p[8]=8;
    m_p[9]=9;
    m_p[10]=6;
    m_p[11]=4;
    m_p[12]=3;
    m_p[13]=8;
    m_p[14]=6;
    m_p[15]=7;
    if(SI<=55)
    {
        for(int i=0;i<MAXSHU;i++)
            m_hf[i]*=1.1,wsad[i]*=2;
    }
    m_game.m_wnd=this;
    for(int n=0;n<MXUAN;n++)
        m_xuan[n]=-1;
    l48=TM=SHU=JSH=ND=0;m_b48=b_zt=false;
    m_nzw=-1;m_dzs=m_tms=0;m_xx=m_xy=-1;
    timer.stop();
    Initicon();
    for(int i=0;i<MN;i++)
        m_lstJ[i]=m_lstC[i]=m_lstD[i]=m_lstT[i]=m_lstB[i]=0;
    int ne[7]={1,6,4,26,22,9,11};
    for(int i=0;i<6;i++)n_2048[m_zw[ne[i]].lx]=m_zw[ne[i+1]].lx;
    n_2048[m_zw[ne[6]].lx]=-1;
}

void MainWindow::Initicon()
{
    for(int i=1;i<=142;i++)
    {
        ico[i].load(QString::asprintf("://icons/%d.png",i));
        if(i>=116&&i<=124)continue;
        else if(i==139||i==140)
            ico[i]=ico[i].scaled(floor(ico[i].width()*SI/250.0*m_sf),floor(ico[i].height()*SI/250.0*m_sf),Qt::KeepAspectRatio);
        else if(i==115||i==111||i==113||i==125)
            ico[i]=ico[i].scaled(floor(ico[i].width()/2.0*m_sf),floor(ico[i].height()/2.0*m_sf),Qt::KeepAspectRatio);
        else
        {
            if(i==49||i==51||i==52)
                ico[i]=ico[i].scaled(floor(ico[i].width()*1.3/3.0*m_sf/(DOU?2:1)),floor(ico[i].height()*1.3/3.0*m_sf/(DOU?2:1)),Qt::KeepAspectRatio);
            else
                ico[i]=ico[i].scaled(floor(ico[i].width()/2.0*m_sf/(DOU?2:1)),floor(ico[i].height()/2.0*m_sf/(DOU?2:1)),Qt::KeepAspectRatio);
            QImage ic=ico[i].toImage();
            for(int x=0;x<ic.width();x++)
            {
                for(int y=0;y<ic.height();y++)
                {
                    QColor rgb=ic.pixelColor(x,y);
                    int r,g,b,a;
                    rgb.getRgb(&r,&g,&b,&a);
                    if(a!=255)continue;
                    r>>=1;g>>=1;b<<=1;if(b>255)b=255;
                    ic.setPixel(x,y,qRgba(r,g,b,255));
                }
            }
            icd[i]=QPixmap::fromImage(ic);
            if(i==28)
            {
                icm[i]=ico[i];
                continue;
            }
            ic=ico[i].toImage();
            /*QImage tt=
            for(int x=0;x<ic.width();x++)
            {
                for(int y=0;y<ic.height();y++)
                {
                    QColor rgb=ic.pixelColor(x,y);
                    int r,g,b,a;rgb.getRgb(&r,&g,&b,&a);
                    if(a!=255)a=0;
                    tt.setPixel(ic.width()-1-x,y,qRgba(r,g,b,a));
                }
            }
            icm[i]=QPixmap::fromImage(tt);*/
            icm[i]=QPixmap::fromImage(ic.mirrored(true,false));
        }
    }
}

bool MainWindow::Plant(int x,int y)
{
    bool rt=false;
    if(m_nzw==-3)
    {
        if(m_game.GetZwShu(x,y)==-1)
        {
            m_game.m_yg+=m_zw[3].yg;
            m_zw[3].x=x;m_zw[3].y=y;
            m_game.NewZw(m_zw[3]);
            m_nzw=-1;return true;
        }
        return false;
    }
    int zz;
    if((zz=m_game.GetZwShu(x,y))!=-1&&m_game.m_zhw[zz].lx==30&&m_game.m_zhw[zz].nfs==10)
    {
        int tz=m_game.GetZwShu(x-SI,y);
        if(tz!=-1&&m_game.m_zhw[tz].lx==31)
        {
            m_nzw=-3;
            m_game.m_zhw[zz].nfs=0;
        }
    }
    if(x>=0&&x<=(MM-1)*SI&&y>=0&&y<MN&&m_nzw!=-1)
    {
        if(TM-m_game.m_ice[x/SI][y]>=(m_hd?800:400))
        {
            if(!m_b11||m_b12)
            {
                if(m_en[m_nzw]>=800)
                {
                    m_zw[m_nzw].x=x;
                    m_zw[m_nzw].y=y;
                    if(m_game.NewZw(m_zw[m_nzw]))
                        m_en[m_nzw]=0;
                }
                gl->repaint();gx->repaint();
            }
            else
            {
                m_zw[m_nzw].x=x;
                m_zw[m_nzw].y=y;
                if(m_game.NewZw(m_zw[m_nzw]))
                {
                    int n = 0;
                    for(n=0;n<SHU;n++)
                    {
                        if(m_xuan[n]==m_nzw)
                            break;
                    }
                    for(n+=1;n<SHU;n++)
                        m_xuan[n-1]=m_xuan[n];
                    SHU-=1;
                    m_xuan[SHU]=-1;
                    m_game.m_yg=9999;
                }
            }
        }
        rt=true;
    }
    if(rt&&m_nzw!=-3)m_nzw=-1;
    return rt;
}

bool MainWindow::Plant(QPoint pt)
{
    return Plant(((pt.x()-150)/SI)*SI,(pt.y()-60)/SI);
}
struct SPt
{
    int x,y;
    SPt(){}
    SPt(int X,int Y)
    {
        x=X;y=Y;
    }
};
typedef SPt SVe;
SPt operator+(const SPt&a,const SVe&b)
{
    return SPt(a.x+b.x,a.y+b.y);
}
SVe operator-(const SPt&a,const SPt&b)
{
    return SVe(a.x-b.x,a.y-b.y);
}
int operator*(const SVe&a,const SVe&b)
{
    return a.x*b.y-a.y*b.x;
}
SVe operator*(const SVe&a,double b)
{
    return SVe(int(a.x*b+0.5),int(a.y*b+0.5));
}
SPt jiaod(const SPt&a,const SVe&va,const SPt&b,const SVe&vb)
{
    double t=double((b-a)*vb)/(va*vb);
    return a+va*t;
}
bool inside(const SPt&a,const SPt&b,const SPt&c)
{
    if((c.x<a.x||c.x>b.x)&&(c.x<b.x||c.x>a.x))
        return false;
    if((c.y<a.y||c.y>b.y)&&(c.y<b.y||c.y>a.y))
        return false;
    return true;
}
bool check(const SPt&a,const SPt&b,const SPt&c,const SPt&d)
{
    if((b-a)*(d-c)==0)
        return false;
    SPt z=jiaod(a,b-a,c,d-c);
    return inside(a,b,z)&&inside(c,d,z);
}
bool jiaorect(int x,int y,const SPt&a,const SPt&b)
{
    SPt z0(x+SI/4,y+SI/6),z1(x+SI*3/4,y+SI/6),z2(x+SI*3/4,y+SI*5/6),z3(x+SI/4,y+SI*5/6);
    return check(z0,z1,a,b)||check(z1,z2,a,b)||check(z2,z3,a,b)||check(z3,z0,a,b);
}
bool MainWindow::DoTimer(int nIDEvent)
{
    if(b_zt)
    {
    	QMessageBox::critical(this,tr(""),tr("出大BUG了"));
    }
    bool rt=false;
    if(m_game.m_over)
    {
        GameOver();
        return true;
    }
    else if(nIDEvent<1000)
        rt=m_game.Goj(nIDEvent);
    else
    {
        if(nIDEvent==1000)//移动僵尸
        {
            for(int i=0;i<m_game.m_jshu;i++)
            {
                if(!m_game.m_hb||m_game.m_jsh[i].lx>=100||m_game.m_jsh[i].lx==12)
                {
                    if(m_game.MoveJs(i))
                        i-=1;
                }
            }
            for(int n=0;n<MAXSHU;n++)//冷却
            {
                m_en[n]+=m_hf[n];
                if(m_en[n]>800)
                    m_en[n]=800;
            }
            tm+=1;
        }
        else if(nIDEvent==1001)//移动子弹
        {
            m_game.MoveAllZd();
            if(tim.elapsed()-lad>15)
                gl->repaint(),gx->repaint();
        }
        else if(nIDEvent==1002)//产生僵尸
        {
            if(m_b11)//Boss关卡
            {
                int r2=rand()%m_p[m_nd],n=0;
                for(n=0;n<JSH;n++)
                {
                    if(r2<m_p[n])
                        break;
                }
                m_js[n].x=(MM-1)*SI+5;
                m_js[n].y=rand()%MN;
                if(m_jwbs[m_js[n].y]>0)
                    m_game.NewJs(m_js[n]);
                return rt;
            }
            if(m_n==ND-1)//结束
            {
                if(m_game.m_jshu==0)
                {
                    KillTimer(1002);rt=true;
                    m_game.m_over=true;
                    int yg=450*m_dzs+m_game.m_yg;
                    for(int i=0;i<m_game.m_zshu;i++)
                        yg+=m_game.m_zhw[i].yg;
                    int sc=jd+yg/50;
                    for(int y=0;y<MN;y++)
                    {
                        if(m_game.m_car[y])
                            sc+=30;
                    }
                    m_game.KillAll();GameOver();
                    QMessageBox::information(this,tr("提示"),QString::asprintf("胜利\r\n得分:%d",sc));
                }
                return rt;
            }
            int r=rand()%m_p[m_nd],n=0;
            for(n=0;n<JSH;n++)
            {
                if(r<m_p[n])
                    break;
            }
            m_js[n].x=(MM-1)*SI+5;
            if(n==10)
            {
                int maxs=-100,wz;
                if(m_hbzw)
                {
                    for(int j=0;j<MN;j++)
                    {
                        int shu=(tm-m_lstB[j])-(tm-m_lstC[j]);
                        if(shu>maxs)
                        {
                            maxs=shu;
                            wz=j;
                        }
                    }
                }
                if(maxs==-100)
                {
                    for(int j=0;j<MN;j++)
                    {
                        int shu=(tm-m_lstB[j])-(tm-m_lstJ[j]);
                        if(shu>maxs)
                        {
                            maxs=shu;
                            wz=j;
                        }
                    }
                    if(maxs==-100)
                    {
                        for(int j=0;j<MN;j++)
                        {
                            int shu=(tm-m_lstB[j])-(tm-m_lstD[j]);
                            if(shu>maxs)
                            {
                                maxs=shu;
                                wz=j;
                            }
                        }
                        if(maxs==-100)
                        {
                            for(int j=0;j<MN;j++)
                            {
                                int shu=(tm-m_lstB[j])-(tm-m_lstT[j]);
                                if(shu>maxs)
                                {
                                    maxs=shu;
                                    wz=j;
                                }
                            }
                            if(maxs==-100)
                                wz=rand()%MN;
                        }
                    }
                }
                m_js[n].y=wz;
                m_lstB[wz]=tm;
            }
            else if(n==8||n==4)
            {
                int maxs=200,wz;
                for(int j=0;j<MN;j++)
                {
                    int shu=tm-m_lstC[j];
                    if(shu<maxs)
                    {
                        maxs=shu;
                        wz=j;
                    }
                }
                if(maxs==200)
                {
                    for(int j=0;j<MN;j++)
                    {
                        int shu=tm-m_lstJ[j];
                        if(shu<maxs)
                        {
                            maxs=shu;
                            wz=j;
                        }
                    }
                    if(maxs==200)
                    {
                        for(int j=0;j<MN;j++)
                        {
                            int shu=tm-m_lstD[j];
                            if(shu<maxs)
                            {
                                maxs=shu;
                                wz=j;
                            }
                        }
                        if(maxs==200)
                        {
                            for(int j=0;j<MN;j++)
                            {
                                int shu=tm-m_lstT[j];
                                if(shu<maxs)
                                {
                                    maxs=shu;
                                    wz=j;
                                }
                            }
                            if(maxs==200)
                                wz=rand()%MN;
                        }
                    }
                }
                m_js[n].y=wz;
            }
            else if(n==12)
            {
                int max=-1,wz;
                for(int j=0;j<MN;j++)
                {
                    int shu=tm-m_lstJ[j];
                    if(shu>max)
                    {
                        max=shu;
                        wz=j;
                    }
                }
                m_js[n].y=wz;
                m_lstJ[wz]=tm;
            }
            else if(n==11)
            {
                int max=-1,wz;
                for(int j=0;j<MN;j++)
                {
                    int shu=tm-m_lstC[j];
                    if(shu>max)
                    {
                        max=shu;
                        wz=j;
                    }
                }
                m_js[n].y=wz;
                m_lstC[wz]=tm;
            }
            else if(n==7)
            {
                int max=-1,wz;
                for(int j=0;j<MN;j++)
                {
                    int shu=tm-m_lstD[j];
                    if(shu>max)
                    {
                        max=shu;
                        wz=j;
                    }
                }
                m_js[n].y=wz;
                m_lstD[wz]=tm;
            }
            else if(n==3)
            {
                int max=-1,wz;
                for(int j=0;j<MN;j++)
                {
                    int shu=tm-m_lstT[j];
                    if(shu>max)
                    {
                        max=shu;
                        wz=j;
                    }
                }
                m_js[n].y=wz;
                m_lstT[wz]=tm;
            }
            else
                m_js[n].y=rand()%MN;
            m_game.NewJs(m_js[n]);
            if(!m_b11)
            {
                jd+=1;
                ui->prog->setValue(jd);
            }
        }
        else if(nIDEvent==1003)//阳光
            m_game.m_yg+=25;
        else if(nIDEvent==1004)//改变速度
        {
            rt=true;
            if(m_b11)
            {
                KillTimer(1002);
                SetTimer(1002,800*pow(SI/110.0,0.9),NULL);
                KillTimer(1004);
                return rt;
            }
            KillTimer(1002);
            int lj=m_jg[m_n];m_n+=1;
            if(m_n>ND-1)
                m_n=ND-1;
            SetTimer(1002,m_time[m_n],NULL);//重置生成速度
            KillTimer(1004);
            SetTimer(1004,m_jg[m_n],NULL);//重置修改速度
            if(lj!=1)
            {
                if((m_gq%10==0||XUAN==0||m_game.b64)&&r_gq!=MAXGQS+5)
                    m_nd+=1;
                else
                    m_nd+=m_n%2;
                if(m_nd>JSH-1)
                    m_nd=JSH-1;
            }
            if(ND==90&&m_n>=40&&m_n%5==0)
            {
                for(int i=10;i<=12;i++)
                {
                    for(int j=i;j<MAXJSH;j++)
                        m_p[j]+=1;
                }
                for(int i=0;i<MAXJSH;i++)
                    m_js[i].shm+=10;
                if(m_n%10==0)
                {
                    for(int i=0;i<MAXJSH;i++)
                    {
                        if(i!=11&&i%2==(m_n/10)%2)m_js[i].sd+=1;
                        if(m_js[i].sd>12)m_js[i].sd=12;
                        if(m_js[i].eat!=40)
                        {
                            if(m_js[i].eat==1)
                                m_js[i].eat*=2;
                            else
                                m_js[i].eat*=1.5;
                        }
                    }
                    m_game.m_yg/=1.5;
                }
                if(m_n%15==0)
                {
                    for(int i=0;i<MAXSHU;i++)
                        m_hf[i]*=0.8;
                }
            }
            if(m_time[m_n]<3000*SI/110&&m_time[m_n]>1&&m_time[m_n-1]>=2000*SI/110&&m_time[m_n]!=100*SI/110)
            {
                if(!m_ai)
                {
                    on_stop_clicked();
                    QMessageBox::warning(this,tr("警告"),tr("一大波僵尸即将来袭！"));
                }
                if(!m_ai)on_stop_clicked();
                SJsh js;
                js.Create(3,m_gq<=6?10:20,1,(MM-1)*SI+5,MN/2,40,-1);
                m_game.NewJs(js);
            }
        }
        else if(nIDEvent==1005)//产生植物
        {
            if(!m_b12&&SHU<XUAN)
            {
                int r;
                while(1)
                {
                    r=rand()%(MAXSHU-1)+1;
                    if((m_zw[r].lx==3||m_zw[r].lx==5||m_zw[r].lx==16||m_zw[r].lx==27||m_zw[r].lx==30||m_zw[r].lx==31)&&rand()%3<2)continue;
                    if(!m_zw[r].mg&&m_zw[r].lx!=14&&m_zw[r].lx!=11&&m_zw[r].lx!=12&&m_zw[r].lx!=20&&m_zw[r].lx!=21&&m_zw[r].lx!=28)
                        break;
                }
                m_xuan[SHU]=r;
                SHU+=1;
            }
            for(int n=0;n<MN;n++)m_jwbs[n]=0;
            int jwz[MMN];
            for(int n=0;n<MN;n++)jwz[n]=-1;
            for(int i=0;i<m_game.m_jshu;i++)
            {
                if(m_game.m_jsh[i].lx==11)
                {
                    m_jwbs[m_game.m_jsh[i].y]=m_game.m_jsh[i].shm;
                    jwz[m_game.m_jsh[i].y]=i;
                }
            }
            jd=0;
            for(int n=0;n<MN;n++)
                jd+=(SI<=55&&m_b12?3000:1000)-m_jwbs[n];
            ui->prog->setValue(jd);
            if(jd==MN*(SI<=55?3000:1000))
            {
                KillTimer(1002);
                m_game.m_over=true;
                m_game.KillAll();GameOver();
                QMessageBox::information(this,tr("提示"),tr("胜利"));
            }
            for(int y=0;y<MN;y++)
            {
                if(jwz[y]==-1)
                    continue;
                int z=jwz[y];
                if(m_game.m_jsh[z].x>(MM-1)*SI+5)
                {
                    m_game.m_jsh[z].x=(MM-1)*SI+5;
                    m_game.m_jsh[z].sd=0;
                }
            }
        }
        else if(nIDEvent==1006)//取消寒冰
        {
            KillTimer(1006);rt=true;
            m_game.m_hb=false;
        }
        else if(nIDEvent==1007)//踩
        {
            int ma[MMN]={0},max=-1,may=-1;
            for(int n=0,i=0;n<m_game.m_zshu;i++)
            {
                if(m_game.m_zhw[i].shm==-1)
                    continue;
                n+=1;
                ma[m_game.m_zhw[i].y]+=m_game.m_zhw[i].yg;
            }
            for(int n=0;n<MN;n++)m_jwbs[n]=0;
            int jwz[MMN];
            for(int n=0;n<MN;n++)jwz[n]=-1;
            for(int i=0;i<m_game.m_jshu;i++)
            {
                if(m_game.m_jsh[i].lx==11)
                {
                    m_jwbs[m_game.m_jsh[i].y]=m_game.m_jsh[i].shm;
                    jwz[m_game.m_jsh[i].y]=i;
                }
            }
            for(int y=0;y<MN;y++)
            {
                if(m_jwbs[y]<=0)//||m_game.m_jsh[y].lx!=11)
                    continue;
                if(ma[y]>max)
                {
                    max=ma[y];
                    may=y;
                }
            }
            if(may!=-1)
            {
                m_game.m_jsh[jwz[may]].sd=10;
                m_game.KillJsLine(may,300);
            }
        }
        else if(nIDEvent==1008)
            m_game.AI();
        else if(nIDEvent==1009)
        {
            for(int i=0;i<m_game.m_zshu;i++)
                m_game.m_zhw[i].id=GetZwIcon(m_game.m_zhw[i]);
            TM+=1;
            if(TM%40==0)
            {
                for(int i=0;i<m_game.m_zshu;i++)
                {
                    if(m_game.m_zhw[i].lx==33)
                    {
                        int x1=m_game.m_zhw[i].x+SI/2,y1=m_game.m_zhw[i].y*SI+SI/2;
                        for(int j=i+1;j<m_game.m_zshu;j++)
                        {
                            if(m_game.m_zhw[j].lx==33)
                            {
                                int x2=m_game.m_zhw[j].x+SI/2,y2=m_game.m_zhw[j].y*SI+SI/2;
                                for(int s=0;s<m_game.m_jshu;s++)
                                {
                                    if(m_game.m_jsh[s].lx>=100||!jiaorect(m_game.m_jsh[s].x,m_game.m_jsh[s].y*SI,SPt(x1,y1),SPt(x2,y2)))
                                        continue;
                                    m_game.m_jsh[s].hb=100;m_game.m_jsh[s].shm-=1;
                                    if(m_game.m_jsh[s].shm<=0)
                                    {
                                        m_game.DestroyJs(s);
                                        s-=1;
                                    }
                                    else
                                        m_game.m_jsh[s].id=GetJsIcon(m_game.m_jsh[s],m_game);
                                }
                                /*if(y2>y1)
                                {
                                    for(int y=y1;y<=y2;y++)
                                    {
                                        int x=int(x1+double(x2-x1)/(y2-y1)*(y-y1)+0.5);
                                        m_game.KillJs(x-20,x+20,y,-1-(x1==x2));
                                    }
                                }
                                if(x2>x1)
                                {
                                    for(int x=x1;x<=x2;x+=SI)
                                    {
                                        int y=int(y1+double(y2-y1)/(x2-x1)*(x-x1)+0.5);
                                        m_game.KillJs(x-20,x+20,y,-1-(y1==y2));
                                    }
                                }*/
                                //else if(y2==y1&&x2>x1)
                                //    m_game.KillJs(x1,x2,y1,-1);
                            }
                        }
                    }
                }
            }
        }
        else if(nIDEvent==1010)
        {
            for(int i=0;i<XUAN;i++)
            {
                int t=m_zw[m_xuan[i]].lx;
                if(t!=3&&t!=5&&t!=7&&t!=16&&t!=30&&t!=31)
                    m_hf[m_xuan[i]]/=9000;
            }
            KillTimer(1010);rt=true;
        }
        else
        {
            KillTimer(nIDEvent);rt=true;
            QxDaZhao(m_game.m_zhw[nIDEvent-10000],this);
        }
    }
    //CDialog::OnTimer(nIDEvent);
    return rt;
}

void MainWindow::OnTimer()
{
    int nIDEvent=nid;
    if(nIDEvent<100000)
    {
        int z=-1;
        for(int i=0;i<m_tms;i++)
        {
            if(tmr[i].id==nIDEvent)
            {
                z=i;
                break;
            }
        }
        if(z!=-1)
        {
            tmr[z].ne+=tmr[z].jg;
            //tmr[z].ne=tim.elapsed()+tmr[z].jg;
            if(rand()%int(1e9)<floor(tmr[z].yu*1e9))
                tmr[z].ne+=1;
        }
        else if(nIDEvent!=704&&nIDEvent!=641)
        {
            QMessageBox::warning(this,tr(""),tr("Timer Bug"));
            //MessageBox(_T("Timer Bug"));
        }
        DoTimer(nIDEvent);
        if(!b_zt)ResetTimer();
    }
}

int MainWindow::MakeGq(int gq)//制作关卡
{
    m_hd=m_b48=false;m_ea=1;
    if(ui->xin->isChecked())
    {
        m_ea=4;
        for(int i=0;i<MAXSHU;i++)
        {
            m_zw[i].shm*=2;
            m_zw[i].time/=1.4;
            m_hf[i]*=1.8;
            m_zw[i].yg-=40;
            if(m_zw[i].yg<0)m_zw[i].yg=0;
        }
        for(int i=0;i<MAXJSH;i++)
            m_js[i].shm/=1.4;
        m_p[10]-=1;m_p[11]-=1;m_p[12]-=1;
    }
    else if(ui->water->isChecked())
    {
        m_ea=3;
        for(int i=0;i<MAXSHU;i++)
        {
            m_zw[i].shm*=1.6;
            m_zw[i].time/=1.3;
            m_hf[i]*=1.5;
            m_zw[i].yg-=25;
            if(m_zw[i].yg<0)m_zw[i].yg=0;
        }
        for(int i=0;i<MAXJSH;i++)
            m_js[i].shm/=1.25;
        m_p[11]-=1;m_p[12]-=1;
    }
    else if(ui->easy->isChecked())
    {
        m_ea=2;
        for(int i=0;i<MAXSHU;i++)
        {
            m_zw[i].shm*=1.3;
            m_zw[i].time/=1.1;
            m_hf[i]*=1.2;
            m_zw[i].yg-=10;
            if(m_zw[i].yg<0)m_zw[i].yg=0;
        }
        for(int i=0;i<MAXJSH;i++)
            m_js[i].shm/=1.1;
        m_p[11]-=1;
    }
    else if(ui->normal->isChecked())
        m_ea=1;
    else if(ui->hard->isChecked())
    {
        m_hd=true;
        for(int i=0;i<MAXSHU;i++)
        {
            m_zw[i].shm/=1.2;
            if(m_zw[i].shm<1)m_zw[i].shm=1;
            m_zw[i].time*=1.1;
            m_hf[i]*=0.9;
            if(i>0)m_zw[i].yg+=15;
        }
        for(int i=0;i<MAXJSH;i++)
        {
            m_js[i].shm*=1.2;
            m_js[i].eat+=1;
        }
        m_p[10]+=1;m_p[11]+=1;
    }
    else if(ui->tui->isChecked())
    {
        m_hd=true;m_tui=true;
        for(int i=0;i<MAXSHU;i++)
        {
            m_zw[i].shm/=1.3;
            if(m_zw[i].shm<1)m_zw[i].shm=1;
            m_zw[i].time*=1.2;
            m_hf[i]*=0.8;
            if(i>0)m_zw[i].yg+=30;
        }
        for(int i=0;i<MAXJSH;i++)
        {
            m_js[i].shm*=1.4;
            m_js[i].eat+=1;m_js[i].sd+=1;
        }
        m_p[10]+=1;m_p[11]+=1;m_p[12]+=1;
    }
    int lx=(gq==2048?0:(gq-1)/GQS);
    if(gq==MAXGQS+3||gq==MAXGQS+5)
    {
        m_p[13]=1;m_p[14]=0;
        m_p[10]-=1;m_p[11]-=2;m_p[12]-=1;m_p[15]=3-m_ea;
    }
    m_game.b64=(gq==MAXGQS+4);
    if(lx==6)lx=0;
    if(lx==5)
    {
        for(int s=10;s<=12;s++)
            m_p[s]+=2;
        for(int s=0;s<=21;s++)
        {
            if(s!=3&&s!=5&&s!=8&&s!=13)
                m_hf[s]*=0.8;
        }
        m_hf[3]*=0.704+0.00;
        m_hf[5]*=0.704+0.00;
        m_hf[8]*=0.704+0.00;
        m_hf[13]*=0.704+0.00;
        m_zw[0].time+=300;
        m_zw[12].time+=150;
        for(int s=1;s<MAXSHU;s++)m_zw[s].yg+=5;
        for(int s=1;s<=14;s++)
            m_js[s].shm+=5;
    }
    for(int n=1;n<MAXJSH;n++)
    {
        if(m_p[n]<0)m_p[n]=0;
        m_p[n]+=m_p[n-1];
    }
    int gq2=gq;
    gq=(gq-1)%GQS+1;
    if(gq2==MAXGQS+3||gq2==MAXGQS+4||gq2==2048)gq=10;
    if(gq2==MAXGQS+5)gq=9;
    m_b11=m_b12=false;
    double mr=(lx==0?1.0:3.0);
    double jad=(MAXJSH-mr)/(MANG-1.0);
    JSH=int(mr)+int(jad*(gq-1)+0.5);
    SHU=m_zws[gq2-1];
    if(JSH>MAXJSH)
        JSH=MAXJSH;
    if(SHU>MAXSHU)
        SHU=MAXSHU;
    ND=5+int(DBS*(gq-1+(lx==5?1:0)+(SI<=55?3:0))*4+0.5)+4;
    m_jg[1]=m_jg[2]=m_jg[3]=1;m_time[1]=m_time[2]=m_time[3]=1e9;
    if(gq<=8)
        m_time[0]=20000,m_jg[0]=41000;
    else
    {
        m_time[0]=16000,m_jg[0]=17000;
        if(gq>=9)m_time[1]=100,m_jg[1]=180;
        if(gq>=10)m_time[2]=100,m_jg[2]=180;
        m_time[3] = 8000; m_jg[3] = 21000;
    }
    if(gq<=8)m_time[4] = 10000;
    else if(gq==9)m_time[4] = 9000;
    else m_time[4] = 7040;
    m_jg[4] = 22000;
    double dad=2010.0/(GQS-1.0);
    int dbs=2704-int(dad*(gq-1)+0.5)-(lx==5?150:0);
    int dbt=5000+(gq-1)*480+(lx==5?704:0);
    if(gq2<=MAXGQS+3||gq2==2048||gq2==MAXGQS+5)
    {
        if(gq2==2048)
            m_b48=true;
        for(int n=5;n<ND;n+=4)
        {
            int i = 0, j = 0;
            for(i=n,j=0;i<ND-1&&j<3;i++,j++)
                m_time[i]=dbs+j*400,m_jg[i]=dbt;
            m_time[i]=4000,m_jg[i]=20000;
            dbt+=2100;
        }
    }
    else
    {
        ND=90;
        int tm=1170;dbt=5000;
        int i = 0, j = 0;
        for(int n=5;n<ND;n+=4)
        {
            for(i=n,j=0;i<ND-1&&j<3;i++,j++)
                m_time[i]=tm+j*400,m_jg[i]=dbt;
            m_time[i]=2000,m_jg[i]=12000;
            if(i>=45)m_jg[i]+=10000;
            if(i>=60)m_jg[i]*=2;
            dbt+=1000;tm-=170;
            if(tm<220)tm=220;
        }
    }
    for(int n=0;n<ND;n++)
    {
        if(n>10)
            m_time[n]=m_time[n]*SI/110;
        if(m_jg[n]%m_time[n]==0)
            m_jg[n]+=500;
    }
    int ms=-1;
    for(int n=0;n<ND-1;n++)
        ms+=int(m_jg[n]/m_time[n]);
    m_time[ND-1]=3000,m_jg[ND-1]=100000000;
    XUAN=9;if(gq2==10||lx!=0||m_b12)XUAN=10;
    if(gq2==MAXGQS+4)XUAN=11;
    if(gq2==MAXGQS+1)
    {
        SHU=0;XUAN=11;
        JSH=MAXJSH;
        m_b11=true;m_zw[32].mg=false;
        ui->prog->setRange(0,MN*(SI<=55?3000:1000));
        return 0;
    }
    else if(gq2==MAXGQS+2)
    {
        m_b11=m_b12=true;
        JSH=MAXJSH;SHU=MAXSHU;
        ui->prog->setRange(0,MN*(SI<=55?3000:1000));
    }
    else if(gq2==MAXGQS+3||gq2==MAXGQS+5)
    {
        XUAN=0;
        ui->prog->setRange(0,ms);
    }
    else if(gq2==2048)
    {
        SHU=MAXSHU;XUAN=8;int xu[8]={11,3,5,7,8,10,25,18};
        for(int s=0;s<8;s++)m_xuan[s]=xu[s];
        ui->prog->setRange(0,ms);
    }
    else
        ui->prog->setRange(0,ms);
    if(!m_b12)
    {
        int tim=0;
        for(int i=0;i<ND-1;i++)
            tim+=m_jg[i];
        m_ztm=tim;
        QMessageBox::information(this,tr("提示"),QString::asprintf("游戏时间约为%.2lf分钟，约生成僵尸%d个。",(tim/60000.0)/ui->speed->text().toDouble(),zj=ms));
    }
    if(SHU<=XUAN)
    {
        for(int n=0;n<XUAN;n++)
            m_xuan[n]=-1;
        for(int n=0;n<SHU;n++)
            m_xuan[n]=n;
    }
    else if(m_ai)
    {
        int xu[12]={0,10,7,8,9,11,3,5,12,22,25};
        for(int i=0;i<11;i++)
            m_xuan[i]=xu[i];
    }
    else if(XUAN&&!m_b48)
    {
        Choose *ch=new Choose(this);
        QFont f("",floor(8*m_sf),false);
        ch->setFont(f);
        ch->SHU=SHU;ch->XUAN=XUAN;ch->JSH=JSH;
        QString bt[6]={"普通","迷雾","黑夜","黑夜+迷雾","水池","极限"};
        ch->setWindowTitle(bt[lx]);
        for(int i=0;i<SHU;i++)
        {
            ch->yg[i]=m_zw[i].yg;
            ch->ico[i].load(QString::asprintf("://icons/%d.png",m_zw[i].id));
        }
        for(int i=0;i<JSH;i++)
            ch->icj[i].load(QString::asprintf("://icons/%d.png",m_js[i].id));
        ch->setWindowModality(Qt::ApplicationModal);
        ch->Init();ch->show();
        if(!ch->exec())throw(1);
        for(int i=0;i<XUAN;i++)
            m_xuan[i]=ch->xuan[i];
        m_hbzw=false;
        for(int n=0;n<XUAN;n++)
        {
            if(m_zw[m_xuan[n]].hb)
                m_hbzw=true;
        }
    }
    if(lx==2||lx==3)
        m_zw[0].time=10000,m_zw[12].time=5300;
    if(m_gq==30||m_gq==62||m_gq==64)m_gq=60;
    return lx;
}

void MainWindow::on_btc_clicked()
{
    if(m_game.m_ks)
    {
        bool bk=false;
        if(!b_zt)on_stop_clicked(),bk=true;
        if(QMessageBox::question(this,tr("提示"),tr("此操作将结束正在进行的游戏"))!=QMessageBox::Yes)
        {
            if(bk)on_stop_clicked();
            return;
        }
        b_zt=false;QPixmap ic;ic.load("://icons/117.png");
        ui->stop->setIcon(ic);
    }
    m_gq=r_gq;DOU=ui->Doub->isChecked();
    m_ai=ui->Auto->isChecked();
    if((m_gq<1||m_gq>MAXGQS+5)&&m_gq!=2048)
        return;
    GameOver();
    ui->speed->setEnabled(0);
    ui->easy->setEnabled(0);
    ui->normal->setEnabled(0);
    ui->hard->setEnabled(0);
    ui->water->setEnabled(0);
    ui->tui->setEnabled(0);
    ui->xin->setEnabled(0);
    ui->Doub->setEnabled(0);
    try
    {
        StartGame(MakeGq(m_gq));
    }
    catch(...)
    {
        GameOver();
        return;
    }
    if(m_b11)
    {
        SJsh j;
        j.Create(0,(SI<=55?3000:1000),2000,(MM-1)*SI+5,0,54,11);
        for(int n=0;n<MN;n++)
        {
            j.y=n;
            m_game.NewJs(j);
        }
    }
    ui->prog->setFocus();
    ShowButton();repaint();
}

void MainWindow::GameOver()
{
    timer.stop();
    ui->speed->setEnabled(1);
    ui->easy->setEnabled(1);
    ui->normal->setEnabled(1);
    ui->hard->setEnabled(1);
    ui->xin->setEnabled(1);
    ui->tui->setEnabled(1);
    ui->water->setEnabled(1);
    ui->Doub->setEnabled(1);
    if(m_game.m_ks)
        m_game.KillAll();
    SHU=JSH=ND=m_tms=0;
    m_game.m_ks=false;Init();
}

void MainWindow::StartGame(int lx)
{
    m_nzw=-1;
    m_game.m_wnd=this;
    m_n=0;
    m_nd=(m_b11?JSH-1:0);
    SetTimer(1000,lx==5?34:40,NULL);
    SetTimer(1001,20,NULL);
    SetTimer(1002,m_b11?6000*pow(SI/110.0,0.9):m_time[0],NULL);
    if(lx==0||lx==1||lx==4||lx==5)
        SetTimer(1003,6000,NULL);
    SetTimer(1004,m_b11?140000:m_jg[0],NULL);
    if(m_ai)SetTimer(1008,400,NULL);
    if(m_b11)
    {
        SetTimer(1005,DOU?1500:2500,NULL);
        SetTimer(1007,100000,NULL);
    }
    SetTimer(1009,25,NULL);
    for(int n=0;n<MAXSHU;n++)
        m_en[n]=800.0;
    m_game.CreateGame(lx);
    if(m_b11)
    {
        for(int n=0;n<MN;n++)
            m_jwbs[n]=(DOU?3000:1000);
        if(m_b12)
            m_game.m_yg=(SI<=55?5000:1500);
        else
        {
            ui->buy->setVisible(0);
            m_game.m_yg=9999;
        }
    }
    if(m_gq==MAXGQS+3)ui->buy->setVisible(0);
    for(int n=0;n<MN;n++)
        m_lstB[n]=-99999999;
    for(int n=0;n<MN;n++)
        m_lstC[n]=-99999999;
    for(int n=0;n<MN;n++)
        m_lstD[n]=-99999999;
    for(int n=0;n<MN;n++)
        m_lstJ[n]=-99999999;
    for(int n=0;n<MN;n++)
        m_lstT[n]=-99999999;
    jd=tm=0;
    m_dzs=5;
    if(lx==5)m_dzs=4;
    else if(lx==2||lx==3||m_b48)m_dzs=3;
    if(m_gq==MAXGQS+3||m_gq==MAXGQS+5)
    {
        int zx=0,zy=0;m_game.m_yg=100000;
        if(m_gq==MAXGQS+3)
        {
            for(int i=0;i<MAXSHU;i++)
            {
                for(int s=0;s<wsad[i];s++)
                {
                    m_zw[i].x=zx;m_zw[i].y=zy;
                    m_game.NewZw(m_zw[i]);
                    zy+=1;
                    if(zy>=MN)zx+=SI,zy=0;
                }
            }
        }
        else
        {
            for(int s=0;s<10*110/SI;s++)
            {
                m_zw[33].x=zx;m_zw[33].y=zy;
                m_game.NewZw(m_zw[33]);
                zy+=1;
                if(zy>=MN)zx+=SI,zy=0;
            }
            for(int s=0;s<330/SI;s++)
            {
                m_zw[10].x=zx;m_zw[10].y=zy;
                m_game.NewZw(m_zw[10]);
                zy+=1;
                if(zy>=MN)zx+=SI,zy=0;
            }
            m_zw[13].x=zx;m_zw[13].y=zy;
            m_game.NewZw(m_zw[13]);
            zy+=1;
            if(zy>=MN)zx+=SI,zy=0;
            m_zw[27].x=zx;m_zw[27].y=zy;
            m_game.NewZw(m_zw[27]);
        }
        m_gq=MAXGQS+3;
    }
    if(m_b48)m_game.m_yg=500;
    gl->repaint();gx->repaint();
}

void MainWindow::SetTimer(int id,int jg,int *nul)
{
    double sd=ui->speed->text().toDouble();
    if(id==1001&&sd<1)sd=1;
    double tj=jg/sd;
    jg=floor(tj+1e-10);
    tmr[m_tms++]=STm(id,tim.elapsed()+jg,jg,tj-jg);
    if(!b_zt)ResetTimer();
}

void MainWindow::NewTimer(int id,int jg,int ne)
{
    double tj=jg/ui->speed->text().toDouble();
    jg=floor(tj+1e-10);
    tmr[m_tms++]=STm(id,tim.elapsed()+ne,jg,tj-jg);
    ResetTimer();
}

void MainWindow::KillTimer(int id)
{
    int z=-1;
    for(int i=0;i<m_tms;i++)
    {
        if(tmr[i].id==id)
        {
            z=i;
            break;
        }
    }
    if(z==-1)return;
    m_tms-=1;
    if(z<m_tms)tmr[z]=tmr[m_tms];
    ResetTimer();
}
void MainWindow::Save(QDataStream &ar)
{
    for(int i=0;i<MAXSHU;i++)
    {
        m_zw[i].output(ar);
        ar<<m_hf[i]<<m_en[i];
    }
    for(int i=0;i<MAXJSH;i++)
    {
        m_js[i].output(ar);
        ar<<m_p[i];
    }
    ar<<MN<<MM<<SI<<XUAN<<TM;
    m_game.output(ar);
    ar<<m_tms;
    for(int i=0;i<m_tms;i++)
        tmr[i].output(ar);
    ar<<ND<<m_nd<<m_n<<SHU<<JSH<<jd<<tm;
    for(int i=0;i<ND;i++)
        ar<<m_time[i]<<m_jg[i];
    ar<<m_nzw<<m_xx<<m_xy<<m_dzs;
    for(int i=0;i<XUAN;i++)
        ar<<m_xuan[i];
    for(int i=0;i<MN;i++)ar<<m_jwbs[i];
    ar<<m_b11<<m_b12<<m_hbzw<<m_gq<<m_ai<<r_gq<<m_ea<<m_hd<<m_tui<<m_b48;
    for(int i=0;i<MN;i++)
        ar<<m_lstJ[i]<<m_lstC[i]<<m_lstD[i]<<m_lstT[i]<<m_lstB[i];
    ar<<ui->speed->text();
}
void MainWindow::Load(QDataStream &ar)
{
    for(int i=0;i<MAXSHU;i++)
    {
        m_zw[i].input(ar);
        ar>>m_hf[i]>>m_en[i];
    }
    for(int i=0;i<MAXJSH;i++)
    {
        m_js[i].input(ar);
        ar>>m_p[i];
    }
    ar>>MN>>MM>>SI>>XUAN>>TM;
    m_game.input(ar);
    ar>>m_tms;
    for(int i=0;i<m_tms;i++)
        tmr[i].input(ar);
    ar>>ND>>m_nd>>m_n>>SHU>>JSH>>jd>>tm;
    for(int i=0;i<ND;i++)
        ar>>m_time[i]>>m_jg[i];
    ar>>m_nzw>>m_xx>>m_xy>>m_dzs;
    for(int i=0;i<XUAN;i++)
        ar>>m_xuan[i];
    for(int i=0;i<MN;i++)ar>>m_jwbs[i];
    ar>>m_b11>>m_b12>>m_hbzw>>m_gq>>m_ai>>r_gq>>m_ea>>m_hd>>m_tui>>m_b48;
    for(int i=0;i<MN;i++)
        ar>>m_lstJ[i]>>m_lstC[i]>>m_lstD[i]>>m_lstT[i]>>m_lstB[i];
    QString sd;ar>>sd;
    ui->speed->setValue(sd.toDouble());
    if(m_ea==4)
        ui->xin->setChecked(1);
    if(m_ea==3)
        ui->water->setChecked(1);
    else if(m_ea==2)
        ui->easy->setChecked(1);
    else if(m_tui)
        ui->tui->setChecked(1);
    else if(m_hd)
        ui->hard->setChecked(1);
    else
        ui->normal->setChecked(1);
}

void MainWindow::ResetTimer()
{
    if(b_zt)return;
    timer.stop();
    int nw=tim.elapsed(),mi=1e9,wz=-1,wi;
    for(int i=0;i<m_tms;i++)
    {
        if(tmr[i].ne<mi)
        {
            mi=tmr[i].ne;
            wz=tmr[i].id;wi=i;
        }
    }
    if(wz==-1)
        return;
    int t=mi-nw;
    if(t<0)t=0;
    nid=wz;timer.start(t);
}







CGame::CGame()
{
    for(int x=0;x<MM*SI+100;x++)
    {
        for(int y=0;y<MN;y++)
            fr[x][y]=bh[x][y]=-1;
    }
    for(int i=0;i<MN;i++)hjs[i]=lst[i]=0;
    m_ks=zk=false;XS=0;t_ai=m_cc=0;
}

CGame::~CGame()
{
}

void CGame::DestroyJs(int z)
{
    m_jsh[z].shm=-1;
    hjs[m_jsh[z].y]-=1;
    if(SZZ)return;
    m_jshu-=1;
    if (z < m_jshu)
    {
        m_jsh[z] = m_jsh[m_jshu];
        m_jsh[z].shu = z;
    }
}

void CGame::CalZw()
{
    for(int y=0;y<MN;y++)
    {
        for(int x=0;x<MM*SI+100;x++)
            fr[x][y]=bh[x][y]=-1;
    }
    for(int i=0;i<m_zshu;i++)
        fr[m_zhw[i].x][m_zhw[i].y]=bh[m_zhw[i].x][m_zhw[i].y]=i;
    for(int y=0;y<MN;y++)
    {
        for(int x=1;x<MM*SI+100;x++)
        {
            if(fr[x][y]==-1)
                fr[x][y]=fr[x-1][y];
        }
    }
}

int CGame::DestroyZw(int z)
{
    int tt=-1,rt=0;
    for(int i=0;i<m_wnd->m_tms;i++)
    {
        if(m_wnd->tmr[i].id==z)
        {
            tt=i;
            break;
        }
    }
    rt=m_wnd->tmr[tt].ne-m_wnd->tim.elapsed();
    if(rt<0)rt=0;
    m_zhw[z].shm=-1;
    bh[m_zhw[z].x][m_zhw[z].y]=-1;
    m_wnd->KillTimer(z);
    m_wnd->KillTimer(z+10000);
    m_zshu-=1;
    if (z < m_zshu)
    {
        m_zhw[z] = m_zhw[m_zshu];
        m_zhw[z].shu = z;
        bh[m_zhw[z].x][m_zhw[z].y]=z;
        int tt=-1;
        for(int i=0;i<m_wnd->m_tms;i++)
        {
            if(m_wnd->tmr[i].id==m_zshu)
            {
                tt=i;
                break;
            }
        }
        if(tt==-1)
            QMessageBox::critical(m_wnd,tr(""),tr("BUG"));
        int te=m_wnd->tmr[tt].ne-m_wnd->tim.elapsed();
        m_wnd->KillTimer(m_zshu);//重置计时器
        if(te<0)te=0;
        m_wnd->NewTimer(z,m_zhw[z].time,te);
        if(m_zhw[z].time==100)//重置大招
        {
            tt=-1;
            for(int i=0;i<m_wnd->m_tms;i++)
            {
                if(m_wnd->tmr[i].id==m_zshu+10000)
                {
                    tt=i;
                    break;
                }
            }
            if(tt==-1)
                QMessageBox::critical(m_wnd,tr(""),tr("BUG"));
            m_wnd->KillTimer(m_zshu+10000);
            m_wnd->NewTimer(z+10000,m_zhw[z].dztm,te);
        }
    }
    CalZw();return rt;
}

void CGame::DestroyZw(int x,int y)
{
    int shu=GetZwShu(x,y);
    if(shu==-1)
        return;
    DestroyZw(shu);
}

void CGame::DestroyZd(int z)
{
    if(m_zid[z].sd<0&&m_zid[z].dx==-2)
        m_cing[m_zid[z].y]=false;
    m_zid[z].y=-1;
    m_dshu-=1;
    if(z<m_dshu)
        m_zid[z]=m_zid[m_dshu];
}
int CGame::GetZwShu(int x,int y)const
{
    if(x<0||x>=MM*SI+100||y<0||y>=MN)return -1;
    return bh[x][y];
}

bool CGame::meihuo(int jshu)
{
    if(m_jsh[jshu].lx==11)return false;//jiangwang
    if((m_jsh[jshu].lx==9||m_jsh[jshu].lx==12)&&rand()%10<9)
        return false;
    SZid zd;zd.x=m_jsh[jshu].x;zd.y=m_jsh[jshu].y;zd.sd=-1;
    zd.dx=m_jsh[jshu].lx;zd.goj=m_jsh[jshu].shm;zd.zx=m_jsh[jshu].id;zd.zy=0;
    NewZd(zd);DestroyJs(jshu);
    return true;
}

bool CGame::Eat(int zshu,int jshu)
{
    if(m_jsh[jshu].lx==15)
    {
        Crepeer(jshu);
        return true;
    }
    if(m_zhw[zshu].id==126&&m_zhw[zshu].shm>m_jsh[jshu].eat)
    {
        m_jsh[jshu].x-=m_jsh[jshu].sd*2;
        return false;
    }
    if(m_jsh[jshu].beat)
    {
        int t=GetJsEat(m_zhw[zshu],m_jsh[jshu],*this);//特殊吃法
        if(t)m_jsh[jshu].id=t;
        return t==0;
    }
    int x=m_zhw[zshu].x/SI,y=m_zhw[zshu].y,et=m_jsh[jshu].eat;
    m_ng[x][y]-=et;et=0;
    if(m_ng[x][y]<0)
    {
        et=-m_ng[x][y];
        m_ng[x][y]=0;
    }
    if(m_jsh[jshu].lx==9)
    {
        int z=m_wnd->m_ea;
        if(z>2)z=2;
        m_jsh[jshu].fss+=1-rand()%z;
        if(m_wnd->m_hd)m_jsh[jshu].fss+=1;
        if(m_wnd->m_tui)m_jsh[jshu].fss+=1;
        if(m_jsh[jshu].fss>=20)
        {
            m_zhw[zshu].shm-=et;
            m_jsh[jshu].fss=0;
        }
    }
    else m_zhw[zshu].shm-=et;
    if(m_jsh[jshu].lx<100&&m_jsh[jshu].eat)//吃东西恢复生命
    {
        if(m_jsh[jshu].lx==0)
            m_jsh[jshu].shm+=rand()%200==0;
        else if(m_jsh[jshu].lx==1)
            m_jsh[jshu].shm+=rand()%160==0;
        else if(m_jsh[jshu].lx==6)
            m_jsh[jshu].shm+=rand()%50==0;
        else if(m_jsh[jshu].lx!=4)
            m_jsh[jshu].shm+=rand()%100==0;
        m_jsh[jshu].id=GetJsIcon(m_jsh[jshu],*this);
    }
    bool rt=false;
    if(m_zhw[zshu].lx==32&&(!m_zhw[zshu].mg||m_lx==2||m_lx==3)&&et&&m_jsh[jshu].lx!=9&&m_jsh[jshu].lx!=11&&m_jsh[jshu].lx!=12)
    {
        rt=true;
        meihuo(jshu);
    }
    m_zhw[zshu].id=GetZwIcon(m_zhw[zshu]);
    if(m_zhw[zshu].shm<=0)//吃光
    {
        m_zhw[zshu].shm=0;
        DestroyZw(zshu);
    }
    return rt;
}

int CGame::GetFirstJs(int x,int y)
{
    int first=10000,rtn=-1;
    for(int i=0;i<m_jshu;i++)//遍历僵尸
    {
        if(m_jsh[i].shm==-1)
            continue;
        if(m_jsh[i].x<first&&m_jsh[i].x>x&&m_jsh[i].y==y&&m_jsh[i].lx<100)
        {
            rtn=i;
            first=m_jsh[i].x;
        }
    }
    return rtn;
}

int CGame::GetFirstZw(int x,int y)
{
    if(x<=0||x-1>=MMM*MSI+100)return -1;
    return fr[x-1][y];
}

bool CGame::Goj(int zshu)//植物攻击
{
    if(m_zhw[zshu].mg&&(m_lx==0||m_lx==1||m_lx==4||m_lx==5))
        return false;
    if(m_zhw[zshu].goj<0)//非标准植物
        return NotZw(zshu);
    if(m_zhw[zshu].zha)
    {
        m_zhw[zshu].fu-=1;
        if(m_zhw[zshu].fu<0)
            m_zhw[zshu].fu=0;
        m_zhw[zshu].id=GetZwIcon(m_zhw[zshu]);
        int rtn=GetFirstJs(m_zhw[zshu].x,m_zhw[zshu].y);
        if(rtn==-1)return false;
        if(m_jsh[rtn].x<m_zhw[zshu].x+250&&m_zhw[zshu].fu==0)
        {
            KillJs(m_jsh[rtn].x,m_jsh[rtn].y,m_zhw[zshu].goj);
            if(m_zhw[zshu].hb)
            {
                DestroyZw(zshu);
                return true;
            }
            m_zhw[zshu].fu=m_zhw[zshu].sd;
            m_zhw[zshu].id=GetZwIcon(m_zhw[zshu]);
        }
        return false;
    }
    SZid zd;//发射子弹
    zd.color=m_zhw[zshu].color;
    zd.sd=m_zhw[zshu].sd;
    zd.x=m_zhw[zshu].x-35;
    zd.y=m_zhw[zshu].y;
    zd.goj=m_zhw[zshu].goj;
    zd.dx=m_zhw[zshu].dx;
    zd.hb=m_zhw[zshu].hb;
    zd.tx=m_zhw[zshu].x-1;
    if(zd.goj>=4)
    {
        zd.sx=m_zhw[zshu].x-35;
        if(hjs[m_zhw[zshu].y]>0)
        {
            NewZd(zd);
            m_zhw[zshu].nfs=TM+m_zhw[zshu].time/25;
        }
    }
    else if(m_zhw[zshu].lx==22)
    {
        for(int y=0;y<MN;y++)
        {
            if(hjs[y]>0)
            {
                zd.y=y;zd.x=zd.sx=m_zhw[zshu].x;
                zd.zx=m_zhw[zshu].x;
                zd.zy=m_zhw[zshu].y;
                zd.mh=(rand()%100==0);NewZd(zd);
                zd.x=zd.sx=-120;
                zd.mh=(rand()%100==0);NewZd(zd);
            }
        }
    }
    else if(hjs[m_zhw[zshu].y]>0)NewZd(zd);
    return false;
}

void CGame::NewZd(SZid zd)
{
    if(m_dshu>400)//上限
    {
        QMessageBox::critical(m_wnd,tr(""),tr("FULL"));
        return;
    }
    m_zid[m_dshu++]=zd;
}

void CGame::NewJs(SJsh js)
{
    if(m_jshu>800)//上限
        return;
    hjs[js.y]+=1;
    m_jsh[m_jshu]=js;
    m_jsh[m_jshu].shu = m_jshu;
    m_jshu+=1;
}

bool CGame::NewZw(SZhw zw)
{
    if(m_keng[zw.x/SI][zw.y]==1||(m_keng[zw.x/SI][zw.y]==2&&zw.lx!=14)||m_dc[zw.x/SI][zw.y])//有坑，或有水且不是荷叶
        return false;
    m_yg-=zw.yg;
    if(m_yg<0)
    {
        m_yg+=zw.yg;
        return false;
    }
    if(zw.lx==24)
    {
        m_ng[zw.x/SI][zw.y]=500;
        return true;
    }
    if(zw.lx==28)
    {
        int z=GetZwShu(zw.x,zw.y);
        if(z!=-1&&m_zhw[z].mg)
        {
            m_zhw[z].mg=false;
            return true;
        }
        m_yg+=zw.yg;
        return false;
    }
    if(zw.lx==29)
    {
        m_dc[zw.x/SI][zw.y]=9;
        return true;
    }
    if(GetZwShu(zw.x,zw.y)!=-1)
    {
        m_yg+=zw.yg;
        return false;
    }
    if(zw.lx==14)
    {
        if(m_keng[zw.x/SI][zw.y]==2)
        {
            m_keng[zw.x/SI][zw.y]=3;
            return true;
        }
        m_yg+=zw.yg;
        return false;
    }
    int i=m_zshu;
    m_zhw[i]=zw;
    bh[zw.x][zw.y]=i;
    m_zhw[i].shu=i;
    m_zhw[i].id=GetZwIcon(m_zhw[i]);
    if(zw.te==-1)
        m_wnd->SetTimer(i,zw.time,NULL);
    else
        m_wnd->NewTimer(i,zw.time,zw.te);
    m_zshu+=1;
    if(zw.time<=1)
        Goj(i);
    CalZw();
    return true;
}

bool CGame::MoveJs(int shu)
{	
    bool rt=false;
    if(m_jsh[shu].hb%2==0||(m_jsh[shu].lx==12&&m_jsh[shu].shm>30)||m_jsh[shu].lx==11||m_jsh[shu].lx>=100)//若寒冰效果，速度变为一半，车不受影响
    {
        int tx=(m_jsh[shu].x+85*SI/110)/SI,ty=m_jsh[shu].y;
        if(tx>=0&&tx<MM&&m_jsh[shu].lx<100&&m_dc[tx][ty]&&rand()%10==0&&m_jsh[shu].lx!=8)
        {
            if(m_jsh[shu].lx==9||m_jsh[shu].lx==11)
                m_dc[tx][ty]=0;
            else if(m_jsh[shu].lx==12&&m_jsh[shu].shm>10)
            {
                m_jsh[shu].shm=10;
                m_dc[tx][ty]-=1;
            }
            else m_jsh[shu].shm-=1;
            m_jsh[shu].id=GetJsIcon(m_jsh[shu],*this);
            if(m_jsh[shu].shm<=0)
            {
                DestroyJs(shu);
                return true;
            }
        }
        if(tx>=0&&tx<MM&&m_jsh[shu].lx==12&&m_jsh[shu].shm>10)
            m_ice[tx][ty]=TM;
        int i=GetFirstZw(m_jsh[shu].x,m_jsh[shu].y);
        m_jsh[shu].x-=m_jsh[shu].sd;
        if(m_jsh[shu].lx==17&&rand()%300==0&&m_jsh[shu].x<=830)
        {
            for(int i=0;i<m_jshu;i++)
            {
                if(i==shu||m_jsh[i].lx>=100||m_jsh[i].shm==-1||m_jsh[i].lx==11||m_jsh[i].lx==16)
                    continue;
                if(m_jsh[i].x>=m_jsh[shu].x-SI&&m_jsh[i].x<=m_jsh[shu].x+SI&&m_jsh[i].y>=m_jsh[shu].y-1&&m_jsh[i].y<=m_jsh[shu].y+1)
                    m_jsh[i].x-=110;
            }
            for(int i=0;i<m_zshu;i++)
            {
                if(m_zhw[i].shm!=-1&&m_zhw[i].y==m_jsh[shu].y)
                {
                    m_zhw[i].shm/=1.5;
                    if(m_zhw[i].shm<=0)
                    {
                        DestroyZw(i);
                        i-=1;
                    }
                    else
                        m_zhw[i].id=GetZwIcon(m_zhw[i]);
                }
            }
        }
        if(i!=-1&&m_jsh[shu].x-80*SI/110<=m_zhw[i].x)
        {
            m_jsh[shu].x+=m_jsh[shu].sd;
            rt=Eat(i,shu);if(rt)return true;
            m_jsh[shu].b_et=true;
        }
        else
        {
            if(m_jsh[shu].lx==9)
                m_jsh[shu].fss=0;
            m_jsh[shu].b_et=false;
        }
        if(m_jsh[shu].x<-110&&!m_over)
        {
            if(m_jsh[shu].lx==16)
            {
                DestroyJs(shu);
                return true;
            }
            if(m_jsh[shu].lx==11)//？
            {
                m_jsh[shu].sd=-10;
                return false;
            }
            if(m_jsh[shu].lx>=100)//僵尸的子弹
            {
                DestroyJs(shu);
                return true;
            }
            if(m_car[m_jsh[shu].y])//车 此处有BUG
            {
                int ty=m_jsh[shu].y;
                m_car[ty]=false;m_cing[ty]=true;
                SZid zd;zd.goj=1e8;zd.dx=-2;zd.y=ty;zd.x=-150;zd.sd=-4;
                zd.zx=28;NewZd(zd);
            }
            else if(!m_cing[m_jsh[shu].y])
            {
                m_over=true;
                int s=0;
                for(int i=0;i<m_jshu;i++)
                {
                    if(m_jsh[i].lx<100)
                        s+=1;
                }
                KillAll();m_wnd->GameOver();
                QMessageBox::information(m_wnd,tr("提示"),QString::asprintf("失败\r\n得分:%d",m_wnd->jd-(m_wnd->m_b11?0:s)));
            }
        }
    }
    if(m_jsh[shu].lx==15)//苦力怕走得慢
        m_jsh[shu].hb^=1;
    else
    {
        m_jsh[shu].hb+=1;
        if(m_jsh[shu].hb>100)
            m_jsh[shu].hb=100;
    }
    if(m_jsh[shu].fs)
    {
        m_jsh[shu].fss+=1;
        int zz=35*m_wnd->m_ea;
        if(zz==70)zz=50;
        if(m_wnd->m_tui)zz=24;
        else if(m_wnd->m_hd)zz=30;
        if(m_jsh[shu].fss>=zz)//发射时间
        {
            m_jsh[shu].fss=0;
            Fash(m_jsh[shu],*this);
        }
    }
    return rt;
}

void CGame::Crepeer(int i)
{
    if(m_jsh[i].lx==15)//苦力怕
    {
        int x=(m_jsh[i].x/SI)*SI,y=m_jsh[i].y;
        for(int a=-2;a<=1;a++)
        {
            for(int b=-1;b<=1;b++)
            {
                int z=GetZwShu(x+a*SI,y+b);
                if(z!=-1)
                {
                    int et=0,tx=(x+a*SI)/SI,ty=y+b;
                    m_ng[tx][ty]-=300;
                    if(m_ng[tx][ty]<0)
                    {
                        et=-m_ng[tx][ty];
                        m_ng[tx][ty]=0;
                    }
                    m_zhw[z].shm-=et;
                    if(m_zhw[z].shm<=0)
                        DestroyZw(z);
                    else
                        m_zhw[z].id=GetZwIcon(m_zhw[z]);
                }
            }
        }
    }
    DestroyJs(i);
}

SZid p_zid[1000];SJsh p_jsh[1000];

bool cmp1(int a,int b)
{
    return p_zid[a].x<p_zid[b].x;
}

bool cmp2(int a,int b)
{
    return p_jsh[a].x<p_jsh[b].x;
}

void CGame::MoveAllZd()//双指针
{
    SZZ=true;double sd=m_wnd->ui->speed->value();
    if(sd>1)sd=1;
    int zs[MMN]={0},js[MMN]={0};SZid nw[200];int ns=0;
    for(int i=0;i<m_dshu;i++)
    {
        int y=m_zid[i].y;
        pd[y][zs[y]++]=i;
        p_zid[i]=m_zid[i];
    }
    for(int i=0;i<m_jshu;i++)
    {
        if(m_jsh[i].lx>=100)continue;
        int y=m_jsh[i].y;
        pj[y][js[y]++]=i;
        p_jsh[i]=m_jsh[i];
    }
    for(int y=0;y<MN;y++)
    {
        std::sort(pd[y],pd[y]+zs[y],cmp1);
        std::sort(pj[y],pj[y]+js[y],cmp2);
        for(int a=0,b=0;a<zs[y];a++)
        {
            int shu=pd[y][a],zx=m_zid[shu].x;
            if(m_zid[shu].sd>0)
                m_zid[shu].x+=m_zid[shu].sd*2*sd;
            else
                m_zid[shu].x-=m_zid[shu].sd*sd;
            while(b<js[y]&&(m_jsh[pj[y][b]].x<=zx||m_jsh[pj[y][b]].shm<0))b+=1;
            int i=(b==js[y]?-1:pj[y][b]);
            if(i!=-1)m_zid[shu].tx=m_jsh[i].x;
            else m_zid[shu].tx=MM*SI+20;
            if(m_zid[shu].sd<0)
            {
                if(m_zid[shu].dx==13)
                {
                    m_zid[shu].zy+=1;
                    if(m_zid[shu].zy>=125)
                    {
                        m_zid[shu].zy=0;
                        SZid zd;zd.hb=false;
                        zd.color=QColor(0,255,0);
                        zd.dx=5;zd.goj=1;zd.sd=5;
                        zd.x=m_zid[shu].x;zd.y=y;
                        nw[ns++]=zd;
                    }
                }
                else if(m_zid[shu].dx==14)
                {
                    m_zid[shu].zy+=1;
                    if(m_zid[shu].zy>=125)
                    {
                        m_zid[shu].zy=0;
                        SZid zd;zd.hb=true;
                        zd.color=QColor(128,128,255);
                        zd.dx=11;zd.goj=4;zd.sd=5;
                        zd.x=m_zid[shu].x;zd.y=y;
                        nw[ns++]=zd;
                    }
                }
            }
            if(i!=-1&&m_zid[shu].x+50>=m_jsh[i].x)
            {
                bool mh=false;
                if(m_zid[shu].sd>0)
                {
                    if(m_zid[shu].mh)
                        mh=meihuo(i);
                    else
                    {
                        m_jsh[i].shm-=m_zid[shu].goj;
                        if(m_zid[shu].hb)
                            m_jsh[i].hb=0;
                    }
                }
                else
                {
                    int z=m_zid[shu].goj;
                    if(m_jsh[i].shm<z)z=m_jsh[i].shm;
                    if(m_jsh[i].lx==11&&m_zid[shu].dx==-2)
                        z=0;
                    m_zid[shu].goj-=z;
                    m_jsh[i].shm-=z;
                }
                if(!mh)
                {
                    if(m_jsh[i].shm<=0)
                    {
                        if(m_jsh[i].lx==15&&(m_wnd->m_hd||rand()%(1+m_wnd->m_ea)==0))
                            Crepeer(i);
                        else
                            DestroyJs(i);
                        b+=1;
                    }
                    else m_jsh[i].id=GetJsIcon(m_jsh[i],*this);
                }
                if(m_zid[shu].sd>0&&m_zid[shu].goj>=4)//群体伤害
                {
                    int tx=m_jsh[i].x;
                    int y=m_zid[shu].y,z=(m_zid[shu].goj==5?-1:1);
                    if(y>0)KillJs(tx,y-1,z);
                    if(y<MN-1)KillJs(tx,y+1,z);
                    KillJs(tx,y,z);
                }
                if(m_zid[shu].sd>0||m_zid[shu].goj<=0)
                    m_zid[shu].y=-1;
                if(m_zid[shu].sd<0)
                {
                    SJsh zz;zz.lx=m_zid[shu].dx;
                    zz.shm=m_zid[shu].goj;zz.y=-1;
                    m_zid[shu].zx=GetJsIcon(zz,*this);
                }
            }
            if(m_zid[shu].x>MM*SI+10)
            {
                if(m_zid[shu].dx==-2)
                    m_cing[m_zid[shu].y]=false;
                m_zid[shu].y=-1;
            }
        }
    }
    int sa=0,sb=0;
    for(int i=0;i<m_dshu;i++)
    {
        if(m_zid[i].y!=-1)
            tmz[sa++]=m_zid[i];
    }
    for(int i=0;i<m_jshu;i++)
    {
        if(m_jsh[i].shm!=-1)
            tmj[sb++]=m_jsh[i];
    }
    m_dshu=sa;m_jshu=sb;
    for(int i=0;i<sa;i++)m_zid[i]=tmz[i];
    for(int i=0;i<ns;i++)m_zid[m_dshu++]=nw[i];
    for(int i=0;i<sb;i++)
    {
        m_jsh[i]=tmj[i];
        m_jsh[i].shu=i;
    }
    SZZ=false;
}

const double pi=acos(-1.0);

bool inwjx(int x1,int y1,int x2,int y2)
{
    double jj=atan2(double(y2-y1),double(x2-x1));
    double h=-pi;
    for(int i=0;i<=5;i++,h+=0.4*pi)
    {
        if(jj>h-0.04*pi&&jj<h+0.04*pi)
            return true;
    }
    return false;
}

void CGame::Draw(QPainter &memdc)
{
    QPixmap im;
    for(int i=0;i<MN;i++)
        memdc.drawPixmap(QRect(MPoint(0,i*SI+60),MSize(SI,SI)),m_wnd->ico[112]);
    memdc.drawPixmap(QRect(MPoint(0,0),MSize(1040,60)),m_wnd->ico[113]);
    memdc.drawPixmap(QRect(MPoint(50,60),MSize(990,550)),m_wnd->ico[111]);
    if(m_ks)
    {
        for(int n=0;n<MN;n++)
        {
            if(m_car[n])
                memdc.drawPixmap(QRect(MPoint(0,n*SI+60),MSize(SI,SI)),m_wnd->ico[28]);
        }
        for(int x=0;x<MM;x++)
        {
            for(int y=0;y<MN;y++)
            {
                int t=GetZwShu(x*SI,y);
                if(t!=-1&&m_zhw[t].lx!=33)
                    memdc.drawPixmap(QRect(MPoint(x*SI+50,y*SI+60),MSize(SI,SI)),m_wnd->ico[114]);
                if(TM-m_ice[x][y]<(m_wnd->m_hd?800:400))
                    memdc.fillRect(QRect(MPoint(x*SI+50,y*SI+60+SI/3),MSize(SI,SI*2/3)),QColor(255,255,255));
            }
        }
        QPen pe(Qt::black);pe.setStyle(Qt::SolidLine);
        for(int x=0;x<=MN;x++)
            memdc.drawLine(MPoint(50,x*SI+60),MPoint(MM*SI+50,x*SI+60));
        for(int x=0;x<=MM;x++)
            memdc.drawLine(MPoint(x*SI+50,60),MPoint(x*SI+50,MN*SI+60));
        if(m_wnd->m_b48)
        {
            pe.setColor(QColor(255,0,0));memdc.setPen(pe);
            memdc.drawLine(MPoint(MN*SI+50,60),MPoint(MN*SI+50,MN*SI+60));
        }
        for(int i=0;i<MM;i++)
        {
            for(int j=0;j<MN;j++)
            {
                if(m_keng[i][j]==0)
                    continue;
                memdc.drawPixmap(QRect(MPoint(i*SI+50,j*SI+60),MSize(SI,SI)),m_wnd->ico[58+m_keng[i][j]]);
            }
        }
        if(m_lx==0||m_lx==1||m_lx==4||m_lx==5)
            memdc.drawPixmap(QRect(MPoint(300+(TM*25.0)/m_wnd->m_ztm*680,0),MSize(60,60)),m_wnd->ico[122]);
        else
            memdc.drawPixmap(QRect(MPoint(300+(TM*25.0)/m_wnd->m_ztm*680,0),MSize(60,60)),m_wnd->ico[123]);
        for(int i=0;i<MM;i++)
        {
            for(int j=0;j<MN;j++)
            {
                if(m_keng[i][j]==0)
                    continue;
                memdc.drawPixmap(QRect(MPoint(i*SI+50,j*SI+60),MSize(SI,SI)),m_wnd->ico[58+m_keng[i][j]]);
            }
        }
        for(int i=0;i<m_zshu;i++)
        {
            if(m_zhw[i].shm==-1)
                continue;
            memdc.drawPixmap(QRect(MPoint(m_zhw[i].x+50,m_zhw[i].y*SI+60),MSize(SI,SI)),m_wnd->ico[m_zhw[i].id]);
            if(m_zhw[i].mg&&(m_lx==0||m_lx==1||m_lx==4||m_lx==5))
                memdc.drawPixmap(QRect(MPoint(m_zhw[i].x+50,m_zhw[i].y*SI+60),MSize(SI,SI)),m_wnd->ico[129]);
        }
        pe.setColor(QColor(174,0,174));QBrush br(QColor(174,0,174));
        br.setStyle(Qt::SolidPattern);
        memdc.setPen(pe);memdc.setBrush(br);
        for(int i=0;i<m_zshu;i++)
        {
            if(m_zhw[i].lx==23&&m_zhw[i].nfs)
            {
                for(int a=(TM*3)%20;a<=SI*4/3;a+=20)
                {
                    if(a<=SI/3)continue;
                    for(int b=-1;b<=1;b++)
                    {
                        for(int c=-1;c<=1;c++)
                        {
                            if(b==0&&c==0)continue;
                            int x=m_zhw[i].x+a*b+SI/2+50,y=m_zhw[i].y*SI+a*c+SI/2+60;
                            memdc.drawEllipse(QRect(MPoint(x-3,y-3),MSize(6,6)));
                        }
                    }
                }
            }
        }
        pe.setColor(QColor(120,0,120));br.setColor(QColor(120,0,120));
        memdc.setPen(pe);memdc.setBrush(br);
        for(int i=0;i<m_zshu;i++)
        {
            if(m_zhw[i].lx==26&&m_zhw[i].nfs)
            {
                int zz=(m_zhw[i].time==100?15:60);
                for(int a=m_zhw[i].x+SI/2+(TM*7)%zz;a<=MM*SI;a+=zz)
                {
                    int x=a+50,y=m_zhw[i].y*SI+SI/2+60;
                    memdc.drawEllipse(QRect(MPoint(x-4,y-4),MSize(8,8)));
                }
                for(int a=m_zhw[i].x+SI/2-(TM*7)%zz;a>=0;a-=zz)
                {
                    int x=a+50,y=m_zhw[i].y*SI+SI/2+60;
                    memdc.drawEllipse(QRect(MPoint(x-4,y-4),MSize(8,8)));
                }
            }
        }
        for(int i=0;i<MM;i++)
        {
            for(int j=0;j<MN;j++)
            {
                if(m_ng[i][j])
                {
                    if(m_ng[i][j]<250)
                        memdc.drawPixmap(QRect(MPoint(i*SI+50,j*SI+60),MSize(SI,SI)),m_wnd->ico[103]);
                    else
                        memdc.drawPixmap(QRect(MPoint(i*SI+50,j*SI+60),MSize(SI,SI)),m_wnd->ico[102]);
                }
                if(m_dc[i][j])
                {
                    if(m_dc[i][j]<5)
                        memdc.drawPixmap(QRect(MPoint(i*SI+50,j*SI+60),MSize(SI,SI)),m_wnd->ico[133]);
                    else
                        memdc.drawPixmap(QRect(MPoint(i*SI+50,j*SI+60),MSize(SI,SI)),m_wnd->ico[132]);
                }
            }
        }
        for(int i=0;i<m_jshu;i++)
        {
            if(m_jsh[i].shm==-1)
                continue;
            if(m_jsh[i].lx!=15)
            {
                int t=(m_jsh[i].b_et&&m_jsh[i].lx!=9?rand()%(8*SI/110+1)-4*SI/110:0),z=SI;
                if(m_jsh[i].lx==9)z*=1.3;
                if(m_jsh[i].hb<100&&m_jsh[i].lx!=12)
                    memdc.drawPixmap(QRect(MPoint(m_jsh[i].x+50+SI-z,m_jsh[i].y*SI+t+60-(m_jsh[i].lx==9?m_jsh[i].fss*SI/55:0)+SI-z),MSize(z,z)),m_wnd->icd[m_jsh[i].id]);
                else
                    memdc.drawPixmap(QRect(MPoint(m_jsh[i].x+50+SI-z,m_jsh[i].y*SI+t+60-(m_jsh[i].lx==9?m_jsh[i].fss*SI/55:0)+SI-z),MSize(z,z)),m_wnd->ico[m_jsh[i].id]);
            }
        }
        for(int i=0;i<m_jshu;i++)
        {
            if(m_jsh[i].lx==15)
                memdc.drawPixmap(QRect(MPoint(m_jsh[i].x+50,m_jsh[i].y*SI+60-(m_jsh[i].lx==9?m_jsh[i].fss*SI/55:0)),MSize(SI,SI)),m_wnd->ico[m_jsh[i].id]);
        }
        bool de[1010];SZid zd[1010];int ds=0;
        for(int i=0;i<m_dshu;i++)
        {
            if(m_zid[i].y==-1)
                continue;
            de[i]=false;SZid zd=m_zid[i];
            if(m_zid[i].sd<0)
            {
                im=m_wnd->icm[m_zid[i].zx];
                memdc.drawPixmap(QRect(MPoint(zd.x+50,zd.y*SI+60),MSize(SI,SI)),im);
                continue;
            }
            zd.x+=28;int left=(SI-zd.dx)/2;
            if(zd.color==QColor(255,255,0))
            {
                QPen p(zd.color);QBrush br(zd.color);
                p.setStyle(Qt::SolidLine);br.setStyle(Qt::SolidPattern);
                memdc.setPen(p);memdc.setBrush(br);
                if(zd.tx<MM*SI+20&&(zd.tes||inwjx(zd.zx,zd.zy*SI,zd.tx,zd.y*SI)))
                {
                    double bl=double(zd.x-zd.sx)/(zd.tx-zd.sx);
                    int hx=bl*(zd.tx-zd.zx)+zd.zx;
                    int hy=bl*(zd.y*SI-zd.zy*SI)+zd.zy*SI;
                    memdc.drawEllipse(QRect(MPoint(hx+left+50,hy+left+60),MSize(zd.dx,zd.dx)));
                    m_zid[i].tes=true;
                }
                else de[i]=true;
            }
            else if(zd.color==QColor(10,170,255)||zd.color==QColor(0,200,0))
            {
                int md=(zd.tx+zd.sx)/2;
                int z=md-zd.sx;z*=z;int hy=zd.y*SI-(z+800)/1600;
                z=md-zd.x;z*=z;hy+=(z+800)/1600;
                if(zd.x+60>=zd.tx&&zd.tx<MM*SI+20)
                {
                    QPen p(zd.color);QBrush br(zd.color);
                    p.setStyle(Qt::SolidLine);br.setStyle(Qt::SolidPattern);
                    memdc.setPen(p);memdc.setBrush(br);
                    for(int s=-1;s<=1;s++)
                        memdc.drawEllipse(QRect(MPoint(zd.x+left+50,hy+left-SI/3.66+s*10+60),MSize(zd.dx/3,zd.dx/3)));
                }
                else
                {
                    im=(zd.color==QColor(10,170,255)?m_wnd->ico[139]:m_wnd->ico[140]);
                    int zx=zd.x+left+50,zy=hy+left-SI/3.66+45;
                    zx=floor(zx*m_wnd->m_sf);zy=floor(zy*m_wnd->m_sf);
                    memdc.translate(zx,zy);memdc.rotate(zd.x-zd.sx);
                    memdc.drawPixmap(QRect(MPoint(-im.size().width()/2,-im.size().height()/2),MSize(im.size().width(),im.size().height())),im);
                    memdc.rotate(zd.sx-zd.x);memdc.translate(-zx,-zy);
                }
            }
            else if(zd.color==QColor(255,0,255))
            {
                QPen p(zd.color);QBrush br(zd.color);
                p.setStyle(Qt::SolidLine);br.setStyle(Qt::SolidPattern);
                memdc.setPen(p);memdc.setBrush(br);
                if(zd.tx-zd.zx>440)
                    de[i]=true;
                else
                    memdc.drawEllipse(QRect(MPoint(zd.x+left+50,zd.y*SI+left+60),MSize(zd.dx,zd.dx)));
            }
            else
            {
                if(zd.x+60>=zd.tx&&zd.tx<MM*SI+20+SI/2)
                {
                    QPen p(zd.color);QBrush br(zd.color);
                    p.setStyle(Qt::SolidLine);br.setStyle(Qt::SolidPattern);
                    memdc.setPen(p);memdc.setBrush(br);
                    for(int s=-1;s<=1;s++)
                        memdc.drawEllipse(QRect(MPoint(zd.x+left+50,zd.y*SI+left-SI/3.66+s*10+60),MSize(zd.dx/3,zd.dx/3)));
                }
                else
                {
                    QPen p(zd.color);p.setStyle(Qt::SolidLine);
                    memdc.setPen(p);int x=(zd.x+left+50)*m_wnd->m_sf,y=(zd.y*SI+left-SI/3.66+60)*m_wnd->m_sf;
                    int dx=zd.dx*m_wnd->m_sf;
                    QRadialGradient jbs(x+dx*2/3,y+dx/2,dx*2/3,x+dx*3/5,y+dx/2);
                    jbs.setColorAt(0.0, Qt::white);
                    jbs.setColorAt(1.0, zd.color);
                    memdc.setBrush(QBrush(jbs));
                    memdc.drawEllipse(QRect(QPoint(x,y),QSize(dx,dx)));
                }
            }
        }
        for(int i=0;i<m_dshu;i++)
        {
            if(!de[i])
                zd[ds++]=m_zid[i];
        }
        m_dshu=ds;
        for(int i=0;i<ds;i++)m_zid[i]=zd[i];
        memdc.setPen(QColor(0,0,0,240));
        memdc.save();QFont f("默认",floor(10*m_wnd->m_sf+1e-10),QFont::Bold,false);memdc.setFont(f);
        if(m_wnd->r_gq!=61&&m_wnd->r_gq!=63)
            memdc.drawText(10,40,QString::asprintf("阳光:%d",m_yg+m_cc));
        memdc.drawText(220,40,QString::asprintf("能量豆:%d",m_wnd->m_dzs));
        memdc.restore();
        if(m_wnd->m_xx!=-1)
        {
            int x=m_wnd->m_xx,y=m_wnd->m_xy;
            QBrush br;br.setStyle(Qt::NoBrush);
            memdc.setBrush(br);
            QPen pe(QColor(0,0,0));pe.setStyle(Qt::SolidLine);
            memdc.setPen(pe);
            memdc.drawEllipse(QRect(MPoint(x+50,y*SI+60),MSize(SI,SI)));
        }
        if(m_wnd->m_nzw!=-1)
        {
            QPoint tcur=m_wnd->cursor().pos();
            tcur=m_wnd->mapFromGlobal(tcur);
            QPoint cur=m_wnd->SPoint(tcur.x(),tcur.y());
            if(cur.x()>=150&&cur.y()>=60)
            {
                int x=(cur.x()-150)/SI*SI+50+SI/6,y=(cur.y()-60)/SI*SI+60+SI/6;
                if(m_wnd->m_nzw==-2)
                    memdc.drawPixmap(QRect(MPoint(x,y),MSize(SI*2/3,SI*2/3)),m_wnd->ico[131]);
                else if(m_wnd->m_nzw==-3)
                    memdc.drawPixmap(QRect(MPoint(x,y),MSize(SI*2/3,SI*2/3)),m_wnd->ico[136]);
                else if(m_wnd->m_nzw==-4)
                    memdc.drawPixmap(QRect(MPoint(x,y),MSize(SI*2/3,SI*2/3)),m_wnd->ico[78]);
                else
                    memdc.drawPixmap(QRect(MPoint(x,y),MSize(SI*2/3,SI*2/3)),m_wnd->ico[m_wnd->m_zw[m_wnd->m_nzw].id]);
            }
        }
        for(int i=0;i<MN;i++)
        {
            if(TM-lalj[i]<=74)
            {
                for(int j=0;j<MM;j++)
                    memdc.drawPixmap(QRect(MPoint(j*SI+50,i*SI+60),MSize(SI,SI)),m_wnd->ico[127]);
            }
            for(int j=0;j<MM;j++)
            {
                if(TM-lazd[j][i]<=74)
                    memdc.drawPixmap(QRect(MPoint(j*SI+50,i*SI+60),MSize(SI,SI)),m_wnd->ico[128]);
                if(TM-lawg[j][i]<=7)
                    memdc.drawPixmap(QRect(MPoint(j*SI+50+SI/10,i*SI+60-SI*2/3+SI*2*(TM-lawg[j][i])/21+SI/10),MSize(SI*0.8,SI*0.8)),m_wnd->ico[12]);
            }
        }
        if(m_lx==1||m_lx==3)
        {
            bool notmw[MMM][MMN]={0};
            int next[8][2]={1,0,1,1,0,1,-1,1,-1,0,-1,-1,0,-1,1,-1};
            for(int n=0,i=0;n<m_zshu;i++)
            {
                if(m_zhw[i].shm==-1)
                    continue;
                n+=1;
                if(m_zhw[i].lx==11)
                {
                    for(int j=0;j<8;j++)
                    {
                        int x=m_zhw[i].x/SI+next[j][0]-330/SI;
                        int y=m_zhw[i].y+next[j][1];
                        if(!(x<0||x>=660/SI||y<0||y>=MN))
                            notmw[x][y]=true;
                    }
                    if(m_zhw[i].x/SI-330/SI>=0)
                        notmw[m_zhw[i].x/SI-330/SI][m_zhw[i].y]=true;
                }
            }
            memdc.setPen(Qt::NoPen);
            memdc.setBrush(QColor(240,240,240,250));
            for(int x=0;x<660/SI;x++)
            {
                for(int y=0;y<MN;y++)
                {
                    if(!notmw[x][y])
                        memdc.drawRect(QRect(MPoint(x*SI+380,y*SI+60),MSize(SI,SI)));
                }
            }
        }
        for(int i=0;i<m_zshu;i++)
        {
            if(m_zhw[i].lx==33)
            {
                int x1=m_zhw[i].x,y1=m_zhw[i].y;
                for(int j=i+1;j<m_zshu;j++)
                {
                    if(m_zhw[j].lx==33)
                    {
                        int x2=m_zhw[j].x,y2=m_zhw[j].y;
                        memdc.setPen(QColor(255,140,0));
                        memdc.drawLine(MPoint(x1+50+SI/2,y1*SI+60+SI/2),MPoint(x2+50+SI/2,y2*SI+60+SI/2));
                    }
                }
            }
        }
    }
}
    /*CDC *dc=m_wnd->GetDC();
    //双缓冲绘图
    CDC memdc;
    memdc.CreateCompatibleDC(NULL);
    CRect r;
    m_wnd->GetClientRect(&r);
    r.bottom = 610; r.right = 1040;
    CBitmap bit,*odb;
    bit.CreateCompatibleBitmap(dc,r.Width(),r.Height());
    odb=memdc.SelectObject(&bit);
    COLORREF color=(m_lx==0||m_lx==1||m_lx==4||m_lx==5)?QColor(240,240,240):QColor(28,28,158);
    bool bu[MMM][MMN]={0};
    ::DrawIconEx(memdc.m_hDC,0,0,m_wnd->ico[262],1040,60,0,NULL,DI_NORMAL);
    for(int i=0;i<MN;i++)
        ::DrawIconEx(memdc.m_hDC,0,i*SI+60,m_wnd->ico[261],SI,SI,0,NULL,DI_NORMAL);
    ::DrawIconEx(memdc.m_hDC,50,60,m_wnd->ico[260],990,550,0,NULL,DI_NORMAL);
    if(m_ks)
    {
        for(int x=0;x<MM;x++)
        {
            for(int y=0;y<MN;y++)
            {
                if(GetZwShu(x*SI,y)!=-1)
                    ::DrawIconEx(memdc.m_hDC,x*SI+50,y*SI+60,m_wnd->ico[263],SI,SI,0,NULL,DI_NORMAL);
            }
        }
        CPen pe(PS_SOLID,1,QColor(0,0,0)),*op=memdc.SelectObject(&pe);
        for(int x=0;x<=MN;x++)
        {
            memdc.MoveTo(50,x*SI+60);
            memdc.LineTo(MM*SI+50,x*SI+60);
        }
        for(int x=0;x<=MM;x++)
        {
            memdc.MoveTo(x*SI+50,60);
            memdc.LineTo(x*SI+50,MN*SI+60);
        }
        memdc.SelectObject(op);pe.DeleteObject();
        if(m_lx==0||m_lx==1||m_lx==4||m_lx==5)
            ::DrawIconEx(memdc.m_hDC,300+(TM*100.0)/m_wnd->m_ztm*680,0,m_wnd->ico[8],60,60,0,NULL,DI_NORMAL);
        else
            ::DrawIconEx(memdc.m_hDC,300+(TM*100.0)/m_wnd->m_ztm*680,0,m_wnd->ico[9],60,60,0,NULL,DI_NORMAL);
        for(int i=0;i<MM;i++)
        {
            for(int j=0;j<MN;j++)
            {
                if(m_keng[i][j]==0)
                    continue;
                ::DrawIconEx(memdc.m_hDC,i*SI+50,j*SI+60,m_wnd->ico[IDI_ICON58+m_keng[i][j]],SI,SI,0,NULL,DI_NORMAL);
            }
        }
        for(int i=0;i<MM;i++)
        {
            for(int j=0;j<MN;j++)
            {
                if(m_ng[i][j])
                {
                    if(m_ng[i][j]<250)
                        ::DrawIconEx(memdc.m_hDC,i*SI+50,j*SI+60,m_wnd->ico[IDI_ICON103],SI,SI,0,NULL,DI_NORMAL);
                    else
                        ::DrawIconEx(memdc.m_hDC,i*SI+50,j*SI+60,m_wnd->ico[IDI_ICON102],SI,SI,0,NULL,DI_NORMAL);
                }
                if(m_dc[i][j])
                {
                    if(m_dc[i][j]<5)
                        ::DrawIconEx(memdc.m_hDC,i*SI+50,j*SI+60,m_wnd->ico[IDI_ICON133],SI,SI,0,NULL,DI_NORMAL);
                    else
                        ::DrawIconEx(memdc.m_hDC,i*SI+50,j*SI+60,m_wnd->ico[IDI_ICON132],SI,SI,0,NULL,DI_NORMAL);
                }
            }
        }
        for(int i=0;i<m_zshu;i++)
        {
            if(m_zhw[i].shm==-1)
                continue;
            ::DrawIconEx(memdc.m_hDC,m_zhw[i].x+50,m_zhw[i].y*SI+60,m_wnd->ico[m_zhw[i].id],SI,SI,0,NULL,DI_NORMAL);
            if(m_zhw[i].mg&&(m_lx==0||m_lx==1||m_lx==4||m_lx==5))
                ::DrawIconEx(memdc.m_hDC,m_zhw[i].x+50,m_zhw[i].y*SI+60,m_wnd->ico[278],SI,SI,0,NULL,DI_NORMAL);
            if(m_zhw[i].lx==23&&m_zhw[i].nfs)
            {
                CPen p(0,1,QColor(174,0,174)),*op;
                CBrush br(QColor(174,0,174)),*ob;
                op=memdc.SelectObject(&p);
                ob=memdc.SelectObject(&br);
                for(int a=(TM*3)%20;a<=SI*4/3;a+=20)
                {
                    if(a<=SI/3)continue;
                    for(int b=-1;b<=1;b++)
                    {
                        for(int c=-1;c<=1;c++)
                        {
                            if(b==0&&c==0)continue;
                            int x=m_zhw[i].x+a*b+SI/2+50,y=m_zhw[i].y*SI+a*c+SI/2+60;
                            memdc.Ellipse(x-3,y-3,x+3,y+3);
                        }
                    }
                }
                memdc.SelectObject(op);memdc.SelectObject(ob);
                p.DeleteObject();br.DeleteObject();
            }
            else if(m_zhw[i].lx==26&&m_zhw[i].nfs)
            {
                CPen p(0,1,QColor(120,0,120)),*op;
                CBrush br(QColor(120,0,120)),*ob;
                op=memdc.SelectObject(&p);
                ob=memdc.SelectObject(&br);
                int zz=(m_zhw[i].time==100?15:60);
                for(int a=m_zhw[i].x+SI/2+(TM*7)%zz;a<=MM*SI;a+=zz)
                {
                    int x=a+50,y=m_zhw[i].y*SI+SI/2+60;
                    memdc.Ellipse(x-4,y-4,x+4,y+4);
                }
                for(int a=m_zhw[i].x+SI/2-(TM*7)%zz;a>=0;a-=zz)
                {
                    int x=a+50,y=m_zhw[i].y*SI+SI/2+60;
                    memdc.Ellipse(x-4,y-4,x+4,y+4);
                }
                memdc.SelectObject(op);memdc.SelectObject(ob);
                p.DeleteObject();br.DeleteObject();
            }
        }
        for(int i=0;i<m_jshu;i++)
        {
            if(m_jsh[i].shm==-1)
                continue;
            if(m_jsh[i].hb<100)
            {
                int x=(m_jsh[i].x+SI/4)/SI;
                if(x>=0&&x<MM)
                {
                    bu[x][m_jsh[i].y]=true;
                    if(x+1<MM)bu[x+1][m_jsh[i].y]=true;
                }
            }
            if(m_jsh[i].lx!=15)
                ::DrawIconEx(memdc.m_hDC,m_jsh[i].x+50,m_jsh[i].y*SI+60-(m_jsh[i].lx==9?m_jsh[i].fss*SI/55:0),m_wnd->ico[m_jsh[i].id],SI,SI,0,NULL,DI_NORMAL);
        }
        for(int i=0;i<m_jshu;i++)
        {
            if(m_jsh[i].lx==15)
                ::DrawIconEx(memdc.m_hDC,m_jsh[i].x+50,m_jsh[i].y*SI+60,m_wnd->ico[m_jsh[i].id],SI,SI,0,NULL,DI_NORMAL);
        }
        bool de[1010];SZid zd[1010];int ds=0;
        for(int i=0;i<m_dshu;i++)
        {
            if(m_zid[i].y==-1)
                continue;
            de[i]=false;
            SZid zd=m_zid[i];zd.x+=28;
            int left=(SI-zd.dx)/2,right=left+zd.dx;
            CPen p(0,1,zd.color),*op;
            CBrush br(zd.color),*ob;
            op=memdc.SelectObject(&p);
            ob=memdc.SelectObject(&br);
            if(zd.color==QColor(255,255,0))
            {
                if(zd.tx<MM*SI+20&&(zd.tes||inwjx(zd.zx,zd.zy*SI,zd.tx,zd.y*SI)))
                {
                    double bl=double(zd.x-zd.sx)/(zd.tx-zd.sx);
                    int hx=bl*(zd.tx-zd.zx)+zd.zx;
                    int hy=bl*(zd.y*SI-zd.zy*SI)+zd.zy*SI;
                    memdc.Ellipse(hx+left+50,hy+left+60,hx+right+50,hy+right+60);
                    m_zid[i].tes=true;
                }
                else de[i]=true;
            }
            else if(zd.color==QColor(10,170,255)||zd.color==QColor(0,200,0))
            {
                int t=zd.tx-zd.sx,md=(zd.tx+zd.sx)/2;
                int z=md-zd.sx;z*=z;int hy=zd.y*SI-z/1500;
                z=md-zd.x;z*=z;hy+=z/1500;//if(hy<-left)hy=-left;
                if(zd.x+60>=zd.tx&&zd.tx<MM*SI+20)
                {
                    for(int s=-1;s<=1;s++)
                        memdc.Ellipse(zd.x+left+50,hy+left-SI/3.66+s*10+60,zd.x+left+zd.dx/3+50,hy+s*10+left+zd.dx/3-SI/3.66+60);
                }
                else memdc.Ellipse(zd.x+left+50,hy+left-SI/3.66+60,zd.x+right+50,hy+right-SI/3.66+60);
            }
            else if(zd.color==QColor(255,0,255))
            {
                if(zd.tx-zd.zx>440)
                    de[i]=true;
                else
                    memdc.Ellipse(zd.x+left+50,zd.y*SI+left+60,zd.x+right+50,zd.y*SI+right+60);
            }
            else
            {
                if(zd.x+60>=zd.tx&&zd.tx<MM*SI+20+SI/2)
                {
                    for(int s=-1;s<=1;s++)
                        memdc.Ellipse(zd.x+left+50,zd.y*SI+left-SI/3.66+s*10+60,zd.x+left+zd.dx/3+50,zd.y*SI+s*10+left+zd.dx/3-SI/3.66+60);
                }
                else memdc.Ellipse(zd.x+left+50,zd.y*SI+left-SI/3.66+60,zd.x+right+50,zd.y*SI+right-SI/3.66+60);
            }
            memdc.SelectObject(op);memdc.SelectObject(ob);
            p.DeleteObject();br.DeleteObject();
        }
        for(int i=0;i<m_dshu;i++)
        {
            if(!de[i])
                zd[ds++]=m_zid[i];
        }
        m_dshu=ds;
        for(int i=0;i<ds;i++)m_zid[i]=zd[i];
        for(int n=0;n<MN;n++)
        {
            if(m_car[n])
                memdc.DrawIcon(0,n*SI+(SI/2.82)+60,m_wnd->ico[0]);
        }
        if(m_lx==1||m_lx==3)
        {
            bool notmw[MMM][MMN]={0};
            int next[8][2]={1,0,1,1,0,1,-1,1,-1,0,-1,-1,0,-1,1,-1};
            for(int n=0,i=0;n<m_zshu;i++)
            {
                if(m_zhw[i].shm==-1)
                    continue;
                n+=1;
                if(m_zhw[i].lx==11)
                {
                    for(int j=0;j<8;j++)
                    {
                        int x=m_zhw[i].x/SI+next[j][0]-330/SI;
                        int y=m_zhw[i].y+next[j][1];
                        if(!(x<0||x>=660/SI||y<0||y>=MN))
                            notmw[x][y]=true;
                    }
                    if(m_zhw[i].x/SI-330/SI>=0)
                        notmw[m_zhw[i].x/SI-330/SI][m_zhw[i].y]=true;
                }
            }
            for(int x=0;x<660/SI;x++)
            {
                for(int y=0;y<MN;y++)
                {
                    if(!notmw[x][y])
                        ::DrawIconEx(memdc.m_hDC,x*SI+380,y*SI+60,m_wnd->ico[213],SI,SI,0,NULL,DI_NORMAL);
                }
            }
        }
        for(int i=0;i<MN;i++)
        {
            if(TM-lalj[i]<=74)
            {
                for(int j=0;j<MM;j++)
                    ::DrawIconEx(memdc.m_hDC,j*SI+50,i*SI+60,m_wnd->ico[276],SI,SI,0,NULL,DI_NORMAL);
            }
            for(int j=0;j<MM;j++)
            {
                if(TM-lazd[j][i]<=74)
                    ::DrawIconEx(memdc.m_hDC,j*SI+50,i*SI+60,m_wnd->ico[277],SI,SI,0,NULL,DI_NORMAL);
                if(TM-lawg[j][i]<=7)
                    ::DrawIconEx(memdc.m_hDC,j*SI+50+SI/10,i*SI+60-SI*2/3+SI*2*(TM-lawg[j][i])/21+SI/10,m_wnd->ico[IDI_ICON12],SI*0.8,SI*0.8,0,NULL,DI_NORMAL);
            }
        }
        CPoint cur;
        GetCursorPos(&cur);
        m_wnd->ScreenToClient(&cur);
        if(m_wnd->m_nzw!=-1)
        {
            HICON ic;
            if(m_wnd->m_nzw==-2)
                ::DrawIconEx(memdc.m_hDC,(cur.x-50)/SI*SI+50+SI/6,(cur.y-60)/SI*SI+60+SI/6,ic=HICON(LoadImage(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(IDI_ICON131),IMAGE_ICON,SI*2/3,SI*2/3,LR_DEFAULTCOLOR)),SI*2/3,SI*2/3,0,NULL,DI_NORMAL);
            else if(m_wnd->m_nzw==-3)
                ::DrawIconEx(memdc.m_hDC,(cur.x-50)/SI*SI+50+SI/6,(cur.y-60)/SI*SI+60+SI/6,ic=HICON(LoadImage(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(IDI_ICON136),IMAGE_ICON,SI*2/3,SI*2/3,LR_DEFAULTCOLOR)),SI*2/3,SI*2/3,0,NULL,DI_NORMAL);
            else
                ::DrawIconEx(memdc.m_hDC,(cur.x-50)/SI*SI+50+SI/6,(cur.y-60)/SI*SI+60+SI/6,ic=HICON(LoadImage(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(m_wnd->m_zw[m_wnd->m_nzw].id),IMAGE_ICON,SI*2/3,SI*2/3,LR_DEFAULTCOLOR)),SI*2/3,SI*2/3,0,NULL,DI_NORMAL);
            ::DestroyIcon(ic);
        }
        CFont *f=new CFont(),*of;//CBrush br,*ob;
        f->CreatePointFont(200,"黑体");//br.CreateStockObject(NULL_BRUSH);
        of=memdc.SelectObject(f);//ob=memdc.SelectObject(&br);
        if(!m_wnd->m_b11||m_wnd->m_b12)
        {
            CString str;
            str.Format("阳光:%d",m_yg+m_cc);
            memdc.SetBkMode(TRANSPARENT);
            if(m_lx==2||m_lx==3)
                memdc.SetTextColor(QColor(20,255,0));
            else
                memdc.SetTextColor(QColor(255,50,50));
            memdc.TextOut(27,15,str);
            str.Format("能量豆:%d",m_wnd->m_dzs);
            memdc.TextOut(170,15,str);
        }
        memdc.SelectObject(of);f->DeleteObject();delete f;
        //while(1)
        //{
            drawblue(memdc.m_hDC,bu);
        //	_CrtDumpMemoryLeaks();
        //}
        if(m_wnd->m_xx!=-1)
        {
            int x=m_wnd->m_xx,y=m_wnd->m_xy;
            CBrush br,*ob;br.CreateStockObject(NULL_BRUSH);
            CPen pe(PS_SOLID,3,QColor(50,50,50)),*op;
            ob=memdc.SelectObject(&br);op=memdc.SelectObject(&pe);
            memdc.Ellipse(x+50,y*SI+60,x+SI+50,y*SI+SI+60);
            memdc.SelectObject(ob);memdc.SelectObject(op);
            br.DeleteObject();pe.DeleteObject();
        }
        for(int i=0;i<MM;i++)
        {
            for(int j=0;j<MN;j++)
            {
                if(m_ng[i][j])
                {
                    if(m_ng[i][j]<250)
                        ::DrawIconEx(memdc.m_hDC,i*SI+50,j*SI+60,m_wnd->ico[IDI_ICON103],SI,SI,0,NULL,DI_NORMAL);
                    else
                        ::DrawIconEx(memdc.m_hDC,i*SI+50,j*SI+60,m_wnd->ico[IDI_ICON102],SI,SI,0,NULL,DI_NORMAL);
                }
            }
        }
    }
    dc->BitBlt(0,0,r.Width(),r.Height(),&memdc,0,0,SRCCOPY);
    memdc.SelectObject(odb);bit.DeleteObject();
    memdc.DeleteDC();m_wnd->ReleaseDC(dc);
}*/

bool CGame::NotZw(int shu)//特殊植物
{
    int gj=-m_zhw[shu].goj;bool rt=false;
    int n,cs[MAXSHU]={0},cf[MAXSHU],fs=0,kx[8],ky[8],ks=0;
    int next[8][2]={SI,0,SI,1,0,1,-SI,1,-SI,0,-SI,-1,0,-1,SI,-1};
    switch(gj)
    {
    case 1://向日葵
        m_yg+=25;
        break;
    case 2://炸弹
        lazd[m_zhw[shu].x/SI][m_zhw[shu].y]=TM;
        for(int s=-1;s<=1;s++)
            KillJs(m_zhw[shu].x-1.682*SI,m_zhw[shu].x+1.682*SI,m_zhw[shu].y+s,100);
        DestroyZw(shu);
        break;
    case 3://火爆辣椒
        lalj[m_zhw[shu].y]=TM;
        KillJsLine(m_zhw[shu].y,90);
        DestroyZw(shu);rt=true;
        break;
    case 4://倭瓜
        {
            bool rtn=false;
            if(KillJs(m_zhw[shu].x,m_zhw[shu].y,80))rtn=true,lawg[m_zhw[shu].x/SI][m_zhw[shu].y]=TM;
            if(!rtn&&KillJs(m_zhw[shu].x-SI,m_zhw[shu].y,80))
            {
                rtn=true;
                if(m_zhw[shu].x)lawg[m_zhw[shu].x/SI-1][m_zhw[shu].y]=TM;
            }
            if(!rtn&&KillJs(m_zhw[shu].x+SI,m_zhw[shu].y,80))
            {
                rtn=true;
                if(m_zhw[shu].x/SI+1<MM)lawg[m_zhw[shu].x/SI+1][m_zhw[shu].y]=TM;
            }
            if(rtn)
                DestroyZw(shu);
            break;
        }
    case 5://阳光菇
        if(m_zhw[shu].fu<10)
            m_yg+=15;
        else
            m_yg+=25;
        m_zhw[shu].fu+=1;//生长
        if(m_zhw[shu].fu>10)
            m_zhw[shu].fu=10;
        m_zhw[shu].id=GetZwIcon(m_zhw[shu]);
        break;
    case 6://毁灭菇
        for(int s=-2;s<=2;s++)
            KillJs(m_zhw[shu].x-2.682*SI,m_zhw[shu].x+2.682*SI,m_zhw[shu].y+s,100);
        DestroyZw(shu);rt=true;
        m_keng[m_zhw[shu].x/SI][m_zhw[shu].y]=1;//坑
        break;
    case 7://寒冰菇
        m_hb=true;
        for(int i=0;i<m_jshu;i++)
            m_jsh[i].hb=0;
        m_wnd->KillTimer(1006);
        m_wnd->SetTimer(1006,6000,NULL);
        DestroyZw(shu);rt=true;
        break;
    case 8://全息坚果
        m_zhw[shu].shm+=(m_wnd->m_hd?10:15);
        if(m_zhw[shu].shm>800*(m_wnd->m_ea>=2?2:1))
            m_zhw[shu].shm=800*(m_wnd->m_ea>=2?2:1);
        m_zhw[shu].id=GetZwIcon(m_zhw[shu]);
        break;
    case 9://双子
        m_yg+=25;
        break;
    case 10://医疗
        for(int a=-1;a<=1;a++)
        {
            for(int b=-1;b<=1;b++)
            {
                int z=GetZwShu(m_zhw[shu].x+a*SI,m_zhw[shu].y+b);
                if(z!=-1)
                {
                    if(m_zhw[z].lx==25||m_zhw[z].lx==32)continue;
                    bool bk=false;
                    m_yg-=(m_wnd->m_hd?4:3);
                    if(m_yg<0)
                    {
                        m_yg=0;
                        goto ov;
                    }
                    if(m_zhw[z].shm<=150)
                    {
                        if(m_zhw[z].shm<150)bk=true;
                        m_zhw[z].shm+=(m_wnd->m_ea>1?4:3);
                        if(m_zhw[z].shm>150)
                            m_zhw[z].shm=150;
                    }
                    else
                    {
                        if(m_zhw[z].shm<1800*(m_wnd->m_ea>=2?2:1))bk=true;
                        m_zhw[z].shm+=(m_wnd->m_hd?15:18)+2*m_wnd->m_ea-2;
                        if(m_zhw[z].shm>1800*(m_wnd->m_ea>=2?2:1))
                            m_zhw[z].shm=1800*(m_wnd->m_ea>=2?2:1);
                    }
                    int tx=(m_zhw[shu].x+a*SI)/SI,ty=m_zhw[shu].y+b;
                    if(m_ng[tx][ty])
                    {
                        if(m_ng[tx][ty]<500)
                            bk=true;
                        m_ng[tx][ty]+=10;
                        if(m_ng[tx][ty]>500)
                            m_ng[tx][ty]=500;
                    }
                    if(!bk)m_yg+=3;
                    m_zhw[z].id=GetZwIcon(m_zhw[z]);
                }
            }
        }
ov:
        break;
    case 11:
        m_zhw[shu].nfs=0;
        for(int s=-1;s<=1;s++)
        {
            if(KillJs(m_zhw[shu].x-1.682*SI,m_zhw[shu].x+1.682*SI,m_zhw[shu].y+s,3))
                m_zhw[shu].nfs=1;
        }
        break;
    case 12:
        m_zhw[shu].nfs=KillJsLine(m_zhw[shu].y,1);
        break;
    case 13:
        for(int a=-1;a<=1;a++)
        {
            for(int b=-1;b<=1;b++)
            {
                if(a==0&&b==0)
                    continue;
                int z=GetZwShu(m_zhw[shu].x+a*SI,m_zhw[shu].y+b);
                if(z==-1)
                {
                    if(m_zhw[shu].x+a*SI>=0&&m_zhw[shu].x+a*SI<=(MM-1)*SI&&m_zhw[shu].y+b>=0&&m_zhw[shu].y+b<MN)
                    {
                        kx[ks]=m_zhw[shu].x+a*SI;
                        ky[ks++]=m_zhw[shu].y+b;
                    }
                    continue;
                }
                cs[m_zhw[z].lx]+=1;
            }
        }
        if(ks)
        {
            for(int i=0;i<MAXSHU;i++)
            {
                if(cs[i]>=2)
                    cf[fs++]=i;
            }
            if(fs)
            {
                int z=cf[rand()%fs],t;
                for(t=0;t<MAXSHU;t++)
                {
                    if(m_wnd->m_zw[t].lx==z)
                        break;
                }
                int nz=rand()%ks;SZhw zw=m_wnd->m_zw[t];
                zw.x=kx[nz];zw.y=ky[nz];
                m_yg+=zw.yg;NewZw(zw);
            }
        }
        break;
    case 14:
        {
            int t=GetZwShu(m_zhw[shu].x-SI,m_zhw[shu].y);
            if(t!=-1&&m_zhw[t].lx==31)
                m_zhw[shu].nfs+=1;
            if(m_zhw[shu].nfs>10)m_zhw[shu].nfs=10;
        }
        break;
    }
    return rt;
}

bool CGame::KillJs(int x, int y, int kill)
{
    return KillJs(x-SI*0.682,x+SI*0.682,y,kill);
}

bool CGame::KillJs(int l, int r,int y, int kill)//杀僵尸
{
    if(y<0||y>=MN)
        return false;
    bool rtn=false;
    for(int i=0;i<m_jshu;i++)
    {
        if(m_jsh[i].shm<0)
            continue;
        if(m_jsh[i].y==y&&m_jsh[i].x>=l&&m_jsh[i].x<=r&&m_jsh[i].lx<100)
        {
            rtn=true;
            if(kill>0)
                m_jsh[i].shm-=kill;
            else
            {
                m_jsh[i].shm+=kill;
                m_jsh[i].hb=0;
            }
            if(m_jsh[i].shm<=0)
            {
                DestroyJs(i);
                if(!SZZ)i-=1;
            }
            else
                 m_jsh[i].id=GetJsIcon(m_jsh[i],*this);
        }
    }
    return rtn;
}

bool CGame::KillJsLine(int y,int kill)//杀行
{
    bool rt=false;
    for(int i=0;i<m_jshu;i++)
    {
        if(m_jsh[i].shm==-1||m_jsh[i].lx==11||m_jsh[i].lx>=100)
            continue;
        if(m_jsh[i].y==y)
        {
            m_jsh[i].shm-=kill;rt=true;
            if(kill==90)m_jsh[i].hb=100;
            if(m_jsh[i].shm<=0)
            {
                DestroyJs(i);
                i-=1;
            }
            else
                m_jsh[i].id=GetJsIcon(m_jsh[i],*this);
        }
    }
    if(kill==90)
    {
        for(int x=0;x<MM;x++)
            m_ice[x][y]=-1e8;
    }
    return rt;
}

void CGame::KillAll()
{
    while(m_zshu>0)
        DestroyZw(0);
    while(m_jshu>0)
        DestroyJs(0);
    while(m_dshu>0)
        DestroyZd(0);
}

void CGame::CreateGame(int lx)
{
    for(int x=0;x<MM*SI+100;x++)
    {
        for(int y=0;y<MN;y++)
            fr[x][y]=bh[x][y]=-1;
    }
    for(int i=0;i<MN;i++)hjs[i]=lst[i]=0;
    zk=false;XS=0;t_ai=m_cc=0;
    m_jshu=m_zshu=m_dshu=0;
    m_yg=50;m_over=false;
    for(int i=0;i<MN;i++)
    {
        m_car[i]=true;
        m_cing[i]=false;
    }
    for(int i=0;i<MM;i++)
    {
        for(int j=0;j<MN;j++)
        {
            m_keng[i][j]=m_ng[i][j]=0;
            m_dc[i][j]=0;m_ice[i][j]=-1e8;
        }
    }
    for(int i=0;i<MN;i++)
    {
        lalj[i]=-1e9;
        for(int j=0;j<MM;j++)
            lazd[j][i]=lawg[j][i]=-1e9;
    }
    if(lx==4)
    {
        for(int i=0;i<MM;i++)
        {
            for(int j=110/SI;j<440/SI;j++)
                m_keng[i][j]=2;
        }
    }
    m_ks=true;
    m_hb=false;
    m_lx=lx;
}
int CGame::Find(int zz[20],int n,int hg)//找到最优的位置
{
    for(int i=0;i<n;i++)
    {
        if(zz[i]==-1)
            return i;
    }
    int zx=1e9,wz=-1;
    for(int i=0;i<n;i++)
    {
        int t=m_zhw[zz[i]].yg;
        if(hg>=10&&(m_zhw[zz[i]].lx==0||m_zhw[zz[i]].lx==20)&&XS<=(22+(m_wnd->m_gq==60?12:0))*pow(110.0/SI,1.4)&&rand()%(XS<=16*pow(110.0/SI,1.4)?24:25)<24)//要保留一些葵
            t=500;
        else if(m_zhw[zz[i]].lx==6&&m_zhw[zz[i]].shm<200)
            t=25;
        if(t<zx)
            zx=t,wz=i;
    }
    return wz;
}

int CGame::Find(int zz[20],int n)//找到最优的位置
{
    for(int i=0;i<n;i++)
    {
        if(zz[i]==-1)
            return i;
    }
    int zx=1e9,wz=-1;
    for(int i=0;i<n;i++)
    {
        int t=m_zhw[zz[i]].yg;
        if((m_zhw[zz[i]].lx==0||m_zhw[zz[i]].lx==20)&&XS<=(22+(m_wnd->m_gq==60?12:0))*pow(110.0/SI,1.4)&&rand()%(XS<=16*pow(110.0/SI,1.4)?24:25)<24)//要保留一些葵
            t=500;
        else if(m_zhw[zz[i]].lx==6&&m_zhw[zz[i]].shm<200)
            t=25;
        if(t<zx)
            zx=t,wz=i;
    }
    return wz;
}

int CGame::Valid(int x,int sy=0)//植物可行
{
    int xu[12]={0,10,7,8,9,11,3,5,12,22,25},z=xu[x];
    if(x<XUAN&&m_wnd->m_en[z]>=800&&m_wnd->m_zw[z].yg+sy<=m_yg)
        return z;
    return -1;
}

int CGame::FakeKill(int xl, int xr, int y, int kill)
{
    if(y<0||y>=5)return 0;
    int jg=0,tx=770;
    if(GetZwShu(tx,y)==-1)tx=660;
    for(int i=0;i<m_jshu;i++)
    {
        if(m_jsh[i].shm<0||m_jsh[i].lx>=100||(m_jsh[i].lx<=5&&m_wnd->jd>100)||((m_jsh[i].eat>=200||(m_jsh[i].eat<=2&&m_jsh[i].lx!=16))&&m_jsh[i].x>tx+90)||(m_jsh[i].eat==40&&m_jsh[i].x>tx+100))
            continue;
        if(m_jsh[i].y==y&&m_jsh[i].x>=xl&&m_jsh[i].x<=xr&&m_jsh[i].lx<100)
        {
            int ad=m_jsh[i].shm;
            if(m_jsh[i].eat==1&&m_jsh[i].x>704)ad=ad*2/3;
            if(m_jsh[i].shm>150&&m_jsh[i].x>641)ad-=60;//不要立刻炸死巨人
            if(m_jsh[i].lx==14||m_jsh[i].lx==15)ad+=25;
            if(m_jsh[i].lx==15)ad+=50;
            if(m_jsh[i].lx==13)ad+=10;
            if(m_jsh[i].lx==16)ad+=70;
            if(m_jsh[i].eat>=4)ad+=20;
            if(m_jsh[i].eat>4)ad+=20;
            jg+=ad;
        }
    }
    return jg;
}

bool CGame::NearGaint(int x,int y)
{
    for(int i=0;i<m_jshu;i++)
    {
        if(m_jsh[i].lx==9&&m_jsh[i].y==y&&m_jsh[i].x>=x-30&&m_jsh[i].x<=x+(m_jsh[i].shm<=50?140:200))//不能在巨人的面前种植
            return true;
    }
    return false;
}

bool CGame::UnderGaint(int x,int y)
{
    for(int i=0;i<m_jshu;i++)
    {
        if(m_jsh[i].lx==9&&m_jsh[i].y==y&&m_jsh[i].x>=x-30)//不能在巨人的面前种植
            return true;
    }
    return false;
}

bool CGame::NearCar(int x,int y)
{
    //return false;
    for(int i=0;i<m_jshu;i++)
    {
        if(m_jsh[i].eat==40&&m_jsh[i].y==y&&m_jsh[i].x>=x-30&&m_jsh[i].x<=x+450)//不能在车的面前种植
            return true;
    }
    return false;
}

bool CGame::UnderCar(int x,int y)
{
    //return false;
    for(int i=0;i<m_jshu;i++)
    {
        if(m_jsh[i].eat==40&&m_jsh[i].y==y&&m_jsh[i].x>=x-30)//不能在车的面前种植
            return true;
    }
    return false;
}

bool CGame::BeforeShe(int x,int y)
{
    int mi=10000;
    for(int i=0;i<m_jshu;i++)
    {
        if((m_jsh[i].lx==8||m_jsh[i].lx==10)&&m_jsh[i].y==y&&m_jsh[i].x>=x-10&&m_jsh[i].x<mi)//不能在射手的面前种植
            mi=m_jsh[i].x;
    }
    if(mi==10000)return false;
    for(int i=x;i<=mi;i+=SI)
    {
        int z=GetZwShu(i,y);
        if(z!=-1&&m_zhw[z].shm>=500)//有坚果挡着
            return false;
    }
    return true;
}

int CGame::FakeKillL(int y,int kill)//计算一行的价值
{
    int jg=0;
    for(int i=0;i<m_jshu;i++)
    {
        if(m_jsh[i].y==y&&m_jsh[i].lx<100&&(m_jsh[i].lx>5||m_wnd->jd<=100)&&(m_jsh[i].eat<40||m_jsh[i].x<750))
        {
            int ad=m_jsh[i].shm;
            if(m_jsh[i].eat==1&&m_jsh[i].x>704)ad=ad*3/4;
            if(m_jsh[i].shm>150&&m_jsh[i].x>641)ad-=60;//不要立刻炸死巨人
            if(m_jsh[i].lx==14||m_jsh[i].lx==15)ad+=25;
            if(m_jsh[i].lx==15)ad+=50;
            if(m_jsh[i].lx==13)ad+=10;
            if(m_jsh[i].lx==16)ad+=80;
            if(m_jsh[i].eat>=4)ad+=15;
            if(m_jsh[i].eat>4)ad+=20;
            jg+=ad;
        }
    }
    return jg;
}

bool CGame::UnderMonth(int x,int y)
{
    for(int i=0;i<m_jshu;i++)
    {
        if(m_jsh[i].y==y&&m_jsh[i].x<=x+(90+(m_jsh[i].lx>=100?30:0))&&m_jsh[i].x>=x)
            return true;
    }
    return false;
}

void CGame::AI()
{
    t_ai+=1;
    if(m_yg<300)//存储阳光（已废弃）
    {
        m_yg+=m_cc;
        m_cc=0;
    }
    int ma=1e9,zy=-1,zx;bool ff=false,tt=false;
    for(int i=0;i<m_jshu;i++)//判断危机情况
    {
        if(m_jsh[i].lx==11||m_jsh[i].lx>=100||m_jsh[i].eat==0||m_jsh[i].lx==16||m_jsh[i].sd==0)continue;//不对子弹，苦力怕，小偷进行处理
        int x=((m_jsh[i].x+30)/SI)*SI,y=m_jsh[i].y,z;
        int he=0;bool ji=false;
        for(int j=x,tm=0;j>=0;j-=SI)
        {
            int z=GetZwShu(j,y);
            if(z!=-1)
            {
                if(m_zhw[z].lx==6||m_jsh[i].lx!=8)//不会跳过
                    tm+=(m_zhw[z].shm/m_jsh[i].eat)*(m_wnd->m_gq==60?34:40);
                if(m_zhw[z].lx==4||m_zhw[z].lx==8||m_zhw[z].lx==10)
                    he+=(tm/m_zhw[z].time)*m_zhw[z].goj;
                if(m_zhw[z].shm>=500)ji=true;//扫描到坚果
            }
            if(m_jsh[i].lx!=8)//特判跳跳
                tm+=SI/m_jsh[i].sd*40;
            else
                tm-=40;
        }
        int t=x/m_jsh[i].sd;//进家的时间
        if(m_jsh[i].fs)t-=100;
        if(!m_car[y])t-=200;
        if(he<=m_jsh[i].shm&&(t<ma||(t==ma&&m_jsh[i].shm>zx)))
            ma=t,zy=y,zx=x,ff=(m_jsh[i].fs&&m_jsh[i].lx!=16),tt=(m_jsh[i].lx==8);
        if(!ji&&m_jsh[i].fs&&m_jsh[i].lx!=16)//种植坚果挡住子弹
        {
            bool zd=false;
            for(int tx=x-SI;tx>=0;tx-=SI)
            {
                if(GetZwShu(tx,y)!=-1)
                    break;
                if(!NearGaint(tx,y)&&!NearCar(tx,y))//找到第一个不在巨人脚下的位置
                {
                    if((z=Valid(2,0))!=-1)
                    {
                        m_wnd->m_nzw=z;
                        int r=GetZwShu(tx,y);
                        if(r==-1||m_zhw[r].lx!=6)
                        {
                            DestroyZw(tx,y);zd=true;
                            m_wnd->Plant(tx,y);
                        }
                    }
                    else if((z=Valid(1,0))!=-1)
                    {
                        m_wnd->m_nzw=z;
                        int r=GetZwShu(tx,y);
                        if(r==-1||m_zhw[r].lx!=6)
                        {
                            DestroyZw(tx,y);zd=true;
                            m_wnd->Plant(tx,y);
                        }
                    }
                    break;
                }
            }
            if(!zd&&m_wnd->m_en[7]<700&&m_wnd->m_en[10]<700&&(z=Valid(10,0))!=-1)
            {
                int wx=-1;
                for(int tx=x-SI;tx>=0;tx-=SI)
                {
                    if(GetZwShu(tx,y)!=-1)
                        break;
                    wx=tx;
                }
                if(wx>=0&&x-wx>=110&&!NearGaint(wx,y)&&!NearCar(wx,y))
                {
                    m_wnd->m_nzw=z;
                    m_wnd->Plant(wx,y);
                }
            }
        }
    }
    if(zy!=-1)//出现紧急情况
    {
        int zz[20],ss=0;
        for(int j=0;j<zx;j+=SI,ss++)
            zz[j/SI]=GetZwShu(j,zy);
        int rt=Find(zz,ss),z;//找到最优位置
        if(rt!=-1)
        {
            bool un=UnderCar(rt*SI,zy)||UnderGaint(rt*SI,zy)||BeforeShe(rt*SI,zy);//不能在巨人和车的下面种植
            if((z=Valid(3,0))!=-1)//窝瓜
            {
                int zz[3]={GetZwShu(zx+SI,zy),GetZwShu(zx,zy),GetZwShu(zx-SI,zy)};
                int r=Find(zz,3);//找到最优位置
                m_wnd->m_nzw=z;DestroyZw(zx+SI-r*SI,zy);
                m_wnd->Plant(zx+SI-r*SI,zy);
            }
            else if((z=Valid(4,0))!=-1&&!un)//西瓜
            {
                m_wnd->m_nzw=z;DestroyZw(rt*SI,zy);
                m_wnd->Plant(rt*SI,zy);
            }
            else if((z=Valid(5,0))!=-1&&!un)
            {
                m_wnd->m_nzw=z;DestroyZw(rt*SI,zy);
                m_wnd->Plant(rt*SI,zy);
            }
            else if(zx<=(220+(tt?SI:0))&&(z=Valid(7,0))!=-1)//马上进家时可以炸死
            {
                m_wnd->m_nzw=z;DestroyZw(zx,zy);
                m_wnd->Plant(zx,zy);
            }
            else if(zx<=(220+(tt?SI:0))&&(z=Valid(6,0))!=-1)//马上进家时可以炸死
            {
                m_wnd->m_nzw=z;DestroyZw(zx,zy);
                m_wnd->Plant(zx,zy);
            }
            else if(tt&&zx>=SI&&zx<=330&&(z=Valid(2,0))!=-1)
            {
                m_wnd->m_nzw=z;DestroyZw(zx-SI,zy);
                m_wnd->Plant(zx-SI,zy);
            }
        }
    }
    int jian[MMN]={0};
    if(m_wnd->jd>5&&Valid(2,25)!=-1)//种植高坚果
    {
        for(int i=0;i<m_zshu;i++)
        {
            if(m_zhw[i].lx==6&&m_zhw[i].shm>200&&m_zhw[i].x>=550)
                jian[m_zhw[i].y]+=1;
        }
        zy=ma=-1;
        for(int y=0;y<MN;y++)
        {
            if(jian[y])continue;
            int sh=0;
            for(int i=0;i<m_jshu;i++)
            {
                if(m_jsh[i].lx<100&&m_jsh[i].y==y)
                    sh+=m_jsh[i].shm+int(m_jsh[i].fs)*40;
            }
            if(SI==110)
            {
                int tx[4]={660,550,770,880},zz[20];
                for(int s=0;s<4;s++)zz[s]=GetZwShu(tx[s],zy);
                int rt=Find(zz,4);
                if(rt!=-1&&!UnderGaint(tx[rt],zy)&&sh>ma)
                    ma=sh,zy=y;
            }
            else if(SI==55)
            {
                int tx[8]={660,605,550,495,715,770,880,825},zz[20];
                for(int s=0;s<8;s++)zz[s]=GetZwShu(tx[s],zy);
                int rt=Find(zz,8);
                if(rt!=-1&&!UnderGaint(tx[rt],zy)&&sh>ma)
                    ma=sh,zy=y;
            }
        }
        if(zy!=-1)
        {
            if(SI==110)
            {
                int tx[4]={660,550,770,880},zz[20];
                for(int s=0;s<4;s++)zz[s]=GetZwShu(tx[s],zy);
                int rt=Find(zz,4);
                if(!UnderGaint(tx[rt],zy)&&(!UnderCar(tx[rt],zy)||rand()%60==0))
                {
                    m_wnd->m_nzw=7;DestroyZw(tx[rt],zy);
                    m_wnd->Plant(tx[rt],zy);jian[zy]+=1;
                }
            }
            else if(SI==55)
            {
                int tx[8]={660,605,550,495,715,770,880,825},zz[20];
                for(int s=0;s<8;s++)zz[s]=GetZwShu(tx[s],zy);
                int rt=Find(zz,8);
                if(!UnderGaint(tx[rt],zy)&&(!UnderCar(tx[rt],zy)||rand()%60==0))
                {
                    m_wnd->m_nzw=7;DestroyZw(tx[rt],zy);
                    m_wnd->Plant(tx[rt],zy);jian[zy]+=1;
                }
            }
        }
        else if(rand()%(b64?10:(ma>80?20:60))==0)//有一定概率种植两排
        {
            zy=ma=-1;
            for(int y=0;y<MN;y++)
            {
                if(jian[y]>=2)continue;
                int sh=0;
                for(int i=0;i<m_jshu;i++)
                {
                    if(m_jsh[i].lx<100&&m_jsh[i].y==y)
                        sh+=m_jsh[i].shm+int(m_jsh[i].fs)*40;
                }
                if(sh>ma)ma=sh,zy=y;
            }
            if(zy!=-1)
            {
                if(SI==110)
                {
                    int tx[4]={660,550,770,880},zz[20];
                    for(int s=0;s<4;s++)zz[s]=GetZwShu(tx[s],zy);
                    int rt=Find(zz,4);
                    if(rt!=-1&&!UnderGaint(tx[rt],zy)&&(!UnderCar(tx[rt],zy)||rand()%60==0))
                    {
                        m_wnd->m_nzw=7;DestroyZw(tx[rt],zy);
                        m_wnd->Plant(tx[rt],zy);jian[zy]+=1;
                    }
                }
                else if(SI==55)
                {
                    int tx[8]={660,605,550,495,715,770,880,825},zz[20];
                    for(int s=0;s<8;s++)zz[s]=GetZwShu(tx[s],zy);
                    int rt=Find(zz,8);
                    if(rt!=-1&&!UnderGaint(tx[rt],zy)&&(!UnderCar(tx[rt],zy)||rand()%60==0))
                    {
                        m_wnd->m_nzw=7;DestroyZw(tx[rt],zy);
                        m_wnd->Plant(tx[rt],zy);jian[zy]+=1;
                    }
                }
            }
        }
    }
    int xs=0,s1=0;
    for(int i=0;i<m_zshu;i++)
    {
        if(m_zhw[i].lx==1)
            xs+=1,s1+=1;
        if(m_zhw[i].lx==20)
            xs+=2;
    }
    int hg[MMN]={0},hj[MMN]={0},HG=0;bool han[MMN]={0};
    for(int i=0;i<m_zshu;i++)
    {
        if(m_zhw[i].lx==4||m_zhw[i].lx==8||m_zhw[i].lx==10)
        {
            hg[m_zhw[i].y]+=m_zhw[i].goj;
            if(m_zhw[i].hb)
                hg[m_zhw[i].y]+=1,han[m_zhw[i].y]=true;
        }
    }
    for(int i=0;i<MN;i++)HG+=hg[i];
    int mx=24+(m_wnd->m_gq==60?18:0);
    if(m_wnd->jd<=10)mx=40;
    if(m_jshu>=10)mx=15+(m_wnd->m_gq==60?10:0);
    if(m_jshu>=15)mx=10+(m_wnd->m_gq==60?8:0);//僵尸太多就不要着急种葵了
    if(m_jshu>=20)mx=0;
    mx=mx*pow(110.0/SI,1.4);
    if(xs<=mx&&Valid(8,25)!=-1)
    {
        bool zd=false;
        bool yj[MMN]={0};
        if(m_wnd->jd>50&&(m_jshu>5||Valid(2,200)==-1))//有僵尸就需要坚果
        {
            for(int y=0;y<MN;y++)
                yj[y]=(jian[y]==0);
        }
        if(m_wnd->jd>250)
        {
            for(int y=0;y<MN;y++)
                yj[y]=(yj[y]||(hg[y]<5&&rand()%20==0));
        }
        for(int x=220;x<=550;x+=SI)
        {
            for(int y=0;y<MN;y++)
            {
                if(yj[y])continue;//后面没有坚果就不种植
                int r=GetZwShu(x,y);
                if(r!=-1&&m_zhw[r].lx==6)yj[y]=true;
                if(r==-1&&!UnderGaint(x,y)&&!UnderCar(x,y)&&!BeforeShe(x,y)&&!UnderMonth(x,y))
                {
                    m_wnd->m_nzw=12;m_wnd->Plant(x,y);
                    xs+=2;zd=true;break;
                }
            }
            if(zd)break;
        }
    }
    if(xs<=mx&&(m_zshu==0||(Valid(0,50)!=-1&&(rand()%100==0||s1<5))))//种植向日葵
    {
        bool zd=false;
        bool yj[MMN]={0};
        if(m_wnd->jd>50&&(m_jshu>5||Valid(2,150)==-1))//有僵尸就需要坚果
        {
            for(int y=0;y<MN;y++)
                yj[y]=(jian[y]==0);
        }
        if(m_wnd->jd>250)
        {
            for(int y=0;y<MN;y++)
                yj[y]=(yj[y]||(hg[y]<4&&rand()%20==0));
        }
        for(int x=0;x<=550;x+=SI)
        {
            for(int y=0;y<MN;y++)
            {
                if(yj[y])continue;//后面没有坚果就不种植
                int r=GetZwShu(x,y);
                if(r!=-1&&m_zhw[r].lx==6)yj[y]=true;
                if(r==-1)
                {
                    if(!UnderGaint(x,y)&&!UnderCar(x,y)&&!BeforeShe(x,y)&&!UnderMonth(x,y))//不能在巨人的面前种植
                    {
                        m_wnd->m_nzw=0;m_wnd->Plant(x,y);
                        if(m_wnd->m_dzs>0&&!zk)
                        {
                            DaZhao(m_zhw[m_zshu-1],m_wnd);
                            m_wnd->m_dzs-=1;
                        }
                        else zk=true;
                        xs+=1;zd=true;break;
                    }
                }
            }
            if(zd)break;
        }
    }
    XS=xs;
    if(xs>=5&&(m_yg>=600||rand()%5==0)&&m_wnd->jd>1)//种瓜
    {
        for(int i=0;i<m_jshu;i++)
        {
            if(m_jsh[i].lx<100)
                hj[m_jsh[i].y]+=(m_jsh[i].shm+(m_jsh[i].eat-1)*4+int(m_jsh[i].fs)*25);
        }
        ma=1e9,zy=-1;int z,zz[20],mz=0;
        for(int y=0;y<MN;y++)
        {
            for(int x=0;x<=(SI==110?330:605);x+=SI)
                zz[x/SI]=GetZwShu(x,y);
            int rt=Find(zz,SI==110?4:12,mz);
            if(rt!=-1&&!UnderGaint(rt*SI,y)&&!UnderCar(rt*SI,y)&&!BeforeShe(rt*SI,y)&&!UnderMonth(rt*SI,y))//不能在危险处种植
            {
                if(!m_car[y])hg[y]-=4;
                if(hg[y]-hj[y]/16<ma)
                    ma=hg[y]-hj[y]/16,mz=hg[y],zy=y;
            }
        }
        if(zy!=-1&&((GetZwShu(0,zy)==-1&&Valid(5,100)!=-1)||m_yg>=3*704||mz<(8+(m_wnd->m_gq==60?1:0))*110/SI||rand()%(mz<10?30:50)==0))//如果攻击力很高就先不种植
        {
            for(int x=0;x<=(SI==110?330:605);x+=SI)
                zz[x/SI]=GetZwShu(x,zy);
            int rt=Find(zz,SI==110?4:12,mz);
            if(rt!=-1&&!UnderGaint(rt*SI,zy)&&!UnderCar(rt*SI,zy)&&!BeforeShe(rt*SI,zy)&&!UnderMonth(rt*SI,zy))//不能在危险处种植
            {
                if((!han[zy]||rand()%6==0||SI==55)&&(z=Valid(5,100))!=-1&&(zz[rt]==-1||m_zhw[zz[rt]].yg<150))
                {
                    m_wnd->m_nzw=z;DestroyZw(rt*SI,zy);
                    m_wnd->Plant(rt*SI,zy);
                }
                else if(((z=Valid(4,75))!=-1)&&rand()%(SI==55?5:12)==0&&(zz[rt]==-1||m_zhw[zz[rt]].yg<150))
                {
                    m_wnd->m_nzw=z;DestroyZw(rt*SI,zy);
                    m_wnd->Plant(rt*SI,zy);
                }
                else if((z=Valid(9,100))!=-1&&rand()%(SI==55?3:14)==0&&(zz[rt]==-1||m_zhw[zz[rt]].yg<=100)&&m_wnd->jd>100&&rt*SI>165)
                {
                    m_wnd->m_nzw=z;DestroyZw(rt*SI,zy);
                    m_wnd->Plant(rt*SI,zy);
                }
                /*else if((z=Valid(10,75))!=-1&&rand()%(SI==55?6:14)==0&&(zz[rt]==-1||m_zhw[zz[rt]].yg<=100))
                {
                    m_wnd->m_nzw=z;DestroyZw(rt*SI,zy);
                    m_wnd->Plant(rt*SI,zy);
                }*/
            }
        }
    }
    if(Valid(3,0)!=-1)//种植窝瓜
    {
        ma=-1;
        for(int x=0;x<=(MM-1)*SI;x+=SI)
        {
            for(int y=0;y<MN;y++)
            {
                int r=GetZwShu(x,y);
                if(r==-1||m_zhw[r].yg<=75)
                {
                    int rt=FakeKill(x-SI*0.682,x+SI*0.682,y,90);
                    if(rt>ma)ma=rt,zx=x,zy=y;
                }
            }
        }
        if(ma>=90)
        {
            m_wnd->m_nzw=8;DestroyZw(zx,zy);
            m_wnd->Plant(zx,zy);
        }
    }
    else if(Valid(6,0)!=-1&&m_wnd->m_en[8]<750)//种植炸弹
    {
        ma=-1;int mx=-1;
        for(int x=0;x<=(MM-1)*SI;x+=SI)
        {
            for(int y=0;y<MN;y++)
            {
                int r=GetZwShu(x,y),g=(r==-1?0:m_zhw[r].yg);
                if(g<=125||(g<=130&&Valid(2,25)!=-1))
                {
                    int rt=0;
                    for(int s=-1;s<=1;s++)
                        rt+=FakeKill(x-SI*1.682,x+SI*1.682,y+s,100);
                    if(rt>ma||(rt==ma&&g<mx))ma=rt,zx=x,zy=y,mx=g;
                }
            }
        }
        if(ma>=(m_wnd->jd<=100?100:170)+(m_wnd->m_gq==60?50:0))
        {
            m_wnd->m_nzw=3;DestroyZw(zx,zy);
            m_wnd->Plant(zx,zy);
        }
    }
    else if(Valid(7,25)!=-1&&m_wnd->m_en[8]<704)//种植辣椒
    {
        ma=-1;
        for(int y=0;y<MN;y++)
        {
            int rt=FakeKillL(y,100);
            if(rt>ma)ma=rt,zy=y;
        }
        if(ma>(m_wnd->jd<=100?90:135))
        {
            int zz[20];
            for(int s=0;s<=(MM-1)*SI;s+=SI)
                zz[s/SI]=GetZwShu(s,zy);
            zx=Find(zz,MM)*SI;
            m_wnd->m_nzw=5;DestroyZw(zx,zy);
            m_wnd->Plant(zx,zy);
        }
    }
    else if(m_wnd->m_en[3]<704&&m_wnd->m_en[5]<680&&m_wnd->m_en[8]<604&&(!b64||HG<250||m_wnd->jd>1000))
    {
        int xh[MMN]={0};bool mc[MMN]={0},da[MMN]={0};
        for(int i=0;i<m_jshu;i++)
        {
            if(m_jsh[i].lx<100&&m_jsh[i].lx!=11)
            {
                int t=m_jsh[i].shm;
                if(m_jsh[i].eat==1)t=t*2/3;
                if(m_jsh[i].eat==40)
                {
                    t+=30,mc[m_jsh[i].y]=true;
                    if(m_jsh[i].x<704&&m_jsh[i].shm>50)
                        t+=40;
                    if(m_jsh[i].x<641&&m_jsh[i].shm>50)
                        t+=100;
                }
                xh[m_jsh[i].y]+=t;
                da[m_jsh[i].y]=(da[m_jsh[i].y]||m_jsh[i].eat>=40||m_jsh[i].x<641);
            }
        }
        ma=-1,zy=-1;
        for(int y=0;y<MN;y++)
        {
            if(da[y]&&xh[y]>ma)
                ma=xh[y],zy=y;
        }
        if(ma>=110&&m_wnd->jd>50)
        {
            if(m_yg>450&&m_wnd->m_dzs<5)
            {
                m_yg-=450;
                m_wnd->m_dzs+=1;
            }
            if(m_wnd->m_dzs)
            {
                bool zd=false;
                for(int x=0;x<=440;x+=SI)
                {
                    int z=GetZwShu(x,zy);
                    if(z!=-1&&m_zhw[z].lx==10&&m_zhw[z].time==100)
                    {
                        zd=true;
                        break;
                    }
                }
                bool dz=false;
                if(!zd&&t_ai>lst[zy]+10)
                {
                    for(int x=0;x<=550;x+=SI)
                    {
                        int z=GetZwShu(x,zy);
                        if(z!=-1&&m_zhw[z].lx==10&&m_zhw[z].time!=100)
                        {
                            DaZhao(m_zhw[z],m_wnd);lst[zy]=t_ai;
                            dz=true;m_wnd->m_dzs-=1;break;
                        }
                    }
                }
                if(!dz&&(ma>=120||mc[zy]))
                {
                    if(!zd&&t_ai>lst[zy]+10)
                    {
                        for(int x=0;x<=550;x+=SI)
                        {
                            int z=GetZwShu(x,zy);
                            if(z!=-1&&m_zhw[z].lx==8&&m_zhw[z].time!=100)
                            {
                                DaZhao(m_zhw[z],m_wnd);lst[zy]=t_ai;
                                dz=true;m_wnd->m_dzs-=1;break;
                            }
                        }
                    }
                }
            }
        }
        if(Valid(1,50)!=-1&&rand()%(m_yg>1600?8:(m_wnd->jd<=150?10:16))==0)
        {
            int qx[MMN]={0};
            for(int i=0;i<m_zshu;i++)
            {
                if(m_zhw[i].lx==19&&m_zhw[i].x>=660)
                    qx[m_zhw[i].y]+=1;
            }
            for(int y=0;y<MN;y++)
            {
                if(qx[y])continue;
                int tx[4]={770,880},zz[20];
                for(int s=0;s<2;s++)zz[s]=GetZwShu(tx[s],y);
                int rt=Find(zz,2);
                if(rt!=-1&&!UnderGaint(tx[rt],y)&&(!UnderCar(tx[rt],y)||rand()%60==0)&&(zz[rt]==-1||m_zhw[zz[rt]].lx!=6))
                {
                    m_wnd->m_nzw=10;DestroyZw(tx[rt],y);
                    m_wnd->Plant(tx[rt],y);break;
                }
            }
            if(Valid(1,50)!=-1)
            {
                for(int y=0;y<MN;y++)
                {
                    if(qx[y]>=2)continue;
                    int tx[4]={770,880},zz[20];
                    for(int s=0;s<2;s++)zz[s]=GetZwShu(tx[s],y);
                    int rt=Find(zz,2);
                    if(rt!=-1&&!UnderGaint(tx[rt],y)&&(!UnderCar(tx[rt],y)||rand()%60==0)&&(zz[rt]==-1||m_zhw[zz[rt]].lx!=6))
                    {
                        m_wnd->m_nzw=10;DestroyZw(tx[rt],y);
                        m_wnd->Plant(tx[rt],y);break;
                    }
                }
            }
        }
    }
    if(Valid(1,70)!=-1&&(m_wnd->jd<10||b64))
    {
        int qx[MMN]={0};
        for(int i=0;i<m_zshu;i++)
        {
            if(m_zhw[i].lx==19&&m_zhw[i].x>=660)
                qx[m_zhw[i].y]+=1;
        }
        for(int y=0;y<MN;y++)
        {
            if(qx[y])continue;
            int tx[4]={770,880},zz[20];
            for(int s=0;s<2;s++)zz[s]=GetZwShu(tx[s],y);
            int rt=Find(zz,2);
            if(rt!=-1&&!UnderGaint(tx[rt],y)&&(!UnderCar(tx[rt],y)||rand()%60==0)&&(zz[rt]==-1||m_zhw[zz[rt]].lx!=6))
            {
                m_wnd->m_nzw=10;DestroyZw(tx[rt],y);
                m_wnd->Plant(tx[rt],y);break;
            }
        }
    }
    if(m_yg>2500&&m_wnd->m_dzs<5)
    {
        m_yg-=450;
        m_wnd->m_dzs+=1;
    }
}
void CGame::input(QDataStream &ar)
{
    ar>>m_zshu;
    for(int i=0;i<m_zshu;i++)
        m_zhw[i].input(ar);
    ar>>m_jshu;
    for(int i=0;i<m_jshu;i++)
        m_jsh[i].input(ar);
    ar>>m_dshu;
    for(int i=0;i<m_dshu;i++)
        m_zid[i].input(ar);
    ar>>m_yg;ar>>m_lx;
    for(int i=0;i<MM;i++)
    {
        for(int j=0;j<MN;j++)
            ar>>m_keng[i][j]>>m_ng[i][j]>>m_dc[i][j]>>lazd[i][j]>>lawg[i][j];
    }
    for(int i=0;i<MN;i++)
    {
        ar>>m_car[i]>>m_cing[i];
        ar>>lst[i]>>lalj[i];
    }
    ar>>m_hb>>t_ai>>b64>>zk;
}
void CGame::output(QDataStream &ar)
{
    ar<<m_zshu;
    for(int i=0;i<m_zshu;i++)
        m_zhw[i].output(ar);
    ar<<m_jshu;
    for(int i=0;i<m_jshu;i++)
        m_jsh[i].output(ar);
    ar<<m_dshu;
    for(int i=0;i<m_dshu;i++)
        m_zid[i].output(ar);
    ar<<m_yg;ar<<m_lx;
    for(int i=0;i<MM;i++)
    {
        for(int j=0;j<MN;j++)
            ar<<m_keng[i][j]<<m_ng[i][j]<<m_dc[i][j]<<lazd[i][j]<<lawg[i][j];
    }
    for(int i=0;i<MN;i++)
    {
        ar<<m_car[i]<<m_cing[i];
        ar<<lst[i]<<lalj[i];
    }
    ar<<m_hb<<t_ai<<b64<<zk;
}

void MainWindow::on_stop_clicked()
{
    double sd=ui->speed->text().toDouble();
    if(!b_zt)
    {
        ui->speed->setDisabled(false);
        QPixmap ic;ic.load("://icons/124.png");
        ui->stop->setIcon(ic);
        int tt=tim.elapsed();timer.stop();
        for(int i=0;i<m_tms;i++)
        {
            tmr[i].ne-=tt;
            if(tmr[i].id!=1001||sd>1)
            {
                tmr[i].jg=int((tmr[i].jg+tmr[i].yu)*sd+0.1);
                tmr[i].ne*=sd;
            }
            if(tmr[i].ne<0)tmr[i].ne=0;
        }
        b_zt=true;
    }
    else
    {
        ui->speed->setDisabled(true);
        QPixmap ic;ic.load("://icons/117.png");
        ui->stop->setIcon(ic);
        for(int i=0;i<m_tms;i++)
        {
            if(tmr[i].id!=1001||sd>1)
            {
                tmr[i].ne/=sd;
                double tj=tmr[i].jg/sd;
                tmr[i].jg=floor(tj);tmr[i].yu=tj-tmr[i].jg;
            }
            tmr[i].ne+=tim.elapsed();
        }
        b_zt=false;
        ResetTimer();
    }
    ui->prog->setFocus();
}

void MainWindow::on_lineEdit_textEdited(const QString &arg1)
{
    r_gq=m_gq=arg1.toInt();
}

void MainWindow::on_save_clicked()
{
    bool bk=false;
    if(!b_zt)on_stop_clicked(),bk=true;
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setAcceptMode(QFileDialog::AcceptSave);
    fileDialog->setWindowTitle(tr("选择文件"));
    fileDialog->setNameFilter(tr("大战僵尸存档(*.dzjs)"));
    fileDialog->setFileMode(QFileDialog::AnyFile);
    fileDialog->setViewMode(QFileDialog::Detail);
    if(fileDialog->exec())
    {
        m_ai=ui->Auto->isChecked();
        auto st=fileDialog->selectedFiles();
        QString na;
        for(auto a:st)na=a;
        QFile file(na);
        file.open(QIODevice::WriteOnly);
        QDataStream ar(&file);
        Save(ar);file.close();
    }
    if(bk)on_stop_clicked();
}

void MainWindow::on_open_clicked()
{
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog->setWindowTitle(tr("选择文件"));
    fileDialog->setNameFilter(tr("大战僵尸存档(*.dzjs)"));
    fileDialog->setFileMode(QFileDialog::ExistingFile);
    fileDialog->setViewMode(QFileDialog::Detail);
    if(fileDialog->exec())
    {
        m_game.KillAll();GameOver();m_game.m_ks=false;
        auto st=fileDialog->selectedFiles();
        QString na;
        for(auto a:st)na=a;
        QFile file(na);
        file.open(QIODevice::ReadOnly);
        QDataStream ar(&file);
        Load(ar);file.close();
        DOU=(SI==55);
        for(int i=0;i<MN;i++)m_game.hjs[i]=0;
        for(int i=0;i<m_game.m_jshu;i++)
        {
            m_game.hjs[m_game.m_jsh[i].y]+=1;
            m_game.m_jsh[i].shu=i;
        }
        for(int i=0;i<m_game.m_zshu;i++)
            m_game.m_zhw[i].shu=i;
        m_game.CalZw();m_game.SZZ=false;
        m_game.m_wnd=this;m_game.m_over=false;
        double sd=ui->speed->text().toDouble();
        //QString str;
        for(int i=0;i<m_tms;i++)
        {
            if(tmr[i].id!=1001||sd>1)
            {
                tmr[i].ne/=sd;
                double tj=tmr[i].jg/sd;
                tmr[i].jg=floor(tj);tmr[i].yu=tj-tmr[i].jg;
            }
            //str+=QString::asprintf("%d %d %d %.2lf\r\n",tmr[i].id,tmr[i].ne,tmr[i].jg,tmr[i].yu);
            tmr[i].ne+=tim.elapsed();
        }
        //QMessageBox::information(this,tr(""),str);
        int ms=-1;ResetTimer();
        for(int n=0;n<ND-1;n++)
            ms+=int(m_jg[n]/m_time[n]);
        if(m_b11)ui->prog->setRange(0,MN*(SI<=55?3000:1000));
        else ui->prog->setRange(0,ms);
        ui->prog->setValue(jd);ui->lineEdit->setText(QString::asprintf("%d",r_gq));
        on_stop_clicked();
        ui->Auto->setChecked(m_ai);ui->Doub->setChecked(DOU);ui->buy->setVisible(1);
        ui->speed->setEnabled(0);
        ui->easy->setEnabled(0);
        ui->normal->setEnabled(0);
        ui->hard->setEnabled(0);
        ui->Doub->setEnabled(0);
        m_game.m_ks=true;ShowButton();Initicon();
    }
}

void MainWindow::on_Auto_stateChanged(int arg1)
{
    if(!m_game.m_ks)return;
    if(arg1)SetTimer(1008,400,NULL);
    else KillTimer(1008);
}

void MainWindow::on_buy_clicked()
{
    bool bk=false;
    if(!b_zt)on_stop_clicked(),bk=true;
    if(QMessageBox::question(this,tr("提示"),tr("花费450阳光购买一颗能量豆"))==QMessageBox::Yes)
    {
        if(m_dzs==5)
        {
            QMessageBox::warning(this,tr("提示"),tr("能量豆数量已达到上限！"));
            ui->prog->setFocus();
            return;
        }
        if(m_game.m_yg<450)
        {
            QMessageBox::warning(this,tr("提示"),tr("阳光不足！"));
            ui->prog->setFocus();
            return;
        }
        m_game.m_yg-=450;
        m_dzs+=1;
        gl->repaint();gx->repaint();
    }
    if(bk)on_stop_clicked();
    ui->prog->setFocus();
}

void MainWindow::on_quick_clicked()
{
    bool bk=false;
    if(!b_zt)on_stop_clicked(),bk=true;
    if(QMessageBox::question(this,tr("提示"),tr("此操作可以加速回复30秒，要求至少5264阳光，操作后阳光将除以2"))==QMessageBox::Yes)
    {
        if(m_game.m_yg<5264)
        {
            QMessageBox::warning(this,tr("提示"),tr("阳光不足！"));
            ui->prog->setFocus();
            return;
        }
        m_game.m_yg/=2;
        if(m_hf[0]>100)
        {
            for(int i=0;i<XUAN;i++)
            {
                int t=m_zw[m_xuan[i]].lx;
                if(t!=3&&t!=5&&t!=7&&t!=16&&t!=30&&t!=31)
                    m_hf[m_xuan[i]]/=10000;
            }
            KillTimer(1010);
        }
        if(!b_zt)SetTimer(1010,30000,NULL);
        else tmr[m_tms++]=STm(1010,30000,30000,0);
        for(int i=0;i<XUAN;i++)
        {
            int t=m_zw[m_xuan[i]].lx;
            if(t!=3&&t!=5&&t!=7&&t!=16)
                m_hf[m_xuan[i]]*=10000;
        }
    }
    if(bk)on_stop_clicked();
    ui->prog->setFocus();
}

void MyGLWidget::paintEvent(QPaintEvent *e)
{
    QPainter memdc(this);
    if(ty==0)
    {
        QFont f("",floor(8*m_wnd->m_sf+1e-10),QFont::Bold,false);memdc.setFont(f);
        memdc.setPen(QPen(Qt::NoPen));
        QBrush b(QColor(0,0,0,150)),b2(QColor(93,0,1));
        b2.setStyle(Qt::SolidPattern);b.setStyle(Qt::SolidPattern);
        memdc.setBrush(b2);memdc.drawRect(QRect(MPoint(0,0),MSize(100,610)));
        for(int n=0;n<XUAN;n++)
        {
            if(m_wnd->m_xuan[n]==-1)break;
            if(n>=m_wnd->SHU-(m_wnd->m_b11&&!m_wnd->m_b12))break;
            memdc.drawRect(QRect(MPoint(0,n*50),MSize(100,50)));
            //memdc.drawPixmap(QRect(MPoint(n*90,610),MSize(90,100)),m_wnd->ico[115]);
            memdc.drawPixmap(QRect(MPoint(20,n*50),MSize(50,50)),m_wnd->ico[m_wnd->m_zw[m_wnd->m_xuan[n]].id]);
            if(!m_wnd->m_b11||m_wnd->m_b12)
            {
                memdc.setBrush(b);
                memdc.drawRect(QRect(MPoint(0,n*50),MSize((800-m_wnd->m_en[m_wnd->m_xuan[n]])/8,50)));
                memdc.setPen(QColor(0,240,0));
                memdc.drawText(MPoint(72,n*50+30),QString::asprintf("%d",m_wnd->m_zw[m_wnd->m_xuan[n]].yg));
            }
            QString nu;memdc.setPen(QColor(0,0,240));
            if(n==10)nu="-> -";
            else nu=QString::asprintf("-> %d",(n+1)%10);
            memdc.drawText(MPoint(0,n*50+30),nu);
            memdc.setPen(Qt::NoPen);memdc.setBrush(b2);
        }
        for(int i=0;i<m_wnd->m_dzs;i++)
            memdc.drawPixmap(QRect(MPoint(50,530+i*8),MSize(50,50)),m_wnd->ico[78]);
        memdc.drawPixmap(QRect(MPoint(0,550),MSize(50,50)),m_wnd->ico[131]);
        if(m_wnd->m_b48)
        {
            memdc.setBrush(b);int t=TM-m_wnd->l48;
            if(t>30)t=30;
            memdc.drawRect(QRect(MPoint(0,XUAN*50),MSize(int((30-t)/30.0*100.0),50)));
        }
    }
    else
    {
        memdc.setRenderHint(QPainter::Antialiasing);
        //memdc.drawPixmap(QRect(MPoint(1040,0),MSize(330,610)),m_wnd->ico[125]);
        m_wnd->m_game.Draw(memdc);
    }
    memdc.end();m_wnd->lad=m_wnd->tim.elapsed();
}

void MainWindow::ShowButton()
{
    if(r_gq==61||r_gq==63)
    {
        ui->buy->setVisible(0);
        ui->quick->setVisible(0);
    }
    else
    {
        ui->buy->setVisible(1);
        ui->quick->setVisible(1);
    }
}

void MainWindow::jiasu()
{
    bool bk=false;
    if(!b_zt)on_stop_clicked(),bk=true;
    double sd=ui->speed->text().toDouble();
    sd+=0.25;if(sd>15)sd=15;
    ui->speed->setValue(sd);
    if(bk)on_stop_clicked();
}

void MainWindow::jiansu()
{
    bool bk=false;
    if(!b_zt)on_stop_clicked(),bk=true;
    double sd=ui->speed->text().toDouble();
    sd-=0.25;if(sd<0.25)sd=0.25;
    ui->speed->setValue(sd);
    if(bk)on_stop_clicked();
}

void MainWindow::ViewStatus()
{
    if(!m_game.m_ks)
        QMessageBox::information(this,tr(""),tr("游戏还没有开始"));
    else
        QMessageBox::information(this,tr(""),QString::asprintf("总共杀死了%d只僵尸，还剩下%d只，进度%.2lf%%\r\n预计剩余%.2lf分钟",jd,zj-jd,double(jd)/zj*100,m_ztm/60000.0/ui->speed->text().toDouble()-TM*25/600000.0));
}

void MainWindow::Quit()
{
    exit(0);
}

QSize MainWindow::MSize(int x,int y)
{
    x=floor(x*m_sf+1e-10);y=floor(y*m_sf+1e-10);
    return QSize(x,y);
}

QPoint MainWindow::MPoint(int x,int y)
{
    x=floor(x*m_sf+1e-10);y=floor(y*m_sf+1e-10);
    return QPoint(x,y);
}

QSize CGame::MSize(int x,int y)
{
    double m_sf=m_wnd->m_sf;
    x=floor(x*m_sf+1e-10);y=floor(y*m_sf+1e-10);
    return QSize(x,y);
}

QPoint CGame::MPoint(int x,int y)
{
    double m_sf=m_wnd->m_sf;
    x=floor(x*m_sf+1e-10);y=floor(y*m_sf+1e-10);
    return QPoint(x,y);
}

QSize MyGLWidget::MSize(int x,int y)
{
    double m_sf=m_wnd->m_sf;
    x=floor(x*m_sf+1e-10);y=floor(y*m_sf+1e-10);
    return QSize(x,y);
}

QPoint MyGLWidget::MPoint(int x,int y)
{
    double m_sf=m_wnd->m_sf;
    x=floor(x*m_sf+1e-10);y=floor(y*m_sf+1e-10);
    return QPoint(x,y);
}

QPoint MainWindow::SPoint(int x, int y)
{
    y-=ME;
    x=floor(x/m_sf+1e-10);y=floor(y/m_sf+1e-10);
    return QPoint(x,y);
}

void MainWindow::left2048()
{
    if(TM-l48<30)return;
    l48=TM;
    SZhw lx[MMM][MMN];int dy[MAXSHU];
    for(int i=0;i<MAXSHU;i++)
        dy[m_zw[i].lx]=i;
    for(int i=0;i<MN;i++)
    {
        for(int j=0;j<MN;j++)
        {
            int z=m_game.GetZwShu(j*SI,i);
            if(z==-1)lx[j][i].lx=-1;
            else lx[j][i]=m_game.m_zhw[z];
        }
    }
    for(int i=0;i<m_game.m_zshu;i++)
    {
        if(m_game.m_zhw[i].x/SI<MN)
        {
            int x=m_game.m_zhw[i].x/SI,y=m_game.m_zhw[i].y;
            lx[x][y].te=m_game.DestroyZw(i);i-=1;
            if(lx[x][y].time==100)
                lx[x][y].time=m_zw[dy[lx[x][y].lx]].time;
        }
    }
    for(int i=0;i<MN;i++)
    {
        SZhw st[MMM];int tp=0;
        for(int j=0;j<MN;j++)
        {
            SZhw z=lx[j][i];lx[j][i].lx=-1;
            while(z.lx!=-1)
            {
                if(tp==0||z.lx!=st[tp-1].lx||n_2048[z.lx]<=0)
                {
                    st[tp++]=z;
                    z.lx=-1;
                }
                else
                {
                    int t=n_2048[st[--tp].lx];
                    if(n_2048[t]==-1)
                    {
                        m_game.m_yg+=1500;
                        if(m_dzs<5)
                            m_dzs+=1;
                    }
                    z=m_zw[dy[t]];
                }
            }
        }
        if(tp<MN&&rand()%(2*GL48)==0)
        {
            if(tp==0)st[tp++]=m_zw[1];
            else
            {
                SZhw t=st[tp-1];
                st[tp++]=(t.lx==0?m_zw[6]:m_zw[1]);
            }
        }
        for(int j=0;j<tp;j++)
            lx[j][i]=st[j];
    }
    for(int i=0;i<MN;i++)
    {
        for(int j=0;j<MN;j++)
        {
            SZhw z=lx[j][i];
            if(z.lx==-1)continue;
            m_game.m_yg+=z.yg;
            z.x=j*SI;z.y=i;
            m_game.NewZw(z);
        }
    }
    m_game.CalZw();gl->repaint();gx->repaint();
}

void MainWindow::up2048()
{
    if(TM-l48<30)return;
    l48=TM;
    SZhw lx[MMM][MMN];int dy[MAXSHU];
    for(int i=0;i<MAXSHU;i++)
        dy[m_zw[i].lx]=i;
    for(int i=0;i<MN;i++)
    {
        for(int j=0;j<MN;j++)
        {
            int z=m_game.GetZwShu(j*SI,i);
            if(z==-1)lx[j][i].lx=-1;
            else lx[j][i]=m_game.m_zhw[z];
        }
    }
    for(int i=0;i<m_game.m_zshu;i++)
    {
        if(m_game.m_zhw[i].x/SI<MN)
        {
            int x=m_game.m_zhw[i].x/SI,y=m_game.m_zhw[i].y;
            lx[x][y].te=m_game.DestroyZw(i);i-=1;
            if(lx[x][y].time==100)
                lx[x][y].time=m_zw[dy[lx[x][y].lx]].time;
        }
    }
    for(int j=0;j<MN;j++)
    {
        SZhw st[MMN];int tp=0;
        for(int i=0;i<MN;i++)
        {
            SZhw z=lx[j][i];lx[j][i].lx=-1;
            while(z.lx!=-1)
            {
                if(tp==0||z.lx!=st[tp-1].lx||n_2048[z.lx]<=0)
                {
                    st[tp++]=z;
                    z.lx=-1;
                }
                else
                {
                    int t=n_2048[st[--tp].lx];
                    if(n_2048[t]==-1)
                    {
                        m_game.m_yg+=1500;
                        if(m_dzs<5)
                            m_dzs+=1;
                    }
                    z=m_zw[dy[t]];
                }
            }
        }
        if(tp<MN&&rand()%GL48==0)
        {
            if(tp==0)st[tp++]=m_zw[1];
            else
            {
                SZhw t=st[tp-1];
                st[tp++]=(t.lx==0?m_zw[6]:m_zw[1]);
            }
        }
        for(int i=0;i<tp;i++)
            lx[j][i]=st[i];
    }
    for(int i=0;i<MN;i++)
    {
        for(int j=0;j<MN;j++)
        {
            SZhw z=lx[j][i];
            if(z.lx==-1)continue;
            m_game.m_yg+=z.yg;
            z.x=j*SI;z.y=i;
            m_game.NewZw(z);
        }
    }
    m_game.CalZw();gl->repaint();gx->repaint();
}

void MainWindow::right2048()
{
    if(TM-l48<30)return;
    l48=TM;
    SZhw lx[MMM][MMN];int dy[MAXSHU];
    for(int i=0;i<MAXSHU;i++)
        dy[m_zw[i].lx]=i;
    for(int i=0;i<MN;i++)
    {
        for(int j=0;j<MN;j++)
        {
            int z=m_game.GetZwShu(j*SI,i);
            if(z==-1)lx[j][i].lx=-1;
            else lx[j][i]=m_game.m_zhw[z];
        }
    }
    for(int i=0;i<m_game.m_zshu;i++)
    {
        if(m_game.m_zhw[i].x/SI<MN)
        {
            int x=m_game.m_zhw[i].x/SI,y=m_game.m_zhw[i].y;
            lx[x][y].te=m_game.DestroyZw(i);i-=1;
            if(lx[x][y].time==100)
                lx[x][y].time=m_zw[dy[lx[x][y].lx]].time;
        }
    }
    for(int i=0;i<MN;i++)
    {
        SZhw st[MMM];int tp=0;
        for(int j=MN-1;j>=0;j--)
        {
            SZhw z=lx[j][i];lx[j][i].lx=-1;
            while(z.lx!=-1)
            {
                if(tp==0||z.lx!=st[tp-1].lx||n_2048[z.lx]<=0)
                {
                    st[tp++]=z;
                    z.lx=-1;
                }
                else
                {
                    int t=n_2048[st[--tp].lx];
                    if(n_2048[t]==-1)
                    {
                        m_game.m_yg+=1500;
                        if(m_dzs<5)
                            m_dzs+=1;
                    }
                    z=m_zw[dy[t]];
                }
            }
        }
        if(tp<MN&&rand()%GL48==0)
        {
            if(tp==0)st[tp++]=m_zw[1];
            else
            {
                SZhw t=st[tp-1];
                st[tp++]=(t.lx==0?m_zw[6]:m_zw[1]);
            }
        }
        for(int j=0;j<tp;j++)
            lx[MN-1-j][i]=st[j];
    }
    for(int i=0;i<MN;i++)
    {
        for(int j=0;j<MN;j++)
        {
            SZhw z=lx[j][i];
            if(z.lx==-1)continue;
            m_game.m_yg+=z.yg;
            z.x=j*SI;z.y=i;
            m_game.NewZw(z);
        }
    }
    m_game.CalZw();gl->repaint();gx->repaint();
}

void MainWindow::down2048()
{
    if(TM-l48<30)return;
    l48=TM;
    SZhw lx[MMM][MMN];int dy[MAXSHU];
    for(int i=0;i<MAXSHU;i++)
        dy[m_zw[i].lx]=i;
    for(int i=0;i<MN;i++)
    {
        for(int j=0;j<MN;j++)
        {
            int z=m_game.GetZwShu(j*SI,i);
            if(z==-1)lx[j][i].lx=-1;
            else lx[j][i]=m_game.m_zhw[z];
        }
    }
    for(int i=0;i<m_game.m_zshu;i++)
    {
        if(m_game.m_zhw[i].x/SI<MN)
        {
            int x=m_game.m_zhw[i].x/SI,y=m_game.m_zhw[i].y;
            lx[x][y].te=m_game.DestroyZw(i);i-=1;
            if(lx[x][y].time==100)
                lx[x][y].time=m_zw[dy[lx[x][y].lx]].time;
        }
    }
    for(int j=0;j<MN;j++)
    {
        SZhw st[MMN];int tp=0;
        for(int i=MN-1;i>=0;i--)
        {
            SZhw z=lx[j][i];lx[j][i].lx=-1;
            while(z.lx!=-1)
            {
                if(tp==0||z.lx!=st[tp-1].lx||n_2048[z.lx]<=0)
                {
                    st[tp++]=z;
                    z.lx=-1;
                }
                else
                {
                    int t=n_2048[st[--tp].lx];
                    if(n_2048[t]==-1)
                    {
                        m_game.m_yg+=1500;
                        if(m_dzs<5)
                            m_dzs+=1;
                    }
                    z=m_zw[dy[t]];
                }
            }
        }
        if(tp<MN&&rand()%GL48==0)
        {
            if(tp==0)st[tp++]=m_zw[1];
            else
            {
                SZhw t=st[tp-1];
                st[tp++]=(t.lx==0?m_zw[6]:m_zw[1]);
            }
        }
        for(int i=0;i<tp;i++)
            lx[j][MN-1-i]=st[i];
    }
    for(int i=0;i<MN;i++)
    {
        for(int j=0;j<MN;j++)
        {
            SZhw z=lx[j][i];
            if(z.lx==-1)continue;
            m_game.m_yg+=z.yg;
            z.x=j*SI;z.y=i;
            m_game.NewZw(z);
        }
    }
    m_game.CalZw();gl->repaint();gx->repaint();
}
