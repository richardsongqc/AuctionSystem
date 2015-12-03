#ifndef _TIMER_H_
#define _TIMER_H_

#include <Windows.h>
#include "TimerThread.h"

class Timer : public Thread
{
    typedef void(CALLBACK *Timerfunc)(void* p);
    typedef Timerfunc TimerHandler;
public:
    Timer()
        :m_handler(0)
        ,m_interval(-1)
    {
    }

    void registerHandler(TimerHandler handler, void* p)
    {
        m_handler = handler;
        m_parameter = p;
    }

    void registerHandlerAfter(TimerHandler handler, void* p)
    {
        m_handlerAfter = handler;
        m_paramAfter = p;
    }

    void setInterval(int millisecond)
    {
        m_interval = millisecond;
    }

    void Run()
    {
        unsigned long tickNow = ::GetTickCount();
        unsigned long tickLastTime = tickNow;
        DWORD dwStartTime = tickNow;
        long lElasped = m_dwPeriod;
        while (!IsStop() && lElasped > 0)
        {
            tickNow = ::GetTickCount();
            if(tickNow - tickLastTime > m_interval)
            {
                if(m_handler)
                {
                    (*m_handler)(m_parameter);
                }
                tickLastTime = ::GetTickCount();
            }

            lElasped -= (tickNow - dwStartTime);

            ::Sleep(1);
        }

        if (m_handlerAfter)
        {
            (*m_handlerAfter)(m_paramAfter);
        }
    }

    void Cancel()
    {
        Stop();
    }

    void SetPeriod(DWORD dwPeriod)
    {
        m_dwPeriod = dwPeriod;
    }
private:
    TimerHandler m_handler;
    TimerHandler m_handlerAfter;
    int             m_interval;
    void*         m_parameter;
    void*         m_paramAfter;
    DWORD        m_dwPeriod;
};

#endif