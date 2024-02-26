#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QOpenGLWidget>
//#include <QOP

class MyPixmap : public QObject
{
public:
    void paint();
    int ty;QPixmap pix;
    int px,py,wi,hi;
    QSize size(){return QSize(wi,hi);}
    void update(){}
    void resize(int x,int y){wi=x;hi=y;}
    QPoint pos(){return QPoint(px,py);}
    void move(int x,int y){px=x;py=y;}
    int width(){return wi;}
    int height(){return hi;}
};
class MyDraw : public QObject
{
    Q_OBJECT
public:
    MyPixmap *gl,*angl;//场地，选择栏
    MyDraw();
    void init();
    void OnTimer();
signals:
    void OnPaint(QPixmap pil,QPixmap anpil);
};

#endif // MYTHREAD_H
