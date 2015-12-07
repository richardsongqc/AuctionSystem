#ifndef _TIMER_H_
#define _TIMER_H_

#include <Windows.h>
#include "TimerThread.h"

#pragma warning( disable : 4018)
#pragma warning( disable : 4244)

typedef void(CALLBACK *Timerfunc)(void* p);
typedef Timerfunc TimerHandler;

class Timer : public Thread
{

public:
    Timer()
        :m_handler(0)
        , m_interval(-1)
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

        LARGE_INTEGER  large_interger;
        double dff;
        QueryPerformanceFrequency(&large_interger);
        dff = large_interger.QuadPart;
        QueryPerformanceCounter(&large_interger);
        double tickNow = large_interger.QuadPart/dff;
        double tickLastTime = tickNow;
        double startTime = tickNow;
        double lElasped = m_dwPeriod;
        TRACE(L"\nlElasped = %f\n", lElasped);
        while (/*!IsStop() &&*/ lElasped > 0)
        {
            QueryPerformanceCounter(&large_interger);
            tickNow = large_interger.QuadPart/dff;
            if (tickNow - tickLastTime > m_interval)
            {
                if (m_handler)
                {
                    (*m_handler)(m_parameter);
                }

                QueryPerformanceCounter(&large_interger);
                tickLastTime = large_interger.QuadPart/dff;
            }
            double l = tickNow - startTime;
            lElasped -= l;
            TRACE(L"lElasped = %f\telasped = %f\n", lElasped, l);
            ::Sleep(1000);
        }

        if (m_handlerAfter)
        {
            (*m_handlerAfter)(m_paramAfter);
        }

        Stop();
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