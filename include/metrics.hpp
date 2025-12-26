#ifndef METRICS_HPP
#define METRICS_HPP

#include <cstdint>
#include <chrono>
#include <string>
#include <vector>
#include <mutex>

namespace jts {

struct TaskMetrics {
    uint64_t task_id;
    std::string task_name;
    std::chrono::steady_clock::time_point start_time;
    std::chrono::steady_clock::time_point end_time;
    double duration_ms;
    bool success;
};

class MetricsCollector {
public:
    void recordStart(uint64_t id, const std::string& name);
    void recordEnd(uint64_t id, bool success = true);
    
    std::vector<TaskMetrics> getAll() const;
    void printSummary() const;
    void clear();

private:
    std::vector<TaskMetrics> metrics_;
    mutable std::mutex mutex_;
};

} // namespace jts

#endif
