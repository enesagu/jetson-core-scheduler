#include "thread_pool.hpp"
#include "cpu_utils.hpp"
#include <iostream>

namespace jts {

ThreadPool::ThreadPool(size_t numThreads) {
    if (numThreads == 0) {
        numThreads = static_cast<size_t>(getCpuCount());
    }

    std::cout << "[ThreadPool] " << numThreads << " worker başlatılıyor\n";

    for (size_t i = 0; i < numThreads; ++i) {
        workers_.emplace_back([this]() { workerLoop(); });
    }
}

ThreadPool::~ThreadPool() {
    shutdown();
}

void ThreadPool::submit(std::function<void()> job) {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        jobs_.push(std::move(job));
    }
    condition_.notify_one();
}

size_t ThreadPool::pending() const {
    std::lock_guard<std::mutex> lock(const_cast<std::mutex&>(mutex_));
    return jobs_.size();
}

void ThreadPool::shutdown() {
    stop_ = true;
    condition_.notify_all();
    
    for (auto& worker : workers_) {
        if (worker.joinable()) {
            worker.join();
        }
    }
    workers_.clear();
}

void ThreadPool::workerLoop() {
    while (!stop_) {
        std::function<void()> job;
        
        {
            std::unique_lock<std::mutex> lock(mutex_);
            condition_.wait(lock, [this]() {
                return stop_ || !jobs_.empty();
            });
            
            if (stop_ && jobs_.empty()) return;
            
            job = std::move(jobs_.front());
            jobs_.pop();
        }
        
        // İşi çalıştır
        if (job) {
            job();
        }
    }
}

} // namespace jts
