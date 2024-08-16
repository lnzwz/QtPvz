//植物和僵尸的配置信息
#include "stdafx.h"
#include "game.h"
#include "mainwindow.h"
int GetZwIcon(const Plant &zw)
{
    int lf=int(zw.life),tm=m_wnd->v_tm,a=-1,b=-1;
    double p=zw.life/zw.mxlf;
    switch(zw.type)
    {
    case 2:
        if(p>1.01)
            return 76;
        else
            return 4;
    case 6:
        if(p>1.01)
            return 77;
        else
            return 11;
    case 8:
        if(a==-1)a=43,b=100;
    case 10:
        if(a==-1)a=55,b=99;
    case 69:
        if(a==-1)a=231,b=232;
    case 87:
        if(a==-1)a=83,b=145;
    case 92:
        if(a==-1)a=257,b=156;
    case 56:
        if(a==-1)a=zw.color==BUTTER&&m_wnd->m_game.row_zmb[zw.y]>0?212:210,b=213;
        if(zw.nxt_tm-tm<1400||m_wnd->m_game.row_zmb[zw.y]==0)
            return a;
        else return b;
    case 12:
        return zw.grow<zw.move_spd?57:64;
    case 16:
        return zw.grow==0?68:69;
    case 17:
        return zw.grow?70:71;
    case 19:
        if(lf<40)
            return 126;
        else
            return 82;
    case 20:
        if(zw.tag)return 173;
        return 94;
    case 24:
        return -1;
    case 25:
        if(p>0.5)
            return 104;
        else
            return 105;
    case 29:
        if(p>0.5)
            return 132;
        return 133;
    case 30:
        return zw.grow<zw.move_spd?137:134;
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
    return -1;
}
int GetJsIcon(Zombie &js)
{
    int lf=js.life/1000;
    double p=double(js.life)/js.mxlf;
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
    case 5://撑杆跳（有杆）
        if(p>0.5)return 29;
        else return 35;
    case 6://大嘴
        if(p>0.7)return 32;
        else if(p>0.4)return 33;
        else return 34;
    case 7://铁门（有盾）
        if(p>0.5)return 262;
        else return 263;
    case 8://跳跳（有杆）
        if(p>0.5)return 44;
        else return 46;
    case 9://巨人
        if(!js.shoot)return p<0.25?52:51;
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
            js.vx=(js.vx<0?-1:1);
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
    case 35://撑杆跳（无杆）
        if(p>0.5)return 30;
        else return 31;
    case 36://跳跳（无杆）
        if(p>0.5)return 45;
        else return 47;
    default:
        return js.id;
    }
}

Bullet GetFanTan(int id)
{
    Bullet zd;zd.vx=8;
    if(id==75)
    {
        zd.color=GREEN;
        zd.siz=10*SI/110;zd.attack=1000;
    }
    else if(id==81)
    {
        zd.color=GREEN;
        zd.siz=20*SI/110;zd.attack=2500;
    }
    else
    {
        zd.freeze=160;zd.color=DBLUE;zd.fire=10;
        zd.siz=20*SI/110;zd.attack=5000;
    }
    return zd;
}

int ClearDun(Zombie&js)
{
    switch(js.type)
    {
    case 4:
        js.type=30;js.eat=5;js.dun=0;
        js.vx=(js.vx<0?-2.5:2.5);
        break;
    case 5:
        js.sp_eat=false;js.type=35;
        break;
    case 8:
        js.sp_eat=false;js.type=36;
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
