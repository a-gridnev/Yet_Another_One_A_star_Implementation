#include "process.hpp"

Process::Process(State state, std::thread thread)
{
    _state = state;
    _thread = std::move(thread);
}

void Process::lock()
{
    _mutex.lock();
}

void Process::unlock()
{
    _mutex.unlock();
}

void Process::sleep_for(std::chrono::milliseconds delay)
{
     std::this_thread::sleep_for(delay);
}

const Process::State Process::state() const
{
    return(_state);
}