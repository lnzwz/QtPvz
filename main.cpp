#include "mainwindow.h"
int XUAN,TM,MM,MN,SI=110;
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

int GetZwIcon(SZhw &zw)
{
    int rtn;
    switch(zw.lx)
    {
    case 0:
        return 1;
    case 1:
        return 2;
    case 2:
        if(zw.shm>600)
            return 76;
        else if(zw.shm>300)
            return 4;
        else if(zw.shm>100)
            return 13;
        else
            return 14;
    case 3:
        return 7;
    case 4:
        return 8;
    case 5:
        return 9;
    case 6:
        if(zw.shm>1600)
            return 77;
        else if(zw.shm>1000)
            return 11;
        else if(zw.shm>500)
            return 15;
        else
            return 16;
    case 7:
        return 12;
    case 8:
        if(TM+35>=zw.nfs)
            return 43;
        return 100;
    case 9:
        return 48;
    case 10:
        if(TM+35>=zw.nfs)
            return 55;
        return 99;
    case 11:
        return 56;
    case 12:
        return zw.fu<10?57:64;
    case 13:
        return 58;
    case 15:
        return 63;
    case 16:
        return zw.fu==0?68:69;
    case 17:
        if(zw.fu>20)
            return 70;
        else if(zw.fu>0)
            return 90;
        else
            return 71;
    case 18:
        return 72;
    case 19:
        if(zw.shm<40)
            return 126;
        rtn=82-(zw.shm-1)/100+7;
        if(rtn<82)
            rtn=82;
        return rtn;
    case 20:
        return 94;
    case 21:
        return 95;
    case 22:
        return 98;
    case 23:
        return 101;
    case 25:
        if(zw.shm>20)
            return 104;
        else
            return 105;
    case 26:
        return 106;
    case 27:
        return 110;
    case 30:
        if(zw.nfs<=4)
            return 137;
        else if(zw.nfs<=9)
            return 138;
        return 134;
    case 31:
        return 135;
    case 32:
        return 141;
    case 33:
        return 142;
    default:
        return 0;
    }
}
int GetJsIcon(SJsh &js,CGame &ga)
{
    switch(js.lx)
    {
    case -2:
        return 28;
    case -1:
        if(js.shm>14)
            return 40;
        else if(js.shm>6)
            return 41;
        else
            return 42;
    case 0:
        if(js.shm>3)
            return 3;
        else if(js.shm>1)
            return 17;
        else
            return 18;
    case 1:
        if(js.shm>9)
            return 5;
        else if(js.shm>6)
            return 19;
        else if(js.shm>3)
            return 3;
        else if(js.shm>1)
            return 17;
        else
            return 18;
    case 2:
        if(js.shm>23)
            return 6;
        else if(js.shm>16)
            return 20;
        else if(js.shm>10)
            return 21;
        else if(js.shm>3)
            return 3;
        else if(js.shm>1)
            return 17;
        else
            return 18;
    case 3:
        if(js.shm>7)
            return 10;
        else if(js.shm>4)
            return 22;
        else
            return 23;
    case 4:
        if(js.shm>11)
            return 24;
        else if(js.shm>8)
            return 25;
        else
        {
            js.sd=js.eat=3;
            if(js.shm>4)
                return 26;
            else
                return 27;
        }
    case 5:
        if(js.shm>5)
            return js.beat?29:30;
        else
            return js.beat?35:31;
    case 6:
        if(js.shm>12)
            return 32;
        else if(js.shm>4)
            return 33;
        else
            return 34;
    case 7:
        if(js.shm>55)
            return 36;
        else if(js.shm>40)
            return 37;
        else if(js.shm>25)
            return 38;
        else if(js.shm>10)
            return 39;
        else if(js.shm>3)
            return 3;
        else if(js.shm>1)
            return 17;
        else
            return 18;
    case 8:
        if(js.shm>9)
            return js.beat?44:45;
        else
            return js.beat?46:47;
    case 9:
        if(js.shm<=90)
        {
            if(!js.fu&&js.y!=-1)
            {
                SJsh j;
                int x=js.x-300;
                if(x<220)
                    x=220;
                j.Create(2,3,1,x,js.y,50,10);
                ga.NewJs(j);
                if(rand()%2==0)
                {
                    j.Create(2,3,1,x+100,js.y,50,10);
                    ga.NewJs(j);
                }
                js.fu=true;
            }
            return js.shm<40?52:51;
        }
        else
            return 49;
    case 10:
        if(js.shm>3)
            return 50;
        else
            return 53;
    case 11:
        return 54;
    case 12:
        if(js.shm>70)
            return 65;
        else if(js.shm>50)
            return 66;
        else if(js.shm>30)
            return 67;
        else if(js.shm>10)
        {
            js.sd=1;
            js.eat=2;
            return 50;
        }
        else
        {
            js.sd=1;
            js.eat=2;
            return 53;
        }
    case 13:
        if(js.shm>8)
            return 73;
        else
            return 74;
    case 14:
        if(js.shm>10)
            return 79;
        else
            return 80;
    case 15:
        if(js.shm>5)
            return 92;
        else
            return 93;
    case 16:
        if(js.shm>12)
            return 96;
        else
            return 97;
    case 17:
        if(js.shm>20)
            return 107;
        else if(js.shm>5)
            return 108;
        return 109;
    case 100:
        return 75;
    case 101:
        return 81;
    default:
        return 0;
    }
}

int GetJsEat(SZhw&zw,SJsh&js,CGame &ga)//��ý�ʬ����Ծ������ͼ��
{
    int et;
    switch(js.lx)
    {
    case 5:
        if(zw.lx!=6)//���Ǹ߼��
            js.x-=SI;
        js.sd=1;
        js.beat=false;
        return 30;
    case 8:
        if(zw.lx!=6)//���Ǹ߼��
        {
            js.x-=js.sd*2;
            return 44;
        }
        js.sd=1;
        js.beat=false;
        return 45;
    case 100://�㶹
        if(zw.lx==17)
        {
            js.x-=js.sd;
            return 75;
        }
        else if(zw.lx==25)
        {
            int x=js.x,y=js.y;
            ga.DestroyJs(js.shu);
            zw.shm-=1;zw.id=GetZwIcon(zw);
            if(zw.shm<=0)
            {
                ga.DestroyZw(zw.shu);
                return 0;
            }
            SZid zd;zd.hb=false;
            zd.color=QColor(0,255,0);
            zd.dx=5;zd.goj=1;zd.sd=4;
            zd.x=x;zd.y=y;
            ga.NewZd(zd);
            return 0;
        }
        if(zw.shm<=100&&!ga.m_ng[zw.x/SI][zw.y])
            et=20;
        else
            et=37;
        ga.m_ng[zw.x/SI][zw.y]-=et;et=0;
        if(ga.m_ng[zw.x/SI][zw.y]<0)
        {
            et=-ga.m_ng[zw.x/SI][zw.y];
            ga.m_ng[zw.x/SI][zw.y]=0;
        }
        zw.shm-=et;zw.id=GetZwIcon(zw);
        if(zw.shm<=0)
            ga.DestroyZw(zw.shu);
        ga.DestroyJs(js.shu);
        return 0;
    case 101://����
        if(zw.lx==17)
        {
            js.x-=js.sd;
            return 81;
        }
        else if(zw.lx==25)
        {
            int x=js.x-SI/3,y=js.y;
            ga.DestroyJs(js.shu);
            zw.shm-=3;zw.id=GetZwIcon(zw);
            if(zw.shm<=0)
            {
                ga.DestroyZw(zw.shu);
                return 0;
            }
            SZid zd;zd.hb=true;
            zd.color=QColor(128,128,255);
            zd.dx=11;zd.goj=4;zd.sd=4;
            zd.x=x;zd.y=y;
            ga.NewZd(zd);
            return 0;
        }
        if(zw.shm<=100&&!ga.m_ng[zw.x/SI][zw.y])
            et=30;
        else
            et=95;
        ga.m_ng[zw.x/SI][zw.y]-=et;et=0;
        if(ga.m_ng[zw.x/SI][zw.y]<0)
        {
            et=-ga.m_ng[zw.x/SI][zw.y];
            ga.m_ng[zw.x/SI][zw.y]=0;
        }
        zw.shm-=et;zw.id=GetZwIcon(zw);
        if(zw.shm<=0)
            ga.DestroyZw(zw.shu);
        ga.DestroyJs(js.shu);
        return 0;
    default:
        return 0;
    }
    //zw.id=GetZwIcon(zw);
}

void Fash(SJsh &js,CGame &ga)
{
    switch(js.lx)
    {
    case 13://�㶹��ʬ
        SJsh js2;
        js2.Create(10,10000,0,js.x,js.y,75,100,true);
        ga.NewJs(js2);
        break;
    case 14://���Ͻ�ʬ
        SJsh js3;
        js3.Create(10,10000,0,js.x,js.y,81,101,true);
        ga.NewJs(js3);
        break;
    case 16://ĩӰ��
        js.x=rand()%(MM-1)*SI;
        ga.hjs[js.y]-=1;
        js.y=rand()%MN;
        ga.hjs[js.y]+=1;
        break;
    }
}

bool DaZhao(SZhw &zw,MainWindow *wnd)
{
    //return true;
    switch(zw.lx)
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
        zw.time=100;
        wnd->KillTimer(zw.shu);
        wnd->SetTimer(zw.shu,100,NULL);
        break;
    case 2:
    case 6:
    case 19:
        zw.shm+=zw.shm/2;
        break;
    case 32:
        for(int i=0;i<MN;i++)
        {
            SZid zd;zd.mh=true;
            zd.color=QColor(240,0,255);
            zd.dx=14;zd.goj=0;zd.sd=4;
            zd.x=zw.x;zd.y=i;
            wnd->m_game.NewZd(zd);
        }
        break;
    default:
        return false;
    }
    zw.id=GetZwIcon(zw);
    wnd->SetTimer(10000+zw.shu,zw.dztm,NULL);
    return true;
}

void QxDaZhao(SZhw &zw,MainWindow *wnd)
{
    switch(zw.lx)
    {
    case 0:
    case 4:
    case 9:
    case 8:
        zw.time=wnd->m_zw[1].time;
        break;
    case 10:
        zw.time=wnd->m_zw[11].time;
        break;
    case 13:
        zw.time=wnd->m_zw[1].time;
        break;
    case 1:
        zw.time=wnd->m_zw[0].time;
        break;
    case 22:
        zw.time=wnd->m_zw[22].time;
        break;
    case 26:
        zw.time=wnd->m_zw[26].time;
        break;
    }
    wnd->KillTimer(zw.shu);
    wnd->SetTimer(zw.shu,zw.time,NULL);
}
