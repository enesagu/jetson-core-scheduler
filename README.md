# 🚀 Jetson Task Scheduler

NVIDIA Jetson cihazları için yüksek performanslı, düşük gecikmeli task zamanlama kütüphanesi.

## 📋 Özellikler

| Özellik | Açıklama |
|---------|----------|
| ✅ Task Modeli | CPU/GPU/IO türleri, priority 0-10, realtime desteği |
| ✅ Task Registry | Görev kayıt, listeleme ve silme işlemleri |
| ✅ Priority Scheduler | Öncelik bazlı akıllı zamanlama |
| ✅ Realtime Scheduling | SCHED_FIFO ile gerçek zamanlı öncelik |
| ✅ CPU Affinity | sched_setaffinity ile çekirdek bağlama |
| ✅ Thread Pool | Paralel execution ile ~4x hız artışı |
| ✅ Metrics | Task timing ve performans ölçümü |
| ✅ Python API | pybind11 ile Python entegrasyonu |

## 🛠️ Kurulum

### Gereksinimler

- CMake 3.14+
- C++17 uyumlu derleyici
- Python 3.8+ (Python bindings için)
- pybind11 (`pip install pybind11`)

### Derleme

```bash
mkdir build && cd build
cmake .. -Dpybind11_DIR=$(python3 -c "import pybind11; print(pybind11.get_cmake_dir())")
make -j$(nproc)
```

### Çalıştırma

```bash
# Ana demo
./task_demo

# Gerçekçi senaryo testleri
./realistic_demo
```

## 📊 Performans Sonuçları

`realistic_demo` çalıştırıldığında aşağıdaki sonuçlar elde edilir:

### Test 1: Öncelik Sıralaması
```
Çalışma sırası: high_priority -> medium_priority -> low_priority -> bitti
✓ BAŞARILI: Yüksek öncelikli görevler önce çalıştı!
```

### Test 2: Realtime Görev Önceliği
```
Çalışma sırası: realtime_low -> normal_high -> bitti
✓ BAŞARILI: Realtime görev her zaman önce çalışır!
  (pri=3 realtime > pri=10 normal)
```

### Test 3: Paralel vs Seri Karşılaştırma
```
Görev sayısı: 8 x 100ms
CPU çekirdek: 6

Seri süre:    800 ms
Paralel süre: 203 ms
Hız artışı:   3.94x
✓ BAŞARILI: ThreadPool 3.94x hız artışı sağladı!
```

### Test 4: CPU Affinity
```
Mevcut CPU sayısı: 6
✓ Thread çekirdek 0-1'e bağlandı
  Avantajlar:
  - Cache locality (L1/L2 cache verileri korunur)
  - Realtime görevler izole edilebilir
  - GPU thread'leri ayrı çekirdeklere atanabilir
```

### Test 5: Task Metrics
```
camera_capture: 50.06ms
ai_inference: 100.04ms
object_tracking: 75.03ms
log_write: 25.00ms
TOTAL: 250.13ms
```

## 🐍 Python Kullanımı

```python
import jetson_scheduler as js

# Scheduler oluştur
scheduler = js.Scheduler()

# Task ekle
task = js.Task()
task.name = "ai_inference"
task.priority = 8
task.task_type = js.TaskType.GPU
task.realtime = True

scheduler.add_task(task)

# Çalıştır
scheduler.run_once()

# Metrics
metrics = js.MetricsCollector()
metrics.record_start(1, "inference")
# ... iş yap ...
metrics.record_end(1, True)
metrics.print_summary()
```

## 📁 Proje Yapısı

```
jetson-task-scheduler/
├── include/           # Header dosyaları
│   ├── task.hpp
│   ├── task_registry.hpp
│   ├── scheduler.hpp
│   ├── thread_pool.hpp
│   ├── cpu_utils.hpp
│   └── metrics.hpp
├── src/               # Kaynak dosyaları
│   ├── task.cpp
│   ├── task_registry.cpp
│   ├── scheduler.cpp
│   ├── thread_pool.cpp
│   ├── cpu_utils.cpp
│   ├── metrics.cpp
│   └── main.cpp
├── python/            # Python bindings
│   └── bindings.cpp
├── tests/             # Test dosyaları
│   ├── realistic_demo.cpp
│   └── test_basic.cpp
├── examples/          # Örnek kullanımlar
│   └── demo.py
└── CMakeLists.txt
```

## 🎯 Kullanım Senaryoları

### 1. Otonom Araç Pipeline
```cpp
// Kamera yakalama -> AI çıkarım -> Nesne takibi -> Karar
jts::Task camera, inference, tracking, decision;
camera.priority = 10;      // En yüksek
inference.priority = 9;
inference.task_type = jts::TaskType::GPU;
tracking.priority = 8;
decision.priority = 7;
decision.realtime = true;  // Kritik karar
```

### 2. Robot Kontrolü
```cpp
// Sensör okuma yüksek frekansta, log yazma düşük öncelikte
jts::Task sensor, motor, log;
sensor.realtime = true;    // 1kHz okuma
motor.realtime = true;     // Hızlı tepki
log.priority = 1;          // Arka planda
```

## 📝 Lisans

MIT License

## 🤝 Katkıda Bulunma

Pull request'ler memnuniyetle karşılanır!

---

**Jetson Task Scheduler** - Gerçek zamanlı performans, sade API 🚀
