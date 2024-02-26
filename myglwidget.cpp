#include "myglwidget.h"
#include "mainwindow.h"

void MyGLWidget::paintEvent(QPaintEvent *event)
{
#ifdef NO_UI
    hide();
    return;
#endif
    QPainter memdc (this);
    if (m_wnd->b_kjc)
    {
        memdc.setRenderHint(QPainter::SmoothPixmapTransform);
        memdc.setRenderHint(QPainter::Antialiasing);
    }
    if(ty==1)m_wnd->m_game.Draw (memdc);
    else if(ty==2&&m_wnd->an_game.zmb_cnt>=0)m_wnd->an_game.Draw(memdc,1);
    if ((!m_wnd->m_game.is_day) || m_wnd->m_game.is_cycle) //夜晚变暗
    {
        memdc.setPen (Qt::NoPen);
        memdc.setBrush (QColor (0, 0, 60, m_wnd->m_game.GetNightAlpha()));
        memdc.drawRect (QRect (0,0,width(),height()));
    }
    memdc.end();
}
