#ifndef CPU_UTILS_HPP
#define CPU_UTILS_HPP

#include <vector>
#include <thread>

namespace jts {

// CPU çekirdek sayısını al
int getCpuCount();

// Thread'i belirli çekirdeklere bağla
// Başarılıysa true, değilse false (fallback)
bool setThreadAffinity(std::thread& thread, const std::vector<int>& cores);
bool setCurrentThreadAffinity(const std::vector<int>& cores);

// Realtime scheduling ayarla (SCHED_FIFO)
// priority: 1-99 (99 en yüksek)
// Başarılıysa true, değilse false (fallback)
bool setRealtimeScheduling(int priority = 50);
bool setNormalScheduling();

// Nice değeri ayarla (-20 en yüksek, 19 en düşük)
bool setNice(int nice_value);

} // namespace jts

#endif
