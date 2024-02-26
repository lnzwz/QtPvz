//#ifdef Q_OS_WIN
#ifndef APCUSERTIMER_H
#define APCUSERTIMER_H

#include <QObject>
#include <qt_windows.h>
#include <QtCore/QObject>

/**
 * @brief The ApCUserTimer class
 * Win平台下，高精度毫秒级定时器
 * 这里使用了win的api，所以在linux下无法使用
 */
class ApCUserTimer : public QObject
{
    Q_OBJECT
public:
    explicit ApCUserTimer(QObject *parent = nullptr);
    ~ApCUserTimer() Q_DECL_OVERRIDE;

    bool isActive();
signals:
    void timeout();
public slots:
    void start(int timeInterval = 10);
    void stop();
    void setInverval(int timeInterval) { m_interval = timeInterval; }
    friend void WINAPI CALLBACK PeriodCycle(uint, uint, DWORD_PTR, DWORD_PTR, DWORD_PTR);

private:
    int m_interval;
    int m_id;
};

#endif // APCUSERTIMER_H
//#endif
