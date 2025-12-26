#include "scheduler.hpp"
#include <iostream>
#include <chrono>
#include <thread>

int main() {
    std::cout << "=== Jetson Task Scheduler Demo ===\n\n";

    jts::Scheduler scheduler;

    // Task 1: Kamera (simülasyon)
    jts::Task t1;
    t1.name = "camera_capture";
    t1.type = jts::TaskType::CPU;
    t1.priority = 7;
    t1.work = []() {
        std::cout << "  [camera] Görüntü alınıyor...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    };
    scheduler.addTask(t1);

    // Task 2: AI (yüksek öncelik, realtime)
    jts::Task t2;
    t2.name = "neural_inference";
    t2.type = jts::TaskType::GPU;
    t2.priority = 10;
    t2.realtime = true;
    t2.work = []() {
        std::cout << "  [ai] Inference yapılıyor...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    };
    scheduler.addTask(t2);

    // Task 3: Log (düşük öncelik)
    jts::Task t3;
    t3.name = "log_writer";
    t3.type = jts::TaskType::IO;
    t3.priority = 2;
    t3.work = []() {
        std::cout << "  [log] Log yazılıyor...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    };
    scheduler.addTask(t3);

    std::cout << "Bekleyen task sayısı: " << scheduler.pendingCount() << "\n\n";

    // Tek tek çalıştır (öncelik sırasına göre)
    std::cout << "--- Öncelik sırasına göre çalıştırma ---\n";
    while (scheduler.runOnce()) {
        // Tüm tasklar bitene kadar
    }

    std::cout << "\nDemo tamamlandı!\n";
    return 0;
}
