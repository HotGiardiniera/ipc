#pragma once

#include <thread>

class ScopedThread {
public:
    explicit ScopedThread(std::thread t_);
    ~ScopedThread();
    ScopedThread(ScopedThread const &) = delete;
    ScopedThread& operator= (ScopedThread const &) = delete;
private:
    std::thread t;

};
