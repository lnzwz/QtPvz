#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include "stdafx.h"

class MyGLWidget : public QOpenGLWidget
{
public:
    int ty;
    MyGLWidget(){}
    MyGLWidget(QWidget *parent) : QOpenGLWidget(parent) { }
    void paintEvent(QPaintEvent *event)override;
protected:
    void paintGL() override
    {
#ifndef Q_OS_ANDROID
        QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
        f->glClear(GL_COLOR_BUFFER_BIT);
#endif
    }
};

#endif // MYGLWIDGET_H
