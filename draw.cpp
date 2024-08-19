//绘制
#include "stdafx.h"
#include "mainwindow.h"
#include "game.h"
#include <QOpenGLPaintDevice>
#include <QOpenGLFramebufferObject>

void Game::GetZwTrans(QPainter &memdc,const Plant&zw,int&hi,int&wi) const
{
    int i=zw.num,ty=0,cx=SI/2,cy=SI,zt=zw.type,tm=m_wnd->v_tm;
    double aa=sin(TM*0.3+i)*1.1,bb=0;hi=wi=SI;
    if(zt==1||zt==2||zt==19||zt==20||zt==35||zt==16||zt==36)//摆动
    {
        if(zt==16)//大嘴花
        {
            double b=2.5;
            if(zw.grow==1||zw.grow==2)
                aa=-(double(zw.time-(zw.nxt_tm-tm))/zw.time+2-zw.grow)*10;
            else
            {
                if(zw.grow>0)b=sqrt(zw.grow)/2;
                aa=sin(TM*0.2+i)*b;
            }
        }
        else if(zt==2||zt==19||zt==35)
            aa=sin(TM*0.12+i)*1.9;//大幅度
    }
    else if(zt==54||zt==27||(zt==23&&zw.tag)||(zt==55&&zw.tag!=-1&&zmb_ii[zw.tag]!=-1))
    {
        ty=0;aa=-double(zw.nxt_tm-tm)/zw.time*360;cy=SI/2;//转动
        if(zt==27)aa=-double(zw.grow+double(zw.nxt_tm-tm)/zw.time)/zw.move_spd*360;
        if(zt==55)
        {
            int t=zmb_ii[zw.tag],tx=zombies[t].x-zw.x,ty=zombies[t].y_flt-zw.y*SI;
            aa=atan2(ty,tx)/acos(-1.0)*180;bb=sqrt(tx*tx+ty*ty);
        }
    }
    else if(zt==6)//高坚果
        ty=1,hi=SI-(0.8+sin(TM*0.15+i))*(SI/20.0);
    else if((zt==22||zt==83)&&zw.tr>0&&zmb_cnt>0)//星星
        ty=1,wi=hi=SI*(1-zw.tr);
    else if(zt==29)//地刺
        ty=1,hi=SI-(0.5+sin(TM+i))*(SI/10.0);
    else if(zt==88)
        ty=0,aa=zw.tr,cy=SI/2;
    else if(zt==26&&zw.tag)//大喷菇
    {
        int h=(0.7+sin(TM*0.5+i))*(SI/15.0);
        ty=1;hi=SI-h;wi=SI+h*2;
    }
    else if(((is_tou[zt]&&row_zmb[zw.y])||((zt==79||zt==82)&&zmb_cnt>0))&&!zw.stop)//投手
    {
        ty=0;cx=SI/2;aa=-zw.tr*60;
        if(zt==79||zt==82)cx=SI/3;
    }
    else if(is_shear[zt])//拉伸类
    {
        ty=2;aa=zw.tr;bb=0;
        if(zt!=41&&!row_zmb[zw.y])aa=0;
        if(zt==41&&!row_zmb[zw.y]&&(zw.y==0||!row_zmb[zw.y-1])&&(zw.y==MN-1||!row_zmb[zw.y+1]))aa=0;
    }
    else if(zt==25&&TM-zw.tag<=50)//反弹
    {
        double z=1+(25-abs(TM-zw.tag-25))/25.0*0.8;
        ty=-1;wi=hi=SI*z;
    }
    else if(zt==80&&zmb_cnt>0&&zw.tr>0)//菜问
        ty=1,wi=hi=SI*(1+zw.tr);
    else if(zt==84)//模仿者
        aa=sin(TM*0.5+i)*zw.tr;//大幅度
    else if(zt==86&&zw.tr>0&&row_zmb[zw.y])//双头豌豆
        ty=1,hi=SI*(1+zw.tr),wi=SI*(1-zw.tr);
    else if(is_zha[zt])//炸弹等
    {
        if(zt==7)//窝瓜
        {
            if(TM-zw.tag<15)
                ty=3,aa=0,bb=(TM-zw.tag)*(-3);
            else
                ty=-1;
        }
        else
        {
            double z=1+(TM-zw.tag)*0.05;
            ty=-1;wi=hi=SI*z;
        }
    }
    else if(zt==34&&TM-zw.tag<=100)//加速器
        ty=1,hi=SI-(0.8+sin(TM*0.3+i))*(SI/10.0);
    else if(zt==7||zt==12||zt==13||zt==21||zt==36||zt==25||zt==32||zt==40||zt==33)
        ty=2,aa=0;
    else if(zt==98)
        ty=0,aa=-zw.tr;
    if(ty==2&&fabs(aa)<1e-7)aa=sin(TM*0.3+i)*0.03,bb=-aa*aa*40;
    if(zw.type==39&&zw.tag==1)hi*=1.4;
    if(zw.stop||zw.frez_stp||(zw.dzsy&&zw.type!=88)||(zw.mush&&is_day)||zt==17||zt==11||zt==30||zt==31||zt==62||zt==-1||m_wnd->low_eff)
        ty=-1,wi=hi=SI;
    if(zw.type==89&&TM-zw.tag<=20)ty=-1;//激光豆发射
    if(zw.type==94)aa=sin(PI*(TM*25-zw.nxt_tm)/zw.time)*1.5;
    if(ty==0)//旋转
    {
        if(zw.left)cx=SI-cx,aa=-aa;
        memdc.translate (zw.x + 50+cx,zw.y * SI + 60+cy);
        memdc.rotate (aa);memdc.translate(-cx,-cy);
    }
    else if(ty==1)//拉伸
        memdc.translate(zw.x + 50-(wi-SI)/2,zw.y * SI + 60+SI-hi);
    else if(ty==2)//斜切
    {
        if(zw.left)aa=-aa;
        memdc.translate(zw.x + 50, zw.y * SI + 60+SI);
        memdc.shear(aa,bb);memdc.translate(0,-hi);
    }
    else if(ty==3)
        memdc.translate(zw.x + 50+aa, zw.y * SI + 60+bb);
    else if(ty==-1)
        memdc.translate(zw.x + 50-(wi-SI)/2, zw.y * SI + 60-(hi-SI)/2);
    if(zw.left)memdc.scale(-1,1),memdc.translate(QPoint(-wi,0));
}

void Game::DrawZwPic(QPainter &memdc,const Plant&zw,int wi,int hi,const QPixmap*ic)const
{
    if(zw.ti)memdc.drawPixmap (QRect (SI/9,0,wi,hi),ic[209]);//梯子
    int tm=m_wnd->v_tm;
    if(zw.id==137)//大炮
    {
        double p=double(zw.grow)/zw.move_spd,t=zoom*m_wnd->b_ret;
        p+=(1.0-double(zw.nxt_tm-tm)/zw.time)/zw.move_spd;
        p=p*0.8+0.2;
        double w=wi*p,h=hi*(1-p)*0.25;
        memdc.drawPixmap(QRectF(0,h,w,hi),ic[138],QRectF((wi-w)*t,0,w*t,hi*t));
        memdc.drawPixmap (QRect (0,0,wi,hi), ic[137]);
    }
    else if(zw.id==70)//地雷
    {
        double p=double(zw.grow-1)/zw.move_spd,t=zoom*m_wnd->b_ret;
        p+=double(zw.nxt_tm-tm)/zw.time/zw.move_spd;
        double h=hi-(hi/2.0)*p;
        memdc.drawPixmap(QRectF(0,hi-h,wi,h),ic[70],QRectF(0,0,wi*t,h*t));
    }
    else if(zw.id==4||zw.id==11||zw.id==82||zw.id==144||zw.id==273||zw.id==275)//坚果
    {
        double p=std::min(1.0,zw.life/zw.mxlf),k=0.36,b=0.5,t=zoom*m_wnd->b_ret;
        if(zw.id==11||zw.id==82)k=0.4,b=0.6;
        else if(zw.id==275)k=0.44,b=0.5;
        else if(zw.id==279)k=0.34,b=0.4;
        int t1=int((b+k*p)*hi+0.5),t2=int((b+k*p)*SI+0.5);
        memdc.drawPixmap (QRect(0,hi-t1,wi,t1), ic[zw.id], QRect(0,(SI-t2)*t,SI*t,t2*t));
    }
    else memdc.drawPixmap (QRect (0,0,wi,hi), ic[zw.id]);
    if(zw.id==88)//小炮
    {
        for(int s=1;s<zw.tag;s++)
            memdc.drawPixmap (QRect (s*SI/5,0,wi,hi), ic[89]);
    }
    else if(zw.id==158&&zw.grow)//磁力菇
    {
        double p=(zw.grow-1+double(zw.nxt_tm-tm)/zw.time)/zw.move_spd;
        double si=SI*p/2.0;
        memdc.drawPixmap(QRect(SI*0.55-si/2,SI*0.2-si/2,si,si),ic[162]);
    }
}

void Game::DrawZw(QPainter &memdc)
{
    QPen pe;pe.setColor(QColor(0,0,0));
    pe.setWidth(1);memdc.setPen(pe);
    memdc.setBrush(Qt::NoBrush);
    for (int i = 0; i < MM; i++)
        for (int j = 0; j < MN; j++)
            if (m_ng[i][j])memdc.drawPixmap (QRect(i*SI+50, j*SI+60,SI, SI),ico[103]);
    for (int i = 0; i < plt_cnt; i++)
        if(plants[i].type==89&&TM-plants[i].tag<=20)
            DrawGradientLine(memdc,plants[i].x+50+SI/2,plants[i].y*SI+60+SI/3,MM*SI,SI/12.0,SI/20.0,SI/12.0,Qt::white,Qt::transparent);
        else if(plants[i].type==91&&TM-plants[i].tag<=20)
            DrawGradientLine(memdc,plants[i].x+50+SI/2,plants[i].y*SI+60+SI/3,MM*SI,SI/12.0,SI/20.0,SI/12.0,DIANCO,Qt::transparent);
    for (int i = 0; i < plt_cnt; i++) {
        const Plant zw=plants[i];
        if (zw.y == -1||zw.id==-1)continue;
        int wi,hi;memdc.save();GetZwTrans(memdc,zw,hi,wi);
        DrawZwPic(memdc,zw,wi,hi,(cantwo[zw.type]&&zw.grow)?ico_red:ico);
        if(zw.type==55&&zw.tag!=-1&&zmb_ii[zw.tag]!=-1)
        {
            int t=zmb_ii[zw.tag],tx=zombies[t].x-zw.x,ty=zombies[t].y_flt-zw.y*SI;
            double bb=sqrt(tx*tx+ty*ty);
            memdc.translate(QPoint(SI/2,SI/2));
            QColor co=m_wnd->mco[203];int l=250,r=50;
            co.setAlpha((r-l)/1200.0*std::min(bb,1200.0)+l);
            DrawGradientLine(memdc,SI/4,-SI/20,bb-SI/4,SI/30.0,SI/30.0,SI/30.0,co,Qt::transparent);
        }
        memdc.restore();
        if(zw.type==71&&TM-zw.tag<=60)
        {
            QPen p(Qt::white);p.setWidth(3);
            memdc.setPen(p);
            for(int i=0;i<zmb_cnt;i++)
            {
                if(zombies[i].y!=-1&&zombies[i].type<=100&&zombies[i].ii<=zw.dztm)
                    memdc.drawLine(QPoint(zw.x+50+SI/2,zw.y*SI+60+SI/2),QPoint(zombies[i].x+50+SI/2,zombies[i].y_flt+60+SI/2));
            }
            for(int i=0;i<shi_cnt;i++)
            {
                if(shiti[i].ty==1)
                    memdc.drawLine(QPoint(zw.x+50+SI/2,zw.y*SI+60+SI/2),QPoint(shiti[i].x+50+SI/2,shiti[i].y+60+SI/2));
            }
            memdc.setPen(pe);
        }
        if (zw.mush && is_day)//蘑菇睡觉
            memdc.drawPixmap (QRect(zw.x + 50, zw.y * SI + 60,SI, SI),ico[129]);
        if(zw.stop)
            memdc.drawPixmap (QRect(zw.x + 50+SI*2/3, zw.y * SI + 60+SI*2/3,SI/3, SI/3),ico[195]);
        if(zw.type==-1)//罐子
        {
            bool zd=0;
            for(int i=-1;i<=1;i++)
            {
                for(int j=-1;j<=1;j++)
                {
                    int z=GetZwShu(zw.x+SI*i,zw.y+j);
                    if(z!=-1&&plants[z].type==11)zd=1;
                }
            }
            if(zd)//找到灯
            {
                int z=zw.attack,id;
                if(z>0)id=m_wnd->plants[z-1].id;
                else id=m_wnd->zombies[-z-1].id;
                memdc.drawPixmap (QRect(zw.x + 50+SI/4, zw.y * SI + 60+SI/4,SI/2, SI/2),ico[id]);
            }
        }
        else if(zw.type==62&&zw.tag!=-1)
        {
            for(int i=0,s=0;i<2;i++)
                for(int j=0;j<2&&s<zw.grow;j++,s++)
                    memdc.drawPixmap (QRect(zw.x + 50+i*SI/3+SI/6, zw.y * SI + 60+j*SI/3+SI/6,SI/3, SI/3),ico[m_wnd->plants[m_wnd->tyToNumP[zw.tag]].id]);
        }
        if(zw.Burning())
        {
            int h=SI*2/3+(m_wnd->isstop?SI/8:RA()%(SI/4));
            memdc.drawPixmap (QRect(zw.x + 50+SI/6, zw.y * SI + 60+SI*5/6-h,SI*2/3, h),ico[127]);
        }
        else if(zw.fire)
            memdc.drawPixmap (QRect(zw.x + 50+SI/6, zw.y * SI + 60+SI*5/6-SI/10,SI*2/3, SI/10),ico[127]);
        if(zw.frez_stp)
        {
            QColor co(0,0,255,zw.frez_stp*5+50);double h=SI/2.0;
            QLinearGradient lf(QPointF(zw.x+50+h,0),QPointF(zw.x+50,0));
            lf.setColorAt(0,co);lf.setColorAt(1,Qt::transparent);
            QLinearGradient ri(QPointF(zw.x+50+h,0),QPointF(zw.x+SI+50,0));
            ri.setColorAt(0,co);ri.setColorAt(1,Qt::transparent);
            memdc.setPen(Qt::NoPen);memdc.setBrush(lf);
            memdc.drawRect (QRectF(zw.x + 50, zw.y * SI + 60,h, SI));
            memdc.setBrush(ri);
            memdc.drawRect (QRectF(zw.x + h+50, zw.y * SI + 60,h, SI));
        }
    }
    for (int i = 0; i < plt_cnt; i++) {
        const Plant zw=plants[i];
        if(zw.y==-1)continue;
        if (zw.type == 23 && zw.tag) {//忧郁菇
            int t = (zw.lev >= 6 ? 2 : 1),ss=SI/18;
            for (int a = (TM * 4) % 30; a <= SI*1.4; a += 30) {
                if (a <= SI / 3)
                    continue;
                for (int b = -t; b <= t; b++) {
                    for (int c = -t; c <= t; c++) {
                        if (b == 0 && c == 0)continue;
                        int x = zw.x + a * b + SI / 2 + 50, y = zw.y * SI + a * c + SI / 2 + 60;
                        memdc.drawPixmap (QRect(x - ss, y - ss,ss*2, ss*2),ico[14]);
                    }
                }
            }
        }
    }
    for (int i = 0; i < MM; i++) {
        for (int j = 0; j < MN; j++) {
            if (m_ng[i][j]){ //南瓜
                if (m_ng[i][j] < 200)
                    memdc.drawPixmap (QRect(i*SI+50, j*SI+60, SI, SI),ico[178]);
                else
                    memdc.drawPixmap (QRect(i*SI+50, j*SI+60 ,SI, SI),ico[102]);
            }
            if(m_wnd->wait_guan&&m_wnd->kp_noguan[i][j])
            {
                QBrush br;
                br.setStyle (Qt::NoBrush);
                memdc.setBrush (br);
                QPen pe (QColor (0, 205, 0));
                pe.setStyle (Qt::SolidLine);
                pe.setWidth(2);
                memdc.setPen (pe);
                memdc.drawEllipse (QRect(i*SI + 50, j * SI + 60,SI, SI));
            }
        }
    }
    int dss=0;
    for (int i = 0; i < plt_cnt; i++)
        if (plants[i].y!=-1 && plants[i].type == 33)dss+=1;
    if(dss<=MN*3)
        for (int i = 0; i < plt_cnt; i++) {
            if (plants[i].y!=-1 && plants[i].type == 33) {//大蒜节点
                int x1 = plants[i].x, y1 = plants[i].y;
                for (int j = i + 1; j < plt_cnt; j++) {
                    if (plants[i].y!=-1 && plants[j].type == 33) {
                        int x2 = plants[j].x, y2 = plants[j].y;
                        memdc.setPen (QColor (255, 140, 0));
                        memdc.drawLine (QPoint(x1 + 50 + SI / 2, y1 * SI + 60 + SI / 2), QPoint(x2 + 50 + SI / 2, y2 * SI + 60 + SI / 2));
                    }
                }
            }
        }
}

QColor Game::GetLifeColor(double z)const
{
    if(z<0)z=0;
    if(z>1)z=1;
    if(z>0.5)return QColor(255*(1-z)/z,255,0);
    else return QColor(255,255*z/(1-z),0);
}

int Game::GetJsDrawY(const Zombie&js,double&aa)
{
    int t=0, z = ico_si[js.id].height();
    bool stp=((TM<js.stp_tm||TM<js.frez_stp)&&js.type != 12 && js.type != 11 && js.type !=19 && js.type < 100);
    int y= js.y * SI + t + 60 + SI - z;
    if(js.type==9||js.type==28)y-=int(js.tag)*SI/550;//巨人抬起
    else if(js.type==21)y-=js.fss * SI / 55;
    if(js.type==8&&js.sp_eat&&!stp)//跳跳
        y-=SI*(sin(TM/(js.freeze?12.0:6.0))+0.8)/6;
    if(js.type==19)y-=SI/2;//气球
    if(js.type==10&&js.fss!=-1)//小鬼投掷
    {
        int l=js.fss,r=js.fs_nd,x=js.x;
        y-=(r-l)*(r-l)/4/300;x-=(l+r)/2;
        y+=x*x/300;
    }
    else if(js.type!=19&&js.type!=8)
    {
        int t=GetZwShu((js.x+SI/2)/SI*SI,js.y);
        if(t!=-1&&plants[t].ti&&(js.x+SI/2)%SI>SI/4)
            y+=2.38*(abs((js.x+SI/2)%SI-SI*5/8)-SI*3/8);
    }
    if(js.type==102)y-=SI*2/3-(SI*2/3)*js.fss/js.fs_nd;//僵尸的窝瓜
    y+=(js.y_flt-js.y*SI);
    aa=sin(TM*0.06*js.vx+js.ii)*3;//大幅度
    if(js.freeze)aa=sin(TM*0.03*js.vx+js.ii)*2;
    if(js.eating&&js.eat>0)aa=sin(TM/3.0/(1+(js.freeze?1:0))*js.eat+js.ii)*4;
    if(stp||js.type>=100||js.type==12||js.type==11)aa=0;
    return y-(js.y_flt+60);
}

void Game::DrawJs(QPainter &memdc,const Zombie&js)
{
    int i=js.ii;double aa;
    bool stp=(TM<js.stp_tm&&js.type != 12 && js.type != 11 && js.type !=19 && js.type < 100);
    int y=js.y_flt+60+GetJsDrawY(js,aa),z=ico_si[js.id].height();;
    int tx=js.x+50+SI-z,ox=z/2,oy=z*0.8;memdc.save();
    if(iswa[js.y])
        memdc.drawPixmap (QRect(tx-z/12,y,z, z),ico[118]);
    memdc.translate(QPoint(tx+ox,y+oy));
    if(!m_wnd->low_eff)memdc.rotate(aa);
    if(js.eating&&js.eat>0&&js.vy==0&& js.type != 19 &&js.type != 9 &&js.type != 21&&js.type!=20&&js.type<100&&!stp&&TM>=js.frez_stp)
        memdc.shear(sin(TM/3.0/(1+(js.freeze?1:0))*js.eat+js.ii)*0.12,0);
    if(js.vx<0)memdc.scale(-1,1);
    int ic_ty=0;
    if(m_wnd->gqe==7)
    {
        if(js.tag<=12)ic_ty=2;
        else ic_ty=1;
    }
    else if(TM<js.frez_stp||js.freeze)ic_ty=2;
    memdc.drawPixmap (QRect(-ox,-oy,z, z),(ic_ty==0?ico:(ic_ty==1?ico_red:ico_bl))[js.id]);
    if(js.dun)
    {
        int t=GetDunIcon(js);
        if(t!=-1)
            memdc.drawPixmap (QRect(-ox,-oy,z, z),(ic_ty==0?ico:(ic_ty==1?ico_red:ico_bl))[t]);
    }
    if(js.vx<0)memdc.scale(-1,1);
    if(js.freeze)
        DrawRandomPoints(memdc,QRect(-ox+z/4,-oy,z/2, z),(js.freeze+19)/20,LBLUE,i,SI/30.0,6);
    if(js.du)
        DrawRandomPoints(memdc,QRect(-ox+z/4,-oy,z/2, z),js.du/5,DUCO,i);
    if(js.xu>1)
        DrawRandomPoints(memdc,QRect(-ox+z/4,-oy,z/2, z),(js.xu-1)*20,Qt::gray,i,SI/35.0,4);
    if(fabs(js.q)>0.01)
        DrawRandomPoints(memdc,QRect(-ox+z/4,-oy,z/2, z),int(fabs(js.q)+0.5),DIANCO,i,SI/25.0,1);
    if(m_wnd->gqe==7&&js.tag<0)//护盾
        memdc.drawPixmap (QRect(-ox,-oy,z, z),ico[61]);
    memdc.restore();
    if(TM<js.frez_stp&&js.type<100)
        memdc.drawPixmap (QRect(js.x + 50 + SI - z+SI*0.2, y+z-z/6,z*0.7, z/6),ico[72]);
    if(stp)
    {
        int s=50*SI/110,wx=js.x+50+SI-z+(SI-s)/2-s/5,wy=y-s/7;
        if(js.id==3||js.id==17||js.id==18||js.id==6||js.id==20||js.id==21||js.id==32||js.id==33||js.id==34)wx-=s/6;
        if(js.id==29||js.id==30||js.id==35)wx+=s/3;
        memdc.drawPixmap (QRect(wx, wy,s,s),ico[211]);
    }
    if (js.fei)memdc.drawPixmap (QRect(js.x + 50 + 0.2 * SI, js.y * SI + 60 - 0.5 * SI,SI * 0.5, SI * 0.5),ico[157]);
    if(js.type==20&&js.eating)//喷火
    {
        for(int x=js.x-SI/2,s=0;s<3;s++,x-=SI/2)
            memdc.drawPixmap (QRect(x+ 50, js.y * SI + 60 -SI/3,SI, SI),ico[127]);
    }
    if(iswa[js.y])
        memdc.drawPixmap (QRect(tx-z/12,y,z, z),ico[277]);
}

void Game::DrawJs(QPainter &memdc)
{
    for (int i = 0; i < zmb_cnt; i++)pub_bl[i]=0;
    for(int i=0;i<plt_cnt;i++)
        if(plants[i].y!=-1 && plants[i].type==55)pub_bl[plants[i].tag]=1;
    for (int i = 0; i < zmb_cnt; i++) {
        const Zombie js=zombies[i];
        if (js.y!=-1&&!pub_bl[i]&&!js.fei&&js.type!=15&&js.type!=17&&js.type!=18&&js.type!=26)
            DrawJs(memdc,js);
    }
    for (int i = 0; i < zmb_cnt; i++) {
        const Zombie&js=zombies[i];
        if (js.y!=-1&&!pub_bl[i]&&!(!js.fei&&js.type!=15&&js.type!=17&&js.type!=18&&js.type!=26))
            DrawJs(memdc,js);
    }
    for (int i = 0; i < zmb_cnt; i++) {
        const Zombie&js=zombies[i];
        if (js.y!=-1&&pub_bl[i])
        {
            DrawJs(memdc,js);
            memdc.drawPixmap(QRect(js.x+50+SI/3,js.y*SI+60-SI/6,SI/3,SI/3),ico[200]);
        }
    }
    if(m_wnd->show_life)
    {
        QFont f (m_wnd->font_fam, 20, QFont::Bold, false);
        f.setPixelSize(floor(20*SI/110.0+0.5));
        QPen pe;pe.setWidth(1);
        memdc.setFont (f);
        for (int i = 0; i < zmb_cnt; i++) {
            const Zombie&js=zombies[i];
            if(js.y==-1||js.type>=100)continue;
            pe.setColor(GetLifeColor(double(js.life)/js.mxlf));memdc.setPen(pe);
            int t=int(js.life/100.0+0.5);
            QString str=QString::asprintf("%d",t);
            if(js.dun>=100)str=QString::asprintf("%d|%d",js.dun/100,t);
            memdc.drawText(QPoint(js.x + 50 + 0.25 * SI, js.y * SI + 60-0.1*SI),str);
        }
    }
}

int Game::GetZdIcon(const Bullet &zd)const
{
    switch(zd.from)
    {
    case 10:
        return 139;
    case 8:
        return 140;
    case 69:
        return 233;
    case 87:
        return 146;
    case 56:
        return 211;
    case 92:
        return 13;
    }
    return 0;
}

void Game::DrawZdPic(QPainter &memdc, double x, double y, double rot, double p, int id) const
{
    memdc.save();
    memdc.translate (x, y);
    if(!m_wnd->low_eff&&fabs(rot)>1e-6)
        memdc.rotate(rot);
    int iw=ico_si[id].width()*p,ih=ico_si[id].height()*p;
    memdc.drawPixmap (QRect(-iw/2,-ih/2,iw,ih), ico[id]);
    memdc.restore();
}

void Game::DrawZd(QPainter &memdc) const
{
    for (int i = 0; i < bul_cnt; i++) {
        if (bullets[i].y == -1)continue;
        Bullet zd = bullets[i];
        QColor co=zd.color;
        if (bullets[i].vx < 0) {//魅惑
            memdc.save();memdc.translate(QPoint(zd.x + 50, zd.y * SI + 60));memdc.scale(-1,1);
            memdc.drawPixmap (QRect(-SI, 0,SI, SI),ico_red[bullets[i].tx]);
            memdc.restore();continue;
        }
        int si=zd.siz;
        if(!zd.xie&&!zd.left)zd.x += SI/7;
        if (is_tou[zd.from]) //投手
        {
            int zx=zd.x+50+SI/4,zy=int(zd.y_flt+0.5)+60+SI/3;
            if(zd.from==56&&zd.color==YUMI)
                DrawBall(memdc,zx,zy,co,si);
            else
                DrawZdPic(memdc,zx,zy,zd.x_flt*0.7,si/24.0,GetZdIcon(zd));
        }
        else if(zd.from==94)//猫尾草
            DrawZdPic(memdc,zd.x_flt+SI/2.0+50,zd.y_flt+SI/2.0+60,zd.gettheta()/PI*180,1.0,161);
        else
        {
            int hx=zd.x,hy=int(zd.y_flt-SI/10.0+0.5),si=zd.siz,fr=zd.from,lf=(SI-si)/2;
            if(zd.xie)hy=int(zd.y_flt+0.5);
            if(fr==39)//尖刺
            {
                int yy=0;if(si==1)yy=SI/8-SI/10;
                else if(si==2)yy=-SI/4.4;
                int x = (hx + lf + 50-(si==2?SI/9:0)), y = (hy + lf - SI / 3.66 + 60-yy);
                memdc.drawPixmap (QRect (QPoint (x, y), ico_si[161]), ico[161]);
            }
            else if(fr==102)//箭
                DrawZdPic(memdc,hx+50+SI/4,hy+60+SI/2,zd.gettheta()/PI*180,si/30.0,282);
            else if ((co==RED||co==DBLUE)&&fr!=60) //火球
            {
                int x = (hx + lf + 60-si), y = (hy + lf + 60-si/17);
                int id = (co==RED?150:204);
                memdc.drawPixmap (QRect (QPoint (x, y), ico_si[id]*si/90), ico[id]);
                if(zd.du)
                    DrawRandomPoints(memdc,QRect (QPoint (x, y), ico_si[id]*si/90),zd.du/2,DUCO,0);
            }
            else if(fr==22||fr==83)//星星
                DrawZdPic(memdc,hx+50+SI/2,hy+60+SI/2,TM*10,si/45.0,fr==22?114:267);
            else if (fr==13) //小喷菇
            {
                memdc.setPen (co);memdc.setBrush (co);
                DrawBall(memdc,hx + lf + 50, hy + lf + 60 + SI / 4+SI/10,co,si,0,zd.du);
            }
            else if (co == Qt::white)//粉兔
                DrawZdPic(memdc,hx+lf+50,hy+lf+60,hx*1.7,1.0,174);
            else //其他
            {
                int t=(fr==60?(zd.left+1):0);
                if(fr==90)t=3;//电
                if(zd.fire>0)co=RED;//火
                else if(zd.freeze>0)co=BLUE;//冰
                DrawBall(memdc,hx + lf + 50,hy + lf + 60,co,si,t,zd.du,zd.freeze);
            }
        }
    }
}

void Game::DrawBall(QPainter &memdc,int x,int y,QColor c,int r,int ty,int du,int hb)const
{
    if(ty==1||ty==2)//箭头（回旋镖）
    {
        QPen pe(c);pe.setWidth(4.0*SI/110.0);memdc.setPen(pe);
        if(ty==1)
        {
            memdc.drawLine(QPoint (x, y),QPoint (x+r, y+r/2));
            if(du>0)pe.setColor(YELLOW),memdc.setPen(pe);
            memdc.drawLine(QPoint (x, y+r),QPoint (x+r, y+r/2));
        }
        else
        {
            memdc.drawLine(QPoint (x+r, y),QPoint (x, y+r/2));
            if(du>0)pe.setColor(YELLOW),memdc.setPen(pe);
            memdc.drawLine(QPoint (x+r, y+r),QPoint (x, y+r/2));
        }
    }
    else
    {
        memdc.setPen (Qt::NoPen);
        if(!m_wnd->low_eff)
        {
            QColor d=c;
            QRadialGradient jbs (x + r/1.8, y + r / 2.2, (ty==3?r/1.5:r/2.0) , x + r / 3.0, y + r / 3.0);
            if(du>0)c=Qt::yellow;//毒
            else if(ty==3)c=Qt::white;//电
            else
            {
                c.setRed(std::min(c.red()+190,255));
                c.setGreen(std::min(c.green()+190,255));
                c.setBlue(std::min(c.blue()+190,255));
            }
            if(ty==3)d.setAlpha(240);
            jbs.setColorAt (0.0, c);
            jbs.setColorAt (1, d);
            memdc.setBrush (QBrush (jbs));
        }
        else memdc.setBrush(c);
        memdc.drawEllipse (QRect (x, y, r, r));
    }
    if(du>0)
        DrawRandomPoints(memdc,QRect (x-2,y-2,r+4,r+4),du/3,DUCO,0);
    if(hb>0)
        DrawXueHua(memdc,LBLUE,x+r*(0.5+sqrt(0.5)/2),y+r*(0.5+sqrt(0.5)/2),SI*sqrt(hb)/2.0/110.0,6,x*0.01);
}

void Game::DrawMw(QPainter &memdc,double aa=1)
{
    if(aa<0.001)return;
    bool notmw[MMM*2][MMN*2] = {0};
    for(int i=0;i<2*MM/3;i++)
        for(int j=0;j<MN*2;j++)notmw[i][j]=1;
    for (int i = 0; i < plt_cnt; i++)
    {
        if (plants[i].y != -1&&plants[i].type == 11)//路灯
        {
            int xx=plants[i].x/SI*2,yy=plants[i].y*2;
            for(int a=-3;a<=4;a++)
                for(int b=-3;b<=4;b++)
                {
                    int x=xx+a,y=yy+b;
                    if(x>=0&&x<MM*2&&y>=0&&y<MN*2)notmw[x][y]=true;
                }
        }
    }
    bool dif=false;
    for (int x = 0; x < MM*2; x++)
        for (int y = 0; y < MN*2; y++)
        {
            if(notmw[x][y]!=lamw[x][y])dif=true;
            lamw[x][y]=notmw[x][y];
        }
    if(dif)
    {
        lamw_pic=QPixmap(QSize((MM+1)*SI+50,(MN+1)*SI+60)*m_wnd->b_ret*zoom);
        lamw_pic.fill(Qt::transparent);
        lamw_pic.setDevicePixelRatio(m_wnd->b_ret);
        QPainter tmpdc(&lamw_pic);
        tmpdc.setPen (Qt::NoPen);
        if(m_wnd->low_mw==2)
            tmpdc.setBrush (QColor(200,200,200,250));
        for (int x = 0; x < MM*2; x++) {
            for (int y = 0; y < MN*2; y++) {
                if (!notmw[x][y])
                {
                    int sx=x*SI/2+50,sy=y*SI/2+60;
                    if(m_wnd->low_mw==2)tmpdc.drawRect(QRect(sx+1,sy+1,SI/2-2,SI/2-2));
                    else
                    {
                        unsigned int w=x*700700+y+23456;
                        for(int s=0;s<10;s++)
                        {
                            int i=sx+w%(SI*2/3+1)-SI/12;w*=3;
                            int j=sy+w%(SI*2/3+1)-SI/12;w*=5;
                            int r=w%(SI*2/3)+SI/6;w*=7;w^=(w>>1);
                            QRadialGradient jbs (QPoint(i , j), r, QPoint(i , j));
                            jbs.setColorAt (0.0, QColor(175+s*7,175+s*7,175+s*7,(40+s*10)));
                            jbs.setColorAt (1.0, QColor(240,240,240,0));
                            tmpdc.setBrush (QBrush (jbs));
                            tmpdc.drawEllipse(QRect(i-r , j-r,r+r, r+r));
                        }
                    }
                }
            }
        }
        tmpdc.end();
    }
    memdc.setOpacity(aa);
    memdc.drawPixmap(QPoint(0,0),lamw_pic);
    memdc.setOpacity(1.0);
}

void Game::DrawRandomPoints(QPainter &memdc,QRect re,int n,QColor co,int w,double si,int ty)const
{
    if(m_wnd->low_eff)return;
    if(n>1000)n=1000;
    int wi=re.width(),he=re.height(),tm=TM+1000;
    QPen pe(co);pe.setWidthF(si);
    memdc.setPen(pe);
    for(int i=0;i<n;i++)
    {
        double x=cosra[(i+w)%10000]*tm,y=sinra[(i+w)%10000]*tm;
        x-=floor(x/wi)*wi;y-=floor(y/he)*he;
        if(ty==0)//粒子
            memdc.drawPoint(re.topLeft()+QPointF(x,y));
        else//雪花
            DrawXueHua(memdc,co,re.left()+x,re.top()+y,si,ty,i+TM*0.1);
    }
}

void Game::DrawXueHua(QPainter &memdc, QColor co, double ox, double oy, double r, int s, double ad) const
{
    QPen pe(co);pe.setWidth(r*0.5);
    memdc.setPen(pe);
    for(int i=0;i<s;i++)
    {
        double t=ad+i*PI*2/s;
        memdc.drawLine(QPointF(ox,oy),QPointF(ox+r*cos(t),oy+r*sin(t)));
    }
}

void Game::DrawSt(QPainter &memdc)const
{
    memdc.setBrush(QColor(255,0,0,220));
    memdc.setPen(QColor(255,0,0,220));
    for(int i=0;i<shi_cnt;i++)
    {
        const ShiTi st=shiti[i];
        int c=TM-st.tm;
        if(st.ty==1)//僵尸黑影
        {
            if(c>=100)continue;
            double py=(10000-c*c)/10000.0,px=(10000-0.7*c*c)/10000.0;
            memdc.drawPixmap (QRect(st.x + 50+SI*(1-px)/2, st.y + 60+SI*(1-py),SI*px, SI*py),ico_blk[st.id]);
        }
        else if(st.ty==0)//僵尸倒下
        {
            if(c>=80)continue;
            if(st.id<=0||st.id>ICCNT)continue;
            int zx=(st.x+50+SI/1.8+(c<=60?SI*c/600.0:SI/10));
            int zy=(st.y+60+SI);
            double a=(c<=60?c*c/3600.0*100:100);
            memdc.translate (zx, zy);
            memdc.rotate (a);
            int si=ico[st.id].width()*(c<=60?1:1-(c-60)/20.0*0.5)/m_wnd->b_ret/zoom;
            memdc.drawPixmap (QRect(-si / 1.8, -si,si,si),ico[st.id]);
            memdc.rotate (-a);
            memdc.translate (-zx, -zy);
        }
        else if(st.ty==2)//僵尸被砸扁
        {
            if(c>50)continue;
            double py=(c<=23?(25-c)/20.0:0.1);
            memdc.drawPixmap (QRect(st.x + 50, st.y + 60+SI*(1-py),SI, SI*py),ico[st.id]);
        }
        else if(st.ty==5)//窝瓜下落
        {
            if(c>50)continue;
            double py=(c<=23?(25-c)/20.0:0.1);int s=SI;
            memdc.drawPixmap (QRect(st.x + 50, st.y + 60+s*(1-py)-s,s, s),ico[12]);
        }
    }
    for(int i=0;i<shi_cnt;i++)
    {
        const ShiTi st=shiti[i];
        int c=TM-st.tm;
        if(st.ty==3)//红点
        {
            double sq=pow(st.id/1000.0,1.0/3.0);
            int mx=int(30+50*sq+0.5);
            if(c>=mx)continue;
            if(c<=30)
            {
                double py=(5*c+c*c)/1750.0+0.4;int si=int(3*sq+0.5);
                memdc.drawEllipse(QRect(st.x+SI/2+50-si/2,st.y+py*SI+60-si/2,si,si));
            }
            else
            {
                int sx=(mx-c)/4,sy=(mx-c)/10;
                memdc.drawEllipse(QRect(st.x+SI/2+50-sx/2,st.y+SI+60-sy/2,sx,sy));
            }
        }
    }
    for(int i=0,lt=-1;i<shi_cnt;i++)
    {
        const ShiTi st=shiti[i];
        int c=TM-st.tm,t=st.id%10000;
        if(st.ty==4)//植物颗粒
        {
            if(t!=lt)
            {
                memdc.setBrush(m_wnd->main_col[t]);
                memdc.setPen(m_wnd->main_col[t]);lt=t;
            }
            double sq=pow(shiti[i].id/10000,1.0/4.0);
            int mx=int(30+50*sq+0.5);
            if(c>=mx)continue;
            if(c<=30)
            {
                double py=(5*c+c*c)/1750.0+0.4;int si=int(3*sq+0.5);
                memdc.drawEllipse(QRect(st.x+SI/2+50-si/2,st.y+py*SI+60-si/2,si,si));
            }
            else
            {
                int sx=(mx-c)/4,sy=(mx-c)/10;
                memdc.drawEllipse(QRect(st.x+SI/2+50-sx/2,st.y+SI+60-sy/2,sx,sy));
            }
        }
    }
    QFont f (m_wnd->font_fam, 25, QFont::Bold, false);
    f.setPixelSize(25);memdc.setFont(f);
    for(int i=0;i<shi_cnt;i++)
    {
        const ShiTi st=shiti[i];
        if(st.ty==6)//寒冰雪花
        {
            int mx=st.id,c=TM-st.tm;
            if(c>=mx)continue;
            double p=acos(-1.0)/3.0;
            memdc.setBrush(QColor(0,255,255,255*(mx-c)/mx));
            memdc.setPen(QColor(0,255,255,255*(mx-c)/mx));
            int le=c*2;
            for(int s=0;s<6;s++)
            {
                QPoint pt[3];
                pt[0]=QPoint(st.x+SI/2+50+le*cos(s*p),st.y+SI/4+60+le*sin(s*p));
                pt[1]=QPoint(st.x+SI/2+50+(le+6)*cos(s*p+p/15),st.y+SI/4+60+(le+6)*sin(s*p+p/15));
                pt[2]=QPoint(st.x+SI/2+50+(le+6)*cos(s*p-p/15),st.y+SI/4+60+(le+6)*sin(s*p-p/15));
                memdc.drawConvexPolygon(pt,3);
            }
        }
        else if(st.ty==7||st.ty==8)//僵尸火焰
        {
            int t=zmb_ii[st.id];
            if(t==-1)continue;
            int x=zombies[t].x,y=zombies[t].y_flt,c=TM-st.tm;
            if(c>=30)continue;
            int wi=(30-c)*SI*0.8/30,hi=(30-c)*SI*0.65/30;
            memdc.drawPixmap(QRect(x+50+(SI-wi)/2,y+60+SI/2-hi,wi,hi),ico[st.ty==7?127:180]);
        }
        else if(st.ty==9)//爆炸冲击波
        {
            int mx=(st.id/1000)%1000,c=TM-st.tm,sd=st.id/1000000;
            if(c>=mx)continue;
            QColor co=m_wnd->main_col[st.id%1000];
            int r=c*sd,a=200-c*200/mx;co.setAlpha(a);
            QRadialGradient ra(QPoint(st.x+50,st.y*SI+SI/2+60),r,QPoint(st.x+50,st.y*SI+SI/2+60));
            ra.setColorAt(0,co);ra.setColorAt(1.0,Qt::transparent);
            memdc.setBrush(ra);memdc.setPen(Qt::NoPen);
            memdc.drawEllipse(QRect(st.x+50-r,st.y*SI+SI/2+60-r,r+r,r+r));
        }
        else if(st.ty==10)//伤害数字
        {
            int c=TM-st.tm,z=st.id/200,o=m_wnd->tyToNumP[st.id%200];
            if(c>=60)continue;
            QColor co=m_wnd->plants[o].color;co.setAlpha((60-c)*255/60);
            memdc.setPen(co);
            memdc.drawText(QRect(st.x+50-SI,st.y+60-c*SI/80,SI*3,SI),Qt::AlignCenter,QString::asprintf("%d",z));
        }
        else if(st.ty==11&&TM<st.tm)//图片
        {
            int h=SI;
            if(st.id<0)h=SI*(double(RA())/RA.max()*0.2+0.9);
            memdc.drawPixmap(QRect(st.x+50,st.y+60+SI-h,SI,h),ico[abs(st.id)]);
        }
        else if(st.ty==13&&TM<=st.tm+20)
        {
            int a=zmb_ii[st.x],b=zmb_ii[st.y];
            if(a==-1||b==-1)continue;
            double x1=zombies[a].x_flt+SI/2.0+50,y1=zombies[a].y_flt+SI/2.0+60;
            double x2=zombies[b].x_flt+SI/2.0+50,y2=zombies[b].y_flt+SI/2.0+60;
            QColor co=DIANCO;co.setAlpha(std::min(int(st.id/100.0*255),255));
            QPen pe(co);pe.setWidthF(st.id/50.0);
            memdc.setPen(pe);
            memdc.drawLine(QPointF(x1,y1),QPointF(x2,y2));
        }
    }
    f.setPixelSize(40);memdc.setFont(f);
    for(int i=0;i<shi_cnt;i++)
    {
        const ShiTi st=shiti[i];
        if(st.ty==12)//文字
        {
            int c=TM-st.tm;
            if(c>=100)continue;
            QString txt[2]={"一大波僵尸即将来袭！","最后一波！"};
            QColor co=Qt::red;co.setAlpha((100-c)*255/100);
            memdc.setPen(co);memdc.drawText(QRect(st.x-1000,st.y-1000,2000,2000),Qt::AlignCenter,txt[st.id]);
        }
    }
}
double Game::GetFogAlpha()const
{
    double aa=0;
    if (is_fog)aa=1;
    if(is_cycle)aa=(cos(TM/300.0)+0.9)/1.9;
    int t=TM-lstfan;
    if(t<40)aa=lmw*(40-t)/40.0;
    else if(t<400)aa=0;
    else if(t<1200)aa*=(t-400)/800.0;
    return aa;
}
void Game::DrawZwLife(QPainter&memdc)
{
    QPen pe;pe.setWidth(1);
    for (int i = 0; i < MM; i++) {
        for(int j=0;j<MN;j++)
        {
            int w=GetZwShu(i*SI,j);
            if (w==-1||plants[w].id==-1)continue;
            pe.setColor(QColor(0,0,0));memdc.setPen(pe);memdc.setBrush(Qt::NoBrush);
            double z=std::min(1.0,double(plants[w].life)/plants[w].mxlf);
            memdc.drawRoundedRect(QRect(i*SI+50+SI/6,j*SI+60+SI*0.9,SI*2/3,SI/10),SI/15,SI/20);
            QColor co=GetLifeColor(z);
            memdc.setBrush(co);pe.setColor(co);memdc.setPen(pe);
            memdc.drawRoundedRect(QRect(i*SI+51+SI/6,j*SI+61+SI*0.9,(SI*2/3-2)*z,SI/10-2),(SI/15-1),(SI/20-1));
        }
    }
    QFont f (m_wnd->font_fam, 18, QFont::Bold, false);
    f.setPixelSize(floor(18.0*SI/110+0.5));
    pe.setWidth(1);memdc.setFont (f);
    for(int i=0;i<plt_cnt;i++)
    {
        const Plant zw=plants[i];
        if(zw.y==-1||zw.id==-1)continue;
        QString str=QString::asprintf("%.1f",zw.life);
        if(zw.life>=1e7)str="∞";
        if(zw.grow)str+=QString::asprintf(",%d",zw.grow);
        if(zw.fire)str+=QString::asprintf(",%d",zw.fire);
        QColor co=m_wnd->main_col[zw.type];
        co.setRed(co.red()<128?255:0);
        co.setGreen(co.green()<128?255:0);
        co.setBlue(co.blue()<128?255:0);
        pe.setColor(co);memdc.setPen(pe);
        memdc.drawText(QRect(zw.x + 50, zw.y * SI + 60,SI,SI),Qt::AlignCenter,str);
    }
    memdc.setPen(Qt::NoPen);
    for(int i=0;i<zmb_cnt;i++)
    {
        if(zombies[i].y==-1||zombies[i].type>=100)continue;
        memdc.setBrush(QColor(0,0,0,30));
        memdc.drawEllipse(QRect(zombies[i].x+50+SI/2-4,zombies[i].y*SI+60+SI-4,8,8));
    }
}
void Game::DrawShadow(QPainter &memdc,double tt)
{
    for(int i=0;i<plt_cnt;i++)
    {
        if(plants[i].y==-1)continue;
        memdc.save();
        memdc.translate(QPoint(plants[i].x+50,plants[i].y*SI+60+SI));
        memdc.shear(2.5*(0.5-tt),0);memdc.scale(1,0.4+fabs(0.5-tt)/2.0);
        memdc.translate(-QPoint(plants[i].x+50,plants[i].y*SI+60+SI));
        int wi,hi;GetZwTrans(memdc,plants[i],hi,wi);
        QPoint p1=QPoint(plants[i].x+50,plants[i].y*SI+60),p2=QPoint(plants[i].x+50+wi,plants[i].y*SI+60+hi);
        hi=p2.y()-p1.y(),wi=p2.x()-p1.x();
        DrawZwPic(memdc,plants[i],wi,hi,ico_sha);
        memdc.restore();
    }
    for(int i=0;i<zmb_cnt;i++)
    {
        if(zombies[i].y==-1)continue;
        double aa;
        int yy=GetJsDrawY(zombies[i],aa),y=zombies[i].y_flt+60+yy,z=ico_si[zombies[i].id].height();
        int tx=zombies[i].x+50+SI-z,ox=z/2,oy=z*0.8;memdc.save();
        memdc.translate(QPoint(tx,SI+y-yy));
        memdc.shear(2.5*(0.5-tt),0);memdc.scale(1,0.4+fabs(0.5-tt)/2.0);
        memdc.translate(QPoint(ox,-SI+oy));memdc.rotate(aa);
        if(zombies[i].vx<0)memdc.scale(-1,1);
        memdc.drawPixmap (QRect(-ox,yy-oy,z, z),ico_sha[zombies[i].id]);
        if(zombies[i].dun)
        {
            int t=GetDunIcon(zombies[i]);
            if(t!=-1)
                memdc.drawPixmap (QRect(-ox,yy-oy,z, z),ico_sha[t]);
        }
        memdc.restore();
    }
}

void Game::DrawGradientLine(QPainter &memdc, double x, double y, double wi, double h1, double h2, double h3, QColor co1, QColor co2)const
{
    memdc.setPen(Qt::NoPen);
    QLinearGradient lf(QPointF(0,y+h1),QPointF(0,y));
    lf.setColorAt(0,co1);lf.setColorAt(1,co2);
    QLinearGradient ri(QPointF(0,y+h1+h2),QPointF(0,y+h1+h2+h3));
    ri.setColorAt(0,co1);ri.setColorAt(1,co2);
    memdc.setBrush(lf);
    memdc.drawRect (QRectF(x, y,wi, h1));
    memdc.setBrush(ri);
    memdc.drawRect (QRectF(x, y+h1+h2,wi, h3));
    memdc.setBrush(co1);
    memdc.drawRect (QRectF(x, y+h1,wi, h2));
}

void Game::Draw (QPainter &memdc,bool an)
{
    if(m_wnd->fir_init)return;
    memdc.scale(zoom,zoom);
    memdc.translate(0,TOP);
    int xs=(W1-50)/(1-XW-XT),ys=(HI-60)/(1-YW-YT);
    int xl=50-xs*XW+W0,yl=60-ys*YW;
    memdc.drawPixmap (QRect(xl,yl,xs,ys),ico[111]);
    memdc.drawPixmap (QRect(xl+xs,yl,ys*1536/1345,ys),ico[125]);
    m_wnd->DrawLeft(memdc);
    memdc.translate(W0,0);
    if (started) {
        QPen pe (Qt::green);pe.setWidth(1);
        pe.setStyle (Qt::SolidLine);memdc.setBrush(QColor(20,20,10,70));
        memdc.setPen(Qt::NoPen);
        if(m_wnd->bgid>=2&&(MN!=5||MM!=9))
        {
            for(int x=0;x<MM;x++)
                for(int y=0;y<MN;y++)
                    memdc.drawPixmap(QRect(x*SI+50,y*SI+60,SI,SI),ico[113-(x+y)%2]);
        }
        else if(m_wnd->bgid<2)
        {
            for(int x=0;x<MM;x++)
                for(int y=0;y<MN;y++)
                    if((x+y)%2==0)
                        memdc.drawRect(QRect(x*SI+50,y*SI+60,SI,SI));
        }
        if(!m_wnd->can_wsad&&m_wnd->ws_le)
            for(int i=0;i<MN;i++)
                memdc.drawPixmap(QRect(50,i*SI+60,SI,SI),ico[247]);
        double tt=(TM * 25.0) / m_wnd->m_ztm;
        if(is_cycle||m_wnd->noztm)tt=(TM%1200)/1200.0;
        if(m_wnd->is_boss)tt=double(jd)/zjd;
        if(tt>1)tt=1;
        memdc.setPen(Qt::NoPen);
        for (int x = 0; x < MM; x++)
            for (int y = 0; y < MN; y++)
            {
                if (IsKeng(x,y))
                    memdc.drawPixmap(QRect(x*SI+50,y*SI+60,SI,SI),ico[59]);
                if (IsIcy(x,y))
                    DrawGradientLine(memdc,x*SI+50,y*SI+60,SI,SI/3.0,SI/3.0,SI/3.0,Qt::white,QColor(255,255,255,50));
            }
        if(!m_wnd->low_eff)DrawShadow(memdc,tt);
        if (m_wnd->m_b48 || m_wnd->imjs) {
            pe.setColor (QColor (255, 0, 0));
            memdc.setPen (pe);
            memdc.drawLine (QPoint(MN * SI + 50, 60), QPoint(MN * SI + 50, MN * SI + 60));
        }
        else if(m_wnd->b_guan)
        {
            pe.setColor (QColor (0, 150, 0));
            pe.setWidth(2);
            memdc.setPen (pe);
            memdc.drawLine (QPoint(SI + 50, 60), QPoint(SI + 50, MN * SI + 60));
            pe.setWidth(1);
        }
        if(m_wnd->fr_mid)
        {
            pe.setColor (QColor (255, 0, 0));
            memdc.setPen (pe);
            memdc.drawLine (QPoint(50+(MM-3)*SI, 60), QPoint(50+(MM-3)*SI, MN * SI + 60));
        }
        pe.setColor (QColor(139,69,19));
        pe.setWidthF(SI/25.0);
        for (int i = 0; i < MM; i++) {//绘制水池
            for (int j = 0; j < MN; j++) {
                if(grid_type[i][j]==1)
                {
                    int a=i * SI + 50, b=j * SI + 60;
                    DrawGradientLine(memdc,a,b,SI,SI*0.4,SI*0.2,SI*0.4,
                                     QColor(0,181+10*sin(TM*0.02),255,170+20*sin(TM*0.023)),
                                     QColor(0,171+15*sin(TM*0.029),255,70+30*sin(TM*0.037)));
                    if(m_hy[i][j]>0)
                        memdc.drawPixmap (QRect(a, b+SI*0.3, SI, SI),ico[62]);
                    int ne[4][2]={1,0,0,1,-1,0,0,-1},fx[4][2]={1,0,1,1,0,1,0,0};
                    for(int s=0;s<4;s++)
                    {
                        int ti=i+ne[s][0],tj=j+ne[s][1];
                        if(ti<0||ti>=MM||tj<0||tj>=MN||grid_type[ti][tj]!=1)
                        {
                            memdc.setPen (pe);
                            memdc.drawLine(a+fx[s][0]*SI,b+fx[s][1]*SI,a+fx[(s+1)%4][0]*SI,b+fx[(s+1)%4][1]*SI);
                        }
                    }
                }
            }
        }
        pe.setWidth(3);pe.setColor(QColor(150,0,200));
        memdc.setPen(pe);memdc.setBrush(Qt::NoBrush);
        for(int x=0;x<MM;x++)
            for(int y=0;y<MN;y++)
                if(spd_mul[x][y]>1.01)
                    memdc.drawRect(QRect(x*SI+53,y*SI+63,SI-6,SI-6));
        pe.setWidth(1);
        for (int n = 0; n < MN; n++)
            if (cars[n])
                memdc.drawPixmap (QRect(0, n * SI + 60,SI, SI),ico[28]);
        //if (!m_wnd->is_boss)
        //    memdc.drawPixmap (QRect(40 + tt* 700, -50, 55, 55),ico[is_day?122:123]);
        DrawZw(memdc);DrawJs(memdc);DrawZd(memdc);DrawSt(memdc);
        for(int a=0;a<MM;a++)
        {
            for(int b=0;b<MN;b++)
            {
                if (wz_fei[a][b]) {//绘制肥料
                    int f=wz_fei[a][b],r=SI*0.4;
                    for(int s=0;s<f;s++)
                    {
                        double t=PI*0.4*s;
                        int x=a*SI+50+r*sin(t)+SI/2,y=b*SI+60-r*cos(t)+SI*0.55;
                        memdc.drawPixmap (QRect(x-SI*0.15, y-SI*0.15,SI * 0.3, SI * 0.3),ico[157]);
                    }
                }
            }
        }
        if(m_wnd->show_life)DrawZwLife(memdc);
        memdc.setPen (QColor (0, 0, 0, 240));
        memdc.save ();
        QFont f (m_wnd->font_fam, 35 ,QFont::Bold, false);
        f.setPixelSize(42);memdc.setFont (f);
        QString str=QString::asprintf ("阳光:%d", m_wnd->m_game.m_yg);
        if(m_wnd->inf_sun)str="";
        if(m_wnd->duiz)str+=QString::asprintf ("  经验:%d", m_wnd->exp/100);
        if(m_wnd->show_life)str+=QString::asprintf ("  DPS:%.2lf", m_wnd->dps_now);
        if (str.length()>0)
        {
            memdc.setPen(TM-laygTM<=20?Qt::yellow:Qt::black);
            memdc.drawText (QPoint(55, 15), str);
        }
        if(!m_wnd->show_life&&!m_wnd->duiz)
        {
            memdc.setPen(TM-m_wnd->labeanTM<=50?Qt::green:Qt::black);
            memdc.drawText (QPoint(350, 15), QString::asprintf ("能量豆:%d", m_wnd->beans));
        }
        memdc.restore ();
        if (m_wnd->sele_x != -1) {//绘制选择的黑圈
            int x = m_wnd->sele_x, y = m_wnd->sele_y;
            QBrush br;
            br.setStyle (Qt::NoBrush);
            memdc.setBrush (br);
            QPen pe (QColor (0, 0, 0));
            pe.setWidth(1);
            pe.setStyle (Qt::SolidLine);
            memdc.setPen (pe);
            memdc.drawEllipse (QRect(x + 50, y * SI + 60,SI, SI));
        }
        if(m_wnd->chui&&TM<10)ico[131]=ico[60];
        int sel=m_wnd->select;
        if (sel != -1) {
            QPoint tcur = m_wnd->mapFromGlobal(m_wnd->cursor ().pos ());
            QPoint cur = m_wnd->SPoint (tcur.x (), tcur.y ());
            QPixmap *im=NULL;
            if (sel == -2)im=&ico[131];
            else if (sel == -3)im=&ico[136];
            else if (sel == -4)im=&ico[78];
            else if (m_wnd->imjs||m_wnd->plajs)im=&ico[m_wnd->zombies[sel].id];
            else im=&ico[m_wnd->plants[sel].id];
            if (cur.x () >= (W0+50) && cur.y () >= 60) {
                int x = cur.x () - (W0+50), y = (cur.y () - 60) / SI;
                if(sel!=-3&&!(sel==-2&&m_wnd->chui))x=(x/SI)*SI;
                else x-=SI/2;
                memdc.drawPixmap (QRect(x+ 50 + SI / 6, y* SI + 60 + SI / 6, SI * 2 / 3, SI * 2 / 3), *im);
                int ty=m_wnd->plants[sel].type,R=0;
                if(is_zha[ty]&&!(ty==5||ty==7||ty==99))R=1;
                else if(ty==15)R=2;
                if(R>0)//绘制炸弹范围
                {
                    memdc.setPen(Qt::NoPen);
                    for(int i=0;i<zmb_cnt;i++)
                    {
                        if(zombies[i].y==-1||zombies[i].type>=100)continue;
                        bool in=(abs(zombies[i].y-y)<=R&&abs(zombies[i].x-x)<=(R+FW)*SI);
                        if(in)memdc.setBrush(Qt::red);
                        else memdc.setBrush(Qt::green);
                        memdc.drawEllipse(QRect(zombies[i].x+50+SI/2-3,zombies[i].y*SI+60+SI-3,6,6));
                    }
                }
            }
#ifdef Q_OS_ANDROID
            memdc.drawPixmap (QRect(400, 0,60, 60)), *im);
#endif
        }
        DrawMw(memdc,GetFogAlpha());
        for(int x=0;x<MM;x++)
            for(int y=0;y<MN;y++)
            {
                if(js_nd[x][y]>=0)memdc.drawPixmap(QRect(x*SI+50+SI/4,y*SI+60+SI/4,SI/2,SI/2),ico[m_wnd->zombies[m_wnd->tyToNumZ[js_nd[x][y]]].id]);
                else if(js_nd[x][y]==-2)memdc.drawPixmap(QRect(x*SI+50+SI/4,y*SI+60+SI/4,SI/2,SI/2),ico[200]);
                else if(js_nd[x][y]==-3)memdc.drawPixmap(QRect(x*SI+50+SI/4,y*SI+60+SI/4,SI/2,SI/2),ico[201]);
            }
        if(zjd>0)
        {
            int xx=W1-300,yy=-23,hi=45,wi=240;
            QPen p(Qt::red);p.setWidth(1);memdc.setPen(p);
            for(int h:vdb)
            {
                int x=double(h)/zjd*wi+0.5;
                memdc.drawLine(QPoint(xx+1+x,yy+2),QPoint(xx+1+x,yy+hi-2));
            }
            memdc.setBrush(Qt::NoBrush);
            p.setWidth(2);p.setColor(Qt::black);memdc.setPen(p);
            memdc.drawRoundedRect(QRect(xx,yy,wi+2,hi),10,10);
            if(jd>0)
            {
                memdc.setPen(Qt::NoPen);memdc.setBrush(QColor(0,255,0,200));
                memdc.drawRoundedRect(QRect(xx+1,yy+1,std::min(1.0,double(jd)/zjd)*wi+0.5,hi-2),10,10);
            }
            if(m_wnd->dbn>0&&TM%40<20)
            {
                p.setColor(Qt::red);memdc.setPen(p);
                int x=double(vdb[m_wnd->dbn-1])/zjd*wi+0.5;
                memdc.drawLine(QPoint(xx+1+x,yy-10),QPoint(xx+1+x,yy+hi-2));
            }
        }
        memdc.drawPixmap(QRect(1000,-25,42,42),ico[131]);
    }
}

//坐标转换
QPoint MainWindow::SPoint (int x, int y) {
    y -= ME;
    x = floor (x / zoom+0.5);
    y = floor (y / zoom+0.5)-TOP;
    return QPoint (x, y);
}

void MyPixmap::paint () {
#ifdef NO_UI
    return;
#endif
    pix=QPixmap(size()*m_wnd->b_ret);
    pix.setDevicePixelRatio(m_wnd->b_ret);
    QPainter memdc (&pix);
    if (m_wnd->b_kjc)
    {
        memdc.setRenderHint(QPainter::Antialiasing);
        memdc.setRenderHint(QPainter::SmoothPixmapTransform);
    }
    if(ty==1)m_wnd->m_game.Draw (memdc);
    else if(ty==2&&m_wnd->an_game.zmb_cnt>=0)m_wnd->an_game.Draw(memdc,1);
    memdc.end();
}

double Game::GetNightAlpha()
{
    int zz=60;
    if(is_cycle)zz=sin((TM%1200)*acos(-1.0)/1200.0)*60+20;
    if(is_cycle&&is_day)
    {
        int tt=TM%1200;tt=std::min(tt,1200-tt);
        if(tt<=200)zz=(200-tt)/10;
        else zz=0;
    }
    return zz;
}

void MainWindow::DrawLeft(QPainter &memdc) const
{
    memdc.translate(0,-50);
    QFont f (font_fam, 12, QFont::Bold, false);
    f.setPixelSize(16);
    memdc.setFont (f);
    memdc.setPen (QPen (Qt::NoPen));
    memdc.setBrush (LEFTCO);
    memdc.drawRect (QRect(0,0,W0,HI));
    if(!m_game.started)return;
    int ms=std::min(XUAN,MXUAN);
    for(int i=0;i<ms;i++)//绘制左框
    {
        int n=(i+xuan_st)%XUAN,w=m_xuan[n];
        if (!imjs &&!plajs&& w == -1)
            break;
        //memdc.drawRect (QRect(0, i * 50,100, 50));
        if (imjs||plajs)
            memdc.drawPixmap (QRect(20, i * 50,50, 50),ico[zombies[x_js[n]].id]);
        else
            memdc.drawPixmap (QRect(20, i * 50,50, 50),ico[plants[w].id]);
        if ((!is_boss || is12)&&!b_guan) {
            if (!imjs&&!plajs) {
                memdc.setBrush (QColor (0, 0, 0, 150));
                if(cd[w]<800)
                    memdc.drawRect (QRectF(0,i*50,(800-cd[w])/800.0*(W0-3)+3,50));
            }
            memdc.setPen (QColor (0, 240, 0));
            int yy;
            if (imjs||plajs)
                yy=j_yg[x_js[n]];
            else if (w == 38)
                yy=fabs (cd[38]) < eps ? 0 : z_fs + 1;
            else
                yy=plants[w].cost;
            QColor co=QColor(255,0,0);
            if(!imjs&&!plajs&&cd[w]<800-1e-9)
                co=Qt::black;
            else if(w == 38||(m_game.m_yg>=yy&&(plajs||exp>=expnd[w])))
                co=Qt::green;
            memdc.setPen (co);
            QString str=QString::asprintf ("%d", yy);
            memdc.drawText (QPoint (76, i * 50 + 30), str);
            //if(!imjs&&!plajs&&expnd[w])
            //    memdc.drawText (QPoint (72, i * 50 + 42), QString::asprintf ("%d", expnd[w]/100));
        }
        memdc.setPen (Qt::NoPen);
        memdc.setBrush (LEFTCO);
    }
    f.setPixelSize(14);
    memdc.setFont (f);
    memdc.setPen (QColor (255, 255, 0));
    for(int i=0;i<ms;i++)//绘制左框
    {
        int n=(i+xuan_st)%XUAN,w=m_xuan[n];
        if (!imjs &&!plajs&& w == -1)
            break;
        QString nu;
        if (i == 10)
            nu = "> -";
        else if(i==11)
            nu="> =";
        else
            nu = QString::asprintf ("> %d", (i + 1) % 10);
#ifndef Q_OS_ANDROID
        memdc.drawText (QPoint (0, i * 50 + 28), nu);
#endif  
    }
    memdc.translate(0,50);
    memdc.drawPixmap (QRect(58, HI-45,33, 33),ico[78]);
    memdc.setPen (QColor (0, 200, 0));
    memdc.drawText (QPoint (75, HI-4), QString::asprintf("×%d",beans));
    memdc.drawPixmap (QRect(-3, HI-43,33, 33),ico[131]);
    memdc.drawPixmap (QRect(25, HI-43,33, 33),ico[184]);
    if (m_b48) {
        memdc.setBrush (QColor (0, 240, 0));
        int t = TM - l48;
        if (t > 30)
            t = 30;
        memdc.drawRect (QRectF(0, XUAN * 50,int ((30 - t) / 30.0 * 100.0), 50));
    }
}
