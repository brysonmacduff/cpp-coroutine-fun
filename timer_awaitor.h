#pragma once
#include "event_loop.h"
#include <coroutine>
#include <chrono>
#include <thread>
#include <iostream>

namespace AsynchronousUtility
{
class TimerAwaitor
{
public:
    ~TimerAwaitor() = default;
    TimerAwaitor(EventLoop& event_loop, std::chrono::milliseconds duration_ms) : m_event_loop(event_loop), m_duration_ms(duration_ms){}

    // Always suspend the coroutine at first
    bool await_ready() { return false; }  

    // Wait until the sleep is complete
    void await_suspend(std::coroutine_handle<> handle) const
    {
        if(!handle)
        {
            return;
        }

        // the event loop will resume the handle
        m_event_loop.RegisterTimer(handle, m_duration_ms);
    }

    // do nothing on resumption
    void await_resume() const {
        std::cout << "TimerAwaitor::await_resume() -> done suspending\n";
    }

protected:
    EventLoop& m_event_loop;
    std::chrono::milliseconds m_duration_ms;
};
}