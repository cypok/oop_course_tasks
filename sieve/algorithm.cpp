#include "stdafx.h"
#include "algorithm.h"

const int DELAY = 60;

const unsigned Algorithm::ARR_SIZE = 1000000;
bool Algorithm::arr[ARR_SIZE];
CRITICAL_SECTION Algorithm::cs;
HANDLE Algorithm::hEnd;
HANDLE Algorithm::hThread;
unsigned Algorithm::passes;

void Algorithm::init()
{
    memset(arr, 1, sizeof(bool)*ARR_SIZE);
    InitializeCriticalSection(&cs);
    hEnd = CreateEvent(0, TRUE, FALSE, NULL);
    passes = 0;
}

void Algorithm::deinit()
{
    DeleteCriticalSection(&cs);
    CloseHandle(hEnd);
}

void Algorithm::stop()
{
    SetEvent(hEnd);
    WaitForSingleObject(hThread, INFINITE);
}

bool Algorithm::is_running()
{
    return WaitForSingleObject(hThread, 0) != WAIT_OBJECT_0;
}

bool Algorithm::start()
{
    hThread = (HANDLE)_beginthreadex(0, 0, sift, NULL, 0, 0);
    return hThread != NULL;
}

unsigned Algorithm::sift(void *)
{
    for (unsigned n = 2; n*n <= ARR_SIZE; ++n)
    {
        if (WaitForSingleObject(hEnd, 0) == WAIT_OBJECT_0)
            return 0;

        if (!arr[n])
            continue;

        EnterCriticalSection(&cs);
        {
            passes = n;

            for (unsigned i = n + n; i < ARR_SIZE; i += n)
            {
                arr[i] = false;
            }
        }
        LeaveCriticalSection(&cs);

        // work hard
        Sleep(DELAY);
    }
    return 0;
}

unsigned Algorithm::count_primes(unsigned left, unsigned right)
{
    unsigned count = 0;

    for(unsigned i=left; i<right; ++i)
        if (arr[i])
            ++count;

    return count;
}
