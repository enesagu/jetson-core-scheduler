# Jetson Task Scheduler

NVIDIA Jetson cihazları için düşük seviyeli task zamanlama kütüphanesi.

## Özellikler

- ✅ Task modeli (CPU/GPU/IO, priority 0-10, realtime)
- ✅ Task Registry (kayıt, listeleme, silme)
- ✅ Priority-based Scheduler
- ✅ CPU Affinity (sched_setaffinity)
- ✅ Realtime Scheduling (SCHED_FIFO)
- ✅ Thread Pool (paralel execution)
- ✅ Metrics (task timing)
- ✅ Python API (pybind11)

## Kurulum

```bash
mkdir build && cd build
cmake .. -Dpybind11_DIR=$(python3 -c "import pybind11; print(pybind11.get_cmake_dir())")
make -j$(nproc)
