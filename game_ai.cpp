#include "game.h"
#include "mainwindow.h"
//游戏的AI
int Game::Find (int zz[20], int n, int hg)const //找到最优的位置(AI)
{
    for (int i = 0; i < n; i++) {
        if (zz[i] == -1)
            return i;
    }
    int zx = 1e9, wz = -1;
    for (int i = 0; i < n; i++) {
        if(plants[zz[i]].type>=45)continue;
        int t = plants[zz[i]].cost;
        if (hg >= 10 && (plants[zz[i]].type == 0 || plants[zz[i]].type == 20) && XS <= (22 + (m_wnd->gqe==0 ? 12 : 0)) * pow (110.0 / SI, 1.4) && rand () % (XS <= 16 * pow (110.0 / SI, 1.4) ? 24 : 25) < 24) //要保留一些葵
            t = 500;
        else if (plants[zz[i]].type == 6 && plants[zz[i]].life < 200)
            t = 25;
        if (t < zx)
            zx = t, wz = i;
    }
    return wz;
}
int Game::Find (int zz[20], int n)const //找到最优的位置(AI)
{
    for (int i = 0; i < n; i++) {
        if (zz[i] == -1)
            return i;
    }
    int zx = 1e9, wz = -1;
    for (int i = 0; i < n; i++) {
        if(plants[zz[i]].type>=45)continue;
        int t = plants[zz[i]].cost;
        if ((plants[zz[i]].type == 0 || plants[zz[i]].type == 20) && XS <= (22 + (m_wnd->gqe==0 ? 12 : 0)) * pow (110.0 / SI, 1.4) && rand () % (XS <= 16 * pow (110.0 / SI, 1.4) ? 24 : 25) < 24) //要保留一些葵
            t = 500;
        else if (plants[zz[i]].type == 6 && plants[zz[i]].life < 200)
            t = 25;
        if (t < zx)
            zx = t, wz = i;
    }
    return wz;
}
int Game::Valid (int x, int sy = 0)const //植物可行(AI)
{
    int xu[21] = {0, 10, 35, 8, 61, 44, 3, 5, 12, 60, 25,4,6,1,53,7,2,50,57,36,58}, z = xu[x];
    //int xu[19] = {0, 10, 2, 8, 9, 44, 3, 5, 12, 41, 7,4,6,0,53,7,2,50,57}, z = xu[x];
    if(!is_day)xu[0]=xu[8]=17,z = xu[x];
    if (m_wnd->cd[z] > 800 - eps && m_wnd->plants[z].cost + sy <= m_yg)
        return z;
    return -1;
}
int Game::FakeKill (int xl, int xr, int y, int kill) {
    if (y < 0 || y >= 5)
        return 0;
    int jg = 0, tx = 770;
    if (GetZwShu (tx, y) == -1)
        tx = 660;
    for (int i = 0; i < zmb_cnt; i++) {
        if (zombies[i].y==-1 || zombies[i].life < 0 || zombies[i].type >= 100 || (zombies[i].type <= 5 && jd > 100) || ((zombies[i].eat >= 200 || (zombies[i].eat <= 2 && zombies[i].type != 16)) && zombies[i].x > tx + 90) || (zombies[i].eat == 40 && zombies[i].x > tx + 100))
            continue;
        if (zombies[i].y == y && zombies[i].x >= xl && zombies[i].x <= xr && zombies[i].type < 100) {
            int ad = zombies[i].life;ad/=1000;
            if (zombies[i].eat == 1 && zombies[i].x > 700)
                ad = ad * 2 / 3;
            if (zombies[i].life > 150 && zombies[i].x > 640)
                ad -= 60; //不要立刻炸死巨人
            if (zombies[i].type == 14 || zombies[i].type == 15)
                ad += 25;
            if (zombies[i].type == 15)
                ad += 50;
            if (zombies[i].type == 13)
                ad += 10;
            if (zombies[i].type == 16)
                ad += 70;
            if (zombies[i].eat >= 4)
                ad += 20;
            if (zombies[i].eat > 4)
                ad += 20;
            jg += ad;
        }
    }
    return jg;
}
bool Game::NearGaint (int x, int y)const {
    for (int i = 0; i < zmb_cnt; i++) {
        if (zombies[i].type == 9 && zombies[i].y == y && zombies[i].x >= x - 30 && zombies[i].x <= x + (zombies[i].life <= 50 ? 140 : 200)) //不能在巨人的面前种植
            return true;
    }
    return false;
}
bool Game::UnderGaint (int x, int y)const {
    for (int i = 0; i < zmb_cnt; i++) {
        if (zombies[i].type == 9 && zombies[i].y == y && zombies[i].x >= x - 30) //不能在巨人的面前种植
            return true;
    }
    return false;
}
bool Game::NearCGT (int x, int y)const {
    for (int i = 0; i < zmb_cnt; i++) {
        if (zombies[i].type == 5 && zombies[i].y == y && zombies[i].x >= x - 30) //不能在巨人的面前种植
            return true;
    }
    return false;
}
bool Game::NearQi (int x, int y)const {
    for (int i = 0; i < zmb_cnt; i++) {
        if (zombies[i].type ==18 && zombies[i].y == y && zombies[i].x >= x + 80&& zombies[i].x <= x + 90) //不能在巨人的面前种植
            return true;
    }
    return false;
}
bool Game::NearCre (int x, int y)const {
    for (int i = 0; i < zmb_cnt; i++) {
        if (zombies[i].type ==15 && zombies[i].y == y && zombies[i].x >= x + 80&& zombies[i].x <= x + 86) //不能在巨人的面前种植
            return true;
    }
    return false;
}
bool Game::NearCar (int x, int y)const {
    for (int i = 0; i < zmb_cnt; i++) {
        if (zombies[i].type==12 && zombies[i].y == y && zombies[i].x >= x - 30 && zombies[i].x <= x + 450) //不能在车的面前种植
            return true;
    }
    return false;
}
bool Game::UnderCar (int x, int y)const {
    for (int i = 0; i < zmb_cnt; i++) {
        if (zombies[i].type==12 && zombies[i].y == y && zombies[i].x >= x - 30) //不能在车的面前种植
            return true;
    }
    return false;
}
bool Game::BeforeShe (int x, int y) {//是否在射手前面（AI）
    int mi = 10000;
    for (int i = 0; i < zmb_cnt; i++) {
        if ((zombies[i].type == 8 || zombies[i].type == 10|| zombies[i].type == 22) && zombies[i].y == y && zombies[i].x >= x - 10 && zombies[i].x < mi) //不能在射手的面前种植
            mi = zombies[i].x;
    }
    if (mi == 10000)
        return false;
    for (int i = x; i <= mi; i += SI) {
        int z = GetZwShu (i, y);
        if (z != -1 && plants[z].life >= 150) //有坚果挡着
            return false;
    }
    return true;
}
int Game::FakeKillL (int y, int kill)const //计算一行的价值
{
    int jg = 0;
    for (int i = 0; i < zmb_cnt; i++) {
        if (zombies[i].y == y && zombies[i].type < 100 && (zombies[i].type > 5 || jd <= 100) && (zombies[i].eat < 40 || zombies[i].x < 750)) {
            int ad = zombies[i].life;
            if (zombies[i].eat == 1 && zombies[i].x > 700)
                ad = ad * 3 / 4;
            if (zombies[i].life > 150 && zombies[i].x > 640)
                ad -= 60; //不要立刻炸死巨人
            if (zombies[i].type == 14 || zombies[i].type == 15)
                ad += 25;
            if (zombies[i].type == 15)
                ad += 50;
            if (zombies[i].type == 13)
                ad += 10;
            if (zombies[i].type == 16)
                ad += 80;
            if (zombies[i].eat >= 4)
                ad += 15;
            if (zombies[i].eat > 4)
                ad += 20;
            jg += ad;
        }
    }
    return jg;
}
bool Game::UnderMonth (int x, int y)const {
    for (int i = 0; i < zmb_cnt; i++) {
        if (zombies[i].y == y && zombies[i].x <= x + (90 + (zombies[i].type >= 100 ? 30 : 0)) && zombies[i].x >= x)
            return true;
    }
    return false;
}
double Game::wsad_cal(int sz[5][9])
{
    double su[5][9]={0};
    for(int y=0;y<5;y++)
    {
        for(int x=0;x<9;x++)
        {
            if(sz[x][y]!=-1)
            {
                int t=m_wnd->tyToNumP[sz[x][y]];
                if(m_wnd->plants[t].attack>0)
                    su[y][x]=double(m_wnd->plants[t].attack)/m_wnd->plants[t].time;
            }
        }
        for(int x=1;x<9;x++)
            su[y][x]+=su[y][x-1];
    }
    double jg=0;
    for(int i=0;i<zmb_cnt;i++)
    {
        Zombie js=zombies[i];
        if(js.type>=100)continue;
        int y=js.y,x=js.x/SI;
        if(x>8)x=8;if(x<0)x=0;
        double z=su[y][x],tx;
        if(fabs(z)<eps)tx=-SI*2;
        else tx=js.x-js.life/z/50*js.vx/2.0;
        if(tx<-SI)tx=-SI;
        tx=885-tx;jg+=tx*tx;
    }
    return jg;
}
void Game::wsad_AI()
{
    //int
}
void Game::AI () {
    //if(m_wnd->tcp.size()>0&&!m_wnd->sender)return;
    if(m_wnd->gqe==3)
    {
        wsad_AI();
        return;
    }
    if (m_wnd->chui||XUAN<=1||m_wnd->b_guan)
        return;
    t_ai += 1;
    int ma = 1e9, zy = -1, zx;
    bool tt = false;
    for (int i = 0; i < zmb_cnt; i++) //判断危机情况
    {
        if (zombies[i].type == 11 || zombies[i].type >= 100 || zombies[i].eat == 0 || zombies[i].type == 16 || zombies[i].vx == 0)
            continue; //不对子弹，苦力怕，小偷进行处理
        int x = ((zombies[i].x + 30) / SI) * SI, y = zombies[i].y, z;
        int he = 0;
        bool ji = false;
        for (int j = x, tm = 0; j >= 0; j -= SI) {
            int z = GetZwShu (j, y);
            if (z != -1) {
                if (plants[z].type == 6 || zombies[i].type != 8) //不会跳过
                    tm += (plants[z].life / zombies[i].eat) * (m_wnd->gqe==0 ? 34 : 40);
                if (plants[z].type == 0||plants[z].type == 4 || plants[z].type == 8 || plants[z].type == 10||plants[z].type == 9||plants[z].type == 44||plants[z].type == 13||plants[z].type == 26||plants[z].type == 60)
                {
                    double atk=plants[z].attack/1000.0;
                    if(plants[i].type == 26)atk=2;
                    if(plants[z].type == 13&&is_day)atk=0;
                    if(plants[z].type == 4 ||plants[z].type == 8 || plants[z].type == 10)atk+=1;
                    if(plants[z].type == 9)atk+=0.5;
                    if(plants[z].type == 44)atk=8;
                    if(plants[z].type == 60)atk=5;
                    if(plants[z].type == 61)atk=3;
                    if(plants[z].type == 67)atk=2;
                    atk*=1000;
                    he += (tm / plants[z].time) * atk;
                }
                if (plants[z].life >= 500)
                    ji = true; //扫描到坚果
            }
            if (zombies[i].type != 8&&zombies[i].type != 18) //特判跳跳
                tm += SI / (zombies[i].vx/2.0) * 40;
            else
                tm -= 40;
        }
        int t = x / (zombies[i].vx/2.0); //进家的时间
        if (zombies[i].shoot)
            t -= 100;
        if (!cars[y])
            t -= 200;
        if (he <= zombies[i].life && (t < ma || (t == ma && zombies[i].life > zx)))
            ma = t, zy = y, zx = x, tt = (zombies[i].type == 8);
        if (!ji && zombies[i].shoot && zombies[i].type != 16) //种植坚果挡住子弹
        {
            bool zd = false;
            for (int tx = x - SI; tx >= 0; tx -= SI) {
                if (GetZwShu (tx, y) != -1)
                    break;
                if (!NearGaint (tx, y) && !NearCar (tx, y)) //找到第一个不在巨人脚下的位置
                {
                    if ((z = Valid (16, 0)) != -1) {
                        m_wnd->select = z;
                        int r = GetZwShu (tx, y);
                        if (r == -1 || plants[r].type != 6) {
                            DestroyZw (tx, y,1);
                            zd = true;
                            m_wnd->AddPlant (tx, y);
                        }
                    }
                    else if ((z = Valid (1, 0)) != -1) {
                        m_wnd->select = z;
                        int r = GetZwShu (tx, y);
                        if (r == -1 || plants[r].type != 6) {
                            DestroyZw (tx, y,1);
                            zd = true;
                            m_wnd->AddPlant (tx, y);
                        }
                    }
                    else if ((z = Valid (2, 0)) != -1) {
                        m_wnd->select = z;
                        int r = GetZwShu (tx, y);
                        if (r == -1 || plants[r].type != 6) {
                            DestroyZw (tx, y,1);
                            zd = true;
                            m_wnd->AddPlant (tx, y);
                        }
                    }
                    else if ((z = Valid (15, 0)) != -1) {
                        m_wnd->select = z;
                        int r = GetZwShu (tx, y);
                        if (r == -1 || plants[r].type != 6) {
                            DestroyZw (tx, y,1);
                            zd = true;
                            m_wnd->AddPlant (tx, y);
                        }
                    }
                    break;
                }
            }
            if (!zd && m_wnd->cd[7] < 700 && m_wnd->cd[10] < 700 && (z = Valid (10, 0)) != -1) {
                int wx = -1;
                for (int tx = x - SI; tx >= 0; tx -= SI) {
                    if (GetZwShu (tx, y) != -1)
                        break;
                    wx = tx;
                }
                if (wx >= 0 && x - wx >= 110 && !NearGaint (wx, y) && !NearCar (wx, y)) {
                    m_wnd->select = z;
                    m_wnd->AddPlant (wx, y);
                }
            }
        }
    }
    if (zy != -1) //出现紧急情况
    {
        int zz[20], ss = 0;
        for (int j = 0; j < zx; j += SI, ss++)
            zz[j / SI] = GetZwShu (j, zy);
        int rt = Find (zz, ss), z; //找到最优位置
        if (rt != -1) {
            bool un = UnderCar (rt * SI, zy) || UnderGaint (rt * SI, zy) || BeforeShe (rt * SI, zy); //不能在巨人和车的下面种植
            if ((z = Valid (3, 0)) != -1) //窝瓜
            {
                int zz[2] = {GetZwShu (zx, zy), GetZwShu (zx - SI, zy)};
                int r = Find (zz, 2); //找到最优位置
                if(r!=-1)
                {
                    m_wnd->select = z;
                    DestroyZw (zx - r * SI, zy,1);
                    m_wnd->AddPlant (zx - r * SI, zy);
                }
            }
            /*else if ((z = Valid (14, 0)) != -1) //巴豆
            {
                int zz[3] = {GetZwShu (zx-SI, zy), GetZwShu (zx - SI*2, zy),GetZwShu (zx - SI*3, zy)};
                int r = Find (zz, 3); //找到最优位置
                if(r!=-1)
                {
                    m_wnd->select = z;
                    DestroyZw (zx -SI- r * SI, zy,1);
                    m_wnd->AddPlant (zx -SI- r * SI, zy);
                }
            }*/
            else if (jd>=2&&jd<=10&&rt*SI<=550&&(z = Valid (13, 0)) != -1 && !un) //
            {
                m_wnd->select = z;
                DestroyZw (rt * SI, zy,1);
                m_wnd->AddPlant (rt * SI, zy);
            }
            else if (jd<=30&&rt*SI<=550&&(z = Valid (12, 0)) != -1 && !un) //
            {
                m_wnd->select = z;
                DestroyZw (rt * SI, zy,1);
                m_wnd->AddPlant (rt * SI, zy);
            }
            else if (jd>20&&rt*SI<=440&&(z = Valid (4, 0)) != -1 && !un) //瓜
            {
                m_wnd->select = z;
                DestroyZw (rt * SI, zy,1);
                m_wnd->AddPlant (rt * SI, zy);
            }
            else if (jd<80&&rt*SI<=550&&(z = Valid (11, 0)) != -1 && !un) //
            {
                m_wnd->select = z;
                DestroyZw (rt * SI, zy,1);
                m_wnd->AddPlant (rt * SI, zy);
            }
            /*else if (jd>40&rt*SI<=550&&(z = Valid (5, 0)) != -1 && !un) {
                m_wnd->select = z;
                DestroyZw (rt * SI, zy,1);
                m_wnd->AddPlant (rt * SI, zy);
            }*/
            else if (zx <= (220 + (tt ? SI : 0)) && (z = Valid (7, 0)) != -1) //马上进家时可以炸死
            {
                m_wnd->select = z;
                DestroyZw (zx, zy,1);
                m_wnd->AddPlant (zx, zy);
            }
            else if (zx <= (220 + (tt ? SI : 0)) && (z = Valid (6, 0)) != -1) //马上进家时可以炸死
            {
                m_wnd->select = z;
                DestroyZw (zx, zy,1);
                m_wnd->AddPlant (zx, zy);
            }
            else if (tt && zx >= SI && zx <= 330 && (z = Valid (2, 0)) != -1) {
                m_wnd->select = z;
                DestroyZw (zx - SI, zy,1);
                m_wnd->AddPlant (zx - SI, zy);
            }
        }
    }
    int jian[MMN] = {0};int ji=2;
    //if(Valid (ji, 25) == -1)ji=16;
    if (jd > 5 && Valid (ji, 25) != -1) //种植高坚果
    {
        for (int i = 0; i < plt_cnt; i++) {
            if (plants[i].life > 150 && plants[i].x >= 550)
                jian[plants[i].y] += 1;
        }
        zy = ma = -1;
        for (int y = 0; y < MN; y++) {
            //jian[y]=1;
            if (jian[y])
                continue;
            int sh = 0;
            for (int i = 0; i < zmb_cnt; i++) {
                if (zombies[i].type < 100 && zombies[i].y == y)
                    sh += zombies[i].life + int (zombies[i].shoot&&zombies[i].type!=16) * 20;
            }
            if (SI == 110) {
                int tx[4] = {660, 550, 770, 880}, zz[20];
                for (int s = 0; s < 4; s++)
                    zz[s] = GetZwShu (tx[s], zy);
                int rt = Find (zz, 4);
                if (rt != -1 && !UnderGaint (tx[rt], zy) && sh > ma)
                    ma = sh, zy = y;
            }
            else if(SI==91)
            {
                int tx[4] = {637, 546, 728, 819}, zz[20];
                for (int s = 0; s < 4; s++)
                    zz[s] = GetZwShu (tx[s], zy);
                int rt = Find (zz, 4);
                if (rt != -1 && !UnderGaint (tx[rt], zy) && sh > ma)
                    ma = sh, zy = y;
            }
            else if (SI == 55) {
                int tx[8] = {660, 605, 550, 495, 715, 770, 880, 825}, zz[20];
                for (int s = 0; s < 8; s++)
                    zz[s] = GetZwShu (tx[s], zy);
                int rt = Find (zz, 8);
                if (rt != -1 && !UnderGaint (tx[rt], zy) && sh > ma)
                    ma = sh, zy = y;
            }
        }
        if (zy != -1) {
            if (SI == 110) {
                int tx[4] = {660, 550, 770, 880}, zz[20];
                for (int s = 0; s < 4; s++)
                    zz[s] = GetZwShu (tx[s], zy);
                int rt = Find (zz, 4);
                if (rt!=-1&&!UnderGaint (tx[rt], zy) && (!UnderCar (tx[rt], zy) || rand () % 60 == 0)) {
                    m_wnd->select = Valid (ji, 25);
                    DestroyZw (tx[rt], zy,1);
                    m_wnd->AddPlant (tx[rt], zy);
                    jian[zy] += 1;
                }
            }
            else if(SI==91)
            {
                int tx[4] = {637, 546, 728, 819}, zz[20];
                for (int s = 0; s < 4; s++)
                    zz[s] = GetZwShu (tx[s], zy);
                int rt = Find (zz, 4);
                if (rt!=-1&&!UnderGaint (tx[rt], zy) && (!UnderCar (tx[rt], zy) || rand () % 60 == 0)) {
                    m_wnd->select = Valid (ji, 25);
                    DestroyZw (tx[rt], zy,1);
                    m_wnd->AddPlant (tx[rt], zy);
                    jian[zy] += 1;
                }
            }
            else if (SI == 55) {
                int tx[8] = {660, 605, 550, 495, 715, 770, 880, 825}, zz[20];
                for (int s = 0; s < 8; s++)
                    zz[s] = GetZwShu (tx[s], zy);
                int rt = Find (zz, 8);
                if (rt!=-1&&!UnderGaint (tx[rt], zy) && (!UnderCar (tx[rt], zy) || rand () % 60 == 0)) {
                    m_wnd->select = Valid (ji, 25);
                    DestroyZw (tx[rt], zy,1);
                    m_wnd->AddPlant (tx[rt], zy);
                    jian[zy] += 1;
                }
            }
        }
    }
    else if (endless&&rand()%5==0) //有一定概率种植两排
    {
        zy = ma = -1;
        for (int y = 0; y < MN; y++) {
            int sh = 0;
            for (int i = 0; i < zmb_cnt; i++) {
                if (zombies[i].type < 100 && zombies[i].y == y)
                    sh += zombies[i].life + int (zombies[i].shoot) * 40;
            }
            if (sh > ma)
                ma = sh, zy = y;
        }
        if (zy != -1) {
            if (SI == 110) {
                int tx[4] = {660, 550, 770, 880}, zz[20];
                for (int s = 0; s < 4; s++)
                    zz[s] = GetZwShu (tx[s], zy);
                int rt = Find (zz, 4);
                if (rt != -1 && !UnderGaint (tx[rt], zy) && (!UnderCar (tx[rt], zy) || rand () % 60 == 0)) {
                    m_wnd->select = (rand()%2==0?36:58);
                    DestroyZw (tx[rt], zy,1);
                    m_wnd->AddPlant (tx[rt], zy);
                    jian[zy] += 1;
                }
            }
            else if(SI==91)
            {
                int tx[4] = {637, 546, 728, 819}, zz[20];
                for (int s = 0; s < 4; s++)
                    zz[s] = GetZwShu (tx[s], zy);
                int rt = Find (zz, 4);
                if (rt != -1 && !UnderGaint (tx[rt], zy) && (!UnderCar (tx[rt], zy) || rand () % 60 == 0)) {
                    m_wnd->select =(rand()%2==0?36:58);
                    DestroyZw (tx[rt], zy,1);
                    m_wnd->AddPlant (tx[rt], zy);
                    jian[zy] += 1;
                }
            }
            else if (SI == 55) {
                int tx[8] = {660, 605, 550, 495, 715, 770, 880, 825}, zz[20];
                for (int s = 0; s < 8; s++)
                    zz[s] = GetZwShu (tx[s], zy);
                int rt = Find (zz, 8);
                if (rt != -1 && !UnderGaint (tx[rt], zy) && (!UnderCar (tx[rt], zy) || rand () % 60 == 0)) {
                    m_wnd->select = (rand()%2==0?36:58);
                    DestroyZw (tx[rt], zy,1);
                    m_wnd->AddPlant (tx[rt], zy);
                    jian[zy] += 1;
                }
            }
        }
    }
    int xs = 0, s1 = 0;
    for (int i = 0; i < plt_cnt; i++) {
        if (plants[i].type == 1)
            xs += 1, s1 += 1;
        if (plants[i].type == 20)
            xs += 2;
    }
    if(m_wnd->gqe==15)xs=s1=1e9;
    int hj[MMN] = {0};
    double hg[MMN] = {0}, HG = 0;
    bool han[MMN] = {0},pao[MMN]={0},dpg[MMN]={0},sp=0;
    for (int i = 0; i < plt_cnt; i++) {
        if (plants[i].type == 0||plants[i].type == 4 || plants[i].type == 8 || plants[i].type == 10||plants[i].type == 9||plants[i].type == 44||plants[i].type == 13||plants[i].type == 26||plants[i].type == 60)
        {
            double atk=plants[i].attack;
            if(plants[i].type == 26)atk=2,dpg[plants[i].y] = true;
            if(plants[i].type == 13&&is_day)atk=0;
            if(plants[i].type == 4 ||plants[i].type == 8 || plants[i].type == 10)atk+=1;
            if(plants[i].type == 9)atk+=0.5;
            if(plants[i].type == 44)atk=8,pao[plants[i].y] = true;
            if(plants[i].type == 60)atk=5;
            if(plants[i].type == 61)atk=3;
            if(plants[i].type == 67)atk=2;
            hg[plants[i].y] += atk;
            if (plants[i].freeze)han[plants[i].y] = true;
        }
    }
    for (int i = 0; i < MN; i++)
    {
        HG += hg[i];
        if(!dpg[i]||hg[i]<3)sp=1;
    }
    int mx = 29 + (m_wnd->gqe==0 ? 21 : 0);
    if (jd <= 10)
        mx = 43;
    if (zmb_cnt >= 5)
        mx = 22 + (m_wnd->gqe==0 ? 10 : 0);
    if (zmb_cnt >= 9)
        mx = 17 + (m_wnd->gqe==0 ? 8 : 0); //僵尸太多就不要着急种葵了
    if (zmb_cnt >= 13)
        mx = 0;
    mx = mx * pow (110.0 / SI, 1.4);
    mx=50-zmb_cnt*2/3;
    if (xs <= mx && Valid (8, 25) != -1&&(m_wnd->beans>0||jd>0)) {
        bool zd = false;
        bool yj[MMN] = {0};
        if (jd > 50 && (zmb_cnt > 5 || Valid (2, 200) == -1)) //有僵尸就需要坚果
        {
            for (int y = 0; y < MN; y++)
                yj[y] = (jian[y] == 0);
        }
        if (jd > 250) {
            for (int y = 0; y < MN; y++)
                yj[y] = (yj[y] || (hg[y] < 5 && rand () % 20 == 0));
        }
        for (int x = 0; x <= 550; x += SI) {
            for (int y = 0; y < MN; y++) {
                if (yj[y]||spd_mul[x/SI][y]<1.01)
                    continue; //后面没有坚果就不种植
                int r = GetZwShu (x, y);
                if (r != -1 && plants[r].type == 6)
                    yj[y] = true;
                if (r == -1 && !UnderGaint (x, y) && !UnderCar (x, y) && !BeforeShe (x, y) && !UnderMonth (x, y)) {
                    m_wnd->select = (!is_day?17:12);
                    m_wnd->AddPlant (x, y);
                    if (m_wnd->beans > 0) {
                        //DaZhao (plants[plt_cnt - 1], m_wnd);
                        //m_wnd->beans -= 1;
                    }
                    else plt_kui = true;
                    xs += 2;
                    zd = true;
                    break;
                }
            }
            if (zd)
                break;
        }
        if(!zd)
        {
            for (int x = 0; x <= 550; x += SI) {
                for (int y = 0; y < MN; y++) {
                    if (yj[y])
                        continue; //后面没有坚果就不种植
                    int r = GetZwShu (x, y);
                    if (r != -1 && plants[r].type == 6)
                        yj[y] = true;
                    if (r == -1 && !UnderGaint (x, y) && !UnderCar (x, y) && !BeforeShe (x, y) && !UnderMonth (x, y)) {
                        m_wnd->select = (!is_day?17:12);
                        m_wnd->AddPlant (x, y);
                        if (m_wnd->beans > 0) {
                            //DaZhao (plants[plt_cnt - 1], m_wnd);
                            //m_wnd->beans -= 1;
                        }
                        else plt_kui = true;
                        xs += 2;
                        zd = true;
                        break;
                    }
                }
                if (zd)
                    break;
            }
        }
    }
    if ((!m_wnd->duiz||TM<20000)&&xs <= mx && (plt_cnt == 0 || (Valid (0, 50) != -1 && (rand () % 200 == 0 || s1 < 27)))) //种植向日葵
    {
        bool zd = false;
        bool yj[MMN] = {0};
        if (jd > 50 && (zmb_cnt > 5 || Valid (2, 150) == -1)) //有僵尸就需要坚果
        {
            for (int y = 0; y < MN; y++)
                yj[y] = (jian[y] == 0);
        }
        if (jd > 250) {
            for (int y = 0; y < MN; y++)
                yj[y] = (yj[y] || (hg[y] < 4 && rand () % 20 == 0));
        }
        for (int x = (SI==91?182:220); x <= 550; x += SI) {
            for (int y = 0; y < MN; y++) {
                if (yj[y])
                    continue; //后面没有坚果就不种植
                int r = GetZwShu (x, y);
                if (r != -1 && plants[r].type == 6)
                    yj[y] = true;
                if (r == -1) {
                    if (!UnderGaint (x, y) && !UnderCar (x, y) && !BeforeShe (x, y) && !UnderMonth (x, y)) //不能在巨人的面前种植
                    {
                        m_wnd->select = (!is_day?17:0);
                        m_wnd->AddPlant (x, y);
                        if (m_wnd->beans && !plt_kui) {
                            DaZhao (plt_cnt - 1, 2.0);
                            m_wnd->beans -= 1;
                        }
                        else plt_kui=1;
                        xs += 1;
                        zd = true;
                        break;
                    }
                }
            }
            if (zd)
                break;
        }
    }
    XS = xs;
    if (((m_wnd->duiz&&sp)||xs >= (m_wnd->duiz?45:5)) && (m_yg >= 800 || rand () % 5 == 0) && jd > 1) //种瓜
    {
        for (int i = 0; i < zmb_cnt; i++) {
            if (zombies[i].type < 100)
                hj[zombies[i].y] += (zombies[i].life + (zombies[i].eat) * 4 + int (zombies[i].shoot) * 25);
        }
        ma = 1e9, zy = -1;
        int z, zz[20], mz = 0;
        for (int y = 0; y < MN; y++) {
            int ss=0;
            for (int x = 0; x <= (SI == 110 ? 330 : 605); x += SI)
                zz[ss++] = GetZwShu (x, y);
            int rt = Find (zz,ss, mz);
            if (rt != -1 && !UnderGaint (rt * SI, y) && !UnderCar (rt * SI, y) && !BeforeShe (rt * SI, y) && !UnderMonth (rt * SI, y)) //不能在危险处种植
            {
                if (!cars[y])
                    hg[y] -= 4;
                if (hg[y] - hj[y] / 15 < ma)
                    ma = hg[y] - hj[y] / 15, mz = hg[y], zy = y;
            }
        }
        if(m_wnd->duiz&&sp)
        {
            for(int i=0;i<MN;i++)if(!dpg[i])zy=i;
            for(int i=0;i<MN;i++)if(hg[i]<3)zy=i;
        }
        if (zy != -1 && (mz < (10 + (m_wnd->gqe==0 ? 1 : 0)) * 110 / SI || rand () % (mz < 12 ? 30 : 50) == 0)) //如果攻击力很高就先不种植
        {
            int ss=0;
            for (int x = 0; x <= (SI == 110 ? 330 : 605); x += SI)
                zz[ss++] = GetZwShu (x, zy);
            int rt = Find (zz,ss, mz);
            if (rt != -1 && !UnderGaint (rt * SI, zy) && !UnderCar (rt * SI, zy) && !BeforeShe (rt * SI, zy) && !UnderMonth (rt * SI, zy)) //不能在危险处种植
            {
                if  ((!pao[zy] || rand () % 40 == 0 || SI == 55)&&(z = Valid (5, 0)) != -1&& (zz[rt] == -1 || plants[zz[rt]].cost < 220)) {
                    m_wnd->select = z;
                    DestroyZw (rt * SI, zy,1);
                    m_wnd->AddPlant (rt * SI, zy);
                }
                else if ((!han[zy] || rand () % 10 == 0 || SI == 55) &&((z = Valid (4, 75)) != -1) && rand () % (SI == 55 ? 5 : 12) == 0 && (zz[rt] == -1 || plants[zz[rt]].cost < 150)) {
                    m_wnd->select = z;
                    DestroyZw (rt * SI, zy,1);
                    m_wnd->AddPlant (rt * SI, zy);
                }
                else if ((z = Valid (9, 0)) != -1 && rand () % (m_wnd->duiz?(dpg[zy]?10:1):5) == 0 && (zz[rt] == -1 || plants[zz[rt]].cost <= 100) && jd > 0) {
                    m_wnd->select = z;
                    DestroyZw (rt * SI, zy,1);
                    m_wnd->AddPlant (rt * SI, zy);
                }
            }
        }
    }
    if (Valid (3, 0) != -1) //种植窝瓜
    {
        ma = -1;
        for (int x = 0; x <= (MM - 1) * SI; x += SI) {
            for (int y = 0; y < MN; y++) {
                int r = GetZwShu (x, y);
                if (r == -1 || plants[r].cost <= 75) {
                    int rt = FakeKill (x - SI * 0.4, x + SI * 0.4, y, 80);
                    if (rt > ma)
                        ma = rt, zx = x, zy = y;
                }
            }
        }
        if (ma >= 70) {
            m_wnd->select = 8;
            DestroyZw (zx, zy,1);
            m_wnd->AddPlant (zx, zy);
        }
    }
    else if (Valid (6, 0) != -1 && m_wnd->cd[8] < 750) //种植炸弹
    {
        ma = -1;
        int mx = -1;
        for (int x = 0; x <= (MM - 1) * SI; x += SI) {
            for (int y = 0; y < MN; y++) {
                int r = GetZwShu (x, y), g = (r == -1 ? 0 : plants[r].cost);
                if (g <= 125 || (g <= 130 && Valid (2, 25) != -1)) {
                    int rt = 0;
                    for (int s = -1; s <= 1; s++)
                        rt += FakeKill (x - SI * (1+FW), x + SI * (1+FW), y + s, 100);
                    if (rt > ma || (rt == ma && g < mx))
                        ma = rt, zx = x, zy = y, mx = g;
                }
            }
        }
        if (ma >= (jd <= 100 ? 100 : 170) + (m_wnd->gqe==0 ? 50 : 0)) {
            m_wnd->select = 3;
            m_wnd->AddPlant (zx, zy);
        }
    }
    /*else if (Valid (18, 50) != -1) //种植炸弹
    {
        ma = -1;
        int mx = -1;
        for (int x = 0; x <= (MM - 1) * SI; x += SI) {
            for (int y = 0; y < MN; y++) {
                int r = GetZwShu (x, y), g = (r == -1 ? 0 : plants[r].cost);
                if (g <= 100) {
                    int rt = 0;
                    for (int s = -1; s <= 1; s++)
                        rt += FakeKill (x - SI * (1+FW), x + SI * (1+FW), y + s, 100);
                    if (rt > ma || (rt == ma && g < mx))
                        ma = rt, zx = x, zy = y, mx = g;
                }
            }
        }
        if (ma >= 120) {
            m_wnd->select = 18;
            DestroyZw (zx, zy,1);
            m_wnd->AddPlant (zx, zy);
        }
    }*/
    else if (Valid (7, 25) != -1 && m_wnd->cd[8] < 700) //种植辣椒
    {
        ma = -1;
        for (int y = 0; y < MN; y++) {
            int rt = FakeKillL (y, 100);
            if (rt > ma)
                ma = rt, zy = y;
        }
        if (ma > (jd <= 100 ? 90 : 135)) {
            int zz[20];
            for (int s = 0; s <= (MM - 1) * SI; s += SI)
                zz[s / SI] = GetZwShu (s, zy);
            zx = Find (zz, MM) * SI;
            if(zx>=0)
            {
                m_wnd->select = 5;
                DestroyZw (zx, zy,1);
                m_wnd->AddPlant (zx, zy);
            }
        }
    }
    else if (m_wnd->cd[3] < 700 && m_wnd->cd[5] < 680 && m_wnd->cd[8] < 604 && (!endless || HG < 250 || jd > 1000)) {
        int xh[MMN] = {0};
        bool mc[MMN] = {0}, da[MMN] = {0};
        for (int i = 0; i < zmb_cnt; i++) {
            if (zombies[i].type < 100 && zombies[i].type != 11) {
                int t = zombies[i].life;
                if (zombies[i].eat == 1)
                    t = t * 2 / 3;
                if (zombies[i].type==12) {
                    t += 30, mc[zombies[i].y] = true;
                    if (zombies[i].x < 700 && zombies[i].life > 50)
                        t += 40;
                    if (zombies[i].x < 640 && zombies[i].life > 50)
                        t += 100;
                }
                xh[zombies[i].y] += t;
                da[zombies[i].y] = (da[zombies[i].y] || zombies[i].eat >= 40 || zombies[i].x < 640);
            }
        }
        ma = -1, zy = -1;
        for (int y = 0; y < MN; y++) {
            if (da[y] && xh[y] > ma)
                ma = xh[y], zy = y;
        }
        if (ma >= (endless?300:200) && jd > 50) {
            if (m_yg > m_wnd->plants[99].cost && m_wnd->beans < 1) {
                m_yg -= m_wnd->plants[99].cost;plants[99].cost+=50;
                m_wnd->beans += 1;
            }
            if (m_wnd->beans) {
                bool zd = false;
                for (int x = 0; x <= 440; x += SI) {
                    int z = GetZwShu (x, zy);
                    if (z != -1 && (plants[z].type == 44 ||plants[z].type == 10)&& plants[z].time == 100) {
                        zd = true;
                        break;
                    }
                }
                if (!zd && t_ai > lst[zy] + 10) {
                    for (int x = 0; x <= 550; x += SI) {
                        int z = GetZwShu (x, zy);
                        if (z != -1 && plants[z].type == 44 && plants[z].time != 100) {
                            DaZhao (z, 2.0);
                            lst[zy] = t_ai;
                            m_wnd->beans -= 1;
                            break;
                        }
                    }
                }
                if ( ma >= (endless?200:150) || mc[zy]) {
                    if (!zd && t_ai > lst[zy] + 10) {
                        for (int x = 0; x <= 550; x += SI) {
                            int z = GetZwShu (x, zy);
                            if (z != -1 && plants[z].type == 10 && plants[z].time != 100) {
                                DaZhao (z, 2.0);
                                lst[zy] = t_ai;
                                m_wnd->beans -= 1;
                                break;
                            }
                        }
                    }
                }
            }
        }
        if (Valid (1, 50) != -1 && rand () % (m_yg > 1600 ? 8 : (jd <= 150 ? 10 : 16)) == 0) {
            int qx[MMN] = {0};
            for (int i = 0; i < plt_cnt; i++) {
                if (plants[i].type == 19 && plants[i].x >= 660)
                    qx[plants[i].y] += 1;
            }
            for (int y = 0; y < MN; y++) {
                if (qx[y])
                    continue;
                int tx[4] = {770, 880}, zz[20];
                if(SI==91)tx[0]=728,tx[1]=819;
                for (int s = 0; s < 2; s++)
                    zz[s] = GetZwShu (tx[s], y);
                int rt = Find (zz, 2);
                if (rt != -1 && !UnderGaint (tx[rt], y) && (!UnderCar (tx[rt], y) || rand () % 60 == 0) && (zz[rt] == -1 || (plants[zz[rt]].type != 6&&plants[zz[rt]].type <45))) {
                    m_wnd->select = 10;
                    DestroyZw (tx[rt], y,1);
                    m_wnd->AddPlant (tx[rt], y);
                    break;
                }
            }
            if (Valid (1, 50) != -1) {
                for (int y = 0; y < MN; y++) {
                    if (qx[y] >= 2)
                        continue;
                    int tx[4] = {770, 880}, zz[20];
                    if(SI==91)tx[0]=728,tx[1]=819;
                    for (int s = 0; s < 2; s++)
                        zz[s] = GetZwShu (tx[s], y);
                    int rt = Find (zz, 2);
                    if (rt != -1 && !UnderGaint (tx[rt], y) && (!UnderCar (tx[rt], y) || rand () % 60 == 0) && (zz[rt] == -1 || (plants[zz[rt]].type != 6&&plants[zz[rt]].type <45))) {
                        m_wnd->select = 10;
                        DestroyZw (tx[rt], y,1);
                        m_wnd->AddPlant (tx[rt], y);
                        break;
                    }
                }
            }
        }
    }
    if (Valid (1, 70) != -1 && (jd < 10 || endless)) {
        int qx[MMN] = {0};
        for (int i = 0; i < plt_cnt; i++) {
            if (plants[i].type == 19 && plants[i].x >= 660)
                qx[plants[i].y] += 1;
        }
        for (int y = 0; y < MN; y++) {
            if (qx[y])
                continue;
            int tx[4] = {770, 880}, zz[20];
            if(SI==91)tx[0]=728,tx[1]=819;
            for (int s = 0; s < 2; s++)
                zz[s] = GetZwShu (tx[s], y);
            int rt = Find (zz, 2);
            if (rt != -1 && !UnderGaint (tx[rt], y) && (!UnderCar (tx[rt], y) || rand () % 60 == 0) && (zz[rt] == -1 || (plants[zz[rt]].type != 6&&plants[zz[rt]].type <45))) {
                m_wnd->select = 10;
                DestroyZw (tx[rt], y,1);
                m_wnd->AddPlant (tx[rt], y);
                break;
            }
        }
    }
    if(m_wnd->cd[38]>700)
    {
        int wx=-1,wy,mi=5;
        for(int i=0;i<MM;i++)
        {
            for(int j=0;j<MN;j++)
            {
                int z=GetZwShu(SI*i,j);
                if(z!=-1&&(plants[z].type==20||plants[z].type==12)&&plants[z].dzsy==0&&wz_fei[i][j]<mi)
                    ma=wz_fei[i][j],wx=i,wy=j;
            }
        }
        if(wx!=-1)
        {
            m_wnd->cd[38]=0;
            m_wnd->select = 38;
            m_wnd->AddPlant (wx*SI,wy);
        }
    }
    /*if(m_wnd->cd[18]>800-(eps))
    {
        int wx=-1,wy=-1,ma=((!is_day)||is_cycle?-1:0);
        for(int i=MM-2;i>=0;i--)
        {
            for(int j=0;j<MN;j++)
                if(GetZwShu(SI*i,j)==-1)
                {
                    int z=(UnderGaint(SI*i,j)?8:0)+(BeforeShe(SI*i,j)?4:0)+(NearCGT(SI*i,j)?2:0)+(i<=MM-4&&UnderMonth(SI*i,j)?1:0)-(gird_type[i][j]==2);
                    if(z>ma)ma=z,wx=i,wy=j;
                }
        }
        if(wx!=-1)
        {
            m_wnd->select = 18;
            m_wnd->AddPlant (wx*SI,wy);
        }
    }*/
    /*if(m_wnd->cd[0]>800-(eps))
    {
        int wx=-1,wy=-1,ma=0;
        for(int i=MM-2;i>=0;i--)
        {
            for(int j=0;j<MN;j++)
                if(GetZwShu(SI*i,j)==-1)
                {
                    int z=(UnderGaint(SI*i,j)?2:0)+(i<=MM-4&&NearCGT(SI*i,j)?1:0)-(m_keng[i][j]==2);
                    if(z>ma)ma=z,wx=i,wy=j;
                }
        }
        if(wx!=-1)
        {
            m_wnd->select = 0;
            m_wnd->AddPlant (wx*SI,wy);
        }
    }*/
    if (m_wnd->beans > 0&&(!endless||jd<300)) {
        int w=-1;
        for(int i=0;i<MM;i++)
        {
            for(int j=0;j<MN;j++)
            {
                int z=GetZwShu(SI*i,j);
                if(z!=-1&&(plants[z].type==20||plants[z].type==12)&&plants[z].time>100&&plants[z].lev==0)
                    w=z;
            }
        }
        if(w!=-1)
        {
            DaZhao (w, 2.0);
            m_wnd->beans -= 1;
        }
    }
    for(int i=0;i<(m_wnd->duiz?MN+1:MN-2);i++)
    {
        for(int j=0;j<MN;j++)
        {
            int z=GetZwShu(SI*i,j);
            if(z!=-1&&(plants[z].type==6||plants[z].type==19||(m_wnd->duiz&&TM>=20000&&plants[z].type==1)))
                DestroyZw(z,1);
        }
    }
    for(int i=0;i<MM;i++)
    {
        for(int j=0;j<MN;j++)
        {
            int z=GetZwShu(SI*i,j);
            if(z!=-1&&(plants[z].type==13||plants[z].type==19)&&NearQi(i*SI,j))
                DestroyZw(z,1);
            else if(z!=-1&&plants[z].type==13&&NearCre(i*SI,j))
                DestroyZw(z,1);
        }
    }
    /*if(Valid(17,0)!=-1)
    {
        int wx=-1,wy=-1,cn=0;
        for(int i=MM-1;i>=0;i--)
        {
            for(int j=0;j<MN;j++)
            {
                int w=GetZwShu(SI*i,j);
                if(w!=-1&&plants[w].B_Burning())
                {
                    if(plants[w].cost<200)
                        DestroyZw(w,1);
                    else
                        wx=i,wy=j,cn+=1;
                }
            }
        }
        if(wx!=-1&&cn<=3)
        {
            m_wnd->select = 50;
            m_wnd->AddPlant (wx*SI,wy);
        }
    }*/
    bool zan=0;
    for(int s=45;s<=47;s++)
    {
        int cn=0;
        for(int i=0;i<plt_cnt;i++)
            if(plants[i].type==s)cn+=1;
        if(cn>=5){continue;}
        if(m_wnd->duiz&&TM>1000&&m_wnd->exp>=m_wnd->expnd[s]&&TM-lstcar>1000&&m_wnd->cd[s]>800-(eps)&&TM-lstup>20000)zan=1;
        if(m_wnd->duiz&&TM>1000&&m_yg>=m_wnd->plants[s].cost*1.5&&m_wnd->exp>=m_wnd->expnd[s]&&TM-lstcar>1000&&m_wnd->cd[s]>800-(eps))
        {
            int wx=-1,wy=-1;
            for(int i=1;i<MM;i++)
            {
                for(int j=0;j<MN;j++)
                {
                    int t=GetZwShu(SI*i,j);
                    if(t==-1||plants[t].cost<=130)
                        {wx=i,wy=j;break;}
                }
                if(wx!=-1)break;
            }
            if(wx!=-1)
            {
                DestroyZw(wx*SI,wy,1);
                m_wnd->select = s;
                m_wnd->AddPlant (wx*SI,wy);lstup=TM;
            }
        }
        break;
    }
    if(m_wnd->duiz&&TM>1000&&m_yg>=600&&rand()%(m_yg>=2000?2:(zan?24:7))==0&&TM-lstcar>1000)
    {
        int r=m_wnd->x_js[rand()%11];
        if(m_yg>=2*m_wnd->j_yg[r]&&(m_yg<2000||m_wnd->j_yg[r]>300))
        {
            m_wnd->select=r;
            m_wnd->plajs=1;
            QPoint pt(150,60+(rand()%MN)*SI);
            if(r==19)
            {
                int ma=0;const Game&ga=m_wnd->an_game;
                for(int i=0;i<ga.plt_cnt;i++)
                    if(ga.plants[i].cost>ma)
                    {
                        ma=ga.plants[i].cost;
                        pt.setX(ga.plants[i].x+150);
                        pt.setY(ga.plants[i].y*SI+60);
                    }
            }
            m_wnd->AddJs(pt);
            m_wnd->plajs=0;
        }
    }
    if(m_wnd->duiz&&m_wnd->beans<MAXDZS&&m_wnd->exp>=70000)m_wnd->beans+=1,m_wnd->exp-=20000;
    if(m_wnd->duiz&&m_yg>=m_wnd->plants[24].cost)
    {
        int w=-1;
        for(int i=0;i<plt_cnt;i++)
        {
            if(m_ng[plants[i].x/SI][plants[i].y]>=200||plants[i].cost<400)
                continue;
            if(w==-1||plants[i].cost>plants[w].cost)w=i;
        }
        if(w==-1)
        {
            for(int i=0;i<MN;i++)
            {
                if(m_ng[0][i]>=100)continue;
                int z=GetZwShu(0,i);
                if(z!=-1/*&&plants[z].type!=13*/)
                    {w=z;break;}
            }
        }
        if(w!=-1)
        {
            m_wnd->select = 24;
            m_wnd->AddPlant (plants[w].x,plants[w].y);
        }
    }
}
