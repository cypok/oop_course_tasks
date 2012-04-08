#pragma once

class Algorithm
{
public:
    static const unsigned ARR_SIZE;

    static void init();
    static void deinit();
    static unsigned count_primes(unsigned left, unsigned right);
    static bool start();
    static bool is_running();
    static void stop();
    static void enter_cs() { EnterCriticalSection(&cs); }
    static void leave_cs() { LeaveCriticalSection(&cs); }
    static unsigned get_passes() { return passes; }

private:
    static bool arr[];
    static HANDLE hEnd;
    static HANDLE hThread;
    static CRITICAL_SECTION cs;
    static unsigned passes;

    Algorithm();
    Algorithm(const Algorithm &);
    ~Algorithm();
    Algorithm & operator=(const Algorithm &);

    static unsigned __stdcall sift(void *);
};
