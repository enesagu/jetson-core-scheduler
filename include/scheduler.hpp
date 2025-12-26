/**
 * ============================================================================
 * SCHEDULER.HPP - GÖREV ZAMANLAYICI BAŞLIK DOSYASI
 * ============================================================================
 * 
 * Bu dosya, görevleri öncelik sırasına göre çalıştıran Scheduler sınıfını
 * tanımlar. Zamanlayıcı, arka planda sürekli çalışabilir veya tek seferlik
 * görev yürütebilir.
 * 
 * MANTIK:
 * - Görevler TaskRegistry'de saklanır
 * - Scheduler en yüksek öncelikli görevi seçip çalıştırır
 * - Ayrı bir thread'de sürekli çalışabilir (start/stop)
 * - Thread-safe: std::atomic ile durum yönetimi
 * 
 * KULLANIM:
 *   Scheduler s;
 *   s.addTask(görev);     // Görev ekle
 *   s.start();            // Arka planda çalıştır
 *   // ... işlemler ...
 *   s.stop();             // Durdur
 * ============================================================================
 */

#ifndef SCHEDULER_HPP  // Include guard: Çoklu dahil etmeyi önle
#define SCHEDULER_HPP

#include "task.hpp"          // Task yapısı
#include "task_registry.hpp" // TaskRegistry sınıfı
#include <atomic>            // std::atomic - thread-safe değişkenler için
#include <thread>            // std::thread - arka plan iş parçacığı için
#include <chrono>            // Zaman işlemleri için (sleep, duration)

namespace jts {

/**
 * ----------------------------------------------------------------------------
 * Scheduler sınıfı - Görev Zamanlayıcı
 * ----------------------------------------------------------------------------
 * Görevleri öncelik sırasına göre yürüten ana sınıf.
 * 
 * ÖZELLİKLER:
 * - Görev ekleme (addTask)
 * - Tek seferlik çalıştırma (runOnce)
 * - Sürekli arka plan çalıştırma (start/stop)
 * - Thread-safe durum yönetimi
 */
class Scheduler {
public:
    /**
     * Yapıcı fonksiyon (Constructor)
     * Zamanlayıcıyı başlatır, varsayılan durumda çalışmıyor (running_=false)
     */
    Scheduler();
    
    /**
     * Yıkıcı fonksiyon (Destructor)
     * Eğer çalışıyorsa durdurur ve kaynakları temizler
     * Worker thread'in düzgün sonlanmasını sağlar
     */
    ~Scheduler();

    // =========================================================================
    // GÖREV YÖNETİMİ
    // =========================================================================
    
    /**
     * addTask() - Yeni Görev Ekle
     * Görevi registry'ye kaydeder ve benzersiz ID döndürür.
     * 
     * @param task Eklenecek görev
     * @return Atanan benzersiz görev ID'si
     */
    uint64_t addTask(Task task);

    // =========================================================================
    // ÇALIŞTIRMA MODLARI
    // =========================================================================
    
    /**
     * runOnce() - Tek Seferlik Çalıştır
     * En yüksek öncelikli görevi bulur ve bir kez çalıştırır.
     * Manuel kontrol istenen durumlarda kullanışlıdır.
     * 
     * @return true: Görev çalıştırıldı, false: Çalıştırılacak görev yok
     */
    bool runOnce();

    /**
     * start() - Sürekli Çalıştırmayı Başlat
     * Ayrı bir thread oluşturur ve görevleri sürekli çalıştırır.
     * stop() çağrılana kadar devam eder.
     * 
     * NOT: Birden fazla kez çağrılmamalı (zaten çalışıyorsa etkisiz)
     */
    void start();
    
    /**
     * stop() - Çalışmayı Durdur
     * running_ bayrağını false yapar ve worker thread'in bitmesini bekler.
     * Thread-safe: İstediğiniz thread'den çağrılabilir
     */
    void stop();
    
    /**
     * isRunning() - Çalışma Durumunu Sorgula
     * @return true: Zamanlayıcı çalışıyor, false: Durmuş
     */
    bool isRunning() const;

    // =========================================================================
    // BİLGİ FONKSİYONLARI
    // =========================================================================
    
    /**
     * pendingCount() - Bekleyen Görev Sayısı
     * @return Registry'deki toplam görev sayısı
     */
    size_t pendingCount() const;
    
    /**
     * registry() - Registry'ye Erişim
     * Doğrudan registry'ye erişim sağlar (ileri düzey kullanım için)
     * @return TaskRegistry referansı
     */
    TaskRegistry& registry();

private:
    // =========================================================================
    // ÖZEL ÜYE DEĞİŞKENLER
    // =========================================================================
    
    TaskRegistry registry_;  // Görev deposu
    
    /**
     * running_ - Çalışma Durumu Bayrağı
     * std::atomic<bool>: Thread-safe boolean değer
     * Birden fazla thread güvenle okuyup yazabilir
     * Başlangıç değeri: false (çalışmıyor)
     */
    std::atomic<bool> running_{false};
    
    /**
     * workerThread_ - Arka Plan İş Parçacığı
     * start() çağrıldığında oluşturulur
     * stop() çağrıldığında sonlandırılır (join)
     */
    std::thread workerThread_;

    // =========================================================================
    // ÖZEL YARDIMCI FONKSİYONLAR
    // =========================================================================
    
    /**
     * executeNextTask() - Sonraki Görevi Çalıştır
     * En yüksek öncelikli görevi bulur ve work fonksiyonunu çağırır.
     * Bu fonksiyon worker thread tarafından sürekli çağrılır.
     * 
     * @return true: Görev çalıştırıldı, false: Görev bulunamadı
     */
    bool executeNextTask();
};

} // namespace jts

#endif  // SCHEDULER_HPP include guard sonu
