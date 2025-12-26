#include "cpu_utils.hpp"
#include <iostream>
#include <cerrno>
#include <cstring>

// Linux headers
#include <sched.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/resource.h>

namespace jts {

int getCpuCount() {
    return static_cast<int>(sysconf(_SC_NPROCESSORS_ONLN));
}

bool setCurrentThreadAffinity(const std::vector<int>& cores) {
    if (cores.empty()) return true;  // Boş = tüm çekirdekler
    
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    
    for (int core : cores) {
        if (core >= 0 && core < getCpuCount()) {
            CPU_SET(core, &cpuset);
        }
    }
    
    int result = sched_setaffinity(0, sizeof(cpu_set_t), &cpuset);
    if (result != 0) {
        std::cerr << "[CPU] Affinity ayarlanamadı: " << strerror(errno) << "\n";
        return false;
    }
    
    std::cout << "[CPU] Affinity ayarlandı: çekirdekler [";
    for (size_t i = 0; i < cores.size(); ++i) {
        std::cout << cores[i];
        if (i < cores.size() - 1) std::cout << ",";
    }
    std::cout << "]\n";
    return true;
}

bool setThreadAffinity(std::thread& thread, const std::vector<int>& cores) {
    if (cores.empty()) return true;
    
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    
    for (int core : cores) {
        if (core >= 0 && core < getCpuCount()) {
            CPU_SET(core, &cpuset);
        }
    }
    
    int result = pthread_setaffinity_np(thread.native_handle(), sizeof(cpu_set_t), &cpuset);
    if (result != 0) {
        std::cerr << "[CPU] Thread affinity ayarlanamadı: " << strerror(result) << "\n";
        return false;
    }
    return true;
}

bool setRealtimeScheduling(int priority) {
    struct sched_param param;
    param.sched_priority = priority;
    
    int result = sched_setscheduler(0, SCHED_FIFO, &param);
    if (result != 0) {
        std::cerr << "[CPU] Realtime scheduling ayarlanamadı: " << strerror(errno);
        std::cerr << " (root gerekli olabilir)\n";
        return false;
    }
    
    std::cout << "[CPU] SCHED_FIFO ayarlandı, priority=" << priority << "\n";
    return true;
}

bool setNormalScheduling() {
    struct sched_param param;
    param.sched_priority = 0;
    
    int result = sched_setscheduler(0, SCHED_OTHER, &param);
    if (result != 0) {
        std::cerr << "[CPU] Normal scheduling ayarlanamadı: " << strerror(errno) << "\n";
        return false;
    }
    return true;
}

bool setNice(int nice_value) {
    int result = nice(nice_value);
    if (result == -1 && errno != 0) {
        std::cerr << "[CPU] Nice ayarlanamadı: " << strerror(errno) << "\n";
        return false;
    }
    std::cout << "[CPU] Nice ayarlandı: " << nice_value << "\n";
    return true;
}

} // namespace jts
