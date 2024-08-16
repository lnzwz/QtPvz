#include "./ui_mainwindow.h"
#include "mainwindow.h"
#include "choogq.h"
#include "other.h"

void MainWindow::SwitchStop()
{
    if(b_fast)return;
    if (!isstop) {
        ui->Auto->setDisabled (false);
        ui->stop->setIcon (ico[124]);
        isstop = true;stop_cnt+=1;
    }
    else {
        ui->Auto->setDisabled (true);
        ui->stop->setIcon (ico[117]);
        isstop = false;
        if(!b_fast)tm_det=v_tm-m_spd*tim.elapsed();
    }
}

void MainWindow::on_stop_clicked () { //暂停
    if(tcp.size()>0)
        SendInt(5,beans);
    SwitchStop();
    ui->label_2->setFocus ();
}
void MainWindow::on_lineEdit_textEdited (const QString &str) {
#ifdef Q_OS_ANDROID
    char c=str.back().toLatin1();
    if(c>='a'&&c<='z')
    {
        QString nw=str;nw.chop(1);
        ui->lineEdit->setText(nw);
        KeyDown(c-'a'+Qt::Key_A);return;
    }
#endif
    m_gq=0;
    for(int i=0;i<str.length()&&str[i]>='0'&&str[i]<='9';i++)
        m_gq=m_gq*10+(str[i].toLatin1()-'0');
    int gq=m_gq%100;gqe=gq-MAXGQS;
    if(gqe==1||gqe==2||gqe==3||gqe==5||gqe==9||gqe==11||gqe==12||gqe==13||gqe==15||gqe==16||gqe==21)
    {
        ui->Short->setChecked(0);
        ui->Short->setEnabled(0);
    }
    else
        ui->Short->setEnabled(1);
    if (gq == MAXGQS + 1 || gq == MAXGQS + 2 || gq == MAXGQS + 4)
        ui->Short->setChecked (0);
    if (gq == MAXGQS + 6)
    {
        ui->Short->setText (tr ("随机"));
        ui->Short->setChecked(1);
    }
    else if (gq == MAXGQS + 4)
        ui->Short->setText (tr ("预设"));
    else
        ui->Short->setText (tr ("快速"));
    if(gq == MAXGQS + 6||gq ==MAXGQS+13)
    {
        ui->huisu->show();
        ui->dfnz->show();
        ui->huisu->setText("种子：");
    }
    else
    {
        ui->huisu->hide();
        ui->dfnz->hide();
    }
}

void MainWindow::SaveToFile(QFile&file)
{
    QByteArray da;
    QDataStream ar (&da,QIODevice::WriteOnly);
    Save (ar);da=qCompress(da);
    file.write(da);
    file.close ();
}

void MainWindow::on_save_clicked () {
    bool bk = false;
    if (!isstop)
        on_stop_clicked (), bk = true;
    auto fileDialog = std::make_unique<QFileDialog> (this);
    fileDialog->setAcceptMode (QFileDialog::AcceptSave);
    fileDialog->setWindowTitle (tr ("选择文件"));
    fileDialog->setNameFilter (tr ("大战僵尸存档(*)"));
    fileDialog->setFileMode (QFileDialog::AnyFile);
    fileDialog->setViewMode (QFileDialog::Detail);
    if (fileDialog->exec ()) {
        m_ai = ui->Auto->isChecked ();
        auto st = fileDialog->selectedFiles ();
        QString na;for (auto a : st)na = a;
        QFile file (na);
        file.open (QIODevice::WriteOnly);
        SaveToFile(file);
    }
    if (bk)
        on_stop_clicked ();
    activateWindow();
    ui->label_2->setFocus ();
}
void MainWindow::on_open_clicked () {
    auto fileDialog = std::make_unique<QFileDialog> (this);
    fileDialog->setAcceptMode (QFileDialog::AcceptOpen);
    fileDialog->setWindowTitle (tr ("选择文件"));
    fileDialog->setNameFilter (tr ("大战僵尸存档(*)"));
    fileDialog->setFileMode (QFileDialog::ExistingFile);
    fileDialog->setViewMode (QFileDialog::Detail);
    if (fileDialog->exec ()) {
        GameOver ();
        m_game.started = false;
        auto st = fileDialog->selectedFiles ();
        QString na;for (auto a : st)na = a;
        RestoreFromFile(na);
    }
    activateWindow();
}
void MainWindow::on_Auto_stateChanged (int arg1) {
    if (!m_game.started)
        return;
    if (arg1)SetTimer(1008,400);
    else KillTimer (1008);
    m_ai = arg1;
}

void MainWindow::on_quick_clicked () {
    bool bk = false;
    if (!isstop)
        on_stop_clicked (), bk = true;
    if (QMessageBox::question (this, tr ("提示"), tr ("此操作可以加速回复60秒，要求至少6000阳光，操作后阳光将除以2")) == QMessageBox::Yes) {
        if (m_game.m_yg < 6000) {
            QMessageBox::warning (this, tr ("提示"), tr ("阳光不足！"));
            ui->label_2->setFocus ();
            return;
        }
        m_game.m_yg /= 2;
        if (cd_spd[0] > 100) {
            for (int i = 0; i < XUAN; i++) {
                int t = plants[m_xuan[i]].type;
                if (t != 3 && t != 5 && t != 7 && t != 16 && t != 30 && t != 31&&t!=42&&t!=53)
                    cd_spd[m_xuan[i]] /= 10000;
            }
            KillTimer (1010);
        }
        SetTimer (1010, 60000);
        for (int i = 0; i < XUAN; i++) {
            int t = plants[m_xuan[i]].type;
            if (t != 3 && t != 5 && t != 7 && t != 16 && t != 30 && t != 31&&t!=42&&t!=53)
                cd_spd[m_xuan[i]] *= 10000;
        }
    }
    if (bk)
        on_stop_clicked ();
    activateWindow();
    ui->label_2->setFocus ();
}

void MainWindow::on_huisu_clicked () { //回溯
    ui->label_2->setFocus ();
    if(b_ycl)//进攻
    {
        b_ycl=false;
        ui->huisu->setEnabled(0);
        for(int i=0;i<MAXSHU;i++)
        {
            if(i!=38)
                cd_spd[i]-=800,cd[i]=0;
        }
        if(m_gq==MAXGQS+13)
        {
            SetTimer (1000, 50);
            return;
        }
        b_fast=ui->Short->isChecked();
        SetTimer (1002, tm_js[0]);
        SetTimer (1004, tm_lst[0]);
        SetTimer (1003, (m_b48 ? 10000 : 6000));
        if(b_fast)//瞬间模式
        {
            while(!m_game.over)
                OnTick();
        }
        return;
    }
    int t = ui->dfnz->text ().toInt ();
    if (t == 0)return;
    bool bb = false;
    if (!m_game.started) {//直接进入
        h_sd = -m_gq;
        b_hui = true;
        b_rd = false;
        bb = true;
        SetTimer (1011, 100);
    }
    w_tm = t;
    HuiFang (true);
    if (bb)
        InitIcon ();
}

void MainWindow::OnStart()
{
    QString str=ui->lineEdit->text();
    str.replace("—","--");
    int ex=0,es=0;bool ok;str.toInt(&ok);
    if(!ok&&str.count('+')+str.count('.')+str.count('-')>0)
    {
        int ji=1;
        while(str.length()>0&&str.back()>='0'&&str.back()<='9')
        {
            es+=(str.back().toLatin1()-'0')*ji;ji*=10;
            str.chop(1);
        }
    }
    if(es==0)es=1;
    while(str.length()>0&&str.back()=='+')
        str.chop(1),ex+=es;
    while(str.length()>0&&(str.back()=='-'||str.back()=='.'))
        str.chop(1),ex-=es;
    str.toInt(&ok);
    if(!ok)
    {
        int ji=1,z=0;
        while(str.length()>0&&str.back()>='0'&&str.back()<='9')
        {
            z+=(str.back().toLatin1()-'0')*ji;ji*=10;
            str.chop(1);
        }
        str.chop(1);userlx=z;
    }
    ext_dif+=ex;
    m_gq = str.toInt();
    w_tm = plajs=duiz=0;h_sd = 1;
    b_rd = b_hui = wait_guan=false;
    an_game.zmb_cnt=-1;an_game.started=0;
    while(q_js.size())q_js.pop();
    if (m_game.started) {
        bool bk = false;
        if (!isstop)
            on_stop_clicked (), bk = true;
        if (QMessageBox::question (this, tr ("提示"), tr ("此操作将结束正在进行的游戏")) != QMessageBox::Yes) {
            if (bk)
                on_stop_clicked ();
            ui->label_2->setFocus ();
            return;
        }
        isstop = false;
        ui->stop->setIcon (ico[117]);
        GameOver();return;
    }
    m_ai = ui->Auto->isChecked ();
    if (m_gq < 0) //回放
    {
        w_tm = 1;
        h_sd = -m_gq;
        b_hui = true;
        SetTimer (1011, 100);
        b_rd = true;
        return;
    }
    int a=m_gq/100,b=m_gq%100;
    if ((a<0||a>7||b<1||b>ZGQS)&& m_gq != 2048)
        return;
    GameOver (1);real_gq=m_gq;
    if(!b_fast)
    {
        DisableButton();
        ui->Auto->setEnabled (0);
        ui->huisu->setEnabled(1);
        ui->hui->setEnabled(0);
        ui->luj->setEnabled(0);
    }
    try {
        StartGame (MakeGq (m_gq));
    }
    catch (...) {
        GameOver ();
        return;
    }
    if(!b_fast)
    {
        ui->label_2->setFocus ();
        ShowGameButton ();
        update ();
    }
}

int MainWindow::ChooseGQ()
{
    Choogq *ch=new Choogq();
    ch->setWindowModality(Qt::ApplicationModal);
    ch->exec();int gg=ch->gq;
    ext_dif=ch->ext_dif;
    userlx=ch->userlx;
    QString str=QString::asprintf("%d",gg);
    if(userlx!=-1)
        str+=QString::asprintf("_%d",userlx);
    if(ext_dif<=2)
        for(int s=0;s<ext_dif;s++)str+='+';
    else
        str+=QString::asprintf("+%d",ext_dif);
    if(gg>0)ui->lineEdit->setText(str);
    on_lineEdit_textEdited(str);
    return gg;
}

void MainWindow::on_pushButton_clicked()
{
    ChooseGQ();
}


void MainWindow::on_btc_clicked () //开始
{
    ui->dfn->setText("");
    bool ok;ai_0=ai_1=ai_2=0;b_fast = false;
    QString str=ui->lineEdit->text();
    str.replace("—","--");str.toInt(&ok);
    if(!ok&&str.count('+')+str.count('.')+str.count('-')>0)
    {
        while(str.length()>0&&str.back()>='0'&&str.back()<='9')str.chop(1);
        while(str.length()>0&&str.back()=='+')str.chop(1);
        while(str.length()>0&&(str.back()=='-'||str.back()=='.'))str.chop(1);
    }
    str.toInt(&ok);
    if(!ok)
    {
        while(str.length()>0&&str.back()>='0'&&str.back()<='9')str.chop(1);
        str.chop(1);
    }
    int gg=str.toInt(&ok);
    if(!ok||gg==0)gg=ChooseGQ();
    else ext_dif=0,userlx=-1;
    if((gg/100)%2==1&&ui->Auto->isChecked())
    {
        int cnt=(gg%10==0?200:100);
        QElapsedTimer tm;tm.start();
        for(int s=0;s<cnt;s++)OnStart();
        if(ai_0==0)ai_1=0;
        else ai_1/=ai_0;ai_2/=cnt;
        int tt=tm.elapsed();
        QMessageBox::information (this, tr ("提示"), QString::asprintf ("胜率：%.2lf%%  得分率：%.2lf%%  平均完成度：%.2lf%% 用时：%.2lfs", double(ai_0)/cnt*100,ai_1*100,ai_2*100,tt/1000.0));
        EnableButton();return;
    }
    OnStart();
}

void MainWindow::DisableButton()
{
    ui->easy->setEnabled (0);
    ui->normal->setEnabled (0);
    ui->hard->setEnabled (0);
    ui->xin->setEnabled (0);
    ui->tui->setEnabled (0);
    ui->water->setEnabled (0);
    ui->Short->setEnabled(0);
    ui->btc->setIcon(ico[91]);
    ui->bt_oth->hide();
    ui->a_last->setDisabled(1);
}

void MainWindow::EnableButton()
{
    ui->easy->setEnabled (1);
    ui->normal->setEnabled (1);
    ui->hard->setEnabled (1);
    ui->xin->setEnabled (1);
    ui->tui->setEnabled (1);
    ui->water->setEnabled (1);
    ui->Short->setEnabled (1);
    ui->Auto->setEnabled (1);
    ui->hui->setEnabled(1);
    ui->luj->setEnabled(1);
    ui->dfn->hide();
    ui->huisu->show();
    ui->hui->show();
    ui->luj->show();
    ui->btc->setIcon(ico[116]);
    ui->huisu->hide();
    ui->dfnz->hide();
    ui->stop->hide();
    //ui->buy->hide();
    ui->quick->hide();
    ui->huisu->setEnabled(1);
    ui->huisu->setFlat(1);
    ui->bt_oth->show();
    ui->a_last->setEnabled(1);
#ifdef NO_UI
    ui->open->hide();
    ui->save->hide();
    ui->stop->hide();
    ui->buy->hide();
#endif
}

void MainWindow::ShowGameButton () {//显示/隐藏按钮
    ui->huisu->show();
    ui->dfnz->show();
    if (gqe == 1 || gqe == 3|| gqe == 9 || b_zhu || b_guan)
        ui->quick->hide();
    else
        ui->quick->show();
    if(!ui->hui->isChecked())
    {
        ui->dfn->hide();
        ui->dfnz->hide();
        ui->hui->hide();
        ui->luj->hide();
        if(!b_ycl)
            ui->huisu->hide();
    }
    ui->huisu->setText(b_ycl?"进攻":"回溯");
    ui->huisu->setFlat(0);
    ui->stop->show();
}

void MainWindow::CheckPao(QPoint pt)
{
    int zz,x=((pt.x () - (W0+50)) / SI) * SI, y=(pt.y () - 60) / SI;
    if ((zz = m_game.GetZwShu (x, y)) != -1 && m_game.plants[zz].type == 30 && m_game.plants[zz].grow == m_game.plants[zz].move_spd &&select==-1) //大炮, 且恢复完毕
    {
        int tz = m_game.GetZwShu (x - SI, y);
        if (tz != -1 && m_game.plants[tz].type == 31) {//完整
            select = -3,sele_x=sele_y=-1;
            m_game.plants[zz].grow = 0;
        }
    }
}

void MainWindow::SolveDig(QPoint pt)
{
    int x = int ((pt.x () - (W0+50)) / SI) * SI;
    int y = int ((pt.y () - 60) / SI);
    if (chui) {
        int wz=m_game.FindZombie(pt);
        if (wz!=-1) {
            if(m_game.zombies[wz].tag<0)
                m_game.zombies[wz].life += 40000, m_game.zombies[wz].vx+=1, chui_sc-=5;
            else
                m_game.zombies[wz].life -= 40000;
            if (m_game.zombies[wz].life <= 0)
                m_game.DestroyJs (wz), chui_sc+=1;
            else
                m_game.zombies[wz].id = GetJsIcon (m_game.zombies[wz]);
        }
        if (imjs)
            select = -1;
    }
    else {
        int w=m_game.GetZwShu(x,y);
        if(w==-1)
        {
            m_game.m_ng[x / SI][y] = 0;
            m_game.nd_calfr=true;
        }
        else
        {
            if(m_game.plants[w].type==24)
                m_game.m_ng[x/SI][y]=0;
            m_game.DestroyZw (w,1);
        }
        select = -1;SendDeltaDig(x,y);
    }
}

void MainWindow::Drag(QPoint pt)
{
    int x = pt.x (), y = pt.y ();
    x -= lmsx; y -= lmsy;
    if (m_game.started&&m_b48 &&x * x + y * y >= 6000*zoom*zoom) {
        double jj = atan2 (y, x), p4 = acos (-1.0) / 4.0;
        if (jj > -p4  && jj < p4)right2048 ();
        else if (jj > p4 && jj < 3 * p4)down2048 ();
        else if (jj > -3 * p4 && jj < -p4)up2048 ();
        else left2048 ();
    }
#ifdef Q_OS_IOS
    if(std::min(y+lmsy,lmsy)<=60*zoom&&std::max(y+lmsy,lmsy)<=200*zoom)
    {
        if(y<-20*zoom)ui->menubar->hide();
        else if(y>20*zoom)ui->menubar->show();
    }
#endif
    if(std::max(x+lmsx,lmsx)>=1040*zoom&&std::min(x+lmsx,lmsx)>=700*zoom)
    {
        if(x<-100*zoom)ui->centralwidget->show(),hiui=0;
        else if(x>100*zoom)ui->centralwidget->hide(),hiui=1;
        W2=(hiui?80:140);
        resizeEvent(NULL);
        if(m_game.started&&abs(y)>250*zoom&&ma_y-std::max(lmsy,y+lmsy)<=10*zoom&&std::min(lmsy,y+lmsy)-mi_y<=10*zoom)
            on_stop_clicked();
    }
    if(m_game.started&&b_guan)
    {
        x/=zoom;y/=zoom;int ox=lmsx/zoom,oy=lmsy/zoom;
        if(ox>=(W0+50)&&oy>=60&&x*x+y*y>=2000)
            SolveDig(QPoint(ox,oy));
    }
    if(m_game.started&&lmsx>=1100*zoom)
    {
        bool ud=(lmsy-mi_y>=200*zoom&&y+lmsy-mi_y>=200*zoom);
        bool du=(ma_y-lmsy>=200*zoom&&ma_y-y-lmsy>=200*zoom);
        bool lr=(lmsx-mi_x>=200*zoom&&x+lmsx-mi_x>=200*zoom);
        //bool rl=(ma_x-lmsx>=200*zoom&&ma_x-x-lmsx>=200*zoom);
        if(lr&&du)ui->menubar->show();
        else if(lr&&ud)ViewStatus();
        else if(ud)RestartGame();
        //else if(du)on_buy_clicked();
        else if(lr)ChangeZw();
    }
    if(m_game.started&&m_game.m_yg>=50)
    {
        QPoint pt1=SPoint(lmsx,lmsy),pt2=SPoint(pt.x(),pt.y());
        pt1-=QPoint(W0+50,60);pt2-=QPoint(W0+50,60);
        if(pt1.x()>=0&&pt1.x()<MM*SI&&pt1.y()>=0&&pt1.y()<MN*SI&&
            pt2.x()>=0&&pt2.x()<MM*SI&&pt2.y()>=0&&pt2.y()<MN*SI)
        {
            int x1=pt1.x()/SI,y1=pt1.y()/SI;
            int x2=pt2.x()/SI,y2=pt2.y()/SI;
            if(m_game.GetZwShu(x1*SI,y1)==-1&&m_game.GetZwShu(x2*SI,y2)==-1)
                std::swap(m_game.spd_mul[x1][y1],m_game.spd_mul[x2][y2]),m_game.m_yg-=50;
        }
    }
}

void MainWindow::RestartGame()
{
    if(!m_game.started)
        return;
    bool bk = false;
    if (!isstop)
        on_stop_clicked (), bk = true;
    if (QMessageBox::question (this, tr ("提示"), tr ("确定要重新开始本关卡吗？")) == QMessageBox::Yes)
    {
        on_btc_clicked();
        if(!m_game.started)
            on_btc_clicked();
    }
    else if(bk)on_stop_clicked ();
}

void MainWindow::mouseReleaseEvent (QMouseEvent *event) {
    if (!m_game.started||isstop)
    {
        Drag(event->pos());
        return;
    }
    if (event->button () == Qt::LeftButton) {
        bool t = false;
        QPoint pt = SPoint (event->pos ().x (), event->pos ().y ());
        if (pt.x () >= (W0+50) && pt.y () >= 60) {
            int x = int ((pt.x () - (W0+50)) / SI) * SI;
            int y = int ((pt.y () - 60) / SI);
            if(select==-1&&wait_guan)
            {
                int tx=x/SI;
                if(tx>0&&tx<MM&&y>=0&&y<MN)
                    kp_noguan[tx][y]^=1;
            }
            t = (select != -1);
            if (select == -2)
            {
                if(!chui)
                    SolveDig(pt);
            }
            else if (select == -4) //大招
                SolveDz (pt);
            else if (imjs||plajs)
                AddJs (pt);
            else if(select != -3||abs(event->pos ().x ()-lmsx)>=5||abs(event->pos ().y ()-lmsy)>=5)//防止大炮原地爆炸
                AddPlant (pt);
            if (!t&&sele_x != -1) //WSAD
            {
                int x = ((pt.x () - (W0+50)) / SI) * SI;
                int y = ((pt.y () - 60) / SI);
                SolveWSAD(x,y);
            }
        }
        if(!t)//手势
            Drag(event->pos());
    }
    else if (event->button () == Qt::RightButton) {
        QPoint pt = SPoint (event->pos ().x (), event->pos ().y ());
        int w=m_game.GetZwShu(sele_x,sele_y),z=-1;
        if(w!=-1)m_game.RightKey(w,pt);
        if (pt.x () >= (W0+50) && pt.y () >= 60)
        {
            int x = int ((pt.x () - (W0+50)) / SI) * SI;
            int y = int ((pt.y () - 60) / SI);
            z=m_game.GetZwShu(x,y);
        }
        if(z!=-1&&m_game.plants[z].type==62)
        {
            Plant&zw=m_game.plants[z];
            if(zw.tag!=-1&&zw.grow>0)
            {
                int t=tyToNumP[zw.tag];
                zw.grow-=1;cd[t]=800;
                if(zw.grow==0)zw.tag=-1;
                select=t;SendDeltaUse(z);
            }
        }
        else SolveDz (pt);
    }
}
void MainWindow::mouseDoubleClickEvent (QMouseEvent *event) {
    if (!m_game.started||isstop)return;
    bool dig=(event->button () == Qt::MiddleButton);
    if (dig) //挖
    {
        QPoint pt = SPoint (event->pos ().x (), event->pos ().y ());
        int x = ((pt.x () - (W0+50)) / SI) * SI;
        int y = ((pt.y () - 60) / SI);
        if (x >= 0 && x <= (MM - 1) * SI && y >= 0 && y < MN && !imjs)
            SolveDig(pt);
    }
    if(event->button () == Qt::RightButton)
    {
        QPoint pt = SPoint (event->pos ().x (), event->pos ().y ());
        int rt=m_game.FindZombie(pt);
        if(rt!=-1)
        {
            for(int i=0;i<m_game.plt_cnt;i++)
                if(m_game.plants[i].y!=-1&&m_game.plants[i].type==55)m_game.plants[i].tag=m_game.zombies[rt].ii;
        }
    }
    else if(event->button () == Qt::LeftButton)
    {
        QPoint pt = SPoint (event->pos ().x (), event->pos ().y ());
        int x = ((pt.x () - (W0+50)) / SI) * SI;
        int y = ((pt.y () - 60) / SI);
        if (x >= 0 && x <= (MM - 1) * SI && y >= 0 && y < MN)
        {
            int w=m_game.GetZwShu(x,y);
            if(w!=-1)m_game.RightKey(w,pt);
            else
            {
                f_shu+=m_game.wz_fei[x/SI][y];
                m_game.wz_fei[x/SI][y]=0;
                int ne[4][2]={1,0,0,1,-1,0,0,-1};
                for(int t=0;t<4;t++)
                    m_game.MoveZw(m_game.GetZwShu(x+ne[t][0]*SI,y+ne[t][1]));
            }
        }
    }
}

void MainWindow::SolveDz (QPoint pt) //处理大招
{
    if(crea)beans=MAXDZS;
    if (beans > 0) {
        int x = int ((pt.x () - (W0+50)) / SI) * SI;
        int y = int ((pt.y () - 60) / SI);
        if (pt.x () >= (W0+50) && pt.y () >= 60 && x >= 0 && x <= (MM - 1) * SI && y >= 0 && y < MN) //位置合法
        {
            int rt = m_game.GetZwShu (x, y);
            if (rt != -1) {
                bool rtn = m_game.DaZhao (rt);
                if (rtn) //成功
                {
                    beans -= 1;
                    SendDeltaDz(rt);
                }
            }
        }
    }
    select = -1;
    if(crea)beans=MAXDZS;
}

void MainWindow::SwiXuan()
{
    if(xuan_st+MXUAN<XUAN)xuan_st+=MXUAN;
    else xuan_st=0;
}

void MainWindow::SolveSel(QPoint pt)
{
    int y = floor(pt.y () / 50.0);
    if (pt.y () > HI-45 && pt.y () <= HI && pt.x () <= W0)
    {
        int x=pt.x();
        if(x >= 55)
        {
            if(beans)
                select = -4,sele_x=-1;
        }
        else if (x < 25)
            select = -2,sele_x=-1;
        else SwiXuan();
    }
    else if (imjs||plajs) {
        if (pt.x () <= W0 && y+1>=0&&y+1 < MXUAN)
            select = x_js[y+1],sele_x=-1;
    }
    else if (pt.x () <= W0 && y+1>=0 && y+1<XUAN && y+1<MXUAN)
        select = m_xuan[(xuan_st+y+1)%XUAN],sele_x=-1;
    int x = int ((pt.x () - (W0+50)) / SI);
    y = int ((pt.y () - 60) / SI);
    if (pt.x () >= (W0+50) && y < MN && x >= 0 && x < MM && m_game.GetZwShu (x * SI, y) != -1) {
        sele_x = x * SI;
        sele_y = y;
    }
}

void MainWindow::mousePressEvent (QMouseEvent *event) //同上
{
    //-1无 -2铲子 -3大炮 -4能量豆
    if(event->button ()==Qt::RightButton)
        ui->label_2->setFocus ();
    lmsx = event->pos ().x ();
    lmsy = event->pos ().y ();
    mi_x=ma_x=lmsx;mi_y=ma_y=lmsy;
    if (!m_game.started||isstop)return;
    if (event->button () == Qt::LeftButton) {
        bool t = (select == -1),tt=t;
        QPoint pt = SPoint (event->pos ().x (), event->pos ().y ());
        if (pt.x () >= (W0+50) && pt.y () >= 60) {
            if (select != -1) {
                if (select == -2)
                    SolveDig(pt);
                else if (select == -4)
                    SolveDz (pt);
                else if (imjs||plajs) {
                    if (!AddJs (pt))
                        t = true;
                }
                else if (!AddPlant (pt))
                    t = true;
            }
            else if (sele_x != -1) {
                int x = ((pt.x () - (W0+50)) / SI) * SI;
                int y = ((pt.y () - 60) / SI);
                SolveWSAD(x,y);
            }
            if(tt)CheckPao(pt);
        }
        else if(pt.x () >= (W0+50) && pt.y () < 60)
        {
            if(pt.x()>=800&&pt.x()<=1100)show_life^=1;
            if(pt.x()>1100&&pt.x()<=1140)select=-2;
            else select=-1;
        }
        else t = true;
        if (t&&select!=-3) SolveSel(pt);
    }
    else if (event->button () == Qt::MiddleButton)
        select=sele_x=sele_y=-1;
}

bool MainWindow::TypeStr(const char*s)
{
    int le=int(strlen(s));
    if(zf_len<le)return 0;
    for(int i=0;i<le;i++)
        if(m_zf[zf_len-le+1+i]!=s[i]-'a'+Qt::Key_A)return 0;
    return 1;
}

void MainWindow::MJ_Kill()
{
    for(int i=0;i<m_game.zmb_cnt;i++)
    {
        m_game.zombies[i].y=-1;
        m_game.zmb_ii[m_game.zombies[i].ii]=-1;
    }
    m_game.zmb_cnt=0;zuobi=1;
    for(int i=0;i<MN;i++)
        m_game.row_zmb[i]=0;
}

void MainWindow::MJ_InfSn()
{
    zuobi=1;inf_sun=true;
    m_game.m_yg=999999;
}

void MainWindow::MJ_AdSn()
{
    zuobi=1;
    m_game.m_yg+=1000;
}

void MainWindow::MJ_ClrCD()
{
    for(int i=0;i<MAXSHU;i++)
        if(cd_spd[i]>0)cd[i]=800;
}

void MainWindow::MJ_Car()
{
    for(int i=0;i<MN;i++)
        if(m_game.cars[i]<=0)
            m_game.cars[i]=1;
}

void MainWindow::KeyDown(int z)
{
    m_zf[++zf_len]=z;
    if(!duiz)
    {
        if(TypeStr("kill"))
            MJ_Kill();
        else if(TypeStr("infsn"))
            MJ_InfSn();
        else if(TypeStr("adsn"))
            MJ_AdSn();
        else if(TypeStr("clrcd"))
            MJ_ClrCD();
        else if(TypeStr("car"))
            MJ_Car();
    }
    if(zf_len>=3&&TypeStr("sun"))//sun指令
        SwitchSun();
    if(z==Qt::Key_Escape)select=sele_x=sele_y=-1;
    else if(z==Qt::Key_C&&can_swi)ChangeZw();
    else if(z==Qt::Key_X)ShowLife();
    /*else if(z==Qt::Key_J&&duiz)plajs^=1;
    else if(z==Qt::Key_B&&duiz&&beans<5&&exp>=20000)beans+=1,exp-=20000,labeanTM=TM;
    else if(z==Qt::Key_F&&duiz&&exp>=25000)z_fs+=1,exp-=25000;*/
    else if(z==Qt::Key_O)OtherPre();
    else if(z==Qt::Key_Z)SwiXuan();
    else if(z==Qt::Key_Q&&crea&&m_game.started)
    {
        int lx,y,z=ui->lineEdit->text().toInt();
        lx=z/10;y=z%10;
        Zombie js=zombies[lx];js.y=y;js.life*=xmul;
        js.x=(MM - (fr_mid?4:1)) * SI + SI / 2;
        if(y>=MN)
        {
            for(int i=0;i<MN;i++)
                js.y=i,m_game.NewJs(js);
        }
        else m_game.NewJs(js);
    }
    else if((z==Qt::Key_L||z==Qt::Key_R)&&m_game.started)
    {
        int w=m_game.GetZwShu(sele_x,sele_y);
        if(w==-1||m_game.plants[w].type!=55)w=-1;
        for(int i=0;i<m_game.plt_cnt;i++)
            if(m_game.plants[i].y!=-1&&m_game.plants[i].type==55&&(w==-1||i==w))
            {
                if(z==Qt::Key_L)m_game.plants[i].tag=m_game.LeftZombie(m_game.plants[i].tag);
                else m_game.plants[i].tag=m_game.RightZombie(m_game.plants[i].tag);
            }
    }
    else if(z==Qt::Key_P)SwitchStop();
    //else if(z==Qt::Key_M&&gqe==7&&select==-2)
    //    on_
    bool b = false;
    if (z == ' ') //暂停
    {
        on_stop_clicked ();
        return;
    }
    if (isstop)
        return;
    if (z >= Qt::Key_1 && z <= Qt::Key_9)//快捷键
        select = z - Qt::Key_1, b = 1,sele_x=-1;
    else if (z == Qt::Key_0)
        select = 9, b = 1,sele_x=-1;
    else if (z == Qt::Key_Minus)
        select = 10, b = 1,sele_x=-1;
    else if(z == Qt::Key_Equal)
        select = 11, b = 1,sele_x=-1;
    else if (z == '`') //铲子
        select = -2,sele_x=-1;
    else if (sele_x != -1) //可以WSAD
    {
        int tx = sele_x, ty = sele_y;
        if (z == Qt::Key_W || z == Qt::Key_Up)
            ty -= 1;
        else if (z == Qt::Key_S || z == Qt::Key_Down)
            ty += 1;
        else if (z == Qt::Key_A || z == Qt::Key_Left)
            tx -= SI;
        else if (z == Qt::Key_D || z == Qt::Key_Right)
            tx += SI;
        SolveWSAD(tx,ty);
    }
    else if (m_b48) //2048
    {
        if (z == Qt::Key_W || z == Qt::Key_Up)
            up2048 ();
        else if (z == Qt::Key_S || z == Qt::Key_Down)
            down2048 ();
        else if (z == Qt::Key_A || z == Qt::Key_Left)
            left2048 ();
        else if (z == Qt::Key_D || z == Qt::Key_Right)
            right2048 ();
    }
    if (b) { //处理选择的项目
        if (imjs||plajs)
            select = x_js[select];
        else if (select >= XUAN)
            select = -1;
        else {
            int t=m_xuan[(xuan_st+select)%XUAN];
            if (m_game.m_yg >= plants[t].cost)
                select = t;
            else
                select = -1;
        }
    }
}

void MainWindow::keyPressEvent (QKeyEvent *event) {
    if (!m_game.started)
        return;
    int z = event->key ();
    KeyDown(z);
}

void MainWindow::ViewStatus () {//查看状态
    if (!m_game.started)
        QMessageBox::information (this, tr (""), tr ("游戏还没有开始"));
    else
    {
        QString str=GetGqStr(real_gq);
        str+=QString::asprintf ("\n总共出现了%d只僵尸，还剩下%d只，进度%.2lf%%\n已进行%.2lf分钟，预计剩余%.2lf分钟\n僵尸生成速度：%.2lfs/个", m_game.jd, zom_cnt-m_game.jd, double (m_game.jd)/zom_cnt*100, TM*25/60000.0/m_spd,(m_ztm  - TM * 25)/ 60000.0/m_spd,tm_js[m_n]/1000.0);
        if(gqe==12)str=GetGqStr(real_gq)+QString::asprintf("\n已完成%d轮",m_n);
        for(int i=0;i<MN;i++)
            str+=QString::asprintf("%d ",m_game.row_zmb[i]);
        str+=QString::asprintf("\ndbg=%d\n",dbg);
        str+=dbg_str;
        QMessageBox::information (this, tr (""),str);
    }
}
void MainWindow::Quit () {
    QCoreApplication::exit(0);
}


void MainWindow::jiasu () {//加速
    bool bk = false;
    if (!isstop)
        on_stop_clicked (), bk = true;
    double sd = m_spd;
    sd += 0.25;
    if (sd > 15)
        sd = 15;
    ui->speed->setValue (sd);
    if (bk)
        on_stop_clicked ();
}
void MainWindow::jiansu () {//减速
    bool bk = false;
    if (!isstop)
        on_stop_clicked (), bk = true;
    double sd = m_spd;
    sd -= 0.25;
    if (sd < 0.25)
        sd = 0.25;
    ui->speed->setValue (sd);
    if (bk)
        on_stop_clicked ();
}
void MainWindow::SwitchSun()
{
    sun_dou^=1;
    if(sun_dou)sun_doued=1;
    if(sun_dou)ui->a_sun->setText("关闭更多阳光");
    else ui->a_sun->setText("开启更多阳光");
}
void MainWindow::ShowLife()
{
    show_life^=1;
    if(show_life)ui->a_xue->setText("隐藏血量");
    else ui->a_xue->setText("显示血量");
}

void MainWindow::on_Short_clicked(bool checked)
{
    if(checked&&gqe!=6&&gqe!=4)
        QMessageBox::warning(this,tr("提示"),tr("开局提供更多阳光\n游戏时长和难度会有所降低"));
}

void MainWindow::SendGameData()
{
    QByteArray wr;
    QDataStream da(&wr,QIODevice::WriteOnly);
    if(duiz)
    {
        da<<4<<TM;
        m_game.output(da);
    }
    else
    {
        int tt=tim.elapsed();
        for(int i=0;i<tmr_cnt;i++)tmr[i].nxt-=tt;
        da<<0;Save(da);
        for(int i=0;i<tmr_cnt;i++)tmr[i].nxt+=tt;
    }
    AddHash(wr);
    for(auto x:tcp)x->write(qCompress(wr));
}

void MainWindow::SendInt(int x)
{
    if(tcp.size()==0)return;
    QByteArray wr;
    QDataStream wd(&wr,QIODevice::WriteOnly);
    wd<<x;AddHash(wr);tcp[0]->write(qCompress(wr));
}

void MainWindow::SendInt(int x,int y)
{
    if(tcp.size()==0)return;
    QByteArray wr;
    QDataStream wd(&wr,QIODevice::WriteOnly);
    wd<<x<<y;AddHash(wr);tcp[0]->write(qCompress(wr));
}

void MainWindow::OnSevChange()
{
    if(tcp.size()==0||sender)return;
    SendGameData();lstcz=MAXCZ;
}

bool MainWindow::DoWSAD(int sx,int sy,int tx,int ty)
{
    sele_x=sx;sele_y=sy;bool rt=0;
    if (tx >= 0 && tx <= (MM - 1) * SI && ty >= 0 && ty < MN
            && wsad_col[tx/SI][ty]!=-1&&(sele_x==-1||wsad_col[tx/SI][ty]==wsad_col[sele_x/SI][sele_y])
            &&m_game.GetZwShu (tx, ty) == -1 && !m_game.IsIcy(tx/SI,ty)
            &&(m_game.gird_type[tx/SI][ty]==0||m_game.gird_type[tx/SI][ty]==3)) {
        int z = m_game.GetZwShu (sele_x, sele_y);
        if (z != -1)m_game.MoveZw(z,tx,ty),rt=1;
        sele_x = tx; sele_y = ty;
    }
    return rt;
}

void MainWindow::SendDeltaWSAD(int sx,int sy,int tx,int ty)
{
    if(tcp.size()==0||sender||duiz)return;
    QByteArray wr;
    QDataStream da(&wr,QIODevice::WriteOnly);
    da<<2<<0<<sx<<sy<<tx<<ty;
    AddHash(wr);tcp[0]->write(qCompress(wr));lstcz=MAXCZ;
}

void MainWindow::SendDeltaPlantZw(const Plant &zw)
{
    if(tcp.size()==0||sender||zw.type==-1||duiz)return;
    QByteArray wr;
    QDataStream da(&wr,QIODevice::WriteOnly);
    da<<2<<1;zw.output(da);
    AddHash(wr);tcp[0]->write(qCompress(wr));lstcz=MAXCZ;
}

void MainWindow::SendDeltaDig(int x,int y)
{
    if(tcp.size()==0||sender||duiz)return;
    QByteArray wr;
    QDataStream da(&wr,QIODevice::WriteOnly);
    da<<2<<2<<x<<y;
    AddHash(wr);tcp[0]->write(qCompress(wr));lstcz=MAXCZ;
}

void MainWindow::SendDeltaDz(int x)
{
    if(tcp.size()==0||sender||duiz)return;
    QByteArray wr;
    QDataStream da(&wr,QIODevice::WriteOnly);
    da<<2<<3<<x;
    AddHash(wr);tcp[0]->write(qCompress(wr));lstcz=MAXCZ;
}

void MainWindow::SendDeltaUse(int x)
{
    if(tcp.size()==0||sender||duiz)return;
    QByteArray wr;
    QDataStream da(&wr,QIODevice::WriteOnly);
    da<<2<<6<<x;
    AddHash(wr);tcp[0]->write(qCompress(wr));lstcz=MAXCZ;
}

void MainWindow::SendDeltaNewJs(const Zombie&js)
{
    if(tcp.size()==0)return;
    QByteArray wr;
    QDataStream da(&wr,QIODevice::WriteOnly);
    da<<2<<4;js.output(da);
    AddHash(wr);tcp[0]->write(qCompress(wr));
}

void MainWindow::SendDeltaExpYg(int expad,int ygad)
{
    if(tcp.size()==0)return;
    QByteArray wr;
    QDataStream da(&wr,QIODevice::WriteOnly);
    da<<2<<5<<expad<<ygad;
    AddHash(wr);tcp[0]->write(qCompress(wr));
}

void MainWindow::SolveWSAD(int tx,int ty)
{
    int sx=sele_x,sy=sele_y;
    if(DoWSAD(sele_x,sele_y,tx,ty))
        SendDeltaWSAD(sx,sy,tx,ty);
}

void MainWindow::SwiMw()
{
    low_mw=(low_mw+1)%3;
}

/*void MainWindow::SwiTmrDelay()
{
    tmr_delay^=1;
    if(tmr_delay)ui->a_eff->setText("关闭低性能模式");
    else ui->a_eff->setText("开启低性能模式");
}*/

void MainWindow::on_bt_oth_clicked()
{
    OtherPre();
}

void MainWindow::SwiDraw()
{
    drty^=1;
    if(drty==0)
    {
        drtmr.start(20);
        gl.hide();
        ui->a_swdr->setText("关闭多线程绘图");
    }
    else
    {
        drtmr.stop();
#ifndef NO_UI
        gl.show();
#endif
        ui->a_swdr->setText("开启多线程绘图");
    }
    ResetIcons();
}

void MainWindow::SwiKJC()
{
    b_kjc^=1;
    if(b_kjc)ui->a_kjc->setText("关闭抗锯齿");
    else ui->a_kjc->setText("开启抗锯齿");
}

void MainWindow::SwiEff()
{
    low_eff^=1;
    if(low_eff)ui->a_tx->setText("开启特效");
    else ui->a_tx->setText("关闭特效");
}

void MainWindow::SwiSound()
{
    b_snd^=1;
    if(b_snd)ui->a_snd->setText("关闭音效");
    else ui->a_snd->setText("开启音效");
}

void MainWindow::OtherPre()
{
    Other *ch = new Other();
    for (int i = 0; i < MAXJSH; i++)
    {
        ch->ico[i].load (QString::asprintf ("://icons/%d.png", od_ico[i]));
        ch->can_del[i]=(to_an[i]!=-1);ch->del_js[i]=del_js[i];
    }
    ch->auto_fire=auto_fire;
    ch->xmul=xmul;
    ch->crea=crea;
    ch->b_two=b_two;
    ch->b_xie=xie;
    ch->b_sun=b_sun;
    ch->car_num=m_game.car_num;
    ch->car_ryg=m_game.car_ryg;
    ch->usermn=usermn;ch->usermm=usermm;ch->usersi=usersi;
    ch->setWindowModality (Qt::ApplicationModal);
    ch->Init ();ch->show ();
    if (ch->exec ())
    {
        for (int i = 0; i < MAXJSH; i++)
            del_js[i] = ch->del_js[i];
        auto_fire=ch->auto_fire;
        xmul=ch->xmul;
        crea=ch->crea;
        b_two=ch->b_two;
        xie=ch->b_xie;
        b_sun=ch->b_sun;
        usermn=ch->usermn;
        usermm=ch->usermm;
        usersi=ch->usersi;
        m_game.car_num=ch->car_num;
        m_game.car_ryg=ch->car_ryg;
    }
}

void MainWindow::on_speed_textChanged(const QString &arg1)
{
    bool ok;
    double sd=ui->speed->text ().toDouble (&ok);
    if(!ok)return;
    m_spd=sd;
    ui->label_2->setFocus ();
    if(m_game.started)
    {
        KillTimer(1001);
        SetTimer(1001,20*(m_spd<1?m_spd:1));
        tm_det=v_tm-m_spd*tim.elapsed();
    }
}

void MainWindow::on_kehu_triggered()
{
    ui->kehu->setDisabled(1);
    ui->fuwu->setDisabled(1);
    if(tcp.size()==0)tcp.push_back(new QTcpSocket());
    QString host=ui->luj->text();
    if(host.isEmpty())host="localhost";
    else
    {
        QFile file (path+"lasthost");
        file.open (QIODevice::WriteOnly);
        QDataStream ar (&file);
        ar<<host;file.close ();
    }
    tcp[0]->connectToHost(host,25641);
    sender=0;
    connect(tcp[0],SIGNAL(readyRead()),&tcpmap,SLOT(map()));
    tcpmap.setMapping(tcp[0],0);
    QTimer *tm=new QTimer(this);
    tm->setSingleShot(1);tm->setTimerType(Qt::PreciseTimer);
    connect(tm,SIGNAL(timeout()),&rdmap,SLOT(map()));
    rdmap.setMapping(tm,0);rdtmr.push_back(tm);
    //connect(tcp[0],&QTcpSocket::readyRead,this,&MainWindow::OnReadData);
}

void MainWindow::on_fuwu_triggered()
{
    ui->kehu->setDisabled(1);
    ui->fuwu->setDisabled(1);
    serv.listen(QHostAddress::Any,25641);
    sender=1;
}

void MainWindow::LoadLast(bool st)
{
    QFile file (path+"lastplay");
    if(file.exists()&&QMessageBox::question(this,"",st?"检测到上次游戏的存档，是否恢复？":"确认要从上次的存档恢复吗？")==QMessageBox::Yes)
    {
        RestoreFromFile(path+"lastplay");
        return;
    }
    else if(!st)QMessageBox::information(this,"","未检测到存档！");
}

void MainWindow::FullScreen()
{
    setWindowState(windowState() ^ Qt::WindowFullScreen);
}

void MainWindow::SwiBG()
{
    bgid=(bgid+1)%BGCNT;
    InitOneIcon(111);
    InitOneIcon(112);
    InitOneIcon(113);
    InitOneIcon(125);
}
