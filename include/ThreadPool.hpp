#pragma once
#ifndef _THREADPOOL_HPP
#define _THREADPOOL_HPP

#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <cstdint>
#include <vector>
#include <atomic>

class ThreadPool
{
private:
    using Self = ThreadPool;

    std::mutex lock_;
    std::condition_variable cond_;
    std::vector<std::function<void()>> tasks_;
    std::vector<std::thread> threads_;
    std::atomic_bool running_;

    void ThreadEntry() noexcept;
public:

    explicit ThreadPool(std::size_t threads);

    ThreadPool(const Self &other) = delete;

    ThreadPool(Self &&other) noexcept = delete;

    Self &operator=(const Self &other) = delete;

    Self &operator=(Self &&other) noexcept = delete;

    ~ThreadPool() noexcept
    {
        this->Stop();
    }

    template<typename _Fn,typename ..._Args,typename _Check = decltype(std::declval<std::function<void()>&>() = std::bind(std::declval<_Fn>(),std::declval<_Args>()...))>
    inline void SubmitTask(_Fn &&fn,_Args &&...args)
    {
        std::unique_lock<std::mutex> lock(this->lock_);
        this->tasks_.emplace_back(std::bind(std::forward<_Fn>(fn),std::forward<_Args>(args)...));
        this->cond_.notify_one();
    }

    void Stop() noexcept;
};

#endif
