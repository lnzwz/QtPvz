#include "mainwindow.h"
#include "stdafx.h"
#include <QApplication>
#include <QStandardPaths>
int RXUAN,XUAN,TM,MM=9,MN=5,SI=110,FIRETM=30,ME,HI=610,W0=110,W1=1040,W2=140,TOP=50;
double dan[100],zoom,sinra[10000],cosra[10000],PI=acos(-1.0),XW=616.0/2433,YW=400.0/1536,XT=81.0/2433,YT=172.0/1536;
MainWindow *m_wnd;
QPixmap *ico,*ico_bl,*icm,*ico_red,*icm_rd,*icm_bl,*ico_blk,*ico_sha;
std::mt19937 RA(std::random_device{}());
QString path;bool pub_bl[1000];
int MyRand(int cnt[100],int p[100],int n);
int GetMin(int a[100],int n)
{
    int mi=1e9,b[100],m=0;
    for(int i=0;i<n;i++)
        if(a[i]<mi)mi=a[i];
    for(int i=0;i<n;i++)
        if(a[i]==mi)b[m++]=i;
    return b[RA()%m];
}
int GetMax(int a[100],int n)
{
    int ma=-1e9,b[100],m=0;
    for(int i=0;i<n;i++)
        if(a[i]>ma)ma=a[i];
    for(int i=0;i<n;i++)
        if(a[i]==ma)b[m++]=i;
    return b[RA()%m];
}
int MyRand(int cnt[100],int p[100],int n)
{
    int m=1,ph=0,k=0,sz[100],sp[100];
    for(int i=0;i<n;i++)
        m+=cnt[i],ph+=p[i];
    for(int i=0;i<n;i++)
        if(cnt[i]*ph<m*p[i])
        {
            if((cnt[i]+1)*ph<m*p[i])return i;
            sp[k]=p[i];sz[k++]=i;
        }
    ph=0;
    for(int i=0;i<k;i++)
        ph+=sp[i];
    int r=RA()%ph;
    for(int i=0;i<k;i++)
    {
        if(r<sp[i])return sz[i];
        r-=sp[i];
    }
    return -1;
}

int MyRand(double cz[100],int p[100],int n)
{
    int ph=0,w=0;
    for(int i=0;i<n;i++)
        ph+=p[i];
    for(int i=n-1;i>=0;i--)
    {
        cz[i]+=double(p[i])/ph;
        if(cz[i]>cz[w])w=i;
    }
    cz[w]-=1;
    return w;
}

bool operator<(const ARC&a,const ARC&b)
{
    return a.a<b.a;
}

std::pair<double,double> solve_ecfc(double a,double b,double c)
{
    double dt=b*b-4*a*c;
    dt=sqrt(dt);
    return std::make_pair((-b+dt)/(a+a),(-b-dt)/(a+a));
}
/*template<class T>
QDataStream& operator<<(QDataStream& ar, T& obj)
{
    obj.output(ar);
    return ar;
}
template<class T>
QDataStream& operator>>(QDataStream& ar, T& obj)
{
    obj.input(ar);
    return ar;
}*/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    path=QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+'/';
    static MainWindow w;
    QIcon ic(":/logo.icns");
    w.setWindowIcon(ic);
    a.setWindowIcon(ic);
    w.show();
    return a.exec();
}
