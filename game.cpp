//游戏主类
#include "stdafx.h"
#include "mainwindow.h"
#include "game.h"
#include "./ui_mainwindow.h"

Game::Game () {
    for (int x = 0; x <MAXX; x++)
        for (int y = 0; y < MN; y++)
            plt_num[x][y] = -1;
    for (int i = 0; i < MN; i++)
        row_zmb[i] = lst[i] = 0;
    started = plt_kui = false;
    XS = 0;t_ai = is_cycle=is_fog=0;is_day=1;
    zmb_cnt=plt_cnt=shi_cnt=bul_cnt=0;
    for(int i=0;i<MAX_ID_CNT;i++)
    {
        low_zw[i]=dun_p[i]=eat_atk[i]=is_tou[i]=cantwo[i]=yg_rtn[i]=frez_fsh[i]=is_shear[i]=zw_wudi[i]=jianguo[i]=0;
        dang_mul[i]=fire_mul[i]=dian_mul[i]=has_lev[i]=has_mouth[i]=can_overzw[i]=1;
    }
    yg_rtn[1]=yg_rtn[20]=yg_rtn[12]=1;
    eat_atk[18]=eat_atk[33]=eat_atk[35]=eat_atk[58]=eat_atk[63]=eat_atk[75]=eat_atk[81]=eat_atk[101]=1;
    is_tou[8]=is_tou[10]=is_tou[56]=is_tou[69]=is_tou[87]=is_tou[92]=1;
    fire_mul[12]=2;fire_mul[1]=1.2;
    fire_mul[15]=fire_mul[4]=fire_mul[13]=fire_mul[14]=fire_mul[17]=fire_mul[20]=fire_mul[22]
        =fire_mul[23]=fire_mul[24]=fire_mul[25]=fire_mul[31]=fire_mul[32]=fire_mul[33]=1.5;
    dian_mul[2]=dian_mul[3]=dian_mul[5]=dian_mul[7]=dian_mul[27]=1.5;dian_mul[11]=0.5;
    dun_p[7]=0.8;dun_p[27]=dun_p[4]=dun_p[31]=0.5;
    low_zw[17]=low_zw[32]=low_zw[53]=low_zw[63]=low_zw[51]=low_zw[75]=1;
    dang_mul[22]=2;dang_mul[19]=dang_mul[14]=dang_mul[32]=dang_mul[15]=dang_mul[18]=dang_mul[17]=dang_mul[20]=1.7;
    dang_mul[6]=dang_mul[13]=dang_mul[12]=1.5;
    int jgs[8]={2,6,19,25,35,95,97,101};
    for(int i=0;i<8;i++)jianguo[jgs[i]]=yg_rtn[jgs[i]]=true;
    int not_lev[]={7,11,17,15,18,27,30,31,32,33,45,46,47,53,54,62,63,70,71,72,73,3,5};
    for(int i=0;i<23;i++)has_lev[not_lev[i]]=0;
    int can_two[21]={0,56,4,9,64,8,61,67,68,41,44,55,10,69,22,26,60,65,21,23,39};
    for(int i=0;i<21;i++)cantwo[can_two[i]]=1;
    has_mouth[9]=has_mouth[12]=has_mouth[11]=has_mouth[20]=has_mouth[28]=0;
    for(int x=0;x<1000;x++)
    {
        if(x<=200)frez_spd[x]=pow(0.5,x/100.0);
        else frez_spd[x]=100.0/(x+200);
    }
    int shr[24]={0,4,9,13,37,39,41,44,53,59,60,61,64,65,67,68,76,77,89,90,91,96,100,102};
    for(int i=0;i<24;i++)is_shear[shr[i]]=1;
    int zha[11]={3,5,42,57,70,72,73,74,7,93,99};
    for(int i=0;i<11;i++)is_zha[zha[i]]=zw_wudi[zha[i]]=1;
    int cntovzw[10]={1,90,22,83,26,13,59,94,39,102};
    for(int i=0;i<10;i++)can_overzw[cntovzw[i]]=0;
    car_num=1;car_ryg=0;
}
Game::~Game () {}
void Game::DestroyJs (int z,int rea) //删除僵尸
{
#ifdef TEST_MODE
    if(zombies[z].y==-1)exit(-1);
#endif
    if(zombies[z].type==25)//向日葵僵尸
        m_yg+=40,laygTM=TM;
    if (zombies[z].fei) //携带化肥
    {
        m_wnd->z_fs += 1;m_wnd->beans += 2;m_wnd->labeanTM=TM;
        if (m_wnd->beans>MAXDZS)m_wnd->beans=MAXDZS;
    }
    if(zombies[z].type<100&&rea!=-1&&zombies[z].id)//创建尸体
    {
        ShiTi st;
        st.id=zombies[z].id;
        st.x=zombies[z].x;
        st.y=zombies[z].y_flt;
        st.ty=rea;st.tm=TM;
        NewSt(st);
        if(zombies[z].dun)
        {
            st.id=GetDunIcon(zombies[z]);
            if(st.id!=-1)NewSt(st);
        }
    }
    if(zombies[z].type<100)
    {
        js_xsum+=zombies[z].x;
        js_sum+=1;
        if(zombies[z].x<SI)dang_cnt+=1;
    }
    row_zmb[zombies[z].y] -= 1;
    zmb_ii[zombies[z].ii]=-1;
    zombies[z].y=-1;//标记删除
    zmb_de.push_back(z);
}
void Game::CalFirst () //计算植物信息
{
    for (int y = 0; y < MN; y++)
        for (int x = 0; x < MAXX; x++)
            plt_num[x][y] = -1;
    for (int i = 0; i < plt_cnt; i++)
        if(plants[i].y!=-1&&plants[i].x%SI==0)
            plt_num[plants[i].x][plants[i].y] = i;
    nd_calfr=0;
}
void Game::DestroyGuan(int z)//破坏罐子
{
    int t=plants[z].attack;
    if(t>0)//植物
    {
        if(XUAN<MXUAN)
        {
            m_wnd->m_xuan[XUAN++]=t-1;//加入卡槽
            NewPicSt(plants[z].x,plants[z].y*SI,172);
        }
    }
    else //僵尸
    {
        Zombie js=m_wnd->zombies[(-t)-1];
        js.x=plants[z].x+SI/5;
        js.y=plants[z].y;
        NewJs(js);
    }
}

void Game::DestroyZw_Pre(int z,int dig)
{
    int pt=plants[z].type;
    if(pt==200)DestroyGuan(z);//罐子
    else if(plants[z].attack==-15)fans-=1;//删除风扇
    if(real&&dig&&pt>=0&&yg_rtn[pt])//返还
        m_yg+=plants[z].cost*0.7*std::max(std::min(double(plants[z].life)/plants[z].mxlf,1.0),0.0),laygTM=TM;
    if(plants[z].frez_stp)//在地面留下冰
        m_ice[plants[z].x/SI][plants[z].y]=TM+plants[z].frez_stp*20;
    int w=GetFirstJs(plants[z].x,plants[z].y,NULL,false);
    if(pt>=0&&!jianguo[pt]&&pt!=1&&w!=-1&&zombies[w].x-plants[z].x<SI)//非坚果，小向日葵类（将要）被吃掉
        zw_eat+=1;
}

int Game::DestroyZw (int z,int dig) //删除植物
{
    DestroyZw_Pre(z,dig);
    plants[z].y=-1;//标记删除
    plt_ii[plants[z].ii]=-1;
    plt_de.push_back(z);
    int rt=m_wnd->KillTimer(-plants[z].ii);
    nd_calfr=true;
    return rt;//TODO: 2048
}
void Game::DestroyZw (int x, int y,int dig) //按照位置删除
{
    int i = GetZwShu (x, y);
    if (i != -1)DestroyZw (i,dig);
}
void Game::DestroyZd_Pre (const Bullet&zd)
{
    if (zd.vx < 0&&zd.from == -2) //小车子弹
        car_driv[zd.y] = false; //清除正在小车的状态
    if(zd.ct&&zd.vis!=-1)
        zd_pol.Del(zd.vis);//回收bitset
}
void Game::DestroyZd (int z) //删除子弹
{
    DestroyZd_Pre(bullets[z]);
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
int Game::GetZwShu (int x, int y) {
    if (x < 0||x >= MAXX||y < 0||y >= MN)
        return -1;
    if(nd_calfr)CalFirst();
    return plt_num[x][y];
}
bool Game::MeiHuo (int j_i) {//TODO
    if (zombies[j_i].type == 11||zombies[j_i].type>=100)
        return false; //不能魅惑BOSS
    if (zombies[j_i].type == 9||zombies[j_i].type == 28||zombies[j_i].type == 12) //小车和巨人
    {
        int t=zombies[j_i].life/2;//生命值减半
        return HurtJs_Fast(j_i,t,0,32);
    }
    Bullet zd;
    zd.x = zombies[j_i].x;
    zd.y = zombies[j_i].y;
    zd.vx = -fabs(zombies[j_i].vx);
    zd.from = zombies[j_i].type;
    zd.attack = zombies[j_i].life;
    zd.siz = zombies[j_i].mxlf;
    zd.tx = zombies[j_i].id;
    zd.vis = 200000;
    zd.tag = 0;
    zd.du=int(std::max(1.0,std::min(zombies[j_i].eat,5.0))+0.5);//记录伤害倍率(1~5)
    dps+=zd.attack;
    NewZd (zd);//把僵尸变为子弹
    DestroyJs (j_i);
    return true;
}

int Game::FindZw(int x,int y,int ty)//(x,y)周围找ty类型
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
    if(ss)return ww[RA()%ss];
    else return -1;
}

void Game::HurtZw_Ty(int x,int y,double et,int ty,bool st)
{
    double aa=et/5,bb=et-aa;
    int w=(st?FindZw(x,y,25):-1);//寻找保护伞
    if(w!=-1)
    {
        plants[w].tag=TM;
        HurtZw(plants[w].x/SI,plants[w].y,bb,1);//分摊伤害
        HurtZw(x,y,aa,1);
    }
    else HurtZw(x,y,et,1);
    if(abs(ty)==1)//寒冰
    {
        int z=GetZwShu(x*SI,y);
        if(z!=-1)
        {
            plants[z].frez_stp=20;
            int ne[3][2]={0,1,0,-1,-1,0};
            if(ty<0)ne[2][0]=1;
            for(int s=0;s<3;s++)
            {
                int t=GetZwShu((x+ne[s][0])*SI,y+ne[s][1]);
                if(t!=-1&&plants[t].frez_stp<5)plants[t].frez_stp=5;
            }
        }
    }
    else if(abs(ty)==2)//火焰
    {
        int z=GetZwShu(x*SI,y);
        if(z!=-1)plants[z].Burn();
    }
}

bool Game::HurtZw(int i,double et,bool upid)
{
    return HurtZw(plants[i].x/SI,plants[i].y,et,upid);
}

bool Game::HurtZw(int x,int y,double et,bool upid)
{
    int w=GetZwShu(x*SI,y);bool rt=0;
    bool ng=(m_ng[x][y]>0);
    if(ng)//有南瓜
    {
        double t=std::min(m_ng[x][y],et);
        NewNgPoint(x,y,t);
        if(t<=10)expad+=3*t;//经验
        if(t>0)le_ng[x][y]=TM;
        m_ng[x][y] -= t;//先吃南瓜
        et -= t;//剩余
    }
    if(w==-1||zw_wudi[plants[w].type])//无敌植物
        return false;
    if(jianguo[plants[w].type]&&plants[w].tag==1)//坚果大招无敌
        return false;
    if(et>0)
    {
        if(plants[w].def)//TODO: 防御
        {
            int d=plants[w].def;
            et=std::min(std::min(et*2.0/(2.0+d),et-d/2.0),pow(et,std::max(0.0,1-d/10.0)));
        }
        double t=std::min(plants[w].life,et);
        NewPlantPoint(w,t);
        if(t<=10)expad+=3*t;
        if (plants[w].type == 1) {//吃向日葵
            if(m_wnd->imjs)
                m_yg += 5 * t;
            else
                ygad += t;
        }
        plants[w].life -= t;plants[w].la_et=TM;
        if(upid)
        {
            if (plants[w].life <= 0) //吃光
                DestroyZw (w,0),rt=1;
            else plants[w].id = GetZwIcon (plants[w]);
        }
    }
    return rt;
}

bool Game::HurtJs_Fast(int i,int ki,int rea,int co,int ct)
{
    ki*=zombies[i].xu;
    dps+=zombies[i].Hurt(ki,ct/10.0);
    if(ki)NewRedPoint(i,ki,true,co);
    bool rt=0;
    if (zombies[i].life <= 0)
        DestroyJs (i,rea),rt=1;
    else
        zombies[i].id = GetJsIcon (zombies[i]);
    return rt;
}

bool Game::HurtJs(int i, int ki, int rea, int co, short frez, int ct, double fire, int du, double xu, double q)
{
    if(zombies[i].type==10&&zombies[i].fss!=-1)
        return 0;
    ki=CalHurt(i,ki,frez,fire,du,xu,q,ct);
    if(ki)NewRedPoint(i,ki,true,co);
    dps+=zombies[i].Hurt(ki,ct/10.0);
    bool rt=0;
    if (zombies[i].life <= 0)
        DestroyJs(i,rea), rt=1;
    else
        zombies[i].id = GetJsIcon (zombies[i]);
    return rt;
}

bool Game::HurtJs(int i, int rea, int co, const HURT &hu)
{
    return HurtJs(i,hu.atk,rea,co,hu.frez,hu.ct,hu.fire,hu.du,hu.xu,hu.q);
}

int Game::SpecialEat(int z_i,int j_i,double sq)
{
    int ty=(z_i==-1?-1:plants[z_i].type);
    Zombie &js=zombies[j_i];
    switch(js.type)
    {
    case 5://撑杆跳
        if(ty!=6)//并非高坚果
            js.x_flt-=(js.vx<0?-SI*1.1:SI*1.1);
        js.setspd(2);js.dun=0;
        js.sp_eat=false;js.type=35;
        return GetJsIcon(js);
    case 8://跳跳
        js.setspd(2);
        js.sp_eat=false;js.type=36;
        return GetJsIcon(js);
    case 15://炸弹僵尸
        NewPicSt(js.x,js.y*SI,128);//砰！
        KillZw(js.x/SI-1,js.x/SI+1,js.y-1,js.y+1,50,2,1,0.2);
        DestroyJs (j_i,1);
        return 0;
    case 17://魔法僵尸
        if(z_i==-1)return -1;
        MagicZombie(j_i);js.eating=0;
        return js.id;
    case 9:
    case 28://巨人
    {
        double ad=1.0;
        if(m_wnd->m_ea>1)ad=1.0/sqrt(m_wnd->m_ea);
        if (m_wnd->m_hd)ad += 0.5;
        if (m_wnd->m_imp)ad += 0.5;
        if(js.type == 28)ad*=2;//超级巨人
        ad*=sq;js.tag+=int(ad*8+0.5);
        if (js.tag >= 200) {
            int ki=0;
            if(ty==53||ty==35||ty==101)
            {
                ki=40000;//砸到有毒植物
                LineXu(js.x,js.y,0.5);
            }
            if(z_i!=-1)HurtZw(z_i,js.eat,1);
            js.tag=0;
            if(ki>0&&HurtJs_Fast(j_i,ki,0,44))
                return 0;
        }
        return js.id;
    }
    case 18://气功
        if(z_i==-1)return -1;
        for(int x=plants[z_i].x;x>=0;x-=SI)
        {
            int t=GetZwShu(x,plants[z_i].y);
            if(t!=-1)
                plants[t].x-=SI;
            else break;
        }
        for(int i=0;i<plt_cnt;i++)
        {
            if(plants[i].y!=-1&&plants[i].x<0)
            {
                DestroyZw(i,0);
                break;
            }
        }
        nd_calfr=true;
        return js.id;
    case 27://扶梯僵尸
        if(z_i==-1)return -1;
        plants[z_i].ti=1;nd_calfr=true;
        return ClearDun(js);
    case 33://窝瓜僵尸
        if(z_i==-1)return -1;
        else
        {
            Zombie njs;
            njs.Create(0,1e4,0,plants[z_i].x,plants[z_i].y,12,102,0,1,20);
            NewJs(njs);
            DestroyJs(js.num);return 0;
        }
    case 100://子弹
        if(z_i==-1)return -1;
        else
        {
            Plant &zw=plants[z_i==-1?0:z_i];
            if(zw.type==25)//反弹
            {
                int x=js.x,y=js.y,d=js.id;
                zw.life-=js.eat;
                zw.id=GetZwIcon(zw);
                if(zw.life<=0)
                {
                    DestroyZw(zw.num,0);
                    return 0;
                }
                Bullet zd=GetFanTan(d);
                zd.attack*=2;
                zd.x=x-SI/2;zd.y=y;
                NewZd(zd);
                DestroyJs(js.num);return 0;
            }
            int w=(js.id==183?1:0);
            if(js.vx<0)w=-w;
            HurtZw_Ty(zw.x/SI,zw.y,js.eat,w,1);
            DestroyJs(js.num);
            return 0;
        }
    case 101://火药桶
        Explode(plants[z_i].x,plants[z_i].y,2,80,30);
        DestroyJs(js.num);
        return 0;
    }
    return js.id;
}

bool Game::JsShoot(int j_i)
{
    Zombie &js=zombies[j_i];
    Zombie nj;int w;nj.vx=0;
    switch(js.type)
    {
    case 9://巨人
        if(js.life<=js.mxlf/2&&js.vx>0)
        {
            int x=std::max(js.x-450,120);
            nj.Create(3,5,1,js.x,js.y,50,10);
            nj.fss=x;nj.fs_nd=js.x;
            if(nj.fss<js.x)NewJs(nj);
            if(m_wnd->m_hd||m_wnd->m_ea==1)
            {
                nj.Create(4,m_wnd->m_hd?10:5,m_wnd->m_hd?2:1,js.x,js.y,50,10);
                nj.fss=x+75;nj.fs_nd=js.x;
                if(nj.fss<js.x)NewJs(nj);
            }
            if(m_wnd->m_hd)
            {
                nj.Create(5,15,2,js.x,js.y,50,10);nj.fss=x+150;
                nj.fs_nd=js.x;
                if(nj.fss<js.x)NewJs(nj);
            }
            if(m_wnd->m_imp)
            {
                nj.Create(6,20,3,js.x,js.y,50,10);
                nj.fss=x+225;nj.fs_nd=js.x;
                if(nj.fss<js.x)NewJs(nj);
            }
            js.shoot=false;
        }
        break;
    case 13://豌豆僵尸
        if(!nj.vx)nj.Create(18,1e4,8,js.x,js.y,75,100,true);
    case 14://冰瓜僵尸
        if(!nj.vx)nj.Create(18,1e4,20,js.x,js.y,81,100,true);
    case 26://火药桶
        if(!nj.vx)nj.Create(22,1e4,1,js.x,js.y,207,101,true);
    case 22://冰球
        if(!nj.vx)nj.Create(18,1e4,13,js.x,js.y,183,100,true);
        if(js.vx<0)nj.vx=-nj.vx;
        nj.y_flt=js.y_flt;NewJs(nj);
        break;
    case 28://超级巨人
        w=std::max(js.x-400,120);
        nj.Create(5,10,1,js.x,js.y,50,10);
        nj.fss=w;nj.fs_nd=js.x;
        if(nj.fss<js.x)NewJs(nj);
        break;
    case 32://辣椒
        if(js.x<MM*SI/2)
        {
            KillZw(0,MM-1,js.y,js.y,20,2,0,0.2);
            DestroyJs(js.num,1);return 1;
        }
        break;
    case 16://末影人
        js.x=RA()%(MM-1)*SI;
        row_zmb[js.y]-=1;
        do{js.y=RA()%MN;}
        while(iswa[js.y]);
        js.y_flt=js.y*SI;
        row_zmb[js.y]+=1;
        w=FindZw(js.x/SI,js.y,25);
        if(w!=-1)
        {
            plants[w].tag=TM;
            DestroyJs(js.num,0);return 1;
        }
        break;
    case 20://喷火
    {
        int tx=js.x,ty = js.y;
        for(int x=tx/SI;x>=0&&tx-1.8*SI<=x*SI;x--)
        {
            int ti=GetZwShu(x*SI,ty);
            if(ti!=-1&&plants[ti].attack==-21&&plants[ti].grow<=0)
            {
                Goj(ti);
                return 0;
            }
            if(m_ng[x][ty]||ti!=-1)
            {
                KillZwPos(x,ty,2,(m_wnd->imjs?0:2),0,0.002);
                js.eating = true;//正在吃
            }
        }
        break;
    }
    case 102://窝瓜僵尸的窝瓜
        KillZwPos(js.x/SI,js.y,100,0,0,0.5);
        DestroyJs(js.num);return 1;
    }
    return 0;
}

void Game::ResetZwTimer(int z_i)
{
    m_wnd->KillTimer(-plants[z_i].ii);
    m_wnd->SetTimer(-plants[z_i].ii,plants[z_i].time);
}

bool Game::DaZhao(int z_i,double p)
{
    bool tm=false;
    Plant &zw=plants[z_i];
    if(zw.dzsy)return 0;
    switch(zw.type)
    {
    case 1:
    case 0:
    case 4:
    case 9:
    case 8:
    case 10:
    case 13:
    case 22:
    case 26:
    case 39:
    case 12:
    case 20:
    case 44:
    case 59:
    case 60:
    case 61:
    case 64:
    case 56:
    case 76:
    case 77:
    case 80:
    case 83:
    case 86:
    case 87:
    case 88:
    case 89:
    case 90:
    case 91:
    case 94:
    case 96:
    case 100:
        zw.odtm=zw.time;zw.time=100;tm=true;
        break;
    case 41:
    case 37:
        zw.odtm=zw.time;zw.time=40;tm=true;
        break;
    case 102:
        zw.odtm=zw.time;zw.time=300;tm=true;
        break;
    case 2:
    case 6:
    case 19:
    case 95:
    case 97:
    case 35:
    case 25:
    case 101:
        zw.odtm=zw.time;zw.time=1000;zw.tag=1;tm=true;
        break;
    case 32:
        for(int i=0;i<MN;i++)
        {
            Bullet zd;zd.mh=true;
            zd.color=MHCO;
            zd.siz=14*SI/110;
            zd.attack=0;zd.vx=4;
            zd.x=zw.x;zd.y=i;
            NewZd(zd);
        }
        break;
    case 98:
        zw.tag+=5;
        break;
    default:
        return false;
    }
    if(zw.life<zw.mxlf)zw.life=zw.mxlf;
    if(tm)
    {
        ResetZwTimer(z_i);
        zw.dzsy=zw.dztm*zw.mul*p;
        if(zw.type==37)zw.dzsy*=(zw.tag+1);
    }
    zw.id=GetZwIcon(zw);
    return true;
}

void Game::EndDaZhao(int z_i)
{
    Plant &zw=plants[z_i];
    bool tm=false;
    if(zw.type>=0&&jianguo[zw.type])
    {
        if(zw.tag==1)zw.dzsy=zw.dztm,zw.def+=1,zw.tag=0;
        else zw.def-=1,zw.time=zw.odtm,tm=1;
    }
    else
        zw.time=zw.odtm,tm=1;
    if(tm)ResetZwTimer(z_i);
}

bool Game::ZombieCanEatPlant(int j_i, int x, int y)
{
    int z_i=GetZwShu(x*SI,y),zt=zombies[j_i].type;
    if(zt==19)return false;//气球
    if(z_i==-1&&m_ng[x][y]==0)return false;//空
    if(z_i!=-1)
    {
        int pt=plants[z_i].type;
        if(pt==29&&!m_ng[x][y])return false;//地刺
        if(zt==8&&pt!=6)return false;//跳跳，并非高坚果
        if(pt==200&&zt!=9&&zt!=28)return false;//非巨人，越过罐子
        if(plants[z_i].id==126&&zt!=9&&zt!=12&&zt!=11&&zt!=28)//越过全息坚果底座
            return false;
        if(zt>=100&&low_zw[pt])return false;//子弹，低矮植物
        if(plants[z_i].ti&&zt!=11&&zt!=12)return false;//梯子，并非boss和车
        return true;
    }
    else//只有南瓜
    {
        if(zt==8)return false;//跳跳
        else return true;
    }
}

bool Game::Eat (int x, int y,int j_i,double sq) //吃,返回是否被杀死
{
    int z_i=GetZwShu(x*SI,y);
    Zombie&js=zombies[j_i];
    if(z_i!=-1&&js.type<100)//对于大嘴花等植物的判定
    {
        int t=plants[z_i].type;
        if(plants[z_i].grow<=0&&(t==7||t==17||t==16))
            {Goj(z_i);return 0;}
        t=GetZwShu(plants[z_i].x-SI,plants[z_i].y);
        if(t!=-1&&plants[t].grow<=0&&plants[t].type==16)//大嘴花，隔位
            {Goj(t);return 0;}
    }
    if (js.sp_eat) //特殊吃法
    {
        int t = SpecialEat (z_i, j_i, sq);
        if(t>0)js.id = t;
        if(t!=-1)return t == 0;
    }
    double et = sq*js.eat;
    HurtZw(x,y,et,0);//吃
    if (z_i!=-1&&js.type < 100 && js.type !=11) //吃东西影响生命（boss除外）
    {
        int ki=0,pt=plants[z_i].type,zt=js.type,co=1;
        if(has_mouth[zt])
        {
            if ((eat_atk[pt]||plants[z_i].Burning())&&m_ng[x][y]==0) //反伤或燃烧
            {
                ki=30*std::fmin(et,8);
                if(pt==58||pt==63||pt==18)//减速
                    FreezeJs(j_i,(pt==63?320:80));
                else if(pt==63)FreezeStopJs(j_i,80);//冰豆
                else if(pt==75)//虚弱豆
                    js.xu=std::max(js.xu,2.0);
                else if(pt==33)//大蒜
                {
                    int ad=(RA()%2==0?-1:1),yy=js.y;
                    if(ad==-1&&yy==0)ad=1;
                    else if(ad==1&&yy==MN-1)ad=-1;
                    row_zmb[yy]-=1;row_zmb[yy+=ad]+=1;
                    if(row_zmb[yy]==1)m_wnd->WakeUp(yy);
                    js.y=yy;js.y_flt=yy*SI;
                }
                else if(pt==81||pt==35)
                    js.du+=int(js.eat+0.5),co=35;
                else if(pt==101)//中毒
                    js.du+=int(3*js.eat+0.5),co=35;
            }
            else if(pt==95)//导电坚果
            {
                double t=js.q*DIAN_TRANS;
                ki+=int(t*t*DIAN_HURT*dian_mul[js.type]+0.5);
                js.q-=t;co=90;
            }
            else //吃东西回复
            {
                ki-=6*et;
                if(pt==97)ki*=20;//脑子坚果
            }
        }
        if(zt == 12&&!m_wnd->m_imp)
            ki+=(m_wnd->m_hd?800:1500);//小车压植物，车受损
        if(pt==51&&js.vx>0&&has_mouth[zt])//给僵尸加速的植物
            js.vx=std::max(js.vx, 6.0);
        else if(pt==53)//巴豆
        {
            if(has_mouth[zt])
                ki=js.life,LineXu(js.x,js.y,0.2);
            else if(zt==9||zt==28)
                LineXu(js.x,js.y,0.5);//砸扁，造成更高虚弱效果
            co=53;
        }
        if(HurtJs_Fast(j_i,ki,0,co))
        {
            if (plants[z_i].life <= 0) //吃光(特判)
                DestroyZw (z_i,0);
            return true;
        }
        else js.id = GetJsIcon(js); //更新图标
    }
    if(z_i==-1)return false;
    bool rt = false;
    if (plants[z_i].type == 32&&m_ng[x][y] == 0&&!(plants[z_i].mush&&is_day)&&!plants[z_i].stop
       &&has_mouth[js.type]) //吃魅惑菇
    {
        rt = true;int w=m_wnd->tyToNumZ[js.type];
        js.life=std::max(js.life,js.mxlf);//恢复
        js.id=m_wnd->zombies[w].id;
        MeiHuo (j_i);plants[z_i].life=0;
    }
    if (plants[z_i].life <= 0) //吃光
        DestroyZw (z_i,0);
    else
        plants[z_i].id = GetZwIcon (plants[z_i]);
    return rt;
}
int Game::GetFirstJs (int x, int y,const Bullet*z,bool ck100) {
    int zx = inf, rt = -1;
    for (int i = 0; i < zmb_cnt; i++) //遍历僵尸
        if (zombies[i].y!=-1 && zombies[i].x < zx&&zombies[i].x > x&&zombies[i].y == y&&(!ck100||zombies[i].type < 100) && !CannotReach(z,i))
            rt = i,zx = zombies[i].x;
    return rt;
}
int Game::GetLastJs (int x, int y,const Bullet*z,bool ck100) {
    int zd = -inf, rt = -1;
    for (int i = 0; i < zmb_cnt; i++) //遍历僵尸
        if (zombies[i].y!=-1 && zombies[i].x > zd&&zombies[i].x < x&&zombies[i].y == y&&(!ck100||zombies[i].type < 100) && !CannotReach(z,i))
            rt = i,zd = zombies[i].x;
    return rt;
}

bool Game::NeedBullet(int y)
{
    if(row_zmb[y])return true;
    for(int i=0;i<MM;i++)
    {
        int w=GetZwShu(i*SI,y);
        if(w!=-1&&(plants[w].frez_stp||(plants[w].type==85&&zmb_cnt>0)))//植物被冻结，或分裂器
            return true;
    }
    return false;
}

Bullet Game::BulletFromZw(int z_i)
{
    double mul=plants[z_i].mul;
    Bullet zd; //发射子弹
    zd.color = plants[z_i].color;
    zd.vx=plants[z_i].move_spd*2;
    if(is_tou[plants[z_i].type])
        zd.x = plants[z_i].x-SI*0.45;
    else zd.x = plants[z_i].x+SI/4;
    zd.y = plants[z_i].y;
    zd.tag=plants[z_i].x;
    zd.attack = plants[z_i].attack*mul;
    zd.siz = plants[z_i].siz;
    zd.freeze=plants[z_i].freeze*mul;
    zd.tx = plants[z_i].x - 1;
    zd.jian = plants[z_i].jian*mul;
    zd.left=plants[z_i].left;
    zd.from=plants[z_i].type;
    zd.du = plants[z_i].du*mul;
    zd.fire=0;
    if(zd.color==RED||zd.color==DBLUE)
        zd.fire=10;
    if(zd.from==92)//辣椒投手
        zd.fire=20;
    if(zd.from==26||zd.from==59||zd.from==60)
        zd.ct=true,zd.vis=-1;
    if(zd.from==76)zd.xu=0.1;//虚弱射手
    if(zd.from==90)zd.q=0.9;//闪电射手
    zd.fire*=mul;zd.xu*=mul;zd.q*=mul;
    if(zd.left)
    {
        if(is_tou[zd.from])zd.x+=SI;
        else zd.x-=SI/2;
    }
    return zd;
}

int Game::GetMWCGoal()
{
    int zx=inf,rt=-1;
    for(int i=0;i<zmb_cnt;i++)
    {
        if(zombies[i].y==-1||zombies[i].type>=100)
            continue;
        int x=(zombies[i].x-HOME_X)/dang_mul[zombies[i].type];
        if(x<zx)zx=x,rt=zombies[i].ii;
    }
    return rt;
}

void Game::Goj (int z_i) //植物攻击
{
    if(plants[z_i].y==-1)return;
    int dzsy=plants[z_i].dzsy;bool ladz=0;//是否为最后一次大招
    if(dzsy)//正在释放大招
    {
        plants[z_i].dzsy-=1;
        if(plants[z_i].dzsy==0)//大招结束
            EndDaZhao(z_i),ladz=1;
    }
    plants[z_i].nxt_tm = m_wnd->v_tm + plants[z_i].time;
    if(plants[z_i].stop||plants[z_i].frez_stp)return;
    if (plants[z_i].mush&&is_day)return;
    if (plants[z_i].attack < 0) //非标准植物
        return SpecialPlt (z_i);
    if(jianguo[plants[z_i].type])return;
    Bullet zd=BulletFromZw(z_i);
    if(is_tou[zd.from]||zd.from==13)//投手，轨迹，小喷菇
    {
        if(zd.left)
        {
            int t=GetLastJs(zd.x,zd.y,&zd);
            zd.tx=(t==-1?-150:zombies[t].x);
        }
        else
        {
            int t=GetFirstJs(zd.x,zd.y,&zd);
            zd.tx=(t==-1?MM*SI+20:zombies[t].x);
        }
        if(zd.from==13)//小喷菇
        {
            if(abs(zd.x-zd.tx)>400)
                return;
        }
        else
        {
            zd.vy=-abs(zd.tx-zd.x)/zd.vx/2*G;
            zd.y_flt=zd.y*SI;
        }
    }
    if(!dzsy&&(zd.from==0||zd.from==4||zd.from==9||zd.from==61||zd.from==60||zd.from==65||zd.from==67||zd.from==86||zd.from==90||zd.from==100))//可以暴击
    {
        int r=RA()%100,bj=plants[z_i].bj;
        if(bj>MAXBJ)bj=MAXBJ;
        if(r<bj*10)
        {
            if(r<bj*bj)zd.attack*=4,zd.du*=4,zd.siz*=1.8,zd.xu*=4,zd.fire*=2,zd.freeze*=2,zd.q*=2,zd.jian+=20;//双重暴击
            else zd.attack*=2,zd.du*=2,zd.siz*=1.4,zd.xu*=4,zd.fire*=1.5,zd.freeze*=1.5,zd.q*=1.5,zd.jian+=10;
        }
    }
    else if(plants[z_i].bj>1)//不能暴击的植物
        zd.attack*=(1.0+0.05*(plants[z_i].bj-1));
    if(ladz)zd.attack*=5,zd.du*=5,zd.siz*=2,zd.jian+=30,zd.xu*=5,zd.fire*=3,zd.freeze*=3,zd.q*=3;//最后一发为强力子弹
    if((plants[z_i].type==41||plants[z_i].type == 37)&&dzsy)//圆形
    {
        zd.vx=4;int s=20;
        double r=(s-1)/2.0;
        double o=s/2.0;
        double t=r*r-((dzsy-1)%s-o)*((dzsy-1)%s-o),h=0;
        if(t>=0)h=sqrt(t);
        if(dzsy%2==0)h=-h;
        if((dzsy-1)/s%2==0)h+=SI/5.0/8;
        zd.y_flt=zd.y*SI+h*8;
        if(plants[z_i].type==41)
        {
            int i=(dzsy-1)/s%3;
            if(i==1)
                zd.color=RED,zd.fire=10;
            else if(i==2)
                zd.color=DBLUE,zd.freeze=160,zd.fire=10;
        }
        if(zd.y_flt>=0&&zd.y_flt<MN*SI)
        {
            zd.y=int(zd.y_flt/SI+0.5);
            NewZd (zd);
        }
    }
    else if((plants[z_i].type == 64||plants[z_i].type == 68)&&dzsy)
    {
        zd.xie=true;zd.x=plants[z_i].x;
        double spd=zd.vx,d=sin(dzsy*0.7)*2;
        for(int i=-1;i<=1;i++)
        {
            zd.setspd(spd,(i+0.1*d)/15.0*2*PI);
            NewZd (zd);
        }
    }
    else if ((plants[z_i].type == 37||plants[z_i].type == 64||plants[z_i].type == 68)&& NeedBullet(plants[z_i].y)) { //豌豆荚，双发
        int t = plants[z_i].tag+1;
        if(plants[z_i].type == 64||plants[z_i].type == 68)
        {
            t=2;
            while(t<10&&RA()%4==0)t+=1;//可发射多发，期望1/3
        }
        zd.y=(zd.y+dzsy)%MN;
        for (int s = 0; s < t; s++) {
            Bullet tz=zd;
            NewZd (tz);
            if(dzsy)zd.y=(zd.y+1)%MN;
            else zd.x += (zd.left?-SI / 3:SI / 3);
        }
    }
    else if((plants[z_i].type==86||plants[z_i].type==26) && (NeedBullet(plants[z_i].y)||dzsy))//双头豌豆
    {
        zd.left=false;NewZd(zd);
        zd.left=true;
        zd.x-=SI/3;NewZd(zd);
        if(plants[z_i].type==86)zd.x-=SI/3,NewZd(zd);
    }
    else if ((plants[z_i].type == 44 ||plants[z_i].type == 77)&& (NeedBullet(plants[z_i].y)||dzsy)) { //机枪豌豆
        if(dzsy)
            zd.xie=true,zd.x=plants[z_i].x;
        double spd=zd.vx;
        for (int s = 0; s<4; s++) {
            Bullet tz=zd;
            if(dzsy)
                tz.setspd(spd,(s+0.15*(dzsy-1))/4.0*2*PI);
            if(plants[z_i].type == 44)//彩虹机枪
            {
                if(s==3)//深蓝
                    tz.color=DBLUE,tz.freeze=160,tz.fire=10;
                else if(s==2)//毒性
                {
                    if(RA()%100==0&& !m_wnd->imjs)//1%魅惑
                        tz.mh=1,tz.color=MHCO;
                    else
                        tz.attack=3000,tz.color=GREEN,tz.du=40;
                }
                else if(s==1)//虚弱
                    tz.color=Qt::gray,tz.xu=0.1;
                else if(s==0)//穿透
                    tz.color=PURPLE,tz.ct=true;
            }
            NewZd (tz);
            if(!dzsy)zd.x += (zd.left?-SI / 3:SI / 3);
        }
    }
    else if(plants[z_i].type==41)//三线
    {
        bool fs=0;
        for (int z = -1; z <= 1; z++)
        {
            int y=plants[z_i].y+z;
            if (y>=0&&y<MN&& NeedBullet(y)){fs=1;break;}
        }
        if(fs||dzsy)
        {
            zd.y_flt=zd.y*SI;
            for (int z = -1; z <= 1; z++) {
                int y=(plants[z_i].y+dzsy)%MN+z;
                if (y>=0&&y<MN) {
                    zd.y = y;
                    if(plants[z_i].tag%5==0)//20%发射火焰
                        zd.color=RED,zd.fire=10;
                    if(plants[z_i].tag==0)//5%发射寒冰
                        zd.color=DBLUE,zd.freeze=160,zd.fire=10;
                    if(y!=plants[z_i].y)
                    {
                        int w=GetFirstJs(zd.x,zd.y,&zd);
                        zd.tx=(w==-1?MM * SI + 20:zombies[w].x);
                        zd.vy=(zd.y*SI-zd.y_flt)/((zd.tx-zd.x)/zd.vx);
                        if(fabs(zd.vy)>8)zd.vy=zd.vy/fabs(zd.vy)*8;
                        if(fabs(zd.vy)<5)zd.vy=zd.vy/fabs(zd.vy)*5;
                    }
                    NewZd (zd);
                }
            }
        }
        plants[z_i].tag=(plants[z_i].tag+1)%20;
    }
    else if((plants[z_i].type==22||plants[z_i].type==83)&&(zmb_cnt>0||dzsy))//星星
    {
        zd.xie=true;zd.x=plants[z_i].x;
        if(dzsy)zd.ct=true;
        double spd=zd.vx;
        for(int i=0;i<(dzsy?10:5);i++)
        {
            if(dzsy)zd.setspd(spd,(i+0.1*dzsy)/10.0*2*PI);
            else zd.setspd(spd,(i+0.5)/5.0*2*PI);
            NewZd (zd);
        }
    }
    else if(plants[z_i].type==88&&(zmb_cnt>0||dzsy))//旋转射手
    {
        zd.xie=true;zd.x=plants[z_i].x;
        double spd=zd.vx;
        for(int i=0;i<5;i++)
        {
            Bullet tz=zd;
            tz.setspd(spd,(-i/5.0+plants[z_i].tr/360.0)*2*PI);
            if(i==1)tz.color=RED,tz.fire=20;
            else if(i==2)tz.color=YELLOW,tz.du=40;
            else if(i==3)tz.color=Qt::gray,tz.xu=0.25;
            else if(i==4)tz.color=BLUE,tz.freeze=200;
            NewZd (tz);
        }
    }
    else if(plants[z_i].type==102&&(zmb_cnt>0||dzsy))//橡木弓手
    {
        zd.xie=true;zd.y_flt=zd.y*SI;
        double spd=zd.vx,p=18;int s=1;
        if(dzsy)s=3,p=36;
        for(int i=-s;i<=s;i++)
        {
            Bullet tz=zd;
            tz.setspd(spd,(i/p)*2*PI);
            tz.y_flt+=sin((i/p)*2*PI)*zd.siz*1.1;
            NewZd (tz);
        }
    }
    else if(plants[z_i].type==94)//猫尾草
    {
        int t=GetMWCGoal();
        if(t!=-1)
        {
            zd.x=plants[z_i].x-SI/4.2;
            zd.y_flt=zd.y*SI-SI/3.15;
            zd.xie=true;zd.tag=t;
            zd.vx=4*cos(PI*0.35);zd.vy=-4*sin(PI*0.35);
            NewZd (zd);
        }
    }
    else
    {
        if(plants[z_i].type==56)//玉米
        {
            if(plants[z_i].tag==0||dzsy)
                plants[z_i].color=BUTTER;
            else plants[z_i].color=YUMI;
            plants[z_i].tag=(plants[z_i].tag+1)%5;
        }
        else if(plants[z_i].type==39)//仙人掌
        {
            bool bk=false;zd.siz=2;
            for(int i=0;i<zmb_cnt;i++)
            {
                if(zombies[i].type==19&&zombies[i].y==plants[z_i].y)
                    {bk=true;break;}
            }
            if(bk)zd.siz=1,plants[z_i].tag=1;//找到气球
            else plants[z_i].tag=0;
        }
        if(dzsy&&zd.y_flt<0&&!is_tou[plants[z_i].type])
            zd.y_flt=zd.y*SI+sin(dzsy)*(SI/6);
        if(NeedBullet(plants[z_i].y)||dzsy)
            NewZd(zd);
    }
}
void Game::NewZd (Bullet zd) {
    if (bul_cnt > MAX_BUL_CNT-10) //上限
        return;
    if(zd.x_flt<0)zd.x_flt=zd.x;
    if(zd.y_flt<0)zd.y_flt=zd.y*SI;
    bullets[bul_cnt++] = zd;
}
void Game::NewJs (Zombie js) {
    if (zmb_cnt > MAX_ZMB_CNT-10) //上限
        return;
    if(js.x_flt<0)js.x_flt=js.x;
    if(js.y_flt<0)js.y_flt=js.y*SI;
    row_zmb[js.y] += 1;
    if (row_zmb[js.y] == 1)//唤醒
        m_wnd->WakeUp (js.y);
    js.ii=iis_z;int t=js.life;
    js.dun=int(t*dun_p[js.type]);
    js.life=t-js.dun;
    js.mxlf=js.life;
    js.mxdun=js.dun;
    zombies[zmb_cnt] = js;
    zombies[zmb_cnt].num = zmb_cnt;
    zmb_ii[iis_z++]=zmb_cnt ++;
}
void Game::NewSt(ShiTi st)
{
    if (shi_cnt > MAX_ST_CNT-10) //上限
        return;
    if(m_wnd->b_fast)return;
    if(m_wnd->low_eff&&(st.ty==0||st.ty==3||st.ty==4||st.ty==6||st.ty==10))
        return;
    shiti[shi_cnt++]=st;
}

void Game::MoveZw(int z,int x,int y)
{
    if(z==-1)return;
    int i=-plants[z].ii;
    int ne=m_wnd->KillTimer (i);
    ClearPlantWzEff(plants[z]);
    if(x>=0)plants[z].x = x, plants[z].y = y;
    AddPlantWzEff(plants[z]);
    if(ne==-1)m_wnd->SetTimer (i, plants[z].time);//更新计时器
    else m_wnd->NewTimer(i,plants[z].time,ne);
    nd_calfr=true;
}

void Game::ChangeLev (Plant &zw,int f) { //施肥，仅数值
    if (zw.dzsy)return;
    if(!has_lev[zw.type])
    {
        zw.mul*=1+f*0.1;
        return;
    }
    if(f>0)
    {
        zw.lev=f;double tm=1+0.3*f;
        zw.time/=pow(tm,0.3);zw.mul*=pow(tm,0.7);
        zw.bj+=f/3;
        double k=1+0.6*f,b=pow(f,1.5)*50;
        zw.life=int(k*zw.life+b+0.5);
        zw.mxlf=int(k*zw.mxlf+b+0.5);
    }
    zw.id = GetZwIcon (zw);
}

void Game::ClearLev(Plant &zw)
{
    int f=zw.lev;zw.lev=0;
    if(!has_lev[zw.type])
    {
        zw.mul/=1+f*0.1;
        return;
    }
    if(f>0)
    {
        zw.bj-=f/3;
        double tm=1+0.3*f;
        zw.time*=pow(tm,0.3);
        zw.mul/=pow(tm,0.7);
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

void Game::AddPlantWzEff(Plant &zw)
{
    int f=CountFei(zw.x/SI,zw.y);
    f=std::min(f+zw.levod,100);
    ChangeLev(zw,f);
    int x=zw.x/SI,y=zw.y;
    zw.time/=spd_mul[x][y];
    zw.life*=lf_mul[x][y];
    zw.id = GetZwIcon (zw);
}

void Game::AddPlantWzEff(int x, int y)
{
    int t=GetZwShu(x,y);
    if(t!=-1)AddPlantWzEff(plants[t]);
}

void Game::ClearPlantWzEff(Plant &zw)
{
    int x=zw.x/SI,y=zw.y;
    zw.time*=spd_mul[x][y];
    zw.life/=lf_mul[x][y];
    ClearLev(zw);
    zw.id = GetZwIcon (zw);
}

void Game::ClearPlantWzEff(int x, int y)
{
    int t=GetZwShu(x,y);
    if(t!=-1)ClearPlantWzEff(plants[t]);
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

double Game::CalJSQ(int x, int y)
{
    double cn=0;
    for (int i=-1;i<=1;i++) //寻找加速器
    {
        for(int j=-1;j<=1;j++)
        {
            int t=GetZwShu(x+i*SI,y+j);
            if (t!=-1&&plants[t].type==34)
                cn+=plants[t].mul,plants[t].tag=TM;
        }
    }
    return 1+0.25*pow(cn,0.8);
}

int Game::FakePlant(const Plant&zw)
{
    int w=GetZwShu(zw.x,zw.y);
    switch(zw.type)
    {
    case 66: //血瓶
        if(w!=-1) {
            int mx=plants[w].mxlf;
            double t=sqrt(double(zw.cost)/plants[w].cost);
            plants[w].life+=int(0.8*t*zw.mul*mx+0.5);
            if(plants[w].life>mx)plants[w].life=mx;
            return 1;
        }
        else return 0;
    case 38: //化肥
        return ShiFei(zw.x/SI,zw.y);
    case 24: //南瓜
        m_ng[zw.x / SI][zw.y] = zw.life*zw.mul;
        return 1;
    case 28: //咖啡豆
        if (w != -1&&plants[w].mush) {
            plants[w].mush = false;
            if(plants[w].odtm<=5)
                Goj(w);//瞬间触发
            return 1;
        }
        else return 0;
    case 14: //荷叶
        if (grid_type[zw.x / SI][zw.y] == 1)
        {
            m_hy[zw.x / SI][zw.y]=zw.life;
            return 1;
        }
        else return 0;
    case 43: //翻转
        if (w != -1&&plants[w].attack>0&&plants[w].type!=86){//双头豌豆不能翻转
            plants[w].left^=1;
            return 1;
        }
        else return 0;
    case 49://点火
        return (w!=-1&&plants[w].Burn());
    case 50://灭火
        if(w!=-1&&plants[w].Burning()) {
            plants[w].fire=FIRETM+1;
            return 1;
        }
        else return 0;
    case 52://开关
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
        if (w != -1&&plants[w].type == 20&& plants[w].dzsy==0) {
            if (plants[w].tag == 1)return 0;
            plants[w].tag = 1;
            plants[w].id = GetZwIcon (plants[w]);
            return 1;
        }
    }
    if(zw.type>=0&&m_wnd->b_two&&cantwo[zw.type])//双倍
    {
        if (w != -1&&plants[w].type == zw.type&& plants[w].dzsy==0&& plants[w].lev==0) {
            if (plants[w].grow == 1)return 0;
            plants[w].time/=2;plants[w].grow=1;
            m_wnd->KillTimer(plants[w].num);
            m_wnd->SetTimer(-plants[w].ii,plants[w].time);
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
    if(plants[w].type==85)
    {
        int cn=0;
        for(int i=0;i<plt_cnt;i++)cn+=1;
        if(cn>=MN*3)return 1;
        else return 2;
    }
    return 2;
}

bool Game::PlantZw(Plant zw, int x, int y)
{
    zw.x=x*SI;zw.y=y;
    return PlantZw(zw);
}

bool Game::Valid_Grid_Type(int x, int y) const
{
    return (grid_type[x][y]==0||m_hy[x][y]>0);
}

bool Game::PlantZw (Plant zw) {
    if(zw.type==3&&zw.tag==1)//大炮的炸弹
    {
        NewZw(zw);
        return true;
    }
    int wx=zw.x/SI,wy=zw.y;
    if(IsKeng(wx,wy))return false;//坑
    if(m_yg<zw.cost||(IsIcy(zw.x/SI,zw.y)&&zw.type!=5&&zw.type!=3&&zw.type!=72))//有冰，而且不是炸弹，辣椒
        return false;
    if (grid_type[wx][wy]==1&&m_hy[wx][wy]==0&&zw.type!=14) //有坑，或有水且不是荷叶
    {
        int co=m_wnd->plants[21].cost;
        if(((m_wnd->m_ai&&m_wnd->cd[21]>800-eps)||m_yg>inf/2)&&m_yg>=zw.cost+co)
        {
            m_yg-=co;m_wnd->cd[21]=0;
            m_hy[wx][wy] = m_wnd->plants[21].life;
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
    if(w!=-1&&jianguo[zw.type]&&plants[w].type==zw.type&&zw.life>plants[w].life)//包扎
    {
        DestroyZw(w,1);
        w=-1;
    }
    if (w != -1) //非空
    {
        m_yg += zw.cost;
        return false;
    }
    if(zw.type==54)fans+=1;//三叶草
    if(zw.type==80)zw.tag=int(zw.life*1000);//菜问
    if(zw.type==98)zw.grow=1;
    if (zw.type==17||zw.type==30||zw.type==27||zw.type==54)//土豆和炮
        zw.grow=zw.move_spd;
    AddPlantWzEff(zw);NewZw(zw);
    return true;
}

void Game::NewZw(Plant zw)
{
    int i = plt_cnt,tt=zw.odtm=zw.time,ty=zw.type;
    if(zw.time==0)
        zw.time=300;
    if(is_zha[ty]&&!(ty==3&&zw.tag==1))//炸弹（膨胀）
        zw.time=tt=300,zw.life=1e8,zw.tag=TM;
    plants[i] = zw;
    plants[i].num = i;
    plants[i].mxlf=zw.life;
    plants[i].id = GetZwIcon (plants[i]);
    plants[i].ii = iis_p;
    plt_ii[iis_p++]= i;
    plt_cnt += 1;
    if (zw.nxt_tm == -1)//初始状态
        m_wnd->NewTimer (-plants[i].ii, zw.time, ty==1||ty==20||ty==12?zw.time/2:zw.time);//向日葵类
    else
        m_wnd->NewTimer (-plants[i].ii, zw.time, zw.nxt_tm);
    plants[i].nxt_tm=m_wnd->v_tm+zw.time;
    if (tt==0||ty==16||ty==54)//瞬时
        Goj (i);
    nd_calfr=true;//更新
}

bool Game::IsIcy(int x,int y)const
{
    if(x<0||x>=MM||y<0||y>=MN)
        return false;
    return TM < m_ice[x][y];
}

bool Game::IsKeng(int x, int y)const
{
    if(x<0||x>=MM||y<0||y>=MN)
        return false;
    return TM < m_keng[x][y];
}

bool Game::Js_On_Grid(int j_i)
{
    if(zombies[j_i].type>=100)return false;
    int tx = (zombies[j_i].x + SI/2 +5) / SI, ty = zombies[j_i].y,tz=GetZwShu(tx*SI,ty);
    if(tx >= 0&&tx < MM)
    {
        if (plants[tz].type==29&&zombies[j_i].type != 8 && !Js_At_Sky(j_i)) {//除了跳跳之外受到地刺伤害
            if (zombies[j_i].type == 9||zombies[j_i].type == 28||zombies[j_i].type == 11) //踩扁地刺
            {
                int t=std::fmin(5,plants[tz].life);
                m_ng[tx][ty]=0;
                HurtZw(tz,t,1);
                if(HurtJs_Fast(j_i,t*1000,0,29))
                    return true;
            }
            else if (zombies[j_i].type == 12) //扎车
            {
                dps+=zombies[j_i].life/2;
                zombies[j_i].life/=2;zombies[j_i].type=29;
                HurtZw(tz,20,1);
            }
            else if(HurtJs_Fast(j_i,30*spd_mul[tx][ty],0,29))
                return true;
        }
        if (zombies[j_i].type == 12 &&m_wnd->gqe != 5)
        {
            int tt=700-m_wnd->m_ea*100;
            if(m_wnd->m_hd)tt=800;
            if(m_wnd->m_imp)tt=1000;
            m_ice[tx][ty] = TM+tt;//小车生成冰
        }
        if (Js_At_Sky(j_i) && GetZwShu (tx * SI, ty) == -1 && m_hy[tx][ty]>0)//除了气球之外可以踩掉荷叶
            m_hy[tx][ty]-=1;
        if(js_nd[tx][ty]>=0)
        {
            int t=zombies[j_i].type,d=zombies[j_i].id;
            if(d==3||d==17||d==18)t=0;
            if(js_nd[tx][ty]==t)js_nd[tx][ty]=-2;
            else js_nd[tx][ty]=-3;
        }
    }
    if (plants[tz].ti&&zombies[j_i].type != 8 && !Js_At_Sky(j_i))//走过扶梯的伤害
    {
        double t=2.0/SI;
        if(zombies[j_i].type==9||zombies[j_i].type==28)t*=5;//巨人伤害更高
        HurtZw(tz,t,1);
    }
    return false;
}

void Game::MagicZombie(int shu)
{
    int x=zombies[shu].x,y=zombies[shu].y;
    for (int i = 0; i < zmb_cnt; i++) {
        if (zombies[i].type >= 100||zombies[i].y == -1||zombies[i].type == 11)
            continue;
        if(ZombieInide(i,x-SI,x+SI,y-1,y+1))
            zombies[i].x_flt -= 2;//瞬移
    }
    if(zombies[shu].x>SI)
    {
        for (int i = 0; i < plt_cnt; i++) //使用咒骂伤害植物
            if (plants[i].y == zombies[shu].y&&plants[i].type!=-1)
                HurtZw(i,plants[i].mxlf/150,1);
    }
}

void Game::Drive_Car(int y)
{
    if(!m_wnd->crea&&!m_wnd->is_coll)
        cars[y] -=1 ;
    car_driv[y] = true;car_TM[y]=TM;
    Bullet zd;//创建子弹
    zd.attack = 2e9;zd.from = -2;
    zd.y = y;zd.x = HOME_X-15;
    zd.vx = -10;zd.tx = 28;zd.du=1;
    NewZd (zd);
    if (m_wnd->imjs) {//我是僵尸
        bool fd = false;
        for (int i = 0; i < MN; i++)
            if (cars[i]) {fd = true; break;}
        if(!fd)
        {
            m_wnd->GameOver ();
            QMessageBox::information (m_wnd, QObject::tr ("提示"), QObject::tr ("胜利"));
        }
    }
    if(m_wnd->duiz)//对战：丢车保护
        expad+=15000,m_yg+=2000,ygad+=300,lstcar=TM;
    m_yg+=car_ryg;car_use+=1;
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
        zombies[shu].x=zombies[shu].x_flt=MM*SI;//瞬移到最右侧
        return false;
    }
    if (zombies[shu].type == 11) //BOSS
    {
        zombies[shu].vx = -zombies[shu].vx;//折返
        zombies[shu].x=zombies[shu].x_flt=HOME_X+1;
        return false;
    }
    if (car_driv[zombies[shu].y])//正在使用车
    {
        DestroyJs (shu);
        return true;
    }
    if (cars[zombies[shu].y]) //车
    {
        Drive_Car(zombies[shu].y);
        return false;
    }
    else //进家
    {
        if (m_wnd->imjs) {
            DestroyJs (shu);
            return true;
        }
        over = true;
        int s = 0;
        for (int i = 0; i < zmb_cnt; i++)
            if (zombies[i].y!=-1 && zombies[i].type < 100)
                s += 1;//计算剩余僵尸数
        m_wnd->GameOver ();
        int fs=jd - (m_wnd->is_boss ? 0 : s);
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
        if(zombies[i].y!=-1)
            HurtJs_Fast(i,zombies[i].du*DU_P,0,44);
}

bool Game::MoveJs (int j_i) { //移动僵尸,返回是否被杀死
    if(zombies[j_i].y==-1)return true;
    int f=zombies[j_i].freeze;
    double s_p=(f<1000?frez_spd[f]:1.0/12.0),spd=zombies[j_i].vx*s_p;
    if(IsKeng((zombies[j_i].x + SI/2) / SI,zombies[j_i].y))spd*=0.8;//坑
    zombies[j_i].xu=std::max(zombies[j_i].xu-s_p*XU_HF/(1.0+zombies[j_i].du*DU_XUHF),1.0);
    if(zombies[j_i].type==10&&zombies[j_i].fss!=-1)//小鬼投掷
    {
        zombies[j_i].x_flt-=9;
        if(zombies[j_i].x<=zombies[j_i].fss)
            zombies[j_i].x=zombies[j_i].fss,zombies[j_i].fss=-1;
        return 0;
    }
    if(Js_On_Grid(j_i))return true;
    if(zombies[j_i].type<100)expad+=1;
    if(TM<zombies[j_i].frez_stp)return 0;
    zombies[j_i].freeze = std::max(zombies[j_i].freeze-1,0);
    if(TM<zombies[j_i].stp_tm)return 0;
    bool rt = false;
    int ex=zombies[j_i].x/SI+(zombies[j_i].vx<0?1:0),ey=zombies[j_i].y,z_i=-2;
    if(ZombieCanEatPlant(j_i,ex,ey))z_i=GetZwShu(ex*SI,ey);//获取要吃的植物
    if(zombies[j_i].vy!=0)//斜线
    {
        zombies[j_i].y_flt+=zombies[j_i].vy;
        int oy=zombies[j_i].y,ny=int(zombies[j_i].y_flt/SI+0.5);
        if(oy!=ny)
        {
            zombies[j_i].y=ny;
            row_zmb[oy]-=1;row_zmb[ny]+=1;
            if(row_zmb[ny]==1)m_wnd->WakeUp(ny);
        }
        if(zombies[j_i].y_flt<0&&zombies[j_i].vy<0)
            zombies[j_i].vy=-zombies[j_i].vy;
        if(zombies[j_i].y_flt>(MN-1)*SI&&zombies[j_i].vy>0)
            zombies[j_i].vy=-zombies[j_i].vy;
    }
    if (z_i!=-2&&abs(zombies[j_i].x-ex*SI)<=zombies[j_i].eat_jl)//吃
    {
        zombies[j_i].eating = true;//正在吃
        rt = Eat (ex,ey,j_i,s_p);
        if (rt||zombies[j_i].y==-1)return true;
    }
    else
    {
        zombies[j_i].eating=false;
        zombies[j_i].x_flt -= spd;
        if (zombies[j_i].type == 9||zombies[j_i].type == 28)//巨人下落
            zombies[j_i].tag = 0;
        if(IsIcy((zombies[j_i].x + SI/2) / SI,zombies[j_i].y))//冰面
            zombies[j_i].x_flt+=(zombies[j_i].vx>0?-0.5:0.5);
    }
    if (zombies[j_i].x < HOME_X&&!over)
        return JsInHome(j_i);
    if (zombies[j_i].x > MM * SI + 20&&zombies[j_i].type != 11&&zombies[j_i].vx < 0)//到达最右侧
    {
        if (zombies[j_i].type >= 100) {//子弹
            DestroyJs (j_i);
            return true;
        }
        zombies[j_i].vx = -zombies[j_i].vx;//折返
    }
    if (zombies[j_i].shoot) {//发射
        if(zombies[j_i].fs_nd==0)//随时发射
            rt=JsShoot (j_i);
        else
        {
            zombies[j_i].fss += 1000;
            if (zombies[j_i].fss >= zombies[j_i].fs_nd) //发射时间
            {
                zombies[j_i].fss = 0;
                rt=JsShoot (j_i);
            }
        }
    }
    return rt;
}
void Game::KillZw(int xl,int xr,int yl,int yr,double et,int ty,bool st,double kp)//范围伤害
{
    for(int x=xl;x<=xr;x++)
        for(int y=yl;y<=yr;y++)
        {
            int z=GetZwShu(x*SI,y);
            if(z==-1||plants[z].type==-1)//罐子
                continue;
            HurtZw_Ty(x,y,et+plants[z].mxlf*kp,ty,st);
        }
}
void Game::KillZwPos(int x, int y, double et, int ty, bool st, double kp)
{
    KillZw(x,x,y,y,et,ty,st,kp);
}
void Game::Explode(int x,int y,int r,double hu,double de)
{
    NewPicSt(x,y*SI,128);
    for (int a = -r; a <= r; a++) {
        for (int b = abs(a)-r; b <= r-abs(a); b++) {//爆炸
            int t=GetZwShu (x + a * SI, y + b);
            if (t != -1)
                HurtZw_Ty(x/SI+a,y+b,hu-(abs(a)+abs(b))*de,2,1);
        }
    }
}
Bullet *p_zid;Zombie *p_jsh;
bool cmp1 (int a, int b) {
    return p_zid[a].x < p_zid[b].x;
}
bool cmp2 (int a, int b) {
    if(fabs(p_jsh[a].x_flt - p_jsh[b].x_flt)<1e-6)
    {
        if(p_jsh[a].freeze == p_jsh[b].freeze)
            return p_jsh[a].xu < p_jsh[b].xu;
        return p_jsh[a].freeze < p_jsh[b].freeze;
    }
    return p_jsh[a].x < p_jsh[b].x;
}
void Game::NewNumberPoint(int x,int y,int z,int co)
{
    z/=100;if(z<=0)return;
    if(z>9999999)z=9999999;
    ShiTi st;st.tm=TM;
    st.x=x;st.y=y;st.ty=10;
    st.id=z*200+co;NewSt(st);
}
void Game::NewRedPoint(int i,int si,bool show_num,int co)
{
    ShiTi st;st.x=zombies[i].x+RA()%7-3;
    st.y=zombies[i].y_flt;st.tm=TM;st.ty=3;
    st.id=si;NewSt(st);
    if(show_num&&!no_num)
        NewNumberPoint(st.x,st.y,si,co);
}
void Game::NewBluePoint(int i,int zz)
{
    if(zz==0)return;
    ShiTi st;st.x=zombies[i].x;
    st.y=zombies[i].y_flt;st.tm=TM;st.ty=6;
    st.id=std::min(zz,1000)/3;NewSt(st);
}
void Game::NewNgPoint(int x, int y, int si)
{
    ShiTi st;st.x=x*SI+RA()%11-5+SI/3;
    st.y=y*SI;st.tm=TM;st.ty=4;
    st.id=si*10000+24;//南瓜
    NewSt(st);
}
void Game::NewPlantPoint(int i,int si)
{
#ifdef TEST_MODE
    if(si>10000)
        exit(-1);
#endif
    ShiTi st;st.x=plants[i].x+RA()%11-5+SI/3;
    st.y=plants[i].y*SI;st.tm=TM;st.ty=4;
    st.id=si*10000+plants[i].type;
    NewSt(st);
}
void Game::JieDong(int x,int y)
{
    if(y>plants[x].frez_stp)y=plants[x].frez_stp;
    plants[x].frez_stp-=y;
    ShiTi st;st.x=plants[x].x+RA()%11-5+SI/3;
    st.y=plants[x].y*SI;st.tm=TM;st.ty=4;
    st.id=y*10000+73;
    NewSt(st);
}
bool Game::BulletOverZw(int bi)
{
    Bullet &zd=bullets[bi];
    int x = (zd.x/SI+zd.left) * SI,y = zd.y,t = GetZwShu (x, y);
    if(zd.vx<0||is_tou[zd.from]||!can_overzw[zd.from]||x==zd.tag||zd.tag==-2)
        return false;
    zd.tag=x;
    if(t==-1)return false;
    if (plants[t].type == 36||plants[t].Burning()) { //火炬（或着火的植物）
        if (zd.color == BLUE)//寒冰
            zd.color = DBLUE,zd.freeze+=60,zd.fire=10;
        else if (zd.color == RED||zd.color==DBLUE)//火焰
            zd.attack=std::min(zd.attack*2,zd.attack+1000),zd.siz+=2,zd.fire+=5;
        else zd.color = RED,zd.fire=10;//普通
        zd.attack=std::min(zd.attack*3/2,zd.attack+plants[t].lev*300);
    }
    else if (plants[t].type == 58) { //冰火炬
        if (zd.color == RED)//火焰
            zd.color = DBLUE,zd.freeze=160;
        else if(zd.color == BLUE||zd.color==DBLUE)//寒冰
            zd.attack=std::min(zd.attack*3/2,zd.attack+500),zd.siz+=2,zd.freeze+=60;
        else zd.color = BLUE,zd.freeze=100;//普通
        zd.attack=std::min(zd.attack*3/2,zd.attack+plants[t].lev*300);
    }
    else if(plants[t].type==81)//毒火炬
        zd.du+=20;
    else if(plants[t].type==85&&!Zd_At_Sky(zd)&&zd.vx>0&&zd.siz>2&&!zd.xie)//分裂
    {
        for(int i=-1;i<=1;i++)
        {
            int yy=y+i;
            if(yy<0||yy>=MN)continue;
            Bullet nz=zd;
            nz.vis=-1;nz.y=yy;
            nz.mul(1.0/3);nz.siz-=2;
            if(i==-1)nz.fire+=5;
            else if(i==0)nz.freeze+=30;
            if(nz.y_flt<nz.y*SI-0.01)
                nz.vy=7;
            else if(nz.y_flt>nz.y*SI+0.01)
                nz.vy=-7;
            else nz.vy=0;
            NewZd(nz);
        }
        DestroyZd(bi);return true;
    }
    //if(zd.freeze&&!is_tou[zd.from])
    //    zd.freeze+=plants[t].freeze/5;
    return false;
}

int Game::CalHurt(int zi,int ki,int frez,double fire,int du,double xu,double q,int ct,int ft)
{
    if(frez<0)//黄油
    {
        int stp=-frez;frez=0;
        if(TM>=zombies[zi].stp_tm+40)
            zombies[zi].stp_tm=TM+stp;
    }
    if(fire>0.1)
    {
        if(zombies[zi].dun)
            fire=(fire-0.1)*(ct/10.0)+0.1;
        double t=fire*RONG_P,p=std::min(std::min(sqrt(fire),fire)*0.4,1.0);
        double d=zombies[zi].freeze*p;
        double ad=pow(d,1.4)*t;//冰火叠加增加伤害
        if(TM<zombies[zi].frez_stp)
            ad+=(zombies[zi].frez_stp-TM)*25*t;
        ki+=int(ad+0.5);
        zombies[zi].freeze-=int(d+0.5);
        if(fire>1)//毒+火
            ki+=zombies[zi].du*FIRE_DU*(fire-1);
        ki*=fire_mul[zombies[zi].type];
        ShiTi st;st.tm=TM;st.id=zombies[zi].ii;
        st.ty=7;NewSt(st);
    }
    if(zombies[zi].dun)
    {
        frez=frez*ct/10;
        if(du>0)du=(du-1)*ct/10+1;
        xu*=(ct/10.0);
    }
    ki*=zombies[zi].xu;
    FreezeJs(zi,frez,ft);
    zombies[zi].xu+=xu;
    zombies[zi].du+=du;
    zombies[zi].q+=q;
    if(fire>1.5&&zombies[zi].type==26&&zombies[zi].life<ki)//火药桶
        JsShoot(zi);
    return ki;
}

void Game::BulletOnJs(int bi,int zi,bool tou)
{
    const Bullet&zd=bullets[bi];
    if (zd.mh)//具有魅惑能力
    {
        MeiHuo (zi);
        return;
    }
    int g=zd.attack,f=zd.from;
    short hb=zd.freeze,zt=zombies[zi].type,ft=0;
    if(zt==19&&((zd.from==39&&zd.siz==1)||zd.from==94))
        g*=5;//尖刺攻击气球
    if(hb>100)ft=std::min((hb-100)/5,20);
    int ct=(zd.ct?10:(tou||zd.from==102?5:0));//大喷菇投手穿透
    double fi=zd.fire/10.0;
    if(hb>0)fi=0;//冰火不能同时出现
    if(zombies[zi].dun)ft=ft*ct/10;
    g=CalHurt(zi,g,hb,fi,zd.du,zd.xu,zd.q,ct,ft);
    dps+=zombies[zi].Hurt(g,ct/10.0);
    if(zd.color==DBLUE)//冰火焰
    {
        ShiTi st;st.tm=TM;st.id=zombies[zi].ii;
        st.ty=8;NewSt(st);
    }
    if (zombies[zi].life <= 0)
        DestroyJs(zi,0);
    else
        zombies[zi].id = GetJsIcon (zombies[zi]);
    if(f==100)//裂变
    {
        double spd=zd.getspd(),st=zd.x_flt/(MM*SI);
        st=double(RA())/RA.max();
        for(int i=0;i<3;i++)
        {
            Bullet tz=zd;tz.xie=true;
            tz.vis=zombies[zi].ii;//防止原地攻击
            tz.setspd(spd,(i/3.0+st)*2*PI);
            if(i==0)tz.siz-=3,tz.mul(1.0/2);
            else tz.siz-=4,tz.mul(1.0/4);
            if(tz.siz<=0||(tz.attack==0&&tz.freeze==0&&tz.fire==0&&tz.du==0))
                continue;
            tz.move(2);
            tz.x=int(tz.x_flt+0.5);
            tz.y=int(tz.y_flt/SI+0.5);
            if(tz.y>=0&&tz.y<MN)
                NewZd (tz);
        }
    }
}

void Game::MhJsSkillTick()//25ms一次
{
    for(int i=0;i<bul_cnt;i++)
    {
        Bullet&zd=bullets[i];
        if(zd.vx>0)continue;
        Bullet nz;nz.siz=-1;
        zd.tag += 1;
        if (zd.from == 13) {//豌豆僵尸魅惑后打子弹
            if (zd.tag >= 60)
                zd.tag = 0,nz=GetFanTan(75);
        }
        else if (zd.from == 14) {//西瓜僵尸魅惑后打子弹
            if (zd.tag >= 60)
                zd.tag = 0,nz=GetFanTan(81);
        }
        else if (zd.from == 22) {//冰瓜僵尸魅惑后打子弹
            if (zd.tag >= 100)
                zd.tag = 0,nz=GetFanTan(183);
        }
        else if (zd.from == 25) {//向日葵僵尸魅惑后产生阳光
            if (zd.tag >= 400)
                zd.tag = 0,m_yg+=25;
        }
        if(nz.siz!=-1)
        {
            nz.x = zd.x;nz.y = zd.y;
            NewZd(nz);
        }
    }
}

void Game::MianYiCheck()
{
    for (int i = 0; i < zmb_cnt; i++) {
        if(zombies[i].y==-1)continue;
        if(zombies[i].type == 12||zombies[i].type == 11||zombies[i].type == 19||zombies[i].type >= 100)
            zombies[i].stp_tm=zombies[i].frez_stp=-1e8,zombies[i].freeze=0;
        if(zombies[i].type==11||zombies[i].type==24||zombies[i].type>=100)
            zombies[i].du=0;
        if(zombies[i].type == 11)zombies[i].xu=1.0,zombies[i].q=0;
        if(zombies[i].du>MAXDU)zombies[i].du=MAXDU;
        if(zombies[i].xu>MAXXU)zombies[i].xu=MAXXU;
        if(zombies[i].freeze>MAXFREZ)zombies[i].freeze=MAXFREZ;
    }
}

bool Game::Zd_At_Sky(const Bullet&zd)const
{
    if(is_tou[zd.from])
        return true;
    if((zd.from==39&&zd.siz==1)||zd.from==94)//尖刺
        return true;
    return false;
}

bool Game::Js_At_Sky(int zi)const
{
    if(zombies[zi].type==19)//气球
        return true;
    if(zombies[zi].type==10&&zombies[zi].fss!=-1)//小鬼投掷
        return true;
    return false;
}

bool Game::CannotReach(const Bullet*zd,int zmb_i)const
{
    if(zombies[zmb_i].y==-1)return true;
    if(zd==NULL)return false;
    if(zd->from==39&&zd->siz==1&&zombies[zmb_i].type!=19)//打气球的刺
        return true;
    if(!zd->ct&&zd->vx>0&&zd->vis==zombies[zmb_i].ii)//忽略
        return true;
    if(Js_At_Sky(zmb_i)&&!Zd_At_Sky(*zd))
        return true;
    if(zd->ct)
    {
        int t=zd->vis;//判断是否攻击过
        if(t!=-1&&zd_atk[t][zombies[zmb_i].ii])
            return true;
    }
    return false;
}

void Game::RemoveExtraSt()
{
    for(int i=0;i<shi_cnt;i++)
    {
        const ShiTi&st=shiti[i];
        int c=TM-st.tm;bool del=0;
        if(st.ty==1)del=(c>=100);
        else if(st.ty==0)del=(c>=80);
        else if(st.ty==2||st.ty==5)del=(c>50);
        else if(st.ty==3)
        {
            double sq=pow(shiti[i].id,1.0/3.0);
            del=(c>=int(30+50*sq+0.5));
        }
        else if(st.ty==4)
        {
            double sq=pow(shiti[i].id/10000,1.0/4.0);
            del=(c>=int(30+50*sq+0.5));
        }
        else if(st.ty==6)del=(c>=st.id);
        else if(st.ty==7||st.ty==8)
            del=(zmb_ii[st.id]==-1||TM-st.tm>=30);
        else if(st.ty==9)
            del=(TM-st.tm>=(st.id/1000)%1000);
        else if(st.ty==10)del=(c>=60);
        else if(st.ty==11)del=(c>=0);
        else if(st.ty==12)del=(c>=100);
        else if(st.ty==13)del=(c>20);
        if(del)DestroySt(i),i-=1;
    }
}

bool Game::Zd_Extra(const Bullet &zd)
{
    if (zd.x > MM * SI + 50||zd.x < -200||zd.y<0||zd.y>=MN)//坐标超限
        return true;
    if (zd.vx<0&&zd.attack<=0)//魅惑子弹死亡
        return true;
    if (is_tou[zd.from]&&zd.y_flt>=(zd.y+1.0)*SI)//投手子弹落地
        return true;
    return false;
}

bool Game::ZdHitJs(const Bullet &zd,int ji)
{
    int fw=(zd.xie&&zd.from!=94?25:15);
    if(ji==-1||zombies[ji].y==-1||zd.x+fw<zombies[ji].x||zd.x-fw>zombies[ji].x)//x坐标碰不到
        return false;
    if(fabs(zd.y_flt-zombies[ji].y_flt)>SI/2)//y坐标距离太大
        return false;
    return true;
}

int Game::ColorToFrom(const QColor &co) const
{
    if(co==GREEN)return 0;
    else if(co==BLUE)return 9;
    else if(co==DBLUE)return 61;
    else if(co==RED)return 4;
    else if(co==PURPLE)return 59;
    else if(co==YELLOW)return 44;
    else if(co==DIANCO)return 90;
    else return -1;
}

void Game::MoveAllZd (double spd) //双指针
{//TODO: BUG：低速帧伤增强
    no_num = true;MianYiCheck();
    int zs[MMN] = {0}, js[MMN] = {0};
    for (int i = 0; i < bul_cnt; i++)
        if(BulletOverZw(i))i-=1;
    for (int i = 0; i < bul_cnt; i++)
    {
        int y = bullets[i].y;
        pd[y][zs[y]++] = i;
    }
    for (int i = 0; i < zmb_cnt; i++) {
        if (zombies[i].y==-1 || zombies[i].type >= 100)
            continue;
        int y = zombies[i].y;
        pj[y][js[y]++] = i;
    }
    p_zid=bullets;p_jsh=zombies;
    for (int y = 0; y < MN; y++) { //枚举行
        std::sort (pd[y], pd[y] + zs[y], cmp1);
        std::sort (pj[y], pj[y] + js[y], cmp2);
        for (int a = 0, b = 0; a < zs[y]; a++) {
            const int bul_i = pd[y][a], zx = bullets[bul_i].x, zw=GetZwShu ((zx / SI) * SI, y);
            Bullet &zd=bullets[bul_i];
            double mv=0;
            if (zd.vx > 0)
            {
                mv=zd.vx * spd;
                if(zd.left)mv=-mv;
            }
            else //魅惑子弹
            {
                mv=-zd.vx*spd*0.4;
                if(zw!=-1&&plants[zw].type==6&&zd.from!=-2)//高坚果阻挡，并非小车
                    mv=0;
                if(zd.from!=-2)
                {
                    zd.vis-=int(100*spd+0.5);
                    if(zd.vis<=0)
                    {
                        zd.du=-1;
                        continue;
                    }
                }
            }
            if (!Zd_At_Sky(zd)&&zd.vx>0&&zw!=-1&&plants[zw].frez_stp)//解冻
            {
                int t=zd.attack;
                if(zd.fire>=3)t=t*zd.fire/3;
                JieDong(zw,std::max(t/1000,1));
                zd.du = -1;continue;
            }
            while (b < js[y]&&(zombies[pj[y][b]].y==-1||zombies[pj[y][b]].x <= zx))//找到右面的第一个
                b += 1;
            int c=b,fr=zd.from;
            if(zd.xie)
            {
                if(c>=js[y])c-=1;
                while(c+1<js[y]&&zombies[pj[y][c+1]].x<=zx+25)c+=1;//跳到最右面
                while(c>=0&&zombies[pj[y][c]].x>=zx-25&&(CannotReach(&zd,pj[y][c])||!ZdHitJs(zd,pj[y][c])))//倒序寻找合法的
                    c-=1;
            }
            else if(zd.left)
            {
                if(c>=js[y])c-=1;
                while(c>=0&&(zombies[pj[y][c]].x>=zx||CannotReach(&zd,pj[y][c])))//倒序寻找合法的
                    c-=1;
            }
            else
            {
                while(c<js[y]&&CannotReach(&zd,pj[y][c]))//继续寻找合法的
                    c+=1;
            }
            int zmb_i = (c<0||c>=js[y] ? -1 : pj[y][c]);
            if(zd.from==94)//猫尾草
                zmb_i=(zd.tag==-1?-1:zmb_ii[zd.tag]);
            if(zd.vx>0)//设定目标坐标
            {
                if (zmb_i!=-1)zd.tx = zombies[zmb_i].x;
                else if(zd.left)zd.tx = -150;
                else zd.tx = MM * SI + 20;
            }
            bool tou=is_tou[zd.from];
            int oddps=dps;
            if (ZdHitJs(zd,zmb_i)) { //打到了
                if (zd.vx > 0)
                    BulletOnJs(bul_i,zmb_i,tou);
                else {//魅惑子弹
                    int z = std::min(zd.attack,zombies[zmb_i].life);//减去最小的
                    if (zombies[zmb_i].type == 11&&fr == -2)//小车不杀死僵王
                        z = 0;
                    if(z&&fr != -2)//并非小车
                    {
                        z=std::min(z,int(200*spd+0.5));
                        zombies[zmb_i].x_flt=std::max(zombies[zmb_i].x_flt,zd.x + 10.0);
                        zombies[zmb_i].eating=true;
                    }
                    zd.attack -= z;
                    if(z>0)mv=0;//啃食，不移动
                    z*=zd.du;
                    HurtJs_Fast(zmb_i,z,0);
                    if(fr==-2)//小车，无限生命
                        zd.attack=inf,js_car+=1;
                    if(z)NewRedPoint(zmb_i,z);
                }
                int oj=zd.jian+zd.fire+int(5*zd.q+0.5),ji=std::min(oj,100);
                if (!zd.ct&&ji>0) //群体伤害（非穿透)
                {
                    int tx = zombies[zmb_i].x,ty = zd.y;
                    int ki=int(pow(zd.attack,0.9)+0.5);//溅射伤害
                    int ct=(tou||zd.from==102?5:0)+2;//溅射可增加2点破盾力
                    double fi=std::max(zd.fire/10.0-1,0.0);//火效果下降一级
                    if(oj>100)ki*=(1+(oj-100)/50.0);
                    const HURT hu(ki,zd.freeze/2,zd.du/2,ct,fi,zd.xu/2,zd.q*0.3);//毒性除以2
                    for(int i=0;i<MN;i++)
                    {
                        int dy=abs(i-ty),tj=ji-dy*dy*10;
                        if(tj<=0)continue;
                        HURT th=hu;
                        if(tj<10)th.mul(tj/10.0),tj=10;//等级不足
                        else
                        {
                            th.mul(1+(tj-10)/90.0);
                            th.atk+=zd.attack*(tj-10)/100;//额外溅射伤害
                        }
                        double fw=tj/10.0-1;
                        KillJsLR(tx,i,fw,fw,th,zd.from,true);
                    }
                }
                if(zd.color==BUTTER)
                    KillJsPos(zombies[zmb_i].x,zd.y,0.25,HURT(zd.attack,-100));
                if(dps>oddps)
                {
                    int co=(zd.vx>0?zd.from:1),t=ColorToFrom(zd.color);
                    NewNumberPoint(zd.x,zd.y*SI,dps-oddps,(t==-1?co:t));
                }
                if(zd.ct)//穿透
                {
                    int t=zd.vis;
                    if(t==-1)
                    {
                        t=zd_pol.New();
                        zd.vis=t;zd_atk[t]=0;
                    }
                    if(zd.from==60)//回旋镖
                    {
                        zd.vx+=(zd.left?2:-2);
                        if(zd.vx<eps)//回旋
                        {
                            zd.left=1;zd.vx=2;
                            zd_atk[t]=0;
                        }
                        if(zd.vx>8)zd.vx=8;
                    }
                    zd_atk[t][zombies[zmb_i].ii]=1;
                }
                else if (zd.vx > 0&&zd.from!=102)//子弹消失
                    zd.du = -1;//标记删除
                if (zd.vx < 0) {//更新魅惑子弹图像
                    Zombie zz;
                    zz.type = zd.from;
                    zz.life = zd.attack;
                    zz.mxlf = zd.siz;
                    zz.y = -1;zz.sp_eat=0;
                    zd.tx = GetJsIcon (zz);
                }
            }
            if(zd.from==94)//调整方向
            {
                int t=zmb_i;
                if(t==-1||zombies[t].y==-1)
                {
                    zd.tag=GetMWCGoal();
                    t=(zd.tag==-1?-1:zmb_ii[zd.tag]);
                }
                if(t!=-1)
                {
                    double of=zd.gettheta();
                    double yy=zombies[t].y_flt-SI/5.0;
                    if(Js_At_Sky(t))//天空
                        yy=zombies[t].y_flt-SI/2.5;
                    double nf=atan2(yy-zd.y_flt,zombies[t].x_flt-zd.x_flt);
                    double d=nf-of;
                    if(d>PI)d-=PI*2;
                    else if(d<-PI)d+=PI*2;
                    if(fabs(d)>0.03)d=d/fabs(d)*0.03;
                    zd.setspd(4.0,of+d);
                }
            }
            if(zd.vx>0&&is_tou[fr])//投手：抛物线
            {
                double t=(zd.y_flt<=zd.y*SI?solve_ecfc(G/2,zd.vy,zd.y_flt-zd.y*SI).first:0);
                double vd=fabs(zd.tx-zd.x_flt)/(t+0.01)-zd.vx;
                if(fabs(vd)>0.1)vd=vd/fabs(vd)*0.1;//最大水平加速度
                zd.vx+=vd*spd;zd.vy+=G*spd;
                zd.move(spd);
            }
            else if(zd.xie)//斜线
            {
                zd.move(spd);
                zd.y=int(zd.y_flt/SI+0.5);
                if(zd.y<0||zd.y>=MN)zd.du=-1;
            }
            else if(zd.vx>0&&fabs(zd.y*SI-zd.y_flt)>0.1)//三线
            {
                zd.move(spd);
                if((zd.vy>eps&&zd.y_flt>=zd.y*SI)||(zd.vy<-eps&&zd.y_flt<=zd.y*SI))
                    zd.y_flt=zd.y*SI,zd.vy=0;
            }
            else zd.x_flt += mv;
            zd.x=int(zd.x_flt+0.5);
            if (zd.from==60&&zd.x > MM * SI&&!zd.left)//回旋
            {
                zd.left=1;
                if(zd.vis!=-1)zd_atk[zd.vis]=0;
            }
        }
    }
    int sb = 0;
    for (int i = 0; i < bul_cnt; i++)
        if (bullets[i].du != -1&&!Zd_Extra(bullets[i]))
            bullets[sb++] = bullets[i];
        else DestroyZd_Pre(bullets[i]);
    bul_cnt=sb;
    DelExtraZmbPlt();
    for (int i = 0; i < zmb_cnt; i++)
        zombies[i].num = zmb_ii[zombies[i].ii]=i;
    no_num = false;
}

void Game::FreezeJs(int i,short frez,int stp)
{
    NewBluePoint(i,frez);
    zombies[i].freeze = std::max(zombies[i].freeze,frez);
    if(iswa[zombies[i].y])stp+=frez/5;//水中：冻结
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

bool Game::ZombieInide(int i,int xl,int xr,int yl,int yr,bool ck100)
{
    if(zombies[i].y==-1)return false;
    if(ck100&&zombies[i].y>=100)return false;
    return zombies[i].x>=xl&&zombies[i].x<=xr&&zombies[i].y>=yl&&zombies[i].y<=yr;
}

int Game::KillJsLRUD(int x,int y,int ls,int rs,int us,int ds,const HURT&hu,int from)
{
    int rt=0;
    for(int i=-us;i<=ds;i++)
    {
        int yy=y+i;
        if(yy>=0&&yy<MN)
            rt+=KillJsLR(x,yy,ls,rs,hu,from);
    }
    return rt;
}

void Game::SpecialPlt (int z_i) //特殊植物
{
    int gj = -plants[z_i].attack,oddps,fw=0,lx,rx;
    Plant&zw=plants[z_i];
    double mul=zw.mul;
    int ty=zw.type,ox=zw.x,oy=zw.y,sn=25*mul,t;
    if(is_cycle)//灯:+0.1
    {
        if(is_day==0)sn=35;
        else
        {
            sn=10;
            if(FindZw(ox/SI,oy,11)!=-1)sn*=2;
        }
    }
    int hu=int(zw.move_spd*1000*mul),sp=zw.move_spd;
    int frez=zw.freeze*mul,du=zw.du*mul;
    switch (gj) {
        case 1: //向日葵
            if (!m_wnd->imjs)
                m_yg += sn*((m_wnd->sun_dou?0.5:0)+1),laygTM=TM;
            break;
        case 26: //小炸弹
        case 2: //炸弹
            NewPicSt(zw.x,zw.y*SI,128,gj==2?80:50);
            KillJsFW (zw.x, zw.y,1,HURT(hu,frez,du,10,2*mul),ty);
            NewBaoZha(zw.x+SI/2,zw.y,ty,30,gj==2?SI/14:SI/20);
            fw=1;DestroyZw (z_i,0);break;
        case 3: //火爆辣椒
            NewPicStL(zw.y*SI,-127);
            KillJsLine(zw.y,HURT(hu,frez,du,10,3*mul),ty);
            DestroyZw (z_i,0);break;
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
                    KillJs (x,x+fw,zw.y,HURT(hu,frez,du,10),ty);//修复判定
                    no_num=0;NewNumberPoint(zw.x,zw.y*SI,dps-oddps,ty);
                    DestroyZw (z_i,0);
                }
                break;
            }
        case 5: //阳光菇
            if (zw.grow < sp)m_yg += (m_wnd->sun_dou?22:15)*mul;
            else m_yg += (m_wnd->sun_dou?37:25)*mul;
            zw.grow = std::min(zw.grow+1,t); //生长
            zw.id = GetZwIcon (zw);
            laygTM=TM;break;
        case 6: //毁灭菇
            KillJsFW (zw.x, zw.y,2,HURT(hu,frez,du,10),ty);
            for(int i=-2;i<=2;i++)
                for(int j=-2;j<=2;j++)
                {
                    int w=GetZwShu(zw.x+i*SI,zw.y+j);
                    if(w!=-1&&plants[w].mxlf<=1e6)
                        HurtZw_Ty(zw.x/SI+i,zw.y+j,plants[w].mxlf/3,0,1);//对植物造成伤害
                }
            m_keng[zw.x/SI][zw.y]=std::max(m_keng[zw.x/SI][zw.y],TM+2400);//坑
            NewBaoZha(zw.x+SI/2,zw.y,ty,50);
            fw=2;DestroyZw (z_i,0);break;
        case 7: //寒冰菇
            for (int i = 0; i < zmb_cnt; i++)
            {
                if(zombies[i].y==-1||zombies[i].type>=100)continue;
                FreezeJs(i,sp*mul,(sp+40)*mul);
            }
            for(int i=0;i<plt_cnt;i++)
                if(plants[i].y!=-1&&plants[i].Burning())plants[i].fire=FIRETM+1;
            NewBaoZha(zw.x+SI/2,zw.y,ty,300,SI/6);
            DestroyZw (z_i,0);
            break;
        case 8: //全息坚果
            if(TM-zw.la_et>=20)
            {
                double t=zw.mxlf/sp*mul;
                if(zw.life>zw.mxlf*0.8)t/=2;
                zw.life=std::min(zw.life+t,zw.mxlf);
                zw.id = GetZwIcon (zw);
            }
            break;
        case 9: //双子向日葵
            t=(sn+(zw.tag?5*mul:0));
            m_yg += (m_wnd->sun_dou?t*1.5:t),laygTM=TM;
            if(zw.tag&&NeedBullet(zw.y))//多头
            {
                Bullet zd=BulletFromZw(z_i);//发射子弹
                zd.attack = 3000*mul;
                zd.ct=true;NewZd(zd);
            }
            break;
        case 10: //医疗
            for (int a = -1; a <= 1; a++) {
                for (int b = -1; b <= 1; b++) {
                    int z = GetZwShu (zw.x + a * SI, zw.y + b);
                    if (z != -1) {
                        if ((m_wnd->imjs&&plants[z].type == 1)||TM-plants[z].la_et<20)
                            continue;
                        double mx=plants[z].mxlf,ad=pow(mx,0.4)*mul;
                        plants[z].life = std::fmin(plants[z].life+ad,mx);
                        int tx = zw.x/SI+a, ty = zw.y + b;
                        if (m_ng[tx][ty]&&TM-le_ng[tx][ty]>=20)
                            m_ng[tx][ty] = std::fmin(m_ng[tx][ty]+10,600);
                        if(plants[z].Burning())plants[z].fire+=2;
                        if(plants[z].frez_stp)JieDong(z,2);
                        plants[z].id = GetZwIcon (plants[z]);
                    }
                }
            }
            break;
        case 11: //忧郁菇
            zw.tag = 0;
            t = (zw.lev >= 6 ? 2 : 1);
            if (KillJsFW(zw.x,zw.y,t,HURT(hu,frez,du,10),ty))
                zw.tag = 1;
            lx=zw.x - (t+FW) * SI,rx=zw.x + (t+FW) * SI;
            for(int i=0;i<zmb_cnt;i++)
                if(ZombieInide(i,lx,rx,zw.y-t,zw.y+t))
                    zombies[i].xu=std::max(zombies[i].xu,1+mul*0.2);
            break;
        case 13: //大葱 TODO:mul
            {
                int cs[MAXSHU] = {0}, cf[MAXSHU], fs = 0, kx[8], ky[8], ks = 0;
                //zw.grow-=int(mul+0.1);
                zw.grow-=1;
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
                        int z = cf[RA () % fs], t;
                        for (t = 0; t < MAXSHU; t++)
                            if (m_wnd->plants[t].type == z)break;
                        int nz = RA () % ks;
                        Plant zw = m_wnd->plants[t];
                        zw.x = kx[nz];
                        zw.y = ky[nz];
                        m_yg += zw.cost;
                        PlantZw (zw);
                    }
                }
                break;
            }
        case 14: //炮 TODO:mul
            t = GetZwShu (zw.x - SI, zw.y);
            if (t != -1&&plants[t].type == 31)//左边有前一半
                zw.grow = std::min(zw.grow+1,sp);
            break;
        case 15://三叶草
            if(zw.grow==sp)
                lstfan=TM,lmw=GetFogAlpha();
            zw.grow-=1;
            for(int i=0;i<zmb_cnt;i++)
            {
                if(zombies[i].y==-1)continue;
                if(zombies[i].type>=100)
                {
                    DestroyJs(i);
                    continue;
                }
                if(zombies[i].type==19)
                {
                    HurtJs_Fast(i,inf,0);
                    continue;
                }
                double xu=1+0.2*mul;
                if(zombies[i].xu<xu)zombies[i].xu=xu;
                zombies[i].freeze+=10;
                HurtJs(i,1000*mul,0,ty,0,10);
            }
            for(int i=0;i<plt_cnt;i++)
                if(plants[i].y!=-1&&plants[i].Burning())plants[i].fire=FIRETM+1;
            NewBaoZha(zw.x+SI/2,zw.y,ty,30,SI/2);
            if(zw.grow<=0)
                DestroyZw (z_i,0);
            break;
        case 16://磁力菇 TODO:mul
            if(zw.grow)zw.grow-=1;
            else
            {
                int w=-1,xz=SI*1.8,mi=inf;
                for(int i=0;i<zmb_cnt;i++)
                {
                    int zt=zombies[i].type;
                    if(ZombieInide(i,zw.x-xz,zw.x+xz,zw.y-1,zw.y+1)&&(zt==2||zt==3||zt==5||zt==8||zt==7||zt==27))
                    {
                        int t=abs(zombies[i].x-zw.x)+SI*abs(zombies[i].y-zw.y);
                        if(t<mi)mi=t,w=i;
                    }
                }
                if(w!=-1)
                {
                    int t=zombies[w].type;
                    if(t==5||t==7||t==8||t==27)ClearDun(zombies[w]);
                    else zombies[w].life=std::min(zombies[w].life,8000);
                    zw.grow=zw.move_spd;
                    zombies[w].id=GetJsIcon(zombies[w]);
                }
            }
            break;
        case 17://魅惑炸弹 TODO:mul
            lx=zw.x - (1+FW) * SI,rx=zw.x + (1+FW) * SI;
            for(int i=0;i<zmb_cnt;i++)
                if(ZombieInide(i,lx,rx,zw.y-1,zw.y+1))
                    MeiHuo(i);
            fw=1;DestroyZw (z_i,0);break;
        case 18://激光豆
        {
            int px[200],ps=0;
            for(int i=0;i<zmb_cnt;i++)
                if(zombies[i].y==zw.y&&zombies[i].type<=100&&!Js_At_Sky(i))
                {
                    px[ps++]=i;
                    p_jsh[i]=zombies[i];
                }
            std::sort(px,px+ps,cmp2);
            double ki=hu;
            const double p1=(zw.dzsy?0.9:0.5),p2=(zw.dzsy?1.0:0.7);
            for(int i=0,lx=zw.x;i<ps;i++)
            {
                int w=px[i];
                if(zombies[i].x<zw.x)continue;
                while(lx+SI<=zombies[i].x)
                {
                    lx+=SI;
                    int t=GetZwShu(lx,zw.y);
                    if(t!=-1&&plants[t].type!=29)ki*=p1;
                }
                HurtJs(w,int(ki+0.5),1,ty,frez*mul,10,0,du*mul);
                ki*=p2;zw.tag=TM;
                if(ki<0.1)break;
            }
        }
            break;
        case 19://金盏花
            if(m_yg<5||!real||zw.left)break;
            m_yg-=5;m_wnd->exp+=25*mul;break;
        case 20://土豆地雷
        case 21://大嘴花
            if(!(m_wnd->b_guan&&zmb_cnt==0))
                zw.grow -= 1; //回复
            if (zw.grow < 0)zw.grow = 0;
            zw.id = GetZwIcon (zw);
            t = GetFirstJs (zw.x-(gj==20?SI*0.3:0), zw.y);
            if (t == -1)break;
            if (zombies[t].x < zw.x + (gj==20?SI*0.8:SI*2)&&zw.grow == 0) {
                oddps=dps;no_num=1;
                int o=KillJsPos (zombies[t].x, zombies[t].y, FW,HURT(gj==20?60000:90000),ty);
                no_num=0;NewNumberPoint(zw.x,zw.y*SI,dps-oddps,ty);
                if (gj==20&&o>0) { //土豆地雷
                    DestroyZw (z_i,0);
                    break;
                }
                else zw.grow = o*sp;
                zw.id = GetZwIcon (zw);
            }
            break;
        case 22://定向
            {
                int n,x,y;double dis;
                zw.tag=(zw.tag==-1?-1:zmb_ii[zw.tag]);
                if(zw.tag==-1||zw.tag>=zmb_cnt||zombies[zw.tag].type>=100)
                    zw.tag=0;
                while(zw.tag<zmb_cnt&&zombies[zw.tag].type>=100)
                    zw.tag+=1;
                if(zw.tag>=zmb_cnt){zw.tag=-1;break;}
                n=zw.tag;zw.tag=zombies[n].ii;
                x=zw.x-zombies[n].x;y=zw.y*SI-zombies[n].y_flt;
                dis=std::min(sqrt(x*x+y*y),1200.0);
                lx=1000,rx=500;
                t=int((rx-lx)/1200.0*dis+lx+0.5)*mul;
                HurtJs(n,t,0,ty,frez,10,0,du);break;
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
        case 12://闪电炸弹
            lx=zw.x - (1+FW) * SI,rx=zw.x + (1+FW) * SI;
            for(int i=0;i<zmb_cnt;i++)
                if(ZombieInide(i,lx,rx,zw.y-1,zw.y+1))
                {
                    if(gj==23)FreezeJs(i,sp*mul,sp*mul*0.4);
                    else if(gj==24)zombies[i].du+=sp*mul;
                    else if(gj==28)zombies[i].freeze+=10;
                    else if(gj==12)zombies[i].q-=sp*mul;
                    else zombies[i].xu=std::max(zombies[i].xu,sp*mul*0.1);
                }
            NewBaoZha(zw.x+SI/2,zw.y,ty,30);
            if(gj!=28)fw=1,DestroyZw (z_i,0);
            break;
        case 25://棱镜
            if(zw.grow)zw.grow-=1;
            zw.id = GetZwIcon(zw);
            break;
        case 27://水炸弹
            lx=zw.x - (1+FW) * SI,rx=zw.x + (1+FW) * SI;
            {
                short frez=0;int du=0;double xu=1;
                for(int i=0;i<zmb_cnt;i++)
                    if(ZombieInide(i,lx,rx,zw.y-1,zw.y+1))
                    {
                        frez=std::max(frez,zombies[i].freeze);
                        du=std::max(du,zombies[i].du);
                        xu=std::fmax(xu,zombies[i].xu);
                    }
                double p=sp/100.0;
                du*=mul;frez*=mul;xu=1+(xu-1)*mul;
                for(int i=0;i<zmb_cnt;i++)
                    if(ZombieInide(i,lx,rx,zw.y-1,zw.y+1))
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
                NewBaoZha(zw.x+SI/2,zw.y,ty,30);
                fw=1;DestroyZw (z_i,0);break;
            }
        case 30://火龙草
        case 32://冰龙草
            if(KillJsLRUD(zw.x+SI,zw.y,0,2,1,1,HURT(hu,frez,du,10,(gj==30?2*mul:0)),ty))
            {
                for(int x=1;x<=3;x++)
                    for(int y=-1;y<=1;y++)
                        if(zw.x/SI+x<MM&&zw.y+y>=0&&zw.y+y<MN)
                            NewPicSt(zw.x+x*SI,(zw.y+y)*SI,(gj==30?-127:-180),60);
            }
            break;
        case 31://菜问
            t=std::max(0,zw.tag-int(zw.life*1000));//损失量
            zw.tag=int(zw.life*1000);if(t>200000)t=200000;//至多200点
            lx=zw.x - (1+FW) * SI,rx=zw.x + (1+FW) * SI;
            {
                int hu=(2000+10*pow(t/1000.0,1.5))*mul;
                double d=1/(2+t/50000.0)/mul;//攻击数目
                if(zw.dzsy)hu*=2,d=0;//大招不限数目
                int ki[100],ps=0;double s=1.0;
                if(zw.life<zw.mxlf)s+=(1-zw.life/zw.mxlf)*0.2;
                for(int i=0;i<zmb_cnt;i++)
                    if(ZombieInide(i,lx,rx,zw.y-1,zw.y+1))
                        ki[ps++]=int(hu*s+0.5),s=std::fmax(s-d,0);
                std::shuffle(ki,ki+ps,RA);
                for(int i=0,j=0;i<zmb_cnt;i++)
                    if(ZombieInide(i,lx,rx,zw.y-1,zw.y+1))
                        HurtJs(i,ki[j++],0,ty,frez,0,0,du);
            }
            break;
        case 33://模仿者
        {
            int w=zw.tag;DestroyZw (z_i,0);
            Plant t=m_wnd->plants[w];
            t.x=ox,t.y=oy,t.cost=0;
            NewBaoZha(t.x+SI/2,t.y,t.type,30);
            PlantZw(t);break;
        }
        case 34://闪电芦苇
            for(int i=0;i<zmb_cnt;i++)
                if(zombies[i].x>=zw.x&&zombies[i].y==zw.y&&zombies[i].type<=100&&!Js_At_Sky(i))
                    zombies[i].q+=sp*mul,zw.tag=TM;
            break;
        case 35://小炮
            zw.grow-=1;
            if(zw.grow<=0)
            {
                if(zw.tag<3)zw.tag+=1;
                zw.grow=10;
            }
            zw.id = GetZwIcon(zw);
            break;
        case 36://能量花
            if(real)
            {
                if(m_wnd->beans<MAXDZS)m_wnd->beans+=1;
                m_wnd->plants[99].cost+=50;
            }
            DestroyZw (z_i,0);
            break;
    }
    if(fw)
    {
        ty=m_wnd->tyToNumP[ty];
        for(int x=-fw;x<=fw;x++)
        {
            for(int y=-fw;y<=fw;y++)
            {
                int z=GetZwShu(ox+x*SI,oy+y);
                if(plants[z].type==78)//火药桶
                {
                    DestroyZw(z,0);
                    Plant t=m_wnd->plants[ty];
                    t.x=ox+x*SI,t.y=oy+y,t.cost=0,t.life=inf,t.time=300,t.mush=0,t.tag=TM;
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
        if(zombies[i].y!=-1&&zombies[i].type<100)s+=1;
    double hu=double(su)/s;
    for(int i=0;i<zmb_cnt;i++)
    {
        if(zombies[i].y==-1||zombies[i].type>=100)continue;
        double x=zombies[i].x_flt-ox,y=zombies[i].y_flt-oy;
        double d=sqrt(x*x+y*y),t=(150.0/(d+150.0)*1.5+0.5)*hu;
        HurtJs_Fast(i,int(t+0.5),1,71,10);
    }
    plants[z].tag=TM;
    plants[z].grow=plants[z].move_spd;
    plants[z].dztm=iis_z;
}
int Game::KillJsPos (int x, int y,double fw, const HURT&hu,int from)
{
    return KillJs (x - SI * fw, x + SI * fw, y, hu,from);
}
int Game::KillJsLR(int x,int y,double ls,double rs,const HURT&hu,int from, bool jian)
{
    return KillJs (x - SI * (ls+FW), x + SI * (rs+FW), y, hu ,from, jian);
}
int Game::KillJsFW(int x, int y, int fw, const HURT &hu, int from)
{
    return KillJsLRUD(x,y,fw,fw,fw,fw,hu,from);
}
int Game::KillJs (int l, int r, int y, const HURT&hu, int from, bool jian) //伤害僵尸
{
    if (y < 0||y >= MN)return 0;
    int rea=(hu.fire>0.1?1:0),rt=0;
    if(from==7)rea=2;//窝瓜
    else if(from==16)rea=-1;
    for (int i = 0; i < zmb_cnt; i++) {
        int zt=zombies[i].type;
        if(zt==19&&from!=3&&from!=5&&from!=72&&from!=15)continue;
        if (zombies[i].y == y&&zombies[i].x >= l&&zombies[i].x <= r&&zt < 100) {
            HURT t=hu;
            if(jian&&r>l)//溅射
            {
                double p=1-fabs(zombies[i].x-(l+r)/2.0)/((r-l)/2.0)*0.5;
                t.mul(p);
            }
            if(zt==11&&is_zha[from])//炸弹类：僵王抵御75%
            {
                t.atk/=4;
                if(zombies[i].vx>0)zombies[i].vx=-zombies[i].vx;
            }
            HurtJs(i,rea,from,t);rt+=1;
        }
    }
    if (rea==1) { //烧掉冰
        for (int x = 0; x < MM; x++)
            if(x*SI>=l&&x*SI<=r)
            {
                int w=GetZwShu(x*SI,y);
                if(w!=-1&&plants[w].frez_stp)
                    JieDong(w,inf),rt+=1;
                if(m_ice[x][y]>-1)
                    m_ice[x][y] = -1,rt+=1;
            }
    }
    return rt;
}
bool Game::KillJsLie (int x, int kill,int ct) { //列伤害
    bool rt = false;short frez=(kill<0?80:0);
    int l = x - SI * 0.4, r = x + SI * 0.4;
    for (int i = 0; i < zmb_cnt; i++) {
        if (zombies[i].y!=-1&&zombies[i].type!=19&&zombies[i].x >= l&&zombies[i].x <= r&&zombies[i].type < 100) {
            rt = true;
            HurtJs(i,abs(kill),0,1,frez,ct);
        }
    }
    return rt;
}
int Game::KillJsLine (int y, const HURT&hu, int from) //行伤害
{
    return KillJs(-inf,inf,y,hu,from);
}

void Game::DelExtraZmbPlt()
{
    std::sort(zmb_de.begin(),zmb_de.end());
    std::sort(plt_de.begin(),plt_de.end());
    while(!zmb_de.empty())
    {
        int t=zmb_de.back();
#ifdef TEST_MODE
        if(zombies[t].y!=-1)
            exit(-1);
#endif
        zmb_de.pop_back();
        zmb_cnt-=1;
        if(t==zmb_cnt)continue;
        zombies[t]=zombies[zmb_cnt];
        zmb_ii[zombies[t].ii]=t;
        zombies[t].num=t;
    }
    if(!plt_de.empty())nd_calfr=true;
    while(!plt_de.empty())
    {
        int t=plt_de.back();
        plt_de.pop_back();
        plt_cnt-=1;
        if(t==plt_cnt)continue;
        plants[t]=plants[plt_cnt];
        plt_ii[plants[t].ii]=t;
        plants[t].num=t;
    }
}

void Game::KillAll () {
    for (int i = 0; i < MM; i++)
        for (int j = 0; j < MN; j++)
            m_ng[i][j]=0;
    for(int i=0;i<zmb_cnt;i++)
        if(zombies[i].y!=-1)DestroyJs(i);
    for(int i=0;i<plt_cnt;i++)
        if(plants[i].y!=-1)DestroyZw(i,2);
    DelExtraZmbPlt();
    while (bul_cnt > 0)
        DestroyZd (0);
    while (shi_cnt > 0)
        DestroySt (0);
}

void Game::ClearGrid()
{
    for (int i = 0; i < MM; i++) {
        for (int j = 0; j < MN; j++) {
            grid_type[i][j] = m_ng[i][j] =m_hy[i][j]=0;
            m_ice[i][j] = m_keng[i][j] = -1;
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
        if(endless)
            MN=8,MM=15,SI=68;
        else if(m_wnd->gqe==12||m_wnd->gqe==3||m_wnd->gqe==22||lx==4||lx==5)
            MN=6,MM=11,SI=91;
        else
            MN=5,MM=9,SI=110;
    }
    else MN=usermn,MM=usermm,SI=usersi;
}

void Game::SetWaterLine(int y,int hs)
{
    iswa[y]=true;
    for(int x=0;x<MM;x++)
        grid_type[x][y]=1;
    for(int x=0;x<hs;x++)
        m_hy[x][y]=800;
}

void Game::CreateGame (int lx) {
    ClearGrid();
    if (lx==4||lx==5) {//水池
        if (endless) {//无尽水池
            for(int y=1;y<=MN/4;y++)
            {
                SetWaterLine(y,2);
                SetWaterLine(MN-1-y,2);
            }
        }
        else
        {
            int s=std::max(1,MN/3);
            for(int y=s;y<MN-s;y++)
                SetWaterLine(y,1);
        }
    }
    lstup=dps=fans=0;lstcar=lstfan=laygTM=-inf;
    for (int x = 0; x < MM * SI + 100; x++)
        for (int y = 0; y < MN; y++)
            plt_num[x][y] = -1;
    for (int i = 0; i < MN; i++)
        row_zmb[i] = lst[i] = 0;
    zmb_cnt = plt_cnt = bul_cnt = shi_cnt=t_ai = XS=iis_z=iis_p=0;
    m_yg = 100;expad=ygad=la_mvzd=0;
    for (int i = 0; i < MN; i++) {
        cars[i] = car_num;
        car_TM[i] = -inf;
        car_driv[i] = false;
    }
    for (int i = 0; i < MN; i++)
        for (int j = 0; j < MM; j++)
            le_ng[j][i]=-inf,js_nd[j][i]=-1,lf_mul[j][i]=spd_mul[j][i]=1.0,wz_fei[j][i]=0;
    started = true;
    over = plt_kui= no_num=false;
    is_day=(lx==0||lx==1||lx==4||lx==5);
    is_fog=(lx==1||lx==3);
    is_cycle=(lx==5);
    if (m_wnd->is_boss) //生成BOSS
    {
        Zombie j;int mx=(SI <= 55&&m_wnd->is12 ? 3000 : 1000);
        if(m_wnd->is12)mx=2000;
        mx*=m_wnd->xmul;
        j.Create (0, mx, inf, (MM - 1) * SI + 5, 0, 54, 11);
        for (int n = 0; n < MN; n++)
            j.y = n,NewJs (j);
        m_wnd->m_jwbs[MN]=mx;zjd=mx*MN;
    }
    HI=MN*SI+60;W1=MM*SI+50;
    zmb_de.clear();plt_de.clear();
    js_xsum=js_sum=dang_cnt=car_use=js_car=zw_eat=0;//计分系统
}

void Game::input (QDataStream &ar) {
    ar>>iis_z>>iis_p;
    for(int i=0;i<iis_z;i++)zmb_ii[i]=-1;
    for(int i=0;i<iis_p;i++)plt_ii[i]=-1;
    ar >> plt_cnt;
    for (int i = 0; i < plt_cnt; i++)
    {
        plants[i].input (ar);
        plt_ii[plants[i].ii]=i;
    }
    ar >> zmb_cnt;
    for (int i = 0; i < zmb_cnt; i++)
    {
        zombies[i].input (ar);
        zmb_ii[zombies[i].ii]=i;
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
            ar >> grid_type[i][j] >> m_ng[i][j] >> m_ice[i][j]>>m_keng[i][j]>>js_nd[i][j]>>le_ng[i][j]>>spd_mul[i][j]>>wz_fei[i][j]>>lf_mul[i][j];
    }
    for (int i = 0; i < MN; i++) {
        ar >> cars[i] >> car_driv[i] >> iswa[i];
        ar >> lst[i]>>car_TM[i];
    }
    ar>>t_ai>>endless>>plt_kui>>lstfan>>lmw>>jd>>zjd;
    if(m_wnd->tcp.size()==0)
    {
        int s;ar>>s;vdb.clear();
        while(s--)
        {
            int x;ar>>x;
            vdb.push_back(x);
        }
    }
    zmb_de.clear();plt_de.clear();
    zd_pol.input(ar);
    for(int i=0;i<zd_pol.sl;i++)
    {
        int s;ar>>s;
        zd_atk[i]=0;
        for(int j=0;j<s;j++)
        {
            int t;ar>>t;
            zd_atk[i][t]=1;
        }
    }
    ar>>car_num>>car_ryg;
}
void Game::output (QDataStream &ar) {
    DelExtraZmbPlt();
    ar<<iis_z<<iis_p<<plt_cnt;
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
            ar << grid_type[i][j] << m_ng[i][j] << m_ice[i][j]<<m_keng[i][j]<<js_nd[i][j]<<le_ng[i][j]<<spd_mul[i][j]<<wz_fei[i][j]<<lf_mul[i][j];
    }
    for (int i = 0; i < MN; i++) {
        ar << cars[i] << car_driv[i] << iswa[i];
        ar << lst[i] <<car_TM[i];
    }
    ar <<t_ai<<endless<<plt_kui<<lstfan<<lmw<<jd<<zjd;
    if(m_wnd->tcp.size()==0)
    {
        ar<<int(vdb.size());
        for(int x:vdb)ar<<x;
    }
    zd_pol.output(ar);
    for(int i=0;i<zd_pol.sl;i++)
    {
        int s=0;
        for(int j=0;j<iis_z;j++)
            s+=zd_atk[i][j];
        ar<<s;
        for(int j=0;j<iis_z;j++)
            if(zd_atk[i][j])ar<<j;
    }
    ar<<car_num<<car_ryg;
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

//double

void Game::WanQuanTu()//TODO: 修改为圆形碰撞/点到线段距离
{
    int s=0;
    for (int i = 0; i < plt_cnt; i++)
        if (plants[i].y!=-1 && plants[i].type == 33)s+=1;
    if(s>MN*3)return;
    for (int i = 0; i < plt_cnt; i++) {
        if (plants[i].y!=-1 && plants[i].type == 33) {
            int x1 = plants[i].x + SI / 2, y1 = plants[i].y * SI + SI / 2;
            for (int j = i + 1; j < plt_cnt; j++) {
                if (plants[j].y!=-1 && plants[j].type == 33) {
                    int x2 = plants[j].x + SI / 2, y2 = plants[j].y * SI + SI / 2;
                    double mul=plants[i].mul*plants[j].mul;
                    for (int s = 0; s < zmb_cnt; s++) {
                        if (zombies[s].y==-1 || zombies[s].type >= 100||!jiaorect (zombies[s].x, zombies[s].y * SI, SPt (x1, y1), SPt (x2, y2)))
                            continue;
                        HurtJs(s,300*mul,1,33,0,10,mul);
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
            if(plants[i].y==-1)continue;
            int f=plants[i].fire;
            if(plants[i].Burning())
            {
                int od=plants[i].la_et;
                if(HurtZw(i,plants[i].mxlf/80.0,1))//火焰伤害
                    continue;
                plants[i].la_et=od;
            }
            if(f)f+=1;
            if(f>=150)f=0;
            plants[i].fire=f;
        }
        for(int i=0;i<plt_cnt;i++)
        {
            if(plants[i].y==-1)continue;
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
            if(plants[i].y!=-1&&plants[i].frez_stp)HurtZw(i,plants[i].mxlf/100.0,1);
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
    int zx = inf, wz = -1;
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
    if(w==-1||zmb_ii[w]==-1)return -1;
    w=zmb_ii[w];
    int rt=w,rx=-inf,y=zombies[w].y,wx=zombies[w].x;
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
    if(w==-1||zmb_ii[w]==-1)return -1;
    w=zmb_ii[w];
    int rt=w,rx=inf,y=zombies[w].y,wx=zombies[w].x;
    for(int i=0;i<zmb_cnt;i++)
    {
        if(zombies[i].type>=100||zombies[i].y!=y)
            continue;
        int x=zombies[i].x;
        if((x>wx||(x==wx&&i>w))&&x<rx)rt=i,rx=x;
    }
    return zombies[rt].ii;
}

void Game::LineXu(int x,int y,double xu)
{
    for(int i=0;i<zmb_cnt;i++)
        if(zombies[i].y==y&&zombies[i].x>=x&&zombies[i].type<100)
            zombies[i].xu+=xu;//施加虚弱效果
}

void Game::DoZwAnimine(int tm,double spd)
{
    for(int i=0;i<plt_cnt;i++)
    {
        Plant &zw=plants[i];
        if(zw.y==-1)continue;
        if((zw.dzsy&&zw.type!=88)||zw.nxt_tm==-1||m_wnd->b_ycl)
        {
            zw.tr=0;
            continue;
        }
        if(zw.type==84)//模仿者
            zw.tr+=0.3;
        else if(zw.type==80)
        {
            if(zw.nxt_tm-tm<150)zw.tr+=0.05;
            else if(zw.tr>0)zw.tr=std::max(zw.tr-0.05,0.0);
        }
        else if(zw.type==88)//旋转射手
        {
            if(zw.dzsy)zw.tr+=zw.dzsy*0.15;
            else zw.tr+=0.34;
        }
        else if(zw.type==98)
        {
            if(zw.tag)zw.tr=60;
            else
            {
                int sy=zw.time*(zw.grow-1)+(zw.nxt_tm-tm);
                if(sy>0)zw.tr+=20*(60-zw.tr)/sy;
            }
        }
        else if(zw.type==22||zw.type==83)//星星
        {
            int t=std::min(zw.time-50,500);
            if(zw.nxt_tm-tm<t)zw.tr+=0.006*spd;
            else zw.tr=0;
        }
        else if(is_tou[zw.type]||zw.type==79||zw.type==82)//投手，火冰龙草
        {
            int t=std::min(zw.time*8/9-50,1000);
            if(zw.nxt_tm-tm<t)zw.tr+=0.01*spd;//逆时针
            else if(zw.tr>0)zw.tr=std::max(zw.tr-0.08*spd,0.0);//顺时针
        }
        else if(is_shear[zw.type]||zw.type==86)//拉伸
        {
            int t=std::min(zw.time-50,750);
            if(zw.nxt_tm-tm<t)zw.tr+=0.006*spd;
            else zw.tr=0;
        }
    }
}

void Game::FlagTick()
{
    int cn=0;
    for(int i=0;i<zmb_cnt;i++)
        if(zombies[i].y!=-1&&zombies[i].type==31)cn+=1;
    if(cn)
    {
        for(int i=0;i<zmb_cnt;i++)
            if(zombies[i].y!=-1&&zombies[i].type<100)
            {
                zombies[i].life+=400*cn;
                if(zombies[i].life>zombies[i].mxlf*2)
                    zombies[i].life=zombies[i].mxlf*2;
                zombies[i].du=std::max(zombies[i].du-cn*10,0);
                zombies[i].id=GetJsIcon(zombies[i]);
            }
    }
}

void Game::DianTick()
{
    double su[MAX_ZMB_CNT];
    for(int i=0;i<zmb_cnt;i++)su[i]=0;
    for(int i=0;i<zmb_cnt;i++)
    {
        if(zombies[i].y!=-1&&zombies[i].type<100&&zombies[i].q>0)
        {
            double t=zombies[i].q*DIAN_TRANS;//放电
            su[i]+=t*t*DIAN_HURT/10;
            zombies[i].q-=t;
        }
    }
    std::vector<std::pair<double,int> > px;
    for(int i=0;i<zmb_cnt;i++)
        if(zombies[i].y!=-1&&zombies[i].type<100)
            for(int j=i+1;j<zmb_cnt;j++)
                if(zombies[j].y!=-1&&zombies[j].type<100)
                {
                    double t=(zombies[j].q-zombies[i].q)*DIAN_TRANS;
                    int dx=zombies[i].x-zombies[j].x,dy=SI*(zombies[i].y-zombies[j].y);
                    double hu=500.0/(500.0+sqrt(dx*dx+dy*dy))*(t*t*DIAN_HURT);//随距离衰减
                    su[i]+=hu;su[j]+=hu;
                    zombies[i].q+=t;zombies[j].q-=t;
                    px.push_back(std::make_pair(fabs(t),i*1000+j));
                }
    for(int i=0;i<zmb_cnt;i++)
    {
        if(zombies[i].y==-1||zombies[i].type>=100)
            continue;
        int t=int(su[i]*dian_mul[zombies[i].type]+0.5);
        if(t>0)HurtJs_Fast(i,t,1,90);
    }
    std::sort(px.begin(),px.end(),std::greater<std::pair<double,int> >());
    int sy=zmb_cnt*2;
    for(auto t:px)
    {
        int a=t.second/1000,b=t.second%1000;
        if(zombies[a].y==-1||zombies[b].y==-1)
            continue;
        ShiTi st;st.ty=13;st.id=int(t.first*1000+0.5);
        st.x=zombies[a].ii;st.y=zombies[b].ii;
        st.tm=TM;NewSt(st);sy-=1;
        if(sy<=0)break;
    }
}

double Game::CalScoreP()
{
    double p1=std::max(0.0,1.0-double(car_use)/MN);//小车
    int mx=(MM-1)*SI;
    double p2=double(js_xsum)/js_sum/mx;//杀速
    if(p2<0)p2=0;
    if(p2>1)p2=1;
    double p3=std::max(0.0,1.0-10*double(dang_cnt)/js_sum);//危机
    int ms=TM/200;//5s暂停一次
    double p4=std::max(0.0,1.0-double(m_wnd->stop_cnt)/ms);//暂停
    double p5=std::max(0.0,1.0-2*double(js_car)/js_sum);//击杀
    zw_eat-=js_sum/10;
    if(zw_eat<0)zw_eat=0;
    double p6=std::max(0.0,1.0-2*double(zw_eat)/js_sum);//损失
    //QMessageBox::information(m_wnd,"提示",QString::asprintf("%lf %lf %lf %lf %lf %lf",p1,p2,p3,p4,p5,p6));
    double rt=(p1*5+p2*1+p3*2+p4*3+p5*4+p6*2)/(5+1+2+3+4+2)*1.05;
    if(rt>1)rt=1;
    return rt;
}

void Game::RightKey(int w, QPoint pt)
{
    int ty=plants[w].type;
    double mul=plants[w].mul;
    if(ty==71&&plants[w].grow==0&&m_yg>=200)
    {
        m_yg-=200;
        LengJing(w,300000*mul);
    }
    else if(ty==55)
    {
        int rt=FindZombie(pt);
        if(rt!=-1)plants[w].tag=zombies[rt].ii;
    }
    else if(ty==98&&plants[w].tag>0)
    {
        Bullet zd=BulletFromZw(w);
        zd.attack=10000*mul;NewZd(zd);
        plants[w].tag-=1;
        if(plants[w].tag==0)
            plants[w].tr=0;
    }
    else if(ty==62&&plants[w].tag!=-1&&plants[w].grow>0)
    {
        int t=m_wnd->tyToNumP[plants[w].tag];
        plants[w].grow-=1;m_wnd->cd[t]=800;
        if(plants[w].grow==0)plants[w].tag=-1;
        m_wnd->select=t;m_wnd->SendDeltaUse(w);
    }
}

void Game::ChuiTick()
{
    for(int i=0;i<zmb_cnt;i++)
    {
        if(zombies[i].y==-1||zombies[i].type>=100)continue;
        if(zombies[i].tag>=0)
        {
            zombies[i].tag-=1;
            if(zombies[i].tag<=0)
                zombies[i].tag=-(40+RA()%41);
        }
        else
        {
            zombies[i].tag+=1;
            if(zombies[i].tag==0)
                zombies[i].tag=(80+RA()%41);
        }
    }
}

void Game::merge2048(int sx, int sy, int dx, int dy, int k)
{
    for(int i=0;i<k;i++)
    {
        int w=GetZwShu(SI*(sx+i*dx),sy+i*dy);

    }
}
