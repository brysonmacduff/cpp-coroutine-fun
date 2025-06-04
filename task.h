#pragma once
#include <coroutine>

namespace AsynchronousUtility
{
template <typename T>
class Task
{
public:

    struct promise_type {
        Task get_return_object() { return {}; }
        std::suspend_never initial_suspend() noexcept { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void return_void() {}
        // who cares about exceptions right?
        void unhandled_exception() { }
    };
};
} // namespace