#include "mythread.h"
#include "mainwindow.h"
MyDraw::MyDraw()
{
}

void MyDraw::init()
{
    connect(this,SIGNAL(OnPaint(QPixmap,QPixmap)),m_wnd,SLOT(Paint(QPixmap,QPixmap)));
    gl=new MyPixmap();
    angl=new MyPixmap();
    gl->ty = 1;
    gl->resize (W0+W1+W2, HI);
    gl->move (0, ME);
    angl->ty = 2;
    angl->resize (W0+W1+W2, HI);
    angl->move (0, ME);
}

void MyDraw::OnTimer()
{
    gl->paint();
    angl->paint();
    emit OnPaint(gl->pix,angl->pix);
}
