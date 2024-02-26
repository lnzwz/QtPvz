//#ifdef Q_OS_WIN
#include "mytimer.h"
#include <MMSystem.h>
#ifdef __MINGW32__
#define TIME_KILL_SYNCHRONOUS 0x0100
#endif

ApCUserTimer::ApCUserTimer(QObject *parent) : QObject{parent}
{
    m_id = 0;
}

ApCUserTimer::~ApCUserTimer()
{
    stop();
}

void WINAPI CALLBACK PeriodCycle(uint timerId, uint, DWORD_PTR user, DWORD_PTR, DWORD_PTR)
{
    Q_UNUSED(timerId)
    ApCUserTimer *t = reinterpret_cast<ApCUserTimer *>(user);
    emit t->timeout();
}
void ApCUserTimer::start(int timeInterval)
{
    m_interval = timeInterval;
    m_id = timeSetEvent(m_interval, 1, PeriodCycle, (DWORD_PTR)this, TIME_CALLBACK_FUNCTION | TIME_ONESHOT | TIME_KILL_SYNCHRONOUS);
}
void ApCUserTimer::stop()
{
    if (m_id)
    {
        timeKillEvent(m_id);
        m_id = 0;
    }
}

bool ApCUserTimer::isActive()
{
    if (m_id)
        return true;
    else
        return false;
}
//#endif
