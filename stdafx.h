#ifndef STDAFX_H
#define STDAFX_H


#define MAXSHU 36
#define MXUAN 12
#define MAXJSH 16
#define MAXND 1000
#define GQS 10
#define DBS 0.7
#define MANG 9
#define MAXGQS 60
#define MMN 10
#define MMM 18
#define MSI 55
#define ME 0
#define GL48 5

struct SZhw;
struct SJsh;
class CGame;
class MainWindow;

int GetZwIcon(SZhw &zw);
int GetJsIcon(SJsh &js,CGame &ga);
int GetJsEat(SZhw &zw,SJsh &js,CGame &ga);
void Fash(SJsh &js,CGame &ga);
bool DaZhao(SZhw &zw,MainWindow* wnd);
void QxDaZhao(SZhw &zw,MainWindow* wnd);

extern int XUAN,TM,MM,MN,SI;

#endif // STDAFX_H
