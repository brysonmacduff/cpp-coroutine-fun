#include "task.h"
#include "timer_awaitor.h"
#include "event_loop.h"
#include <iostream>
#include <iomanip>
#include <string>

using namespace AsynchronousUtility;

void PrintNow(std::string source)
{
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

    std::time_t time_now = std::chrono::system_clock::to_time_t(now);
    std::tm* time_info = std::localtime(&time_now);

    std::cout << source << " -> " << std::put_time(time_info, "%Y-%m-%d %H:%M:%S")
              << '.' << (millis % 1000) << '\n';

}

int main()
{
    EventLoop event_loop;

    auto sleeper_handle = [&](std::string coro_name, std::chrono::milliseconds duration_ms) -> Task<void>
    {   
        PrintNow("coro "+coro_name+" start");
        co_await TimerAwaitor(event_loop,duration_ms);
        PrintNow("coro "+coro_name+" end");
    };

    sleeper_handle("1",std::chrono::milliseconds{3000});
    sleeper_handle("2",std::chrono::milliseconds{2000});
    sleeper_handle("3",std::chrono::milliseconds{1000});
    event_loop.Run();
}