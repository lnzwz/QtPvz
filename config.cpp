//植物和僵尸的配置信息
#include "stdafx.h"
#include "game.h"
#include "mainwindow.h"
int GetZwIcon(const Plant &zw)
{
    int rtn,lf=int(zw.life);
    switch(zw.type)
    {
    case -1:
        return 165;
    case 2:
        if(lf>750)
            return 76;
        else if(lf>500)
            return 4;
        else if(lf>200)
            return 13;
        else
            return 14;
    case 6:
        if(lf>1500)
            return 77;
        else if(lf>900)
            return 11;
        else if(lf>400)
            return 15;
        else
            return 16;
    case 8:
        if(TM+std::min(zw.time,2000)/50>=zw.nxt_tm||m_wnd->m_game.row_zmb[zw.y]==0)
            return 43;
        return 100;
    case 10:
        if(TM+std::min(zw.time,2000)/50>=zw.nxt_tm||m_wnd->m_game.row_zmb[zw.y]==0)
            return 55;
        return 99;
    case 69:
        if(TM+std::min(zw.time,2000)/50>=zw.nxt_tm||m_wnd->m_game.row_zmb[zw.y]==0)
            return 231;
        return 232;
    case 56:
        if(TM+std::min(zw.time,2000)/50>=zw.nxt_tm||m_wnd->m_game.row_zmb[zw.y]==0)
            return zw.color==BUTTER?212:210;
        return 213;
    case 12:
        return zw.grow<zw.move_spd?57:64;
    case 16:
        return zw.grow==0?68:69;
    case 17:
        return zw.grow?70:71;
    case 19:
        if(lf<40)
            return 126;
        rtn=89-8*int(lf-40)/int(zw.mxlf-40);
        if(rtn<82)rtn=82;
        return rtn;
    case 20:
        if(zw.tag)return 173;
        return 94;
    case 24:
        return -1;
    case 25:
        if(lf>400)
            return 104;
        else
            return 105;
    case 29:
        if(lf>60)
            return 132;
        return 133;
    case 30:
        return zw.grow<zw.move_spd?137:134;
    case 35:
        if(lf>400)
            return 144;
        else if(lf>200)
            return 145;
        else
            return 146;
    case 37:
        return 151+zw.tag;
    case 71:
        return zw.grow?237:236;
    default:
        return zw.id;
    }
}
int GetDunIcon(const Zombie &js)
{
    double p=double(js.dun)/js.mxdun;
    switch (js.type) {
    case 4:
        return (p>0.5?24:25);
    case 7:
        if(p>0.7)return 36;
        else if(p>0.4)return 37;
        else return 38;
    case 31:
        return (p>0.5?40:41);
    }
    return 1;
}
int GetJsIcon(Zombie &js)
{
    int lf=js.life/1000;double p=double(js.life)/js.mxlf;
    switch(js.type)
    {
    case -2://小车
        return 28;
    case 0://普通
        if(lf>4)return 3;
        else if(lf>1)return 17;
        else return 18;
    case 1://路障
        if(lf<=8)
        {
            js.type=0;
            return GetJsIcon(js);
        }
        if(p>0.7)return 5;
        else return 19;
    case 2://铁桶
        if(lf<=8)
        {
            js.type=0;
            return GetJsIcon(js);
        }
        if(p>0.7)return 6;
        else if(p>0.5)return 20;
        else return 21;
    case 3://跑步
        if(p>0.7)return 10;
        else if(p>0.4)return 22;
        else return 23;
    case 4://报纸（有报纸）
        if(p>0.5)return 260;
        else return 261;
    case 5://撑杆跳
        if(p>0.5)return js.sp_eat?30:29;
        else return js.sp_eat?31:35;
    case 6://大嘴
        if(p>0.7)return 32;
        else if(p>0.4)return 33;
        else return 34;
    case 7://铁门（有盾）
        if(p>0.5)return 262;
        else return 263;
    case 8://跳跳
        if(p>0.5)return js.sp_eat?44:45;
        else return js.sp_eat?46:47;
    case 9://巨人
        if(js.sta&1)return p<0.25?52:51;
        else return 49;
    case 10://小鬼
        if(p>0.5)return 50;
        else return 53;
    case 12://小车（有车）
        if(p>0.7)return 65;
        else if(p>0.5)return 66;
        else if(p>0.3)return 67;
        else
        {
            js.spd=(js.spd<0?-1:1);
            js.eat=1;js.type=29;return GetJsIcon(js);
        }
    case 13://豌豆射手僵尸
        if(p>0.5)return 73;
        else return 74;
    case 14://西瓜射手僵尸
        if(p>0.5)return 79;
        else return 80;
    case 15://炸弹僵尸
        if(p>0.5)return 92;
        else return 93;
    case 16://末影人
        if(p>0.5)return 96;
        else return 97;
    case 17://魔法僵尸
        if(p>0.7)return 107;
        else if(p>0.4)return 108;
        else return 109;
    case 18://气功
        if(p>0.5)return 147;
        else return 148;
    case 20://喷火
        if(p>0.5)return 163;
        return 164;
    case 22://寒冰射手僵尸
        if(p>0.5)return 181;
        else return 182;
    case 23://高坚果僵尸
        return 188+std::min(3,int(std::fmax(1-p,0.0)*4));
    case 24://毒坚果僵尸
        if(p>0.5)return 192;
        return 193;
    case 25://向日葵僵尸
        if(p>0.5)return 197;
        return 198;
    case 26://火药桶僵尸
        if(p>0.5)return 205;
        return 206;
    case 27://扶梯（有梯子）
        if(p>0.5)return 208;
        else return 250;
    case 28://超级巨人
        if(p>0.75)return 216;
        else if(p>0.5)return 217;
        else if(p>0.25)return 218;
        return 219;
    case 29://小车（无车）
        if(p>0.15)return 166;
        else return 167;
    case 30://报纸（失去报纸）
        if(p>0.5)return 26;
        else return 27;
    case 31://旗帜（有旗）
        if(p>0.5)return 264;
        else return 265;
    case 32://辣椒
        if(p>0.5)return 244;
        else return 245;
    case 33://窝瓜
        if(p>0.5)return 246;
        else return 249;
    case 34://扶梯（无梯子）
        if(p>0.5)return 214;
        else return 251;
    default:
        return js.id;
    }
}

Bullet GetFanTan(int id)
{
    Bullet zd;zd.spd=8;
    if(id==75)
    {
        zd.color=GREEN;
        zd.siz=5;zd.attack=1000;
    }
    else if(id==81)
    {
        zd.color=GREEN;
        zd.siz=11;zd.attack=2500;
    }
    else
    {
        zd.freeze=160;zd.color=DBLUE;
        zd.siz=11;zd.attack=5000;
    }
    return zd;
}

int ClearDun(Zombie&js)
{
    switch(js.type)
    {
    case 4:
        js.type=30;js.eat=5;js.dun=0;
        js.spd=(js.spd<0?-2.5:2.5);
        break;
    case 7:
        js.type=0;js.dun=0;
        break;
    case 27:
        js.type=34;js.sp_eat=0;js.dun=0;
        break;
    case 31:
        js.type=0;js.dun=0;
        break;
    }
    return GetJsIcon(js);
}

int SpecialEat(Plant&zw,Zombie&js,Game &ga,double spd,double sq)//僵尸的特殊吃法
{
    int w;
    switch(js.type)
    {
    case 5:
        if(zw.type!=6)//并非高坚果
            js.x_flt-=(js.spd<0?-SI:SI);
        js.spd=(js.spd<0?-2:2);js.dun=0;
        js.sp_eat=false;
        return GetJsIcon(js);
    case 8:
        if(zw.type!=6)//并非高坚果
        {
            js.x_flt-=spd;
            return 44;
        }
        js.spd=(js.spd<0?-2:2);
        js.sp_eat=false;
        return GetJsIcon(js);
    case 15:
        ga.Crepeer (js.num);
        return 0;
    case 17:
        ga.ZhangNan(js.num);js.eating=0;
        return js.id;
    case 9:
    case 28:
    {
        if(RA()>RA.max()*double(sq))break;
        int z=std::min(m_wnd->m_ea,2),ad=1-rand()%z;
        if (m_wnd->m_hd)ad += 1;
        if (m_wnd->m_imp)ad += 1;
        if(js.type == 28)ad*=3;
        js.sta+=ad*16;
        if (js.sta/16 >= 20) {
            int hj=0;
            if(zw.type==53||zw.type==35)hj=40000;
            ga.HurtZw(zw.num,js.eat,1);
            js.sta%=16;
            ga.BaDou(js.x,js.y,1.5);
            if(ga.HurtJs_Fast(js.num,hj,0))return 0;
        }
        return js.id;
    }
    case 18:
        for(int x=zw.x;x>=0;x-=SI)
        {
            int t=ga.GetZwShu(x,zw.y);
            if(t!=-1)
                ga.plants[t].x-=SI;
            else break;
        }
        for(int i=0;i<ga.plt_cnt;i++)
        {
            if(ga.plants[i].x<0)
            {
                ga.DestroyZw(i,0);
                break;
            }
        }
        ga.CalFirst();
        return js.id;
    case 19:
        js.x_flt-=spd;
        return 160;
    case 21:
        if(RA()>RA.max()*double(sq))break;
        w=ga.FindZw(zw.x/SI,zw.y,25);
        if(js.fss>=10&&w!=-1)
        {
            ga.plants[w].tag=TM;
            ga.DestroyJs(js.num);return 0;
        }
        js.fss+=1;
        ga.m_ng[zw.x/SI][zw.y]=std::fmax(ga.m_ng[zw.x/SI][zw.y]-js.eat,0);
        ga.le_ng[zw.x/SI][zw.y]=TM;
        if(ga.m_ng[zw.x/SI][zw.y]==0)ga.CalFirst();
        if(js.fss>=60)
        {
            if(ga.m_ng[zw.x/SI][zw.y]==0)
                ga.DestroyZw(zw.num,0);
            ga.DestroyJs(js.num);return 0;
        }
        return 178;
    case 27:
        zw.ti=1;ga.CalFirst();
        return ClearDun(js);
    case 33:
    {
        Zombie njs;
        njs.Create(0,1e4,0,zw.x,zw.y,12,102,0,1,20);
        ga.NewJs(njs);
        ga.DestroyJs(js.num);return 0;
    }
    case 100:
        if(ga.low_zw[zw.type])
        {
            js.x_flt-=spd;
            return js.id;
        }
        else if(zw.type==25)
        {
            int x=js.x,y=js.y,d=js.id;
            ga.DestroyJs(js.num);
            zw.life-=js.eat;
            zw.id=GetZwIcon(zw);
            if(zw.life<=0)
            {
                ga.DestroyZw(zw.num,0);
                return 0;
            }
            Bullet zd=GetFanTan(d);
            zd.attack*=2;
            zd.x=x-SI/2;zd.y=y;
            ga.NewZd(zd);return 0;
        }
        ga.ShootZw(zw.x/SI,zw.y,js.eat);
        ga.DestroyJs(js.num);
        return 0;
    case 101:
        ga.Explode(zw.x,zw.y,2,m_wnd->m_hd?120:80,30);
        ga.DestroyJs(js.num);
        return 0;
    }
    return js.id;
}

bool JsShoot(Zombie &js,Game &ga)
{
    Zombie njs;int w;njs.spd=0;
    switch(js.type)
    {
    case 9://巨人
        if(js.life<=js.mxlf/2&&js.spd>0&&js.sta%2==0&&js.y!=-1)
        {
            Zombie j;int x=std::max(js.x-450,120);
            j.Create(3,5,1,js.x,js.y,50,10);j.fss=x;j.fs_nd=js.x;
            if(j.fss<js.x)ga.NewJs(j);
            if(m_wnd->m_hd||m_wnd->m_ea==1)
            {
                j.Create(4,m_wnd->m_hd?10:5,m_wnd->m_hd?2:1,js.x,js.y,50,10);j.fss=x+75;j.fs_nd=js.x;
                if(j.fss<js.x)ga.NewJs(j);
            }
            if(m_wnd->m_hd)
            {
                j.Create(5,15,2,js.x,js.y,50,10);j.fss=x+150;j.fs_nd=js.x;
                if(j.fss<js.x)ga.NewJs(j);
            }
            if(m_wnd->m_imp)
            {
                j.Create(6,20,3,js.x,js.y,50,10);j.fss=x+225;j.fs_nd=js.x;
                if(j.fss<js.x)ga.NewJs(j);
            }
            js.sta|=1;
        }
        break;
    case 13://豌豆僵尸
        if(!njs.spd)njs.Create(18,1e4,8,js.x,js.y,75,100,true);
    case 14://冰瓜僵尸
        if(!njs.spd)njs.Create(18,1e4,20,js.x,js.y,81,100,true);
    case 26:
        if(!njs.spd)njs.Create(22,1e4,1,js.x,js.y,207,101,true);
    case 22://冰球
        if(!njs.spd)njs.Create(18,1e4,13,js.x,js.y,183,100,true);
        if(js.spd<0)njs.spd=-njs.spd;
        njs.ry=js.ry;ga.NewJs(njs);
        break;
    case 28://超级巨人
        w=std::max(js.x-400,120);
        njs.Create(5,10,1,js.x,js.y,50,10);
        njs.fss=w;njs.fs_nd=js.x;
        if(njs.fss<js.x)ga.NewJs(njs);
        break;
    case 32://辣椒
        if(js.x<MM*SI/2)
        {
            for(int i=0;i<MM;i++)
            {
                ga.HurtZw(i*SI,js.y,m_wnd->m_hd?40:25,1);
                int w=ga.GetZwShu(i*SI,js.y);
                if(w!=-1)ga.plants[w].Burn();
            }
            ga.DestroyJs(js.num,1);return 1;
        }
        break;
    case 16://末影人
        js.x=RA()%(MM-1)*SI;
        ga.row_zmb[js.y]-=1;
        do{js.y=RA()%MN;}
        while(ga.iswa[js.y]);
        js.ry=js.y*SI;
        ga.row_zmb[js.y]+=1;
        w=ga.FindZw(js.x/SI,js.y,25);
        if(w!=-1)
        {
            ga.plants[w].tag=TM;
            ga.DestroyJs(js.num,0);return 1;
        }
        break;
    case 20://喷火
    {
        int i = ga.GetFirstZw (js.x, js.y);
        int tx=js.x,ty = js.y,sx=tx/SI;
        if(i!=-1)sx=ga.plants[i].x/SI;
        for(int x=sx;x>=0&&tx-1.8*SI<=x*SI;x--)
        {
            int ti=ga.GetZwShu(x*SI,ty);
            if(ti!=-1&&ga.plants[ti].attack==-21&&ga.plants[ti].grow<=0)
            {
                ga.Goj(ti);
                return 0;
            }
            if(ga.m_ng[x][ty]||ti!=-1)
            {
                if(ti!=-1&&!m_wnd->imjs)ga.plants[ti].Burn();
                ga.HurtZw(x,ty,3,1);js.eating = true;//正在吃
            }
        }
        break;
    }
    case 102:
        ga.HurtZw(js.x/SI,js.y,200,1);
        ga.DestroyJs(js.num);return 1;
    }
    return 0;
}

bool DaZhao(Plant &zw,MainWindow *wnd)
{
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
    case 37:
    case 39:
    case 41:
    case 12:
    case 20:
    case 44:
    case 59:
    case 60:
    case 61:
    case 64:
    case 19:
    case 56:
    case 76:
        zw.odtm=zw.time;zw.time=100;
        wnd->KillTimer(zw.num);
        wnd->SetTimer(zw.num,zw.time);
        break;
    case 2:
    case 6:
    case 35:
        zw.odtm=zw.time;zw.time=5000;zw.def+=1+zw.lev/3;
        wnd->KillTimer(zw.num);
        wnd->SetTimer(zw.num,zw.time);
        break;
    case 32:
        for(int i=0;i<MN;i++)
        {
            Bullet zd;zd.mh=true;
            zd.color=MHCO;
            zd.siz=14;zd.attack=0;zd.spd=4;
            zd.x=zw.x;zd.y=i;
            wnd->m_game.NewZd(zd);
        }
        break;
    default:
        return false;
    }
    zw.id=GetZwIcon(zw);
    zw.dzsy=zw.dztm*(1+0.1*zw.lev);
    return true;
}

void QxDaZhao(Plant &zw,MainWindow *wnd,Game &ga)
{
    switch(zw.type)
    {
    case 2:
    case 6:
    case 35:
        zw.def-=1;
    case 0:
    case 4:
    case 9:
    case 8:
    case 10:
    case 13:
    case 1:
    case 22:
    case 26:
    case 37:
    case 39:
    case 41:
    case 12:
    case 20:
    case 44:
    case 59:
    case 60:
    case 19:
    case 56:
    case 76:
        zw.time=zw.odtm;
        break;
    }
    wnd->KillTimer(zw.num);
    wnd->SetTimer(zw.num,zw.time);
}
