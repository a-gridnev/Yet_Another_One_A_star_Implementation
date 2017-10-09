#pragma once


#include <chrono>
#include <thread>
#include <mutex>
#include <utility>


class Process
{
public:
    enum class State
    {
        Work,
        Pause,
        Stop,
        Restart
    };

protected:
    State _state;

    std::thread _thread;
    std::mutex  _mutex;

public:
    Process(State, std::thread);

    void lock();
    void unlock();

    void sleep_for(std::chrono::milliseconds);

    const State state() const;
};