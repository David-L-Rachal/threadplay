/*
 * ThreadPlay
 *
 * Historical-style Win32 multithreading example demonstrating:
 *   - Native Windows threads
 *   - Shared mutable state
 *   - CRITICAL_SECTION synchronization
 *   - Thread shutdown signaling
 *   - Thread lifecycle / handle cleanup
 *
 * Originally explored during my Visual C++ / Win32 development work.
 */

#include <windows.h>
#include <process.h>
#include <iostream>

struct SharedState
{
    CRITICAL_SECTION lock;
    unsigned long counter;
    bool running;
};

unsigned __stdcall CounterThread(void* context)
{
    SharedState* state = static_cast<SharedState*>(context);

    while (true)
    {
        EnterCriticalSection(&state->lock);

        if (!state->running)
        {
            LeaveCriticalSection(&state->lock);
            break;
        }

        ++state->counter;

        LeaveCriticalSection(&state->lock);

        Sleep(10);
    }

    return 0;
}

unsigned __stdcall DisplayThread(void* context)
{
    SharedState* state = static_cast<SharedState*>(context);

    while (true)
    {
        unsigned long currentValue;
        bool running;

        EnterCriticalSection(&state->lock);

        currentValue = state->counter;
        running = state->running;

        LeaveCriticalSection(&state->lock);

        if (!running)
            break;

        std::cout << "Counter: " << currentValue << std::endl;

        Sleep(100);
    }

    return 0;
}

int main()
{
    SharedState state = {};

    state.counter = 0;
    state.running = true;

    InitializeCriticalSection(&state.lock);

    HANDLE counterThread =
        reinterpret_cast<HANDLE>(
            _beginthreadex(
                nullptr,
                0,
                CounterThread,
                &state,
                0,
                nullptr));

    HANDLE displayThread =
        reinterpret_cast<HANDLE>(
            _beginthreadex(
                nullptr,
                0,
                DisplayThread,
                &state,
                0,
                nullptr));

    if (!counterThread || !displayThread)
    {
        std::cerr << "Failed to create worker threads." << std::endl;

        if (counterThread)
            CloseHandle(counterThread);

        if (displayThread)
            CloseHandle(displayThread);

        DeleteCriticalSection(&state.lock);
        return 1;
    }

    std::cout << "Threads running. Press Enter to stop..." << std::endl;
    std::cin.get();

    EnterCriticalSection(&state.lock);
    state.running = false;
    LeaveCriticalSection(&state.lock);

    WaitForSingleObject(counterThread, INFINITE);
    WaitForSingleObject(displayThread, INFINITE);

    CloseHandle(counterThread);
    CloseHandle(displayThread);

    DeleteCriticalSection(&state.lock);

    std::cout << "Final counter value: "
              << state.counter
              << std::endl;

    return 0;
}
