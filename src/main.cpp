#include "scheduler.hpp"
#include "thread_pool.hpp"
#include "metrics.hpp"
#include "cpu_utils.hpp"
#include <iostream>
#include <chrono>

int main() {
    std::cout << "=== Jetson Task Scheduler ===\n\n";

    jts::MetricsCollector metrics;
    jts::ThreadPool pool(4);

    // runTask: Görev simülasyonu yapan yardımcı lambda fonksiyon
    // Parametreler: id (görev kimliği), name (görev adı), ms (süre milisaniye)
    auto runTask = [&](uint64_t id, const std::string& name, int ms) {
        metrics.recordStart(id, name);
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
        metrics.recordEnd(id, true);
    };

    pool.submit([&]() { runTask(1, "camera", 100); });
    pool.submit([&]() { runTask(2, "ai", 150); });
    pool.submit([&]() { runTask(3, "tracking", 80); });
    pool.submit([&]() { runTask(4, "logger", 30); });

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    metrics.printSummary();

    return 0;
}
