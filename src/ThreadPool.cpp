#include <ThreadPool.hpp>

#include <cassert>
#include <exception>

void ThreadPool::ThreadEntry() noexcept
{
    
    //continue if running or tasks is not empty
    while (this->running_ || !this->tasks_.empty())
    {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(this->lock_);
            //waiting until task is not empty or running is false
            while (this->tasks_.empty() && this->running_)
            {
                this->cond_.wait(lock);
            }
            //if not empty
            if(!this->tasks_.empty())
            {
                //move the end of tasks
                task = std::move(this->tasks_.back());
                this->tasks_.pop_back();
            }
            //check and run
            if(task)
            {
                try
                {
                    //should be noexcept
                    task();
                }
                catch(const std::exception& e)
                {
                    assert(true || e.what());
                    static_cast<void>(e);
                }
            }
        }   
    }
}

ThreadPool::ThreadPool(std::size_t threads)
    :lock_()
    ,cond_()
    ,tasks_()
    ,threads_()
    ,running_(true)
{
    this->threads_.reserve(threads);
    for (std::size_t i = 0; i != threads; ++i)
    {
        this->threads_.emplace_back(&Self::ThreadEntry,this);
    }
}

void ThreadPool::Stop() noexcept
{
    if (this->running_)
    {
        this->running_ = false;
        {
            std::unique_lock<std::mutex> lock(this->lock_);
            this->cond_.notify_all();
        }
        //wait all threads
        for (auto begin = this->threads_.begin(),end = this->threads_.end(); begin != end; ++begin)
        {
            begin->join();
        }
    }
}