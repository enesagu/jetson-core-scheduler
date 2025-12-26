#include "metrics.hpp"
#include <iostream>

namespace jts {

void MetricsCollector::recordStart(uint64_t id, const std::string& name) {
    std::lock_guard<std::mutex> lock(mutex_);
    TaskMetrics m;
    m.task_id = id;
    m.task_name = name;
    m.start_time = std::chrono::steady_clock::now();
    m.duration_ms = 0;
    m.success = false;
    metrics_.push_back(m);
}

void MetricsCollector::recordEnd(uint64_t id, bool success) {
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto& m : metrics_) {
        if (m.task_id == id && m.duration_ms == 0) {
            m.end_time = std::chrono::steady_clock::now();
            m.duration_ms = std::chrono::duration<double, std::milli>(
                m.end_time - m.start_time).count();
            m.success = success;
            break;
        }
    }
}

std::vector<TaskMetrics> MetricsCollector::getAll() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return metrics_;
}

void MetricsCollector::printSummary() const {
    std::lock_guard<std::mutex> lock(mutex_);
    std::cout << "\n=== METRICS ===\n";
    double total = 0;
    for (const auto& m : metrics_) {
        std::cout << m.task_name << ": " << m.duration_ms << "ms\n";
        total += m.duration_ms;
    }
    std::cout << "TOTAL: " << total << "ms\n";
}

void MetricsCollector::clear() {
    std::lock_guard<std::mutex> lock(mutex_);
    metrics_.clear();
}

} // namespace jts