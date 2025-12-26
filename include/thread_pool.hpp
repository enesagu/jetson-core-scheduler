#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>

namespace jts {

class ThreadPool {
public:
    explicit ThreadPool(size_t numThreads = 0);  // 0 = CPU sayısı kadar
    ~ThreadPool();

    // İş ekle
    void submit(std::function<void()> job);

    // Havuz boyutu
    size_t size() const { return workers_.size(); }

    // Bekleyen iş sayısı
    size_t pending() const;

    // Durdur
    void shutdown();

private:
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> jobs_;
    std::mutex mutex_;
    std::condition_variable condition_;
    std::atomic<bool> stop_{false};

    void workerLoop();
};

} // namespace jts

#endif
