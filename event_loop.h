#pragma once
#include "timer_awaitor.h"
#include <coroutine>
#include <deque>
#include <chrono>
#include <iostream>
#include <string>

namespace AsynchronousUtility
{
/* I am aware that this event loop is essentially a busy waiter or poller. It would ideally be completely event-driven, but for now
 * it is only for learning purposes.
 */
class TimerAwaitor;
class EventLoop
{
public:

    friend TimerAwaitor;

    ~EventLoop() = default;
    EventLoop() = default;

    // Runs the event loop. Currently runs until all timers are complete.
    void Run()
    {
        m_is_active = true;

        while(m_is_active)
        {
            if(ProcessTimers())
            {
                break;
            }
        }

        m_is_active = false;
    }

protected:

    struct Timer
    {
        std::coroutine_handle<> handle;
        std::chrono::steady_clock::time_point wake_time;
    };

    bool m_is_active {true};
    std::deque<Timer> m_timers;

    void RegisterTimer(std::coroutine_handle<> handle, std::chrono::milliseconds duration_ms)
    {   
        auto wake_time = std::chrono::steady_clock::now() + duration_ms;
        m_timers.emplace_back(Timer{.handle = handle, .wake_time = wake_time});
    }

    // returns true when all timers are complete
    bool ProcessTimers()
    {
        if(m_timers.empty())
        {
            return true;
        }

        for(auto it = m_timers.begin(); it != m_timers.end(); ++it)
        {
            const auto now = std::chrono::steady_clock::now();
            if(now >= it->wake_time)
            {
                //std::cout << "EventLoop::ProcessTimers() -> timer count before erase : {" << std::to_string(m_timers.size()) << "\n";
                it->handle.resume();
                m_timers.erase(it);
                //std::cout << "EventLoop::ProcessTimers() -> timer count after erase: {" << std::to_string(m_timers.size()) << "\n";
                
                // break out after erasing because iterating further after an erase can cause segfaults
                break;
            }
        }

        return false;
    }
};
} // namespace AsyncrhonousUtility