# Jetson Task Scheduler

NVIDIA Jetson cihazları için düşük seviyeli task zamanlama kütüphanesi.

## Özellikler

- ✅ Task modeli (CPU/GPU/IO türleri, öncelik, deadline)
- ✅ Task Registry (kayıt, listeleme, silme)
- ✅ Priority-based Scheduler
- ✅ CPU Affinity (çekirdek bağlama)
- ✅ Realtime Scheduling (SCHED_FIFO)
- ✅ Thread Pool
- 🔄 GPU Job Queue (yakında)
- 🔄 Python API (yakında)

## Gereksinimler

- Ubuntu 20.04/22.04 (JetPack)
- CMake >= 3.16
- GCC >= 9 (C++17)
- pthread

## Kurulum

```bash
git clone <repo>
cd jetson-task-scheduler
mkdir build && cd build
cmake ..
make -j$(nproc)
