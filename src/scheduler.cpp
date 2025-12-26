#include "scheduler.hpp"
#include <iostream>
#include <algorithm>

namespace jts {

Scheduler::Scheduler() = default;

Scheduler::~Scheduler() {
    stop();
}

uint64_t Scheduler::addTask(Task task) {
    return registry_.registerTask(task);
}

bool Scheduler::runOnce() {
    return executeNextTask();
}

void Scheduler::start() {
    if (running_) return;
    
    running_ = true;
    workerThread_ = std::thread([this]() {
        std::cout << "[Scheduler] Başlatıldı\n";
        while (running_) {
            if (!executeNextTask()) {
                // Çalıştırılacak task yok, bekle
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }
        std::cout << "[Scheduler] Durduruldu\n";
    });
}

void Scheduler::stop() {
    running_ = false;
    if (workerThread_.joinable()) {
        workerThread_.join();
    }
}

bool Scheduler::isRunning() const {
    return running_;
}

size_t Scheduler::pendingCount() const {
    return registry_.count();
}

TaskRegistry& Scheduler::registry() {
    return registry_;
}

bool Scheduler::executeNextTask() {
    auto tasks = registry_.listTasks();
    if (tasks.empty()) return false;

    // En yüksek öncelikli task'ı bul
    auto maxIt = std::max_element(tasks.begin(), tasks.end(),
        [](const Task& a, const Task& b) {
            // Önce realtime kontrol
            if (a.realtime != b.realtime) return b.realtime;
            // Sonra priority
            return a.priority < b.priority;
        });

    if (maxIt == tasks.end()) return false;

    Task task = *maxIt;
    
    // Task'ı sil (çalıştırılıyor)
    registry_.removeTask(task.id);

    // Log
    std::cout << "[Scheduler] Çalıştırılıyor: " << task.summary() << "\n";

    // Çalıştır
    if (task.work) {
        auto start = std::chrono::steady_clock::now();
        task.work();
        auto end = std::chrono::steady_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << "[Scheduler] Tamamlandı: " << task.name << " (" << ms << "ms)\n";
    }

    return true;
}

} // namespace jts
