//游戏主类
#include "stdafx.h"
#include "mainwindow.h"
#include "game.h"
#include "./ui_mainwindow.h"

Game::Game () {
    for (int x = 0; x <MAXX; x++)
        for (int y = 0; y < MN; y++)
            fir_plt[x][y] = plt_num[x][y] = -1;
    for (int i = 0; i < MN; i++)
        row_zmb[i] = lst[i] = 0;
    started = zk = false;
    XS = 0;t_ai = is_cycle=is_fog=0;is_day=1;
    zmb_cnt=plt_cnt=shi_cnt=bul_cnt=0;
    for(int i=0;i<100;i++)
        low_zw[i]=dun_p[i]=eat_atk[i]=is_tou[i]=cantwo[i]=yg_rtn[i]=0,fire_mul[i]=has_lev[i]=1;
    yg_rtn[1]=yg_rtn[2]=yg_rtn[6]=yg_rtn[20]=yg_rtn[35]=yg_rtn[19]=yg_rtn[12]=1;
    eat_atk[18]=eat_atk[33]=eat_atk[35]=eat_atk[58]=eat_atk[63]=eat_atk[75]=1;
    is_tou[8]=is_tou[10]=is_tou[56]=is_tou[69]=is_tou[39]=1;
    fire_mul[12]=2;fire_mul[1]=1.2;
    fire_mul[15]=fire_mul[4]=fire_mul[13]=fire_mul[14]=fire_mul[17]=fire_mul[20]=fire_mul[22]
        =fire_mul[23]=fire_mul[24]=fire_mul[25]=fire_mul[31]=fire_mul[32]=fire_mul[33]=1.5;
    dun_p[7]=0.7;dun_p[27]=dun_p[4]=dun_p[31]=0.5;
    low_zw[17]=low_zw[32]=low_zw[53]=low_zw[63]=low_zw[51]=low_zw[75]=1;
    int not_lev[]={7,11,17,15,18,27,30,31,32,33,45,46,47,53,54,62,63,70,71,72,73,3,5};
    for(int i=0;i<23;i++)has_lev[not_lev[i]]=0;
    int can_two[21]={0,56,4,9,64,8,61,67,68,41,44,55,10,69,22,26,60,65,21,23,39};
    for(int i=0;i<21;i++)cantwo[can_two[i]]=1;
    for(int x=0;x<1000;x++)
    {
        if(x<=200)frez_spd[x]=pow(0.5,x/100.0);
        else frez_spd[x]=100.0/(x+200);
        frez_spd_q[x]=sqrt(frez_spd[x]);
    }
}
Game::~Game () {}
void Game::DestroyJs (int z,int rea) //删除僵尸
{
    if(zombies[z].type==25)
        m_yg+=40,laygTM=TM;
    if(zombies[z].type<100&&rea!=-1&&zombies[z].id)
    {
        ShiTi st;
        st.id=zombies[z].id;
        st.x=zombies[z].x;
        st.y=zombies[z].ry;
        st.ty=rea;st.tm=TM;
        NewSt(st);
        if(zombies[z].dun)
            st.id=GetDunIcon(zombies[z]),NewSt(st);
    }
    if (zombies[z].fei) //化肥
    {
        m_wnd->z_fs += 1;m_wnd->beans += 2;
        if (m_wnd->beans>MAXDZS)m_wnd->beans=MAXDZS;
    }
    row_zmb[zombies[z].y] -= 1;
    iinum[zombies[z].ii]=-1;
    zombies[z].life=-1;
    if (SZZ)return; //正在双指针
    zmb_cnt -= 1;
    if (z < zmb_cnt) {
        zombies[z] = zombies[zmb_cnt];
        zombies[z].num = iinum[zombies[z].ii]=z;
    }
}
void Game::CalFirst () //计算植物信息
{
    for (int y = 0; y < MN; y++) {
        for (int x = 0; x < MM * SI + 100; x++)
            fir_plt[x][y] = plt_num[x][y] = -1;
    }
    for (int i = 0; i < plt_cnt; i++)
        plt_num[plants[i].x][plants[i].y] = i;
    for(int x=0;x<MM;x++)
        for(int y=0;y<MN;y++)
            if(m_ng[x][y]&&plt_num[x*SI][y]==-1)
            {
                Plant zw;zw.Create(Qt::black,0,1e8,0,0,1,x*SI,y,-1,0,24);
                plt_num[x*SI][y]=zw.num=plt_cnt;plants[plt_cnt++]=zw;
            }
    for (int i = 0; i < plt_cnt; i++)
        if(((plants[i].type!=29&&plants[i].type!=24)||m_ng[plants[i].x/SI][plants[i].y])&&!plants[i].ti)
            fir_plt[plants[i].x][plants[i].y] = i;
    for (int y = 0; y < MN; y++) {
        for (int x = 1; x < MM * SI + 100; x++) {
            if (fir_plt[x][y] == -1)
                fir_plt[x][y] = fir_plt[x - 1][y]; //第一个植物
        }
    }
}
void Game::DestroyGuan(int z)
{
    int t=plants[z].attack;
    if(t>0)
    {
        if(m_wnd->SHU<MXUAN)
        {
            m_wnd->m_xuan[m_wnd->SHU++]=t-1;
            NewPicSt(plants[z].x,plants[z].y*SI,172);
            XUAN=m_wnd->SHU;
        }
    }
    else
    {
        Zombie js=m_wnd->zombies[(-t)-1];
        js.x=plants[z].x+SI/5;
        js.y=plants[z].y;
        NewJs(js);
    }
}

int Game::DestroyZw (int z,int dig) //删除植物
{
    if(plants[z].type==-1)DestroyGuan(z);
    else if(plants[z].attack==-15)fans-=1;
    if(real&&dig&&yg_rtn[plants[z].type])//返还
        m_yg+=plants[z].cost*0.7*std::max(std::min(double(plants[z].life)/plants[z].mxlf,1.0),0.0),laygTM=TM;
    if(plants[z].frez_stp)m_ice[plants[z].x/SI][plants[z].y]=TM+plants[z].frez_stp*20;
    int tt = m_wnd->FindTimer(z), rt = 0,v_tm=m_wnd->v_tm;//寻找对应计时器
    rt = (tt==-1?0:m_wnd->tmr[tt].nxt - v_tm); //返回这个植物距离发射还有多久(2048)
    if (rt < 0)rt = 0;
    if(real)m_wnd->KillTimer (z);
    plt_cnt -= 1;
    if (z < plt_cnt) {
        plants[z] = plants[plt_cnt];plants[z].num = z;
        if(real)
        {
            int tt = m_wnd->FindTimer(plt_cnt);
            int te = (tt==-1?0:m_wnd->tmr[tt].nxt - v_tm);
            if (te < 0)te = 0;
            m_wnd->KillTimer (plt_cnt); //重置计时器
            if(tt!=-1)m_wnd->NewTimer (z, plants[z].time, te); //更换计时器
        }
    }
    if(dig!=2)CalFirst ();
    return rt;
}
void Game::DestroyZw (int x, int y,int dig) //按照位置删除
{
    int shu = GetZwShu (x, y);
    if (shu != -1)DestroyZw (shu,dig);
}
void Game::DestroyZd (int z) //删除子弹
{
    if (bullets[z].spd < 0&&bullets[z].siz == -2) //小车子弹
        car_driv[bullets[z].y] = false; //清除正在小车的状态
    bul_cnt -= 1;
    if (z < bul_cnt)
        bullets[z] = bullets[bul_cnt];
}
void Game::DestroySt(int w)
{
    shi_cnt-=1;
    if(w<shi_cnt)
        shiti[w]=shiti[shi_cnt];
}
int Game::GetZwShu (int x, int y) const {
    if (x < 0||x >= MAXX||y < 0||y >= MN)
        return -1;
    return plt_num[x][y];
}
bool Game::MeiHuo (int jshu) {
    if (zombies[jshu].type == 11||zombies[jshu].type>=100)
        return false; //不能魅惑BOSS
    if (zombies[jshu].type == 9||zombies[jshu].type == 28||zombies[jshu].type == 12) //小车和巨人
    {
        int t=zombies[jshu].life/2;
        return HurtJs_Fast(jshu,t,0);
    }
    Bullet zd;
    zd.x = zombies[jshu].x;
    zd.y = zombies[jshu].y;
    zd.spd = int(fabs(zombies[jshu].spd)+0.5);
    if(zd.spd<=0)zd.spd=1;
    zd.spd=-zd.spd;
    zd.siz = zombies[jshu].type;
    zd.attack = zombies[jshu].life;
    zd.sx = zombies[jshu].id;
    zd.tag = 0;zd.du=zombies[jshu].eat;
    if(zd.du<1)zd.du=1;
    if(zd.du>5)zd.du=5;
    dps+=zd.attack;
    NewZd (zd);
    DestroyJs (jshu); //把僵尸变为子弹
    return true;
}

int Game::FindZw(int x,int y,int ty)
{
    int ww[10],ss=0;
    for(int a=-1;a<=1;a++)
    {
        for(int b=-1;b<=1;b++)
        {
            int z=GetZwShu((x+a)*SI,y+b);
            if(z!=-1&&plants[z].type==ty)
                ww[ss++]=z;
        }
    }
    if(ss)return ww[rand()%ss];
    else return -1;
}

void Game::ShootZw(int x,int y,int et)
{
    int aa=et/5,bb=et-aa,w=FindZw(x,y,25);
    if(w!=-1)
    {
        plants[w].tag=TM;
        HurtZw(plants[w].x/SI,plants[w].y,bb,1);
        HurtZw(x,y,aa,1);
    }
    else HurtZw(x,y,et,1);
    if(et==13)
    {
        int z=GetZwShu(x*SI,y);
        if(z!=-1)
        {
            plants[z].frez_stp=20;
            int ne[3][2]={0,1,-1,0,0,-1};
            for(int s=0;s<3;s++)
            {
                int t=GetZwShu((x+ne[s][0])*SI,y+ne[s][1]);
                if(t!=-1&&plants[t].frez_stp<5)plants[t].frez_stp=5;
            }
        }
    }
}

bool Game::HurtZw(int i,double et,bool upid)
{
    return HurtZw(plants[i].x/SI,plants[i].y,et,upid);
}

bool Game::HurtZw(int x,int y,double et,bool upid)
{
    int w=GetZwShu(x*SI,y);bool rt=0;
    if(m_ng[x][y]&&w!=-1)
    {
        NewPlantPoint(w,-std::min(m_ng[x][y],et));
        if(et<=10)expad+=3*std::min(m_ng[x][y],et);
        if(et)le_ng[x][y]=TM;
    }
    bool ng=(m_ng[x][y]>0);
    if(ng)
    {
        m_ng[x][y] -= et;
        et = 0; //先吃南瓜
        if (m_ng[x][y] < 0) {
            et = -m_ng[x][y];
            m_ng[x][y] = 0;
            if(ng)CalFirst();
        }
    }
    if(et&&w!=-1&&(!ng||plants[w].type!=29))
    {
        if(plants[w].def)
        {
            int d=plants[w].def;
            if(et<=d)et=(rand()%(d+1)==0?1:0);
            else et=std::fmin(et-d,et/(0.3*d+1));
            if(et>10)et=pow(et,1-0.1*d);
            if(et<=0)return 0;
        }
        NewPlantPoint(w,std::min(plants[w].life,et));
        if(et<=10)expad+=3*std::min(plants[w].life,et);
        plants[w].life -= et;plants[w].la_et=TM;
        if (plants[w].type == 1) {
            if(m_wnd->imjs)
            {
                m_yg += 5 * et;
                if (plants[w].life < 0)
                    m_yg += 5 * plants[w].life;
            }
            else
            {
                ygad += et;
                if (plants[w].life < 0)
                    ygad += plants[w].life;
            }
        }
        if(upid)
        {
            if (plants[w].life <= 0) //吃光
            {
                plants[w].life = 0;
                DestroyZw (w,0);rt=1;
            }
            else plants[w].id = GetZwIcon (plants[w]);
        }
    }
    return rt;
}

bool Game::HurtJs_Fast(int i,int ki,int rea,int ct)
{
    ki*=zombies[i].xu;
    dps+=zombies[i].Hurt(ki,ct/10.0);
    if(ki)NewRedPoint(i,ki);
    bool rt=0;
    if (zombies[i].life <= 0)
        DestroyJs (i,rea),rt=1;
    else
        zombies[i].id = GetJsIcon (zombies[i]);
    return rt;
}

bool Game::HurtJs(int i, int ki, int rea, short frez, int ct, int fire)
{
    if(zombies[i].type==10&&zombies[i].fss!=-1)
        return 0;
    ki=CalHurt(i,ki,frez,fire);
    dps+=zombies[i].Hurt(ki,ct/10.0);
    FreezeJs(i,frez);
    bool rt=0;
    if (zombies[i].life <= 0)
        DestroyJs (i,rea),rt=1;
    else
        zombies[i].id = GetJsIcon (zombies[i]);
    return rt;
}

bool Game::Eat (int zshu, int jshu,double spd,double sq,bool snd) //吃,返回是否被杀死
{
    if(zombies[jshu].type<100&&zombies[jshu].type!=19)//对于大嘴花等植物的判定
    {
        int t=plants[zshu].attack;
        if(plants[zshu].grow<=0&&(t==-4||t==-20||t==-21))
            {Goj(zshu);return 0;}
        t=GetZwShu(plants[zshu].x-SI,plants[zshu].y);
        if(t!=-1&&plants[t].grow<=0&&plants[t].attack==-21)
            {Goj(zshu);return 0;}
    }
    if(plants[zshu].type==-1&&zombies[jshu].type!=9)//罐子
    {
        if(!snd||zombies[jshu].type!=20)
            zombies[jshu].x_flt-=spd;
        zombies[jshu].eating=false;
        return false;
    }
    if (plants[zshu].id == 126&&plants[zshu].life > zombies[jshu].eat&&zombies[jshu].type!=20) //越过全息坚果
    {
        zombies[jshu].x_flt -= spd;plants[zshu].la_et=TM;zombies[jshu].eating=0;
        return false;
    }
    if (zombies[jshu].sp_eat) //特殊吃法
    {
        int t = SpecialEat (plants[zshu], zombies[jshu], *this,spd,sq); //特殊吃法
        if (t)zombies[jshu].id = t;
        return t == 0;
    }
    int x = plants[zshu].x / SI, y = plants[zshu].y;
    double et = sq*zombies[jshu].eat;HurtZw(x,y,et,0);
    if (zombies[jshu].type < 100&&zombies[jshu].eat&&zombies[jshu].type !=11) //吃东西影响生命
    {
        int ki=0,pt=plants[zshu].type,zt=zombies[jshu].type;m_wnd->sd_et=1;
        if ((eat_atk[pt]||plants[zshu].Burning())&&m_ng[x][y]==0) //反伤
        {
            if(pt==58||pt==63||pt==18)//frez
                FreezeJs(jshu,(pt==63?310:80));
            else if(pt==63)FreezeStopJs(jshu,80);
            else if(pt==75)
                zombies[jshu].xu=std::max(zombies[jshu].xu,2.0);
            else if(pt==33&&zt!=12)//大蒜
            {
                int ad=(rand()%2==0?-1:1),yy=zombies[jshu].y;
                if(ad==-1&&yy==0)ad=1;
                else if(ad==1&&yy==MN-1)ad=-1;
                row_zmb[yy]-=1;row_zmb[yy+=ad]+=1;
                if(row_zmb[yy]==1)m_wnd->WakeUp(yy);
                zombies[jshu].y=yy;zombies[jshu].ry=yy*SI;
            }
            if(pt==35&&zt!=12)ki+=60*std::fmin(et,8);
        }
        else ki-=7*et;
        if(zt == 12&&!m_wnd->m_imp)ki+=(m_wnd->m_hd?700:1500);
        if(pt==51&&zombies[jshu].spd>0&&zombies[jshu].spd<6&&zt!=9&&zt!=12&&zt!=16)
            zombies[jshu].spd=6;
        else if(pt==53)
        {
            if(zt!=12)ki=zombies[jshu].life,BaDou(zombies[jshu].x,zombies[jshu].y,1.2);
            else BaDou(zombies[jshu].x,zombies[jshu].y,1.5);
        }
        if(HurtJs_Fast(jshu,ki,0,10))
        {
            if (plants[zshu].life <= 0) //吃光(特判)
                plants[zshu].life = 0,DestroyZw (zshu,0);
            return true;
        }
        else zombies[jshu].id = GetJsIcon (zombies[jshu]); //更新图标
    }
    bool rt = false;
    if (plants[zshu].type == 32&&m_ng[plants[zshu].x/SI][plants[zshu].y] == 0&&!(plants[zshu].mush&&is_day)&&et&&zombies[jshu].type != 9&&zombies[jshu].type != 11&&zombies[jshu].type != 12) //吃魅惑菇
    {
        rt = true;int w=m_wnd->tyToNumZ[zombies[jshu].type];
        zombies[jshu].life=std::max(zombies[jshu].life,m_wnd->zombies[w].life);//恢复
        zombies[jshu].id=m_wnd->zombies[w].id;
        MeiHuo (jshu);plants[zshu].life=0;
    }
    plants[zshu].id = GetZwIcon (plants[zshu]);
    if (plants[zshu].life <= 0) //吃光
        plants[zshu].life = 0,DestroyZw (zshu,0);
    return rt;
}
int Game::GetFirstJs (int x, int y) {
    int zx = 10000, rtn = -1;
    for (int i = 0; i < zmb_cnt; i++) //遍历僵尸
    {
        if (zombies[i].life == -1)
            continue;
        if (zombies[i].x < zx&&zombies[i].x > x&&zombies[i].y == y&&zombies[i].type < 100 &&zombies[i].type!=19) {
            rtn = i;
            zx = zombies[i].x;
        }
    }
    return rtn;
}
int Game::GetLastJs (int x, int y) {
    int zd = -10000, rtn = -1;
    for (int i = 0; i < zmb_cnt; i++) //遍历僵尸
    {
        if (zombies[i].life == -1)
            continue;
        if (zombies[i].x > zd&&zombies[i].x < x&&zombies[i].y == y&&zombies[i].type < 100 &&zombies[i].type!=19) {
            rtn = i;
            zd = zombies[i].x;
        }
    }
    return rtn;
}
int Game::GetFirstZw (int x, int y) {
    if (x <= 0||x-1 >= MAXX)
        return -1;
    return fir_plt[x - 1][y];
}
bool Game::NeedBullet(int y)
{
    if(row_zmb[y])return 1;
    for(int i=0;i<MM;i++)
    {
        int w=GetZwShu(i*SI,y);
        if(w!=-1&&plants[w].frez_stp)return 1;
    }
    return 0;
}

void Game::Goj (int zshu) //植物攻击
{
    int dzsy=plants[zshu].dzsy;bool ladz=0;
    if(dzsy)
    {
        plants[zshu].dzsy-=1;ladz=1;
        if(plants[zshu].dzsy==0)
            QxDaZhao(plants[zshu],m_wnd,*this);
    }
    plants[zshu].nxt_tm = TM + plants[zshu].time / 25;
    if(plants[zshu].stop||plants[zshu].frez_stp)return;
    if (plants[zshu].mush&&is_day)return;
    if (plants[zshu].attack < 0) //非标准植物
        return SpecialPlt (zshu);
    Bullet zd; //发射子弹
    zd.color = plants[zshu].color;
    zd.spd = plants[zshu].move_spd*2;
    if(is_tou[plants[zshu].type])zd.sx=zd.x = plants[zshu].x-SI/2;
    else zd.sx=zd.x = plants[zshu].x+SI/4;
    zd.y = plants[zshu].y;
    zd.tag=plants[zshu].x;
    zd.attack = plants[zshu].attack;
    zd.siz = plants[zshu].siz;
    if(plants[zshu].freeze)zd.freeze=100;
    zd.tx = plants[zshu].x - 1;
    zd.sy = plants[zshu].y;
    zd.jian = plants[zshu].jian;
    zd.left=plants[zshu].left;
    zd.from=plants[zshu].type;
    if(zd.from==67||zd.from==68)zd.du=3;
    if(zd.from==69)zd.du=7;
    if(zd.from==59||zd.from==60)zd.sy=-1;
    if(zd.du)zd.du*=plants[zshu].sp_mul;
    if(!dzsy&&(zd.from==0||zd.from==4||zd.from==9||zd.from==61||zd.from==60||zd.from==65||zd.from==67))
    {
        int r=RA()%100,bj=plants[zshu].bj;
        if(bj>MAXBJ)bj=MAXBJ;
        if(r<bj*10)
        {
            if(r<bj*bj)zd.attack*=4,zd.du*=4,zd.siz*=1.8;
            else zd.attack*=2,zd.du*=2,zd.siz*=1.4;
        }
    }
    else if(plants[zshu].bj)zd.attack*=(1.0+0.05*plants[zshu].bj);
    if(ladz)zd.attack*=5,zd.du*=5,zd.siz*=2;
    if ((plants[zshu].type == 37||plants[zshu].type == 64||plants[zshu].type == 68)&&NeedBullet(plants[zshu].y)) { //豌豆荚
        int t = plants[zshu].tag+1;
        if(plants[zshu].type == 64||plants[zshu].type == 68)
        {
            t=2;
            while(t<10&&RA()%4==0)t+=1;
        }
        zd.y=(zd.y+dzsy)%MN;
        for (int s = 0; s < t; s++) {
            Bullet tz=zd;
            NewZd (tz);
            if(dzsy)zd.y=(zd.y+1)%MN;
            else zd.x += (zd.left?-SI / 3:SI / 3);
        }
    }
    else if ((plants[zshu].type == 44 ||plants[zshu].type == 77)&& (NeedBullet(plants[zshu].y)||dzsy)) { //机枪豌豆
        zd.y=(zd.y+dzsy)%MN;
        for (int s = 0; s <4; s++) {
            Bullet tz=zd;
            if(plants[zshu].type == 44)
            {
                if(s==3)
                {
                    tz.color=DBLUE;tz.freeze=100;
                    if(tz.jian==0)tz.jian=1;
                    else tz.attack+=1000;
                }
                else if(s==2)
                {
                    if(RA()%100==0&& !m_wnd->imjs)//1%魅惑
                        tz.mh=1,tz.color=MHCO;
                    else
                        tz.attack=3000,tz.color=YELLOW,tz.du=4;
                }
                else if(s==1)
                    tz.color=Qt::gray,tz.from=76;
                else if(s==0)
                    tz.color=PURPLE,tz.from=59;
            }
            NewZd (tz);
            if(dzsy)zd.y=(zd.y+1)%MN;
            else zd.x += (zd.left?-SI / 3:SI / 3);
        }
    }
    else if(plants[zshu].type==41)//三线
    {
        zd.x = zd.sx = plants[zshu].x;
        bool fs=0;
        for (int z = -1; z <= 1; z++)
        {
            int y=plants[zshu].y+z;
            if (y>=0&&y<MN&& NeedBullet(y)){fs=1;break;}
        }
        if(fs||dzsy)
        {
            for (int z = -1; z <= 1; z++) {
                int y=(plants[zshu].y+dzsy)%MN+z;
                if (y>=0&&y<MN) {
                    zd.y = y;
                    if(plants[zshu].tag%5==0)
                    {
                        zd.color=RED;
                        if(zd.jian==0)zd.jian=1;
                        else zd.attack+=1;
                    }
                    if(plants[zshu].tag==0)
                        zd.color=DBLUE,zd.freeze=100;
                    NewZd (zd);
                }
            }
        }
        plants[zshu].tag=(plants[zshu].tag+1)%20;
    }
    else
    {
        if(plants[zshu].type==56)
        {
            if(plants[zshu].tag==0||dzsy)
                plants[zshu].color=BUTTER;
            else plants[zshu].color=YUMI;
            plants[zshu].tag=(plants[zshu].tag+1)%5;
        }
        else if(plants[zshu].type==39)//仙人掌
        {
            bool bk=false;zd.siz=2;
            for(int i=0;i<zmb_cnt;i++)
            {
                if(zombies[i].type==19&&zombies[i].y==plants[zshu].y)
                    {bk=true;break;}
            }
            if(bk)zd.siz=1;//找到气球
        }
        if(NeedBullet(plants[zshu].y)||dzsy)
            NewZd(zd);
    }
}
void Game::NewZd (Bullet zd) {
    if (bul_cnt > 990) //上限
        return;
    bullets[bul_cnt++] = zd;
}
void Game::NewJs (Zombie js) {
    if (zmb_cnt > 790) //上限
        return;
    if(js.ry<0)js.ry=js.y*SI;
    js.x_flt=js.x;
    row_zmb[js.y] += 1;
    if (row_zmb[js.y] == 1)//唤醒
        m_wnd->WakeUp (js.y);
    js.ii=iis;int t=js.life;
    js.dun=int(t*dun_p[js.type]);
    js.life=t-js.dun;
    js.mxlf=js.life;
    js.mxdun=js.dun;
    zombies[zmb_cnt] = js;
    zombies[zmb_cnt].num = zmb_cnt;
    iinum[iis++]=zmb_cnt ++;
}
void Game::NewSt(ShiTi st)
{
    if (shi_cnt > 1990) //上限
        return;
    if(m_wnd->b_fast)return;
    if(m_wnd->low_eff&&(st.ty==0||st.ty==3||st.ty==4||st.ty==6||st.ty==10))
        return;
    shiti[shi_cnt++]=st;
}
void Game::MoveZw(int z,int x,int y)
{
    if(z==-1)return;
    int t=m_wnd->FindTimer(z),ne=0;
    if(t!=-1)
    {
        ne=m_wnd->tmr[t].nxt-m_wnd->v_tm;
        m_wnd->KillTimer (z);
    }
    ClearLev(plants[z]);
    if(x>=0)plants[z].x = x, plants[z].y = y;
    PlantOnWz(plants[z]);
    plants[z].id = GetZwIcon (plants[z]);
    if(t==-1)m_wnd->SetTimer (z, plants[z].time);//更新计时器
    else m_wnd->NewTimer(z,plants[z].time,ne);
    CalFirst ();
}

void Game::ChangeLev (Plant &zw,int f) { //施肥
    if (!has_lev[zw.type]||zw.dzsy)return;
    zw.lev=f;double tm=(f<10?lev_mul[f]:lev_mul[9]+0.3*(f-9));
    zw.time/=pow(tm,0.3);zw.sp_mul=pow(tm,0.7);
    if(zw.attack>0)zw.attack*=zw.sp_mul;
    if(f>=3)
    {
        if(zw.jian==0&&zw.attack>0)zw.attack/=2;
        zw.jian+=(f-1)/2;
    }
    if(f>=4&&f<=10)zw.bj+=(f-2)/2;
    if(f>10)zw.bj+=(f-10)/4;
    if(zw.bj>MAXBJ&&zw.attack>0)zw.attack+=400*(zw.bj-MAXBJ);
    double k=1+0.6*f,b=pow(f,1.5)*50;
    zw.life=int(k*zw.life+b+0.5);
    zw.mxlf=int(k*zw.mxlf+b+0.5);
    double p=spd_mul[zw.x/SI][zw.y];
    zw.time/=p;zw.life*=p;zw.mxlf*=p;
    zw.id = GetZwIcon (zw);
    if(zw.time<20)zw.time=20;
}

void Game::ClearLev(Plant &zw)
{
    double p=spd_mul[zw.x/SI][zw.y];
    zw.time*=p;zw.life/=p;zw.mxlf/=p;
    int f=zw.lev;zw.lev=0;
    if(f>0)
    {
        if(zw.bj>MAXBJ&&zw.attack>0)zw.attack-=400*(zw.bj-MAXBJ);
        if(f>=4&&f<=10)zw.bj-=(f-2)/2;
        if(f>10)zw.bj-=(f-10)/4;
        if(f>=3)
        {
            zw.jian-=(f-1)/2;
            if(zw.jian==0&&zw.attack>0)zw.attack*=2;
        }
        double tm=(f<10?lev_mul[f]:lev_mul[9]+0.3*(f-9));
        zw.time*=pow(tm,0.3);
        if(zw.attack>0)zw.attack/=pow(tm,0.7);
        double k=1+0.6*f,b=pow(f,1.5)*50;
        zw.life=int((zw.life-b)/k+0.5);
        zw.mxlf=int((zw.mxlf-b)/k+0.5);
    }
    if(zw.life<=0)zw.life=1;
}

int Game::CountFei(int x,int y)
{
    int rt=wz_fei[x][y];
    int ne[4][2]={1,0,0,1,-1,0,0,-1};
    for(int t=0;t<4;t++)
    {
        int a=x+ne[t][0],b=y+ne[t][1];
        if(a>=0&&a<MM&&b>=0&&b<MN&&wz_fei[a][b]==5)rt+=1;
    }
    return rt;
}

void Game::PlantOnWz(Plant &zw)
{
    int f=CountFei(zw.x/SI,zw.y);
    f=std::min(f+zw.levod,100);
    ChangeLev(zw,f);
}
bool Game::ShiFei(int x,int y)
{
    int w=GetZwShu(x*SI,y);
    if ((w == -1||plants[w].dzsy==0)&& wz_fei[x][y]<5)
    {
        int s=(m_wnd->crea?5:1);
        while(s--)
        {
            wz_fei[x][y]+=1;
            MoveZw(GetZwShu(x*SI,y));
            if(wz_fei[x][y]>=5)
            {
                int ne[4][2]={1,0,0,1,-1,0,0,-1};
                for(int t=0;t<4;t++)
                MoveZw(GetZwShu((x+ne[t][0])*SI,y+ne[t][1]));
                break;
            }
        }
        return true;
    }
    return false;
}

int Game::FakePlant(const Plant&zw)
{
    int w=GetZwShu(zw.x,zw.y);
    switch(zw.type)
    {
    case 66: //血瓶
        if(w!=-1) {
            int mx=plants[w].mxlf;
            plants[w].life+=0.8*zw.cost/plants[w].cost*mx+0.5;
            if(plants[w].life>mx)plants[w].life=mx;
            return 1;
        }
        else return 0;
    case 38: //化肥
        return ShiFei(zw.x/SI,zw.y);
    case 24: //南瓜
        m_ng[zw.x / SI][zw.y] = 600;
        CalFirst();return 1;
    case 28: //咖啡豆
        if (w != -1&&plants[w].mush) {
            plants[w].mush = false;
            if(plants[w].odtm<=5)
            Goj(w);return 1;
        }
        else return 0;
    case 14: //荷叶
        if (m_keng[zw.x / SI][zw.y] == 2) {
            m_keng[zw.x / SI][zw.y] = 3;
            m_hy[zw.x / SI][zw.y]=800;return 1;
        }
        else return 0;
    case 43: //翻转
        if (w != -1&&plants[w].attack>0){
            plants[w].left^=1;
            return 1;
        }
        else return 0;
    case 49:
        return (w!=-1&&plants[w].Burn());
    case 50:
        if(w!=-1&&plants[w].Burning()) {
            plants[w].fire=FIRETM+1;
            return 1;
        }
        else return 0;
    case 52:
        if(w!=-1){plants[w].stop^=1;return 1;}
        return 0;
    }
    if (zw.type == 37) { //豌豆荚
        if (w != -1&&plants[w].type == 37) {
            if (plants[w].tag == 4)return 0;
            plants[w].tag += 1;
            plants[w].id = GetZwIcon (plants[w]);
            return 1;
        }
    }
    if(zw.type==20)//双子向日葵
    {
        if (w != -1&&plants[w].type == 20&& plants[w].time!= 100) {
            if (plants[w].tag == 1)return 0;
            plants[w].tag = 1;
            plants[w].id = GetZwIcon (plants[w]);
            return 1;
        }
    }
    if(m_wnd->b_two&&cantwo[zw.type])//双倍
    {
        if (w != -1&&plants[w].type == zw.type&& plants[w].time!= 100&& plants[w].lev==0) {
            if (plants[w].grow == 1)return 0;
            plants[w].time/=2;plants[w].grow=1;
            m_wnd->KillTimer(plants[w].num);
            m_wnd->SetTimer(plants[w].num,plants[w].time);
            return 1;
        }
    }
    if(plants[w].type==62&&zw.cost>0)//保存盒子
    {
        if(plants[w].grow<4&&(plants[w].tag==-1||plants[w].tag==zw.type))
        {
            plants[w].tag=zw.type;m_yg+=zw.cost*4/5;
            plants[w].grow+=1;return 1;
        }
        return 0;
    }
    return 2;
}

bool Game::PlantZw (Plant zw) {
    if(zw.type==3&&zw.cost==0)
    {
        NewZw(zw);
        return true;
    }
    if(m_yg<zw.cost||(IsIcy(zw.x/SI,zw.y)&&zw.type!=5&&zw.type!=3&&zw.type!=72))return false;
    if (m_keng[zw.x / SI][zw.y] == 1||(m_keng[zw.x / SI][zw.y] == 2&&zw.type != 14)) //有坑，或有水且不是荷叶
    {
        int co=m_wnd->plants[21].cost;
        if(m_wnd->m_ai&&m_keng[zw.x / SI][zw.y]==2&&m_yg>=zw.cost+co&&m_wnd->cd[21]>800-1e-9)
        {
            m_yg-=co;m_wnd->cd[21]=0;
            m_keng[zw.x / SI][zw.y] = 3;m_hy[zw.x / SI][zw.y] = 800;
        }
        else return false;
    }
    m_yg -= zw.cost;
    int rt=FakePlant(zw);
    if(rt==1)return true;
    else if(rt==0)
    {
        m_yg += zw.cost;
        return false;
    }
    int w=GetZwShu(zw.x,zw.y);
    if (w != -1&&plants[w].type!=24) //非空
    {
        m_yg += zw.cost;
        return false;
    }
    if(zw.type==55||zw.type==62)zw.tag=-1;
    if(zw.type==71)zw.tag=-1e9;
    if(w!=-1&&zw.type!=3)DestroyZw(w,2);
    if(zw.attack==-15)fans+=1;
    if(zw.type==80)zw.tag=int(zw.life*1000);
    if (zw.type==17||zw.type==30||zw.type==27||zw.type==54)//土豆和炮
        zw.grow=zw.move_spd;
    PlantOnWz(zw);NewZw(zw);
    return true;
}

void Game::NewZw(Plant zw)
{
    int i = plt_cnt,tt=zw.odtm=zw.time,ak=zw.attack;
    zw.time=(zw.time == 0?(zw.type==7?300:3000):zw.time);
    if(tt==0&&ak!=-4&&ak!=-6&&ak!=-7&&!(ak==-2&&zw.cost==0))
        zw.time=tt=300,zw.life=1e8,zw.tag=TM;
    plants[i] = zw;
    plants[i].num = i;
    plants[i].id = GetZwIcon (plants[i]);
    plt_cnt += 1;
    if (zw.nxt_tm == -1)//初始状态
        m_wnd->NewTimer (i, zw.time, zw.type==1||zw.type==20||zw.type==12?zw.time/2:zw.time);
    else
        m_wnd->NewTimer (i, zw.time, zw.nxt_tm);
    plants[i].nxt_tm=TM+zw.time/25;
    if (tt==0||ak==-21||ak==-15)//瞬时
        Goj (i);
    CalFirst ();//更新
}

bool Game::IsIcy(int x,int y)const
{
    return TM < m_ice[x][y];
}

bool Game::JsCai(int shu)
{
    int tx = (zombies[shu].x + SI/2) / SI, ty = zombies[shu].y,tz=GetZwShu(tx*SI,ty);
    if(tx >= 0&&tx < MM)
    {
        if (plants[tz].type==29&&zombies[shu].type < 100&&zombies[shu].type != 8 &&zombies[shu].type!=19&&!(zombies[shu].type==10&&zombies[shu].fss!=-1)) {//除了跳跳之外受到地刺伤害
            if (zombies[shu].type == 9||zombies[shu].type == 28||zombies[shu].type == 11) //踩扁地刺
            {
                int t=std::fmin(5,plants[tz].life);
                bool ng=(m_ng[tx][ty]>0);
                if(ng)
                {
                    m_ng[tx][ty]=0;
                    CalFirst();
                }
                HurtZw(tz,t,1);
                if(HurtJs_Fast(shu,t*1000,0,10))return true;
            }
            else if (zombies[shu].type == 12) //扎车
            {
                dps+=zombies[shu].life/2;
                zombies[shu].life/=2;zombies[shu].type=29;
                HurtZw(tz,20,1);
            }
            else if(HurtJs_Fast(shu,60*spd_mul[tx][ty],0,10))return true;
        }
        if (zombies[shu].type == 12 &&m_wnd->gqe != 5)
        {
            int tt=500-m_wnd->m_ea*100;
            if(m_wnd->m_hd)tt=600;
            if(m_wnd->m_imp)tt=800;
            m_ice[tx][ty] = TM+tt;//小车生成冰
        }
        if (zombies[shu].type != 19&& m_keng[tx][ty] == 3&&zombies[shu].type < 100&&GetZwShu (tx * SI, ty) == -1)//除了气球之外可以踩掉荷叶
        {
            m_hy[tx][ty]-=1;
            if(m_hy[tx][ty]<=0)
                m_keng[tx][ty] = 2;
        }
        if(zombies[shu].type<100&&js_nd[tx][ty]>=0)
        {
            int t=zombies[shu].type,d=zombies[shu].id;
            if(d==3||d==17||d==18)t=0;
            if(js_nd[tx][ty]==t)js_nd[tx][ty]=-2;
            else js_nd[tx][ty]=-3;
        }
    }
    if (plants[tz].ti &&zombies[shu].type < 100&&zombies[shu].type != 8 &&zombies[shu].type!=19&&!(zombies[shu].type==10&&zombies[shu].fss!=-1))
    {
        double t=2.0/SI;
        if(zombies[shu].eat>=10)t*=10;
        HurtZw(tz,t,1);
    }
    return false;
}

void Game::ZhangNan(int shu)
{
    for (int i = 0; i < zmb_cnt; i++) {
        if (zombies[i].type >= 100||zombies[i].life == -1||zombies[i].type == 11||zombies[i].type == 16)
            continue;
        if (zombies[i].x >= zombies[shu].x - SI&&zombies[i].x <= zombies[shu].x + SI&&zombies[i].y >= zombies[shu].y - 1&&zombies[i].y <= zombies[shu].y + 1&&zombies[shu].x>=SI)
            zombies[i].x_flt -= 2;//瞬移
    }
    if((zombies[shu].x>SI&&zombies[shu].x%4<2)||(zombies[shu].x<=SI&&zombies[shu].x%10<2))
    {
        for (int i = 0; i < plt_cnt; i++) { //使用咒骂伤害植物
            if (plants[i].life != -1&&plants[i].y == zombies[shu].y&&plants[i].type!=-1)
                if(HurtZw(i,(plants[i].mxlf+79)/80,1))i-=1;
        }
    }
}

bool Game::JsInHome(int shu)
{
    if(!real||zombies[shu].type >= 100)//僵尸的子弹
    {
        DestroyJs (shu);
        return true;
    }
    if (zombies[shu].type == 16)//末影人
    {
        zombies[shu].x=MM*SI;
        return false;
    }
    if (zombies[shu].type == 11) //BOSS
    {
        zombies[shu].spd = -20;
        return false;
    }
    if (car[zombies[shu].y]) //车
    {
        int ty = zombies[shu].y;
        if(!m_wnd->crea&&!m_wnd->is82)car[ty] = false;
        car_driv[ty] = true;car_TM[ty]=TM;
        Bullet zd;//创建子弹
        zd.attack = 2e9;zd.siz = -2;
        zd.y = ty;zd.x = -150;
        zd.spd = -4;zd.sx = 28;zd.du=1;
        NewZd (zd);DestroyJs (shu,0);
        bool bb = false;
        for (int i = 0; i < MN; i++) {
            if (car[i]) {
                bb = true;
                break;
            }
        }
        if (!bb&&m_wnd->imjs) {//我是僵尸
            m_wnd->GameOver ();
            QMessageBox::information (m_wnd, QObject::tr ("提示"), QObject::tr ("胜利"));
        }
        if(m_wnd->duiz)expad+=15000,m_yg+=2000,ygad+=300,lstcar=TM;
        return true;
    }
    else if (!car_driv[zombies[shu].y]) {//正在使用车
        if (m_wnd->imjs) {
            DestroyJs (shu);
            return true;
        }
        over = true;
        int s = 0;
        for (int i = 0; i < zmb_cnt; i++) {
            if (zombies[i].type < 100)
                s += 1;//计算分数
        }
        m_wnd->GameOver ();
        int fs=jd - (m_wnd->is11 ? 0 : s);
        if(m_wnd->b_guan)fs=m_wnd->m_n;
        int fm=zjd;
        if(m_wnd->b_guan)fm=20;
        if(!m_wnd->b_fast||!m_wnd->m_ai)
            QMessageBox::information (m_wnd, QObject::tr ("提示"), QString::asprintf ("失败！\r\n得分:%d 完成度:%.2lf%%", fs,std::min(double(fs)/fm,1.0)*100.0));
        else m_wnd->ai_2+=std::min(double(fs)/fm,1.0);
    }
    return false;
}

void Game::DuTick()
{
    for(int i=0;i<zmb_cnt;i++)
    {
        int du=zombies[i].du*10;
        if(zombies[i].freeze)du*=(1+zombies[i].freeze*0.003);
        if(HurtJs_Fast(i,du,0,10))i-=1;
    }
}

bool Game::MoveJs (int shu) { //移动僵尸,返回是否被杀死
    zombies[shu].xu=std::max(zombies[shu].xu-0.0015/(1.0+zombies[shu].du*0.03),1.0);
    if(zombies[shu].type==10&&zombies[shu].fss!=-1)//小鬼投掷
    {
        zombies[shu].x_flt-=9;
        if(zombies[shu].x<=zombies[shu].fss)
            zombies[shu].x=zombies[shu].fss,zombies[shu].fss=-1;
        return 0;
    }
    if(JsCai(shu))return true;
    if(zombies[shu].type<100)expad+=1;
    if(TM<zombies[shu].frez_stp)return 0;
    zombies[shu].freeze = std::max(zombies[shu].freeze-1,0);
    if(TM<zombies[shu].stp_tm)return 0;
    bool rt = false;int f=zombies[shu].freeze;
    double s_p=(f<1000?frez_spd[f]:1.0/12.0),spd=zombies[shu].spd*s_p,s_q=(f<1000?frez_spd_q[f]:0.2887);
    int i = GetFirstZw (zombies[shu].x+(zombies[shu].spd<0?SI/2:0), zombies[shu].y);
    zombies[shu].x_flt -= spd;
    if(zombies[shu].spdy!=0)
    {
        zombies[shu].ry+=zombies[shu].spdy;
        int oy=zombies[shu].y,ny=int(zombies[shu].ry/SI+0.5);
        if(oy!=ny)
        {
            zombies[shu].y=ny;
            row_zmb[oy]-=1;row_zmb[ny]+=1;
            if(row_zmb[ny]==1)m_wnd->WakeUp(ny);
        }
        if(zombies[shu].ry<0&&zombies[shu].spdy<0)zombies[shu].spdy=-zombies[shu].spdy;
        if(zombies[shu].ry>(MN-1)*SI&&zombies[shu].spdy>0)zombies[shu].spdy=-zombies[shu].spdy;
    }
    zombies[shu].eating=false;
    if (i != -1&&zombies[shu].x -SI/2 <= plants[i].x) {//吃
        zombies[shu].x_flt += spd;
        zombies[shu].eating = true;//正在吃
        rt = Eat (i, shu,spd,s_q);
        if (rt)return true;
    }
    if(!zombies[shu].eating) {
        if (zombies[shu].type == 9||zombies[shu].type == 28)//巨人下落
            zombies[shu].sta %= 16;
        if(IsIcy((zombies[shu].x + SI/2) / SI,zombies[shu].y))
            zombies[shu].x_flt-=0.5;
    }
    if (zombies[shu].x < -115&&!over)
        return JsInHome(shu);
    if (zombies[shu].x > MM * SI + 20&&zombies[shu].type != 11&&zombies[shu].spd < 0) {
        if (zombies[shu].type >= 100) {//子弹
            DestroyJs (shu);
            return true;
        }
        zombies[shu].spd = -zombies[shu].spd;
    }
    if (zombies[shu].shoot) {//发射
        zombies[shu].fss += int(1000*s_q+0.5);
        if (zombies[shu].fss >= zombies[shu].fs_nd) //发射时间
        {
            zombies[shu].fss = 0;
            rt=JsShoot (zombies[shu], *this);
        }
    }
    return rt;
}
void Game::Crepeer (int i) {
    if (zombies[i].type == 15) //苦力怕
    {
        int x = (zombies[i].x / SI) * SI, y = zombies[i].y;
        NewPicSt(x,y*SI,128);
        for (int a = -2; a <= 1; a++) {
            for (int b = -1; b <= 1; b++) {//4*3爆炸
                int t=GetZwShu (x + a * SI, y + b);
                if (t != -1)
                {
                    if(a==0&&b==0)plants[t].Burn();
                    ShootZw(x/SI+a,y+b,m_wnd->m_hd?100:60);
                }
            }
        }
    }
    DestroyJs (i,1);
}
void Game::Explode(int x,int y,int r,int hu,int de)
{
    NewPicSt(x,y*SI,128);
    for (int a = -r; a <= r; a++) {
        for (int b = abs(a)-r; b <= r-abs(a); b++) {//爆炸
            int t=GetZwShu (x + a * SI, y + b);
            if (t != -1)
            {
                plants[t].Burn();
                ShootZw(x/SI+a,y+b,hu-(abs(a)+abs(b))*de);
            }
        }
    }
}
Bullet p_zid[5000];Zombie p_jsh[1000];
bool cmp1 (int a, int b) {
    return p_zid[a].x < p_zid[b].x;
}
bool cmp2 (int a, int b) {
    if(p_jsh[a].x == p_jsh[b].x)
    {
        if(p_jsh[a].freeze == p_jsh[b].freeze)
            return p_jsh[a].xu < p_jsh[b].xu;
        return p_jsh[a].freeze < p_jsh[b].freeze;
    }
    return p_jsh[a].x < p_jsh[b].x;
}
void Game::NewNumberPoint(int x,int y,int z,int co)
{
    if(z<500)return;
    ShiTi st;st.tm=TM;
    st.x=x;st.y=y;st.ty=10;
    st.id=z*100+co;NewSt(st);
}
void Game::NewRedPoint(int i,int si,bool show_num)
{
    ShiTi st;st.x=zombies[i].x+rand()%7-3;
    st.y=zombies[i].ry;st.tm=TM;st.ty=3;
    st.id=si;NewSt(st);
    if(show_num&&!no_num&&si>=500)
        st.ty=10,st.id=st.id*100+1,NewSt(st);
}
void Game::NewBluePoint(int i,int zz)
{
    if(zz==0)return;
    ShiTi st;st.x=zombies[i].x;
    st.y=zombies[i].ry;st.tm=TM;st.ty=6;
    st.id=std::min(zz,1000)/3;NewSt(st);
}
void Game::NewPlantPoint(int i,int si)
{
    ShiTi st;st.x=plants[i].x+rand()%11-5+SI/3;
    st.y=plants[i].y*SI;st.tm=TM;st.ty=4;
    st.id=si*10000+plants[i].type;
    if(si<0)st.id=(-si)*10000+24;//南瓜
    NewSt(st);
}
void Game::JieDong(int x,int y)
{
    if(y>plants[x].frez_stp)y=plants[x].frez_stp;
    plants[x].frez_stp-=y;
    ShiTi st;st.x=plants[x].x+rand()%11-5+SI/3;
    st.y=plants[x].y*SI;st.tm=TM;st.ty=4;
    st.id=y*10000+73;
    NewSt(st);
}
bool Game::CannotReach(int zx,int i)const
{
    return zombies[i].x <= zx||zombies[i].life < 0||zombies[i].type==19||(zombies[i].type==10&&zombies[i].fss!=-1);
}
void Game::BulletOverZw(Bullet&zd)
{
    int x = (zd.x / SI) * SI,y = zd.y,t = GetZwShu (x, y);
    if(t==-1||x==zd.tag||zd.from==-1)return;
    zd.tag=x;
    if (plants[t].type == 36||plants[t].Burning()) { //火炬
        bool ji=0;
        if (zd.color == GREEN)//普通豌豆
            zd.color = RED,zd.attack+=1000,ji=1;
        else if (zd.color == BLUE)//寒冰
            zd.color = DBLUE,zd.attack+=1000,ji=1;
        else if (zd.color == YELLOW)
            zd.color = LYELLOW,zd.attack*=(1.2+plants[t].lev*0.2),ji=1;
        else if (zd.color == RED||zd.color==DBLUE)
            zd.attack+=1000,zd.siz+=2;
        if(ji&&zd.jian==0)zd.jian=1,zd.attack-=1000;
        zd.attack+=plants[t].lev*300;
    }
    if (plants[t].type == 58) { //冰火炬
        if (zd.color == GREEN)//普通豌豆
            zd.color = BLUE,zd.freeze=100;
        else if (zd.color == RED)//火焰
            zd.color = DBLUE,zd.freeze=100;
        else if (zd.color == YELLOW)
            zd.color = FYELLOW,zd.attack*=(1.2+plants[t].lev*0.2),zd.freeze=150;
        else if(zd.color == BLUE||zd.color==DBLUE)
            zd.attack+=500,zd.siz+=2,zd.freeze+=50;
        zd.attack+=plants[t].lev*300;
    }
    if(zd.freeze&&!is_tou[zd.from]&&plants[t].freeze)zd.freeze+=20;
}

int Game::CalHurt(int zi,int ki,int frez,int fire)
{
    if(fire)
    {
        double t=(fire==1?1.5:std::max(1.0,log(ki/2000.0))+0.7);
        int d=zombies[zi].freeze;
        if(fire==1&&d>50)d=std::max(int(d*0.5),50);
        int ad=int(pow(d,1.6)*t+0.5);//冰火叠加增加伤害;
        if(fire==1&&ki>1000)ad*=(1.0+(ki-1000)/3000.0);
        ki+=ad;zombies[zi].freeze -= d;
        ki*=fire_mul[zombies[zi].type];
        ShiTi st;st.tm=TM;st.id=zombies[zi].ii;
        st.ty=7;NewSt(st);
    }
    ki*=zombies[zi].xu;
    if(fire==2&&zombies[zi].type==26&&zombies[zi].life<ki)
        JsShoot(zombies[zi],*this);
    if(ki)NewRedPoint(zi,ki);
    return ki;
}

void Game::BulletOnJs(int bi,int zi,bool tou)
{
    int g=bullets[bi].attack,f=bullets[bi].from;
    short hb=bullets[bi].freeze,zt=zombies[zi].type,ft=0;
    if(bullets[bi].color==DBLUE)hb+=60;
    if(zt==19&&bullets[bi].from==39)g*=5;//气球僵尸
    if(g>1000)
    {
        int t=(g-1000)/100;
        hb+=hb*t/100;
    }
    if(hb>100)ft=std::min((hb-100)/4,20);
    int ct=(f==59?10:(tou?5:0));//大喷菇投手穿透
    if(!tou&&zombies[zi].dun)hb/=2,ft/=2;
    g=CalHurt(zi,g,hb,(bullets[bi].color==RED?1:0));
    dps+=zombies[zi].Hurt(g,ct/10.0);
    FreezeJs(zi,hb,ft);
    if(bullets[bi].color==DBLUE)
    {
        ShiTi st;st.tm=TM;st.id=zombies[zi].ii;
        st.ty=8;NewSt(st);
    }
    zombies[zi].du+=bullets[bi].du;
    if(bullets[bi].from==76)zombies[zi].xu+=0.1;
}

Bullet Game::MhJsSkill(int bi)
{
    Bullet zd;zd.siz=-1;bullets[bi].tag += 1;
    if (bullets[bi].siz == 13) {//豌豆僵尸魅惑后打子弹
        if (bullets[bi].tag >= 75)
            bullets[bi].tag = 0,zd=GetFanTan(75);
    }
    else if (bullets[bi].siz == 14) {//西瓜僵尸魅惑后打子弹
        if (bullets[bi].tag >= 75)
            bullets[bi].tag = 0,zd=GetFanTan(81);
    }
    else if (bullets[bi].siz == 22) {//冰瓜僵尸魅惑后打子弹
        if (bullets[bi].tag >= 125)
            bullets[bi].tag = 0,zd=GetFanTan(183);
    }
    else if (bullets[bi].siz == 25) {//向日葵僵尸魅惑后产生阳光
        if (bullets[bi].tag >= 500)
            bullets[bi].tag = 0,m_yg+=25;
    }
    zd.x = bullets[bi].x;zd.y = bullets[bi].y;
    return zd;
}

void Game::MianYiCheck()
{
    for (int i = 0; i < zmb_cnt; i++) {
        if(zombies[i].type == 12||zombies[i].type == 11||zombies[i].type == 19||zombies[i].type >= 100)
            zombies[i].stp_tm=zombies[i].frez_stp=-1e8,zombies[i].freeze=0;
        if(zombies[i].type==11||zombies[i].type==24||zombies[i].type>=100)
            zombies[i].du=0;
        if(zombies[i].type == 11)zombies[i].xu=1.0;
        if(zombies[i].du>MAXDU)zombies[i].du=MAXDU;
        if(zombies[i].xu>MAXXU)zombies[i].xu=MAXXU;
        if(zombies[i].freeze>MAXFREZ)zombies[i].freeze=MAXFREZ;
    }
}

void Game::MoveAllZd () //双指针
{
    SZZ = no_num = true;MianYiCheck();
    double sd = m_wnd->m_spd/ceil(m_wnd->m_spd);
    int zs[MMN] = {0}, js[MMN] = {0},ns = 0;
    for (int i = 0; i < bul_cnt; i++) {
        BulletOverZw(bullets[i]);
        int y = bullets[i].y;
        pd[y][zs[y]++] = i;
        p_zid[i] = bullets[i];
    }
    for (int i = 0; i < zmb_cnt; i++) {
        if (zombies[i].type >= 100)
            continue;
        int y = zombies[i].y;
        pj[y][js[y]++] = i;
        p_jsh[i] = zombies[i];
    }
    for (int y = 0; y < MN; y++) { //枚举行
        std::sort (pd[y], pd[y] + zs[y], cmp1);
        std::sort (pj[y], pj[y] + js[y], cmp2);
        for (int a = 0, b = 0; a < zs[y]; a++) {
            const int bul_i = pd[y][a], zx = bullets[bul_i].x;
            bool mh_stp=0;int mv=0;
            if (bullets[bul_i].spd > 0)
            {
                mv=bullets[bul_i].spd * sd;
                if(mv==0)mv=1;
                if(bullets[bul_i].left)mv=-mv;
            }
            else //魅惑
            {
                int x = (bullets[bul_i].x / SI) * SI, t = GetZwShu (x, y);
                if(t!=-1&&plants[t].type==6)mh_stp=1;//高坚果阻挡
                bullets[bul_i].freeze^=1;
                if((!mh_stp&&bullets[bul_i].freeze)||bullets[bul_i].siz==-2)//慢速移动
                    mv=std::max(-bullets[bul_i].spd * sd,1.0);
            }
            if(bullets[bul_i].from==59)//大喷菇
            {
                int r=bullets[bul_i].x,l=r-mv,oddps=dps;if(l>r)std::swap(l,r);
                for (int s = 0; s < js[y]; s++)
                {
                    int i=pj[y][s];
                    if (!CannotReach(-1e9,i)&&zombies[i].x >= l&&zombies[i].x < r)
                        BulletOnJs(bul_i,i,0);
                }
                if(dps>oddps)NewNumberPoint(bullets[bul_i].x,bullets[bul_i].y*SI,dps-oddps,59);
                bullets[bul_i].x += mv;
                if (bullets[bul_i].x>MM*SI+10||bullets[bul_i].x<-200)bullets[bul_i].y = -1;
                continue;
            }
            while (b < js[y]&&(zombies[pj[y][b]].x <= zx||zombies[pj[y][b]].life < 0))//找到右面的第一个
                b += 1;
            int zmb_i = (b == js[y] ? -1 : pj[y][b]);
            if(bullets[bul_i].left)
            {
                if(b>0)bullets[bul_i].tx = zombies[pj[y][b-1]].x;
                else bullets[bul_i].tx = -20;
            }
            else
            {
                if (zmb_i != -1)bullets[bul_i].tx = zombies[zmb_i].x;
                else bullets[bul_i].tx = MM * SI + 20;
            }
            if (bullets[bul_i].spd < 0) { //魅惑子弹
                Bullet zd=MhJsSkill(bul_i);
                if(zd.siz!=-1)bul_nw[ns++] = zd;
            }
            bool tou=(is_tou[bullets[bul_i].from]||bullets[bul_i].spd<=0);
            if (!tou&&bullets[bul_i].spd > 0)//解冻
            {
                int w = GetZwShu ((bullets[bul_i].x / SI) * SI,bullets[bul_i].y);
                if(w!=-1&&plants[w].frez_stp)
                {
                    int t=bullets[bul_i].attack;
                    if(bullets[bul_i].color == RED)t*=3;
                    t/=1000;if(t<1)t=1;
                    JieDong(w,t);bullets[bul_i].y = -1;continue;
                }
            }
            bool wx=0;int c=b;
            if(bullets[bul_i].from==39&&zmb_i!=-1&&zombies[zmb_i].type!=19&&bullets[bul_i].siz==1&&bullets[bul_i].spd>0)//尖刺气球优先
                wx=1;
            else
            {
                if(!tou)
                {
                    while (c < js[y]&&CannotReach(zx,pj[y][c]))//找到右面的第一个
                        c += 1;
                    if(c>=js[y])wx=1,zmb_i=-1;
                    else zmb_i=pj[y][c];
                }
            }
            if(zmb_i!=-1&&bullets[bul_i].from==60&&bullets[bul_i].sy==zombies[zmb_i].ii)//防止多次伤害
            {
                c+=1;//下一个
                while (c < js[y]&&CannotReach(zx,pj[y][c]))//找到右面的第一个
                    c += 1;
                if(c>=js[y])wx=1,zmb_i=-1;
                else zmb_i=pj[y][c];
            }
            if(wx)
            {
                bullets[bul_i].tx = MM * SI + 20;
                zmb_i=-1;
            }
            else if(zmb_i!=-1)bullets[bul_i].tx=zombies[zmb_i].x;
            int oddps=dps;
            if (zmb_i != -1&&bullets[bul_i].x + 12 >= zombies[zmb_i].x) { //打到了
                bool mh = false;
                if (bullets[bul_i].spd > 0) {
                    if (bullets[bul_i].mh)mh = MeiHuo (zmb_i);
                    else BulletOnJs(bul_i,zmb_i,tou);
                }
                else {//魅惑子弹
                    int z = std::min((int)bullets[bul_i].attack,zombies[zmb_i].life);//减去最小的
                    if (zombies[zmb_i].type == 11&&bullets[bul_i].siz == -2)//小车不杀死僵王
                        z = 0;
                    if(z&&bullets[bul_i].siz != -2)
                    {
                        z=std::min(z,int(200*sd));
                        zombies[zmb_i].x_flt=bullets[bul_i].x + 10;
                        zombies[zmb_i].eating=true;
                    }
                    bullets[bul_i].attack -= z;
                    z*=bullets[bul_i].du;mv=0;
                    zombies[zmb_i].life -= z;dps+=z;
                    if(bullets[bul_i].siz==-2)bullets[bul_i].attack=2e9;
                    if(z)NewRedPoint(zmb_i,z);
                }
                if (!mh) {
                    if (zombies[zmb_i].life <= 0)
                        dps+=zombies[zmb_i].life,DestroyJs (zmb_i,0);
                    else
                        zombies[zmb_i].id = GetJsIcon (zombies[zmb_i]);
                }
                if (bullets[bul_i].jian) //群体伤害
                {
                    int tx = zombies[zmb_i].x, ji = bullets[bul_i].jian;
                    int td=std::max(bullets[bul_i].du-2,0),atk=bullets[bul_i].attack;
                    int y = bullets[bul_i].y,z=int(pow(atk,0.9)+0.5);
                    if(ji>2)ji+=200*(ji-2);
                    if(ji>5)ji+=100*(ji-5);
                    const HURT hu(z,bullets[bul_i].freeze*0.8,td,(tou?5:0),0);
                    if(ji>5)ji=5;
                    if (y > 0&&ji >= 2)KillJsLR(tx,y-1,0,ji-2,tou,hu,1);
                    if (y < MN - 1&&ji >= 2)KillJsLR(tx,y+1,0,ji-2,tou,hu,1);
                    KillJsLR(tx,y,0,(ji>=3?ji-2:0),tou,hu,1);
                }
                if(bullets[bul_i].color==BUTTER)
                    KillJsPos(zombies[zmb_i].x,bullets[bul_i].y,0.25,1,HURT(bullets[bul_i].attack,-120));
                if(dps>oddps)
                {
                    int co=(bullets[bul_i].spd>0?bullets[bul_i].from:1);
                    if(bullets[bul_i].color==DBLUE)co=61;
                    else if(bullets[bul_i].color==RED)co=4;
                    else if(bullets[bul_i].color==PURPLE)co=59;
                    NewNumberPoint(bullets[bul_i].x,bullets[bul_i].y*SI,dps-oddps,co);
                }
                if ((bullets[bul_i].spd > 0||bullets[bul_i].attack <= 0)&&bullets[bul_i].from!=59&&bullets[bul_i].from!=60)//魅惑子弹消失
                    bullets[bul_i].y = -1;
                else if(bullets[bul_i].from==60)//回旋镖
                {
                    bullets[bul_i].sy=zombies[zmb_i].ii;
                    if(bullets[bul_i].left)bullets[bul_i].spd+=2;
                    else bullets[bul_i].spd-=2;
                    if(bullets[bul_i].spd==0)
                        bullets[bul_i].left=1,bullets[bul_i].spd=2;
                    if(bullets[bul_i].spd>8)bullets[bul_i].spd=8;
                }
                if (bullets[bul_i].spd < 0) {
                    Zombie zz;
                    zz.type = bullets[bul_i].siz;
                    zz.life = bullets[bul_i].attack;
                    zz.y = -1;zz.sp_eat=0;
                    bullets[bul_i].sx = GetJsIcon (zz);//更新子弹图像
                }
            }
            bullets[bul_i].x += mv;
            if (bullets[bul_i].from==60&&bullets[bul_i].x > MM * SI&&!bullets[bul_i].left)
                bullets[bul_i].left=1,bullets[bul_i].sy=-1;
            if (bullets[bul_i].x > MM * SI + 10||bullets[bul_i].x<-200) {
                if (bullets[bul_i].siz == -2)car_driv[bullets[bul_i].y] = false;
                bullets[bul_i].y = -1;
            }
        }
    }
    int sa = 0, sb = 0;
    for (int i = 0; i < bul_cnt; i++)
        if (bullets[i].y != -1)
            bullets[sa++] = bullets[i];
    for (int i = 0; i < zmb_cnt; i++)
        if (zombies[i].life != -1)
            zombies[sb++] = zombies[i];
    bul_cnt = sa;zmb_cnt = sb;
    for (int i = 0; i < ns; i++)
        bullets[bul_cnt++] = bul_nw[i];
    for (int i = 0; i < sb; i++)
        zombies[i].num = iinum[zombies[i].ii]=i;
    SZZ = no_num = false;
}

void Game::FreezeJs(int i,short frez,int stp)
{
    NewBluePoint(i,frez);
    zombies[i].freeze = std::max(zombies[i].freeze,frez);
    if(iswa[zombies[i].y])stp+=frez/8;
    if(stp)FreezeStopJs(i,stp);
}

void Game::FreezeStopJs(int i,int tm)
{
    if(TM+tm>zombies[i].frez_stp)
        zombies[i].frez_stp=TM+tm;
}

void Game::NewBaoZha(int x,int y,int z,int t,int sd)
{
    ShiTi st;
    st.id=sd*1000000+t*1000+z;
    st.x=x;st.y=y;
    st.tm=TM;st.ty=9;
    NewSt(st);
}

void Game::NewPicSt(int x,int y,int id,int tm)
{
    ShiTi st;
    st.id=id;st.tm=TM+tm;
    st.ty=11;st.x=x;st.y=y;
    NewSt(st);
}

void Game::NewPicStL(int y,int id,int tm)
{
    for(int x=0;x<MM;x++)
        NewPicSt(x*SI,y,id,tm);
}

void Game::SpecialPlt (int shu) //特殊植物
{
    int gj = -plants[shu].attack,oddps,fw=0,lx,rx;
    int sn=25,t;Plant&zw=plants[shu];
    int ty=zw.type,ox=zw.x,oy=zw.y;
    if(is_cycle)//light:+0.1
    {
        if(is_day==0)sn=35;
        else
        {
            sn=10;
            if(FindZw(ox/SI,oy,11)!=-1)sn*=2;
        }
    }
    int f=wz_fei[zw.x/SI][zw.y]+zw.levod,hu=int(zw.move_spd)*1000,sp=zw.move_spd;
    double mul=zw.sp_mul;
    switch (gj) {
        case 1: //向日葵
            if (!m_wnd->imjs)
                m_yg += sn*((m_wnd->sun_dou?0.5:0)+1)*mul,laygTM=TM;
            break;
        case 26: //小炸弹
        case 2: //炸弹
            oddps=dps;no_num=1;
            NewPicSt(zw.x,zw.y*SI,128,gj==2?80:50);
            for (int s = -1; s <= 1; s++)
                KillJsLR (zw.x, zw.y + s,1,1,1,HURT(hu*(1+f*0.3),0,0,0,1),0,ty);
            if(f)
                for (int s = -2; s <= 2; s++)
                    KillJsLR (zw.x, zw.y + s,2,2,1,HURT(f*hu/10,0,0,0,1),0,ty);
            no_num=0;NewNumberPoint(zw.x,zw.y*SI,dps-oddps,4);
            NewBaoZha(zw.x+SI/2,zw.y,zw.type,30,gj==2?SI/14:SI/20);
            fw=1;DestroyZw (shu,0);break;
        case 3: //火爆辣椒
            NewPicStL(zw.y*SI,-127);
            oddps=dps;no_num=1;
            KillJsLine (zw.y, hu,1,1,ty);
            if(f)KillJsLine (zw.y, f*hu/3,1,1,ty);
            no_num=0;NewNumberPoint(zw.x,zw.y*SI,dps-oddps,4);
            DestroyZw (shu,0);break;
        case 4: //窝瓜
            lx=GetLastJs(zw.x,zw.y),rx=GetFirstJs(zw.x,zw.y);
            {
                int x=-1000,fw=SI*0.6,ks=fw*0.35;t=SI*1.3;
                lx=(lx==-1?-1e8:zombies[lx].x);rx=(rx==-1?1e8:zombies[rx].x);
                if(rx-lx<=fw)x=rx-fw;
                else if(zw.x-lx<=t)x=std::max(zw.x-t,lx-fw);
                else if(rx-zw.x<=t)x=rx;
                if(x!=-1000)
                {
                    oddps=dps;no_num=1;
                    ShiTi st;st.ty=5;st.tm=TM;
                    st.x=x+ks;st.y=zw.y*SI;NewSt(st);
                    KillJs (x,x+fw,zw.y,1,HURT(hu),0,ty);//修复判定
                    if(f)KillJs (x,x+fw,zw.y,1,HURT(f*hu/3),0,ty);
                    no_num=0;NewNumberPoint(zw.x,zw.y*SI,dps-oddps);
                    DestroyZw (shu,0);
                }
                break;
            }
        case 5: //阳光菇
            t=zw.move_spd;
            if (zw.grow < t)m_yg += (m_wnd->sun_dou?22:15)*mul;
            else m_yg += (m_wnd->sun_dou?37:25)*mul;
            zw.grow = std::min(zw.grow+1,t); //生长
            zw.id = GetZwIcon (zw);
            laygTM=TM;break;
        case 6: //毁灭菇
            oddps=dps;no_num=1;
            for (int s = -2; s <= 2; s++)
                KillJsLR (zw.x, zw.y + s,2,2,1,HURT(hu*(1+f*0.4)),0,ty);
            m_keng[zw.x / SI][zw.y] = 1; //坑
            NewBaoZha(zw.x+SI/2,zw.y,zw.type,50);
            no_num=0;NewNumberPoint(zw.x,zw.y*SI,dps-oddps);
            fw=2;DestroyZw (shu,0);break;
        case 7: //寒冰菇
            for (int i = 0; i < zmb_cnt; i++)
            {
                if(zombies[i].type>=100)continue;
                FreezeJs(i,sp,(sp+40)*(1+f*0.2));
            }
            for(int i=0;i<plt_cnt;i++)
                if(plants[i].Burning())plants[i].fire=FIRETM+1;
            NewBaoZha(zw.x+SI/2,zw.y,zw.type,300,SI/6);
            DestroyZw (shu,0);
            break;
        case 8: //全息坚果
            if(TM-zw.la_et<20)break;
            t=zw.mxlf/(m_wnd->m_hd ? 100 : 60)*mul;
            if(zw.life>zw.mxlf*0.8)t/=2;
            zw.life=std::min(zw.life+t,zw.mxlf);
            zw.id = GetZwIcon (zw);
            break;
        case 9: //双子
            t=(sn+(zw.tag?5:0));
            m_yg += (m_wnd->sun_dou?t*1.5:t)*mul,laygTM=TM;
            if(zw.tag&&NeedBullet(zw.y))
            {
                Bullet zd; //发射子弹
                zd.color = Qt::white;
                zd.spd = 4;zd.siz = 1;
                zd.x = zw.x+SI/4; zd.y = zw.y;
                zd.attack = 3000*mul;
                zd.tx = zw.x - 1;
                zd.jian = zw.jian;
                zd.left=zw.left;
                zd.sy=-1;zd.from=59;NewZd(zd);
            }
            break;
        case 10: //医疗
            sn=(m_wnd->m_hd ? 1 : 0);
            for (int a = -1; a <= 1; a++) {
                for (int b = -1; b <= 1; b++) {
                    int z = GetZwShu (zw.x + a * SI, zw.y + b);
                    if (z != -1) {
                        if ((m_wnd->imjs&&plants[z].type == 1)||TM-plants[z].la_et<20)
                            continue;
                        bool bk = false;
                        if (!m_wnd->imjs)m_yg -=sn;
                        if (m_yg < 0) {m_yg += sn;goto ov;}
                        double mx=plants[z].mxlf,ad=2*pow(mx,0.3)*mul;
                        if (plants[z].life < mx)bk = true;
                        plants[z].life = std::fmin(plants[z].life+ad,mx);
                        int tx = zw.x/SI+a, ty = zw.y + b;
                        if (m_ng[tx][ty]&&TM-le_ng[tx][ty]>=40) {
                            if (m_ng[tx][ty] < 600)bk = true;
                            m_ng[tx][ty] = std::fmin(m_ng[tx][ty]+10,600);
                        }
                        if (!bk&&!m_wnd->imjs)m_yg += sn;
                        if(plants[z].Burning())plants[z].fire+=2;
                        if(plants[z].frez_stp)JieDong(z,2);
                        plants[z].id = GetZwIcon (plants[z]);
                    }
                }
            }
        ov:
            break;
        case 11: //忧郁菇
            zw.tag = 0;
            t = (zw.lev >= 6 ? 2 : 1);
            for (int s = -t; s <= t; s++)
                if (KillJsLR(zw.x,zw.y+s,t,t,1,HURT(1000*mul,0,0,10)))
                    zw.tag = 1;
            lx=zw.x - (t+FW) * SI,rx=zw.x + (t+FW) * SI;
            for(int i=0;i<zmb_cnt;i++)
                if(zombies[i].x>=lx&&zombies[i].x<=rx&&zombies[i].y>=zw.y-t&&zombies[i].y<=zw.y+t&&zombies[i].type<100)
                    zombies[i].xu=std::max(zombies[i].xu,1.2*(1+(mul-1)*0.2));
            break;
        case 12: //双头喷
            t=(zw.lev >= 5 ? -1000 : 1000)*mul;
            zw.tag = KillJsLine (zw.y, t,10);
            if (zw.lev >= 3)
                zw.tag = (zw.tag||KillJsLie (zw.x, t,10));
            break;
        case 13: //大葱
            {
                int cs[MAXSHU] = {0}, cf[MAXSHU], fs = 0, kx[8], ky[8], ks = 0;
                if(m_wnd->b_ycl&&m_wnd->ui->huisu->isEnabled())break;
                zw.grow-=int(mul+0.1);
                if(zw.grow>0)break;
                zw.grow=zw.move_spd;
                for (int a = -1; a <= 1; a++) {
                    for (int b = -1; b <= 1; b++) {
                        if (a == 0&&b == 0)
                            continue;
                        int z = GetZwShu (zw.x + a * SI, zw.y + b);
                        if (z == -1) {
                            if (zw.x + a * SI >= 0&&zw.x + a * SI <= (MM - 1) * SI&&zw.y + b >= 0&&zw.y + b < MN) {
                                kx[ks] = zw.x + a * SI;
                                ky[ks++] = zw.y + b;
                            }
                            continue;
                        }
                        cs[plants[z].type] += 1;
                    }
                }
                if (ks) {
                    for (int i = 0; i < MAXSHU; i++)
                        if (cs[i] >= 2)cf[fs++] = i;
                    if (fs) {
                        int z = cf[rand () % fs], t;
                        for (t = 0; t < MAXSHU; t++)
                            if (m_wnd->plants[t].type == z)break;
                        int nz = rand () % ks;
                        Plant zw = m_wnd->plants[t];
                        zw.x = kx[nz];
                        zw.y = ky[nz];
                        m_yg += zw.cost;
                        PlantZw (zw);
                    }
                }
                break;
            }
        case 14: //炮
            t = GetZwShu (zw.x - SI, zw.y);
            if (t != -1&&plants[t].type == 31)//左边有前一半
                zw.grow = std::min(zw.grow+int(mul+0.1),(int)zw.move_spd);
            break;
        case 15://三叶草
            if(zw.grow==zw.move_spd)
                lstfan=TM,lmw=GetFogAlpha();
            zw.grow-=1;
            for(int i=0;i<zmb_cnt;i++)
            {
                if(zombies[i].type>=100)
                {
                    DestroyJs(i);
                    i-=1;continue;
                }
                if(zombies[i].type==19)
                {
                    HurtJs_Fast(i,2e9,0);
                    i-=1;continue;
                }
                if(zombies[i].xu<1.2)zombies[i].xu=1.2;
                if(HurtJs(i,1000*(f+1),0,100,10))i-=1;
            }
            for(int i=0;i<plt_cnt;i++)
                if(plants[i].Burning())plants[i].fire=FIRETM+1;
            NewBaoZha(zw.x+SI/2,zw.y,zw.type,30,SI/2);
            if(zw.grow<=0)
                DestroyZw (shu,0);
            break;
        case 16://磁力菇
            if(zw.grow)zw.grow-=int(mul+0.1);
            else
            {
                int w=-1,xz=SI*1.8,mi=1e9;
                for(int i=0;i<zmb_cnt;i++)
                {
                    if(abs(zombies[i].x-zw.x)<=xz&&abs(zombies[i].y-zw.y)<=1&&(zombies[i].type==2||zombies[i].type==3||zombies[i].type==5||zombies[i].type==7||zombies[i].type==27))
                    {
                        int t=abs(zombies[i].x-zw.x)+SI*abs(zombies[i].y-zw.y);
                        if(t<mi)mi=t,w=i;
                    }
                }
                if(w!=-1)
                {
                    if(zombies[w].type==5||zombies[w].type==7||zombies[w].type==27)ClearDun(zombies[w]);
                    else zombies[w].life=std::min(zombies[w].life,8000);
                    zw.grow=zw.move_spd;
                    zombies[w].id=GetJsIcon(zombies[w]);
                }
            }
            break;
        case 17://魅惑炸弹
            lx=zw.x - (1+FW) * SI,rx=zw.x + (1+FW) * SI;
            for(int i=0;i<zmb_cnt;i++)
                if(zombies[i].x>=lx&&zombies[i].x<=rx&&zombies[i].y>=zw.y-1&&zombies[i].y<=zw.y+1&&zombies[i].type!=11&&zombies[i].type<100)
                    if(MeiHuo(i))i-=1;
            fw=1;DestroyZw (shu,0);break;
        case 18://杨桃
            zw.tag=0;
            for(int i=0;i<zmb_cnt;i++)
            {
                if(zombies[i].type==19||zombies[i].type>=100)continue;
                bool fd=0;
                int zx=zombies[i].x,zy=zombies[i].y;
                if(zy==oy)fd=(zx<ox);
                else fd=(abs(ox-zx)<=60||abs(ox+abs(oy*SI-zombies[i].ry)-zx)<=60);
                if(fd)
                {
                    zw.tag=1;
                    if(HurtJs(i,1000*mul,0,50,0))i-=1;
                }
            }
            break;
        case 19://金盏花
            if(m_yg<5||!real||zw.left)break;
            m_yg-=5;m_wnd->exp+=25;break;
        case 20://土豆地雷
        case 21://大嘴花
            zw.grow -= int(mul+0.1); //回复
            if(m_wnd->b_guan&&zmb_cnt==0)zw.grow+=1;
            if (zw.grow < 0)zw.grow = 0;
            zw.id = GetZwIcon (zw);
            t = GetFirstJs (zw.x-(gj==20?SI*0.3:0), zw.y);
            if (t == -1)break;
            if (zombies[t].x < zw.x + (gj==20?SI*0.8:SI*2)&&zw.grow == 0) {
                oddps=dps;no_num=1;
                int o=KillJsPos (zombies[t].x, zombies[t].y, FW,1,HURT(gj==20?70000:90000));
                no_num=0;NewNumberPoint(zw.x,zw.y*SI,dps-oddps);
                if (gj==20&&o>0) { //土豆地雷
                    DestroyZw (shu,0);
                    break;
                }
                else zw.grow = sqrt(o)*zw.move_spd;
                zw.id = GetZwIcon (zw);
            }
            break;
        case 22://定向
            {
                int n,x,y;double dis;
                zw.tag=(zw.tag==-1?-1:iinum[zw.tag]);
                if(zw.tag==-1||zw.tag>=zmb_cnt||zombies[zw.tag].type>=100)
                    zw.tag=0;
                while(zw.tag<zmb_cnt&&zombies[zw.tag].type>=100)
                    zw.tag+=1;
                if(zw.tag>=zmb_cnt){zw.tag=-1;break;}
                n=zw.tag;zw.tag=zombies[n].ii;
                x=zw.x-zombies[n].x;y=zw.y*SI-zombies[n].ry;
                dis=std::min(sqrt(x*x+y*y),1200.0);
                lx=1000,rx=500;
                t=int((rx-lx)/1200.0*dis+lx+0.5)*mul;
                HurtJs(n,t,0,0,10);break;
            }
        case 23://冰冻炸弹
            for(int x=-1;x<=1;x++)
                for(int y=-1;y<=1;y++)
                {
                    int w=GetZwShu(zw.x+x*SI,zw.y+y);
                    if(w!=-1&&plants[w].Burning())plants[w].fire=FIRETM+1;
                }
        case 24://有毒炸弹
        case 28://冰豆
        case 29://虚弱炸弹
            lx=zw.x - (1+FW) * SI,rx=zw.x + (1+FW) * SI;
            for(int i=0;i<zmb_cnt;i++)
                if(zombies[i].x>=lx&&zombies[i].x<=rx&&zombies[i].y>=zw.y-1&&zombies[i].y<=zw.y+1&&zombies[i].type<100)
                {
                    if(gj==23)FreezeJs(i,sp*(1+f*0.2),sp*(1+f*0.2)*0.4);
                    else if(gj==24)zombies[i].du+=sp*(1+f*0.2);
                    else if(gj==28)zombies[i].freeze+=10;
                    else zombies[i].xu=std::max(zombies[i].xu,sp*(1+f*0.1)*0.1);
                }
            NewBaoZha(zw.x+SI/2,zw.y,zw.type,30);
            if(gj!=28)fw=1,DestroyZw (shu,0);
            break;
        case 25://棱镜
            if(zw.grow)
            {
                zw.grow-=1;
                zw.id = (zw.grow?237:236);
            }
            break;
        case 27://水炸弹
            lx=zw.x - (1+FW) * SI,rx=zw.x + (1+FW) * SI;
            {
                short frez=0;int du=0;double xu=1;
                for(int i=0;i<zmb_cnt;i++)
                    if(zombies[i].x>=lx&&zombies[i].x<=rx&&zombies[i].y>=zw.y-1&&zombies[i].y<=zw.y+1&&zombies[i].type<100)
                    {
                        frez=std::max(frez,zombies[i].freeze);
                        du=std::max(du,zombies[i].du);
                        xu=std::fmax(xu,zombies[i].xu);
                    }
                double p=sp/100.0;
                du*=(p+f*0.1);frez*=(p+f*0.1);xu=1+(xu-1)*(p+f*0.1);
                for(int i=0;i<zmb_cnt;i++)
                    if(zombies[i].x>=lx&&zombies[i].x<=rx&&zombies[i].y>=zw.y-1&&zombies[i].y<=zw.y+1&&zombies[i].type<100)
                    {
                        FreezeJs(i,frez,frez*2);
                        zombies[i].du=std::max(zombies[i].du,du);
                        zombies[i].xu=std::fmax(zombies[i].xu,xu);
                    }
                for(int x=-1;x<=1;x++)
                    for(int y=-1;y<=1;y++)
                    {
                        int w=GetZwShu(zw.x+x*SI,zw.y+y);
                        if(w!=-1&&plants[w].Burning())plants[w].fire=FIRETM+1;
                    }
                NewBaoZha(zw.x+SI/2,zw.y,zw.type,30);
                fw=1;DestroyZw (shu,0);break;
            }
        case 30://火龙草
            oddps=dps;no_num=1;t=0;
            for (int s = -1; s <= 1; s++)
                t+=KillJsLR (zw.x+SI, zw.y + s,0,2,1,HURT(3000*mul,0,0,0,1),1,ty);
            if(t>0)
            {
                for(int x=1;x<=3;x++)
                    for(int y=-1;y<=1;y++)
                        if(zw.x/SI+x<MM&&zw.y+y>=0&&zw.y+y<MN)
                            NewPicSt(zw.x+x*SI,(zw.y+y)*SI,-127,60);
            }
            no_num=0;NewNumberPoint(zw.x,zw.y*SI,dps-oddps,4);
            break;
        case 31://菜问
            t=std::max(0,zw.tag-int(zw.life*1000));
            zw.tag=int(zw.life*1000);if(t>200000)t=200000;
            lx=zw.x - (1+FW) * SI,rx=zw.x + (1+FW) * SI;
            oddps=dps;no_num=1;
            {
                int hu=2000+10*pow(t/1000.0,1.5);
                double d=1/(2+t/50000.0);
                if(zw.dzsy)hu*=2,d=0;
                int ki[100],ps=0;double p=1.0;
                for(int i=0;i<zmb_cnt;i++)
                    if(zombies[i].x>=lx&&zombies[i].x<=rx&&zombies[i].y>=zw.y-1&&zombies[i].y<=zw.y+1&&zombies[i].type<100)
                        ki[ps++]=int(hu*p+0.5),p=std::fmax(p-d,0);
                std::shuffle(ki,ki+ps,RA);
                for(int i=0,j=0;i<zmb_cnt;i++)
                    if(zombies[i].x>=lx&&zombies[i].x<=rx&&zombies[i].y>=zw.y-1&&zombies[i].y<=zw.y+1&&zombies[i].type<100&&HurtJs(i,ki[j++],0))i-=1;
            }
            no_num=0;NewNumberPoint(zw.x,zw.y*SI,dps-oddps);
    }
    if(fw)
    {
        ty=m_wnd->tyToNumP[ty];
        for(int x=-fw;x<=fw;x++)
        {
            for(int y=-fw;y<=fw;y++)
            {
                int z=GetZwShu(ox+x*SI,oy+y);
                if(plants[z].type==78)
                {
                    DestroyZw(z,0);
                    Plant t=m_wnd->plants[ty];
                    t.x=ox+x*SI,t.y=oy+y,t.cost=0,t.life=1e9,t.time=300,t.mush=0,t.tag=TM;
                    PlantZw(t);
                }
            }
        }
    }
}
void Game::LengJing(int z,int su)
{
    int ox=plants[z].x,oy=plants[z].y*SI,s=0;
    for(int i=0;i<zmb_cnt;i++)
        if(zombies[i].type<100)s+=1;
    double hu=double(su)/s;
    for(int i=0;i<zmb_cnt;i++)
    {
        if(zombies[i].type>=100)continue;
        double x=zombies[i].x-ox,y=zombies[i].ry-oy;
        double d=sqrt(x*x+y*y),t=(150.0/(d+150.0)*1.5+0.5)*hu;
        if(HurtJs_Fast(i,int(t*1000+0.5),1,10))i-=1;
    }
    plants[z].tag=TM;
    plants[z].grow=plants[z].move_spd;
    plants[z].dztm=iis;
}
int Game::KillJsPos (int x, int y,double fw,bool sky,const HURT&hu,int from)
{
    return KillJs (x - SI * fw, x + SI * fw, y, sky,hu,from);
}
int Game::KillJsLR(int x,int y,int ls,int rs,bool sky,const HURT&hu,bool jian,int from)
{
    return KillJs (x - SI * (ls+FW), x + SI * (rs+FW), y, sky,hu,jian&&rs>0,from);
}
int Game::KillJs (int l, int r, int y, bool sky,const HURT&hu,bool jian,int from) //伤害僵尸
{
    if (y < 0||y >= MN)return 0;
    int rt = 0,kill=hu.atk,frez=hu.frez,stp=0;
    bool wg=(from==7),fi=hu.fire;
    if(frez<0)stp=-frez,frez=0;
    int rea=(wg?2:(fi?1:0));
    for (int i = 0; i < zmb_cnt; i++) {
        int zt=zombies[i].type;
        if(zt==19&&from!=3&&from!=5&&from!=72&&from!=15)continue;
        if (zombies[i].life>=0&&zombies[i].y == y&&zombies[i].x >= l&&zombies[i].x <= r&&zt < 100) {
            rt+=1;int ki=kill,fz=frez;
            if(jian)
            {
                int mi=l+SI*FW;
                if(zombies[i].x>mi)
                {
                    double p=1-0.3*double(zombies[i].x-mi)/(r-mi);
                    ki*=p;fz*=p;
                }
            }
            if(stp)
            {
                if(TM>=zombies[i].stp_tm+40)
                    zombies[i].stp_tm=TM+stp;
                else ki*=3;
            }
            if(ki>=30000&&zt==11)//僵王抵御一半
            {
                if(ki<=60000)ki=30000;else ki/=2;
                if(zombies[i].spd>0)zombies[i].spd=-zombies[i].spd;
            }
            zombies[i].du+=hu.du;
            if(!sky&&zombies[i].dun)
            {
                if(HurtJs(i,0,0,fz/2,hu.ct)&&!SZZ)
                    i-=1;
            }
            else if(HurtJs(i,ki,rea,fz,hu.ct,fi?2:0)&&!SZZ)i-=1;
        }
    }
    if (fi) { //烧掉冰
        for (int x = 0; x < MM; x++)
            if(x*SI>=l&&x*SI<=r)
            {
                int w=GetZwShu(x*SI,y);
                if(w!=-1&&plants[w].frez_stp)JieDong(w,1e9),rt+=1;
                if(m_ice[x][y]>-1)m_ice[x][y] = -1,rt+=1;
            }
    }
    return rt;
}
bool Game::KillJsLie (int x, int kill,int ct) { //列伤害
    bool rtn = false;short frez=(kill<0?80:0);
    int l = x - SI * 0.4, r = x + SI * 0.4;
    for (int i = 0; i < zmb_cnt; i++) {
        if (zombies[i].life>=0&&zombies[i].type!=19&&zombies[i].x >= l&&zombies[i].x <= r&&zombies[i].type < 100) {
            rtn = true;
            if(HurtJs(i,abs(kill),0,frez,ct)&&!SZZ)
                i-=1;
        }
    }
    return rtn;
}
int Game::KillJsLine (int y, int kill,int ct,bool fire,int from) //行伤害
{
    return KillJs(-1e9,1e9,y,1,HURT(abs(kill),(kill<0?80:0),0,ct,fire),from);
}
void Game::KillAll () {
    for (int i = 0; i < MM; i++)
        for (int j = 0; j < MN; j++)
            m_ng[i][j]=0;
    while (plt_cnt > 0)
        DestroyZw (0,0);
    while (zmb_cnt > 0)
        DestroyJs (0);
    while (bul_cnt > 0)
        DestroyZd (0);
    while (shi_cnt > 0)
        DestroySt (0);
}

void Game::ClearCd()
{
    for (int i = 0; i < MM; i++) {
        for (int j = 0; j < MN; j++) {
            m_keng[i][j] = m_ng[i][j] =m_hy[i][j]=0;
            m_ice[i][j] = -1;
            m_wnd->kp_noguan[i][j]=0;
        }
    }
    for (int i = 0; i < MN; i++)
        iswa[i] = false;
}

void Game::MakeSI(int lx,int usermn,int usermm,int usersi)
{
    if(usermn==-1)
    {
        if(m_wnd->b_guan||m_wnd->gqe==3||m_wnd->is82||lx==4||lx==5)
            MN=6,MM=11,SI=91;
        else if(endless)
            MN=8,MM=15,SI=68;
        else
            MN=5,MM=9,SI=110;
    }
    else MN=usermn,MM=usermm,SI=usersi;
}

void Game::CreateGame (int lx) {
    ClearCd();
    if (lx==4||lx==5) {//水池
        int s=MN/3;if(s<1)s=1;
        for(int i=0;i<MM;i++)
            for(int j=s;j<MN-s;j++)
                m_keng[i][j] = 2, iswa[j] = true;
        for(int j=s;j<MN-s;j++)m_keng[0][j] = 3;
    }
    else if (endless) {//无尽水池
        for (int i = 0; i < MM; i++) {
            for(int j=1;j<=MN/4;j++)
            {
                m_keng[i][j] = 2, iswa[j] = true;
                m_keng[i][MN - 1-j] = 2, iswa[MN - 1-j] = true;
            }
        }
    }
    lstup=dps=fans=0;lstcar=lstfan=laygTM=-1e9;
    for (int x = 0; x < MM * SI + 100; x++) {
        for (int y = 0; y < MN; y++)
            fir_plt[x][y] = plt_num[x][y] = -1;
    }
    for (int i = 0; i < MN; i++)
        row_zmb[i] = lst[i] = 0;
    zmb_cnt = plt_cnt = bul_cnt = shi_cnt=t_ai = XS=iis=0;
    m_yg = 100;expad=ygad=0;
    for (int i = 0; i < MN; i++) {
        car[i] = true;car_TM[i]=-1e9;
        car_driv[i] = false;
    }
    for (int i = 0; i < MN; i++)
        for (int j = 0; j < MM; j++)
            le_ng[j][i]=-1e9,js_nd[j][i]=-1,spd_mul[j][i]=1.0,wz_fei[j][i]=0;
    started = true;
    SZZ = over = zk= no_num=false;
    is_day=(lx==0||lx==1||lx==4||lx==5);
    is_fog=(lx==1||lx==3);
    is_cycle=(lx==5);
    if (m_wnd->is11) //生成BOSS
    {
        Zombie j;int mx=(SI <= 55&&m_wnd->is12 ? 3000 : 1000);
        if(m_wnd->is12)mx=2000;
        mx*=m_wnd->xmul;
        j.Create (0, mx, 1000000, (MM - 1) * SI + 5, 0, 54, 11);
        for (int n = 0; n < MN; n++)
            j.y = n,NewJs (j);
        m_wnd->m_jwbs[MN]=mx;zjd=mx*MN;
    }
    HI=MN*SI+60;W1=MM*SI+50;
}

void Game::Load (QDataStream &ar) {
    ar >> plt_cnt;
    for (int i = 0; i < plt_cnt; i++)
        plants[i].input (ar);
    ar >> zmb_cnt;
    for (int i = 0; i < zmb_cnt; i++)
    {
        zombies[i].input (ar);
        iinum[zombies[i].ii]=i;
    }
    ar >> bul_cnt;
    for (int i = 0; i < bul_cnt; i++)
        bullets[i].input (ar);
    if(m_wnd->tcp.size()==0)
    {
        ar >> shi_cnt;
        for (int i = 0; i < shi_cnt; i++)
            shiti[i].input (ar);
    }
    ar >> m_yg>>laygTM>>is_day>>is_fog>>is_cycle;
    for (int i = 0; i < MM; i++) {
        for (int j = 0; j < MN; j++)
            ar >> m_keng[i][j] >> m_ng[i][j] >> m_ice[i][j]>>js_nd[i][j]>>le_ng[i][j]>>spd_mul[i][j]>>wz_fei[i][j];
    }
    for (int i = 0; i < MN; i++) {
        ar >> car[i] >> car_driv[i] >> iswa[i];
        ar >> lst[i]>>car_TM[i];
    }
    ar>>t_ai>>endless>>zk>>lstfan>>lmw>>jd>>zjd;
    if(m_wnd->tcp.size()==0)
    {
        int s;ar>>s;vdb.clear();
        while(s--)
        {
            int x;ar>>x;
            vdb.push_back(x);
        }
    }
    ar>>iis;
}
void Game::Save (QDataStream &ar) {
    ar << plt_cnt;
    for (int i = 0; i < plt_cnt; i++)
        plants[i].output (ar);
    ar << zmb_cnt;
    for (int i = 0; i < zmb_cnt; i++)
        zombies[i].output (ar);
    ar << bul_cnt;
    for (int i = 0; i < bul_cnt; i++)
        bullets[i].output (ar);
    if(m_wnd->tcp.size()==0)
    {
        ar << shi_cnt;
        for (int i = 0; i < shi_cnt; i++)
            shiti[i].output (ar);
    }
    ar << m_yg<< laygTM<<is_day<<is_fog<<is_cycle;
    for (int i = 0; i < MM; i++) {
        for (int j = 0; j < MN; j++)
            ar << m_keng[i][j] << m_ng[i][j] << m_ice[i][j]<<js_nd[i][j]<<le_ng[i][j]<<spd_mul[i][j]<<wz_fei[i][j];
    }
    for (int i = 0; i < MN; i++) {
        ar << car[i] << car_driv[i] << iswa[i];
        ar << lst[i] <<car_TM[i];
    }
    ar << t_ai << endless << zk;
    ar<<lstfan<<lmw<<jd<<zjd;
    if(m_wnd->tcp.size()==0)
    {
        ar<<int(vdb.size());
        for(int x:vdb)ar<<x;
    }
    ar<<iis;
}

//计算几何开始
struct SPt {
    int x, y;
    SPt () {
    }
    SPt (int X, int Y) {
        x = X;
        y = Y;
    }
};
typedef SPt SVe;
SPt operator+ (const SPt &a, const SVe &b) {
    return SPt (a.x + b.x, a.y + b.y);
}
SVe operator- (const SPt &a, const SPt &b) {
    return SVe (a.x - b.x, a.y - b.y);
}
int operator* (const SVe &a, const SVe &b) {
    return a.x * b.y - a.y * b.x;
}
SVe operator* (const SVe &a, double b) {
    return SVe (int (a.x * b + 0.5), int (a.y * b + 0.5));
}
SPt jiaod (const SPt &a, const SVe &va, const SPt &b, const SVe &vb) {
    double t = double ((b - a) * vb) / (va * vb);
    return a + va * t;
}
bool inside (const SPt &a, const SPt &b, const SPt &c) {
    if ((c.x < a.x||c.x > b.x)&&(c.x < b.x||c.x > a.x))
        return false;
    if ((c.y < a.y||c.y > b.y)&&(c.y < b.y||c.y > a.y))
        return false;
    return true;
}
bool check (const SPt &a, const SPt &b, const SPt &c, const SPt &d) {
    if ((b - a) * (d - c) == 0)
        return false;
    SPt z = jiaod (a, b - a, c, d - c);
    return inside (a, b, z)&&inside (c, d, z);
}
bool jiaorect (int x, int y, const SPt &a, const SPt &b) {
    SPt z0 (x + SI / 4, y + SI / 6), z1 (x + SI * 3 / 4, y + SI / 6), z2 (x + SI * 3 / 4, y + SI * 5 / 6), z3 (x + SI / 4, y + SI * 5 / 6);
    return check (z0, z1, a, b)||check (z1, z2, a, b)||check (z2, z3, a, b)||check (z3, z0, a, b);
}
//计算几何结束

void Game::WanQuanTu()
{
    int s=0;
    for (int i = 0; i < plt_cnt; i++)
        if (plants[i].type == 33)s+=1;
    if(s>MN*2)return;
    for (int i = 0; i < plt_cnt; i++) {
        if (plants[i].type == 33) {
            int x1 = plants[i].x + SI / 2, y1 = plants[i].y * SI + SI / 2;
            for (int j = i + 1; j < plt_cnt; j++) {
                if (plants[j].type == 33) {
                    int x2 = plants[j].x + SI / 2, y2 = plants[j].y * SI + SI / 2;
                    for (int s = 0; s < zmb_cnt; s++) {
                        if (zombies[s].type >= 100||!jiaorect (zombies[s].x, zombies[s].y * SI, SPt (x1, y1), SPt (x2, y2)))
                            continue;
                        if(HurtJs(i,300,1,0,10,1))s-=1;
                    }
                }
            }
        }
    }
}

void Game::FireTick()
{
    int nx[4][2]={-SI,0,0,-1,SI,0,0,1};
    if(TM%20==0)
    {
        for(int i=0;i<plt_cnt;i++)
        {
            int f=plants[i].fire;
            if(plants[i].Burning())
            {
                int od=plants[i].la_et;
                if(HurtZw(i,plants[i].mxlf/80.0,1))//火焰伤害
                {i-=1;continue;}plants[i].la_et=od;
            }
            if(f)f+=1;
            if(f>=150)f=0;
            plants[i].fire=f;
        }
        for(int i=0;i<plt_cnt;i++)
        {
            int f=plants[i].fire;
            if(f&&f<=FIRETM&&(f+i)%7==0)
            {
                for(int s=0;s<4;s++)
                {
                    int w=GetZwShu(plants[i].x+nx[s][0],plants[i].y+nx[s][1]);
                    if(w!=-1&&RA()%4==0)plants[w].Burn();//火焰蔓延(25%)
                }
            }
        }
    }
    if(TM%30==0)
    {
        for(int i=0;i<plt_cnt;i++)
            if(plants[i].frez_stp&&HurtZw(i,plants[i].mxlf/100.0,1))i-=1;
    }
    if(m_wnd->auto_fire&&TM%600==0&&plt_cnt)//15s 随机火焰
    {
        int w=RA()%plt_cnt;
        plants[w].Burn();
    }
}

int Game::FindZombie(QPoint pt)
{
    int x=pt.x () - 150 - SI / 2,y = int ((pt.y () - 60) / SI);
    int zx = 1e9, wz = -1;
    for (int i = 0; i < zmb_cnt; i++) {
        if (zombies[i].type >= 100||zombies[i].y != y)
            continue;
        int t = abs (x - zombies[i].x);
        if (t < zx)
            zx = t, wz = i;
    }
    if(zx<=SI/2)return wz;
    else return -1;
}

int Game::LeftZombie(int w)
{
    if(w==-1||iinum[w]==-1)return -1;
    w=iinum[w];
    int rt=w,rx=-1e9,y=zombies[w].y,wx=zombies[w].x;
    for(int i=zmb_cnt-1;i>=0;i--)
    {
        if(zombies[i].type>=100||zombies[i].y!=y)
            continue;
        int x=zombies[i].x;
        if((x<wx||(x==wx&&i<w))&&x>rx)rt=i,rx=x;
    }
    return zombies[rt].ii;
}

int Game::RightZombie(int w)
{
    if(w==-1||iinum[w]==-1)return -1;
    w=iinum[w];
    int rt=w,rx=1e9,y=zombies[w].y,wx=zombies[w].x;
    for(int i=0;i<zmb_cnt;i++)
    {
        if(zombies[i].type>=100||zombies[i].y!=y)
            continue;
        int x=zombies[i].x;
        if((x>wx||(x==wx&&i>w))&&x<rx)rt=i,rx=x;
    }
    return zombies[rt].ii;
}

void Game::BaDou(int x,int y,double xu)
{
    for(int i=0;i<zmb_cnt;i++)
        if(zombies[i].y==y&&zombies[i].x>=x&&zombies[i].type<100)
            zombies[i].xu=std::max(zombies[i].xu,xu);
}
