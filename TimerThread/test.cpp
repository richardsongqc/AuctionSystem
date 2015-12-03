#include <iostream>
#include <Windows.h>
#include "Timer.h"

using namespace std;


void CALLBACK TimerProc(void* p)
{
    int i = *((int*)p);
    cout << i << endl;
}

void CALLBACK TimerAfterProc(void* p)
{
    cout << "Done!" << endl;
}


void main() 
{ 
    Timer timer;

    int i = 10;

    int *p = &i;

    timer.registerHandler(TimerProc, p);
    timer.registerHandlerAfter(TimerAfterProc, p);
    timer.setInterval(1000);
    timer.SetPeriod(10000000);
    timer.Start();

    //::Sleep(10000);

    //for(; i > 0; i--)
    //{
    //    cout << "hello" << endl;
    //    ::Sleep(1000);
    //}

    system("pause");
    //timer.Cancel();
}