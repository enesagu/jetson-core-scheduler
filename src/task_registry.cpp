/**
 * ============================================================================
 * TASK_REGISTRY.CPP - GÖREV KAYIT DEFTERİ UYGULAMA DOSYASI
 * ============================================================================
 * 
 * Bu dosya, TaskRegistry sınıfının fonksiyonlarının uygulamalarını içerir.
 * Tüm görev yönetim işlemleri burada gerçekleştirilir.
 * 
 * THREAD-SAFE (İŞ PARÇACIĞI GÜVENLİ) TASARIM:
 * - Her fonksiyon mutex ile korunur
 * - Birden fazla thread aynı anda erişse bile veri bozulmaz
 * - std::lock_guard otomatik kilit yönetimi sağlar
 * ============================================================================
 */

#include "task_registry.hpp"  // Sınıf tanımları
#include <algorithm>          // std::remove_if için - dizi elemanı silme

namespace jts {

/**
 * ----------------------------------------------------------------------------
 * registerTask() - Yeni Görev Kaydet
 * ----------------------------------------------------------------------------
 * Verilen görevi kayıt defterine ekler ve benzersiz bir ID atar.
 * 
 * MANTIK:
 * 1. Mutex kilitle (diğer thread'ler beklesin)
 * 2. Görevin ID'sini otomatik artan sayaçtan al
 * 3. Görevi listeye ekle
 * 4. Atanan ID'yi döndür
 * 
 * @param task Kaydedilecek görev (kopyalanarak alınır)
 * @return Atanan benzersiz ID
 */
uint64_t TaskRegistry::registerTask(Task task) {
    // lock_guard: Constructor'da kilitle, destructor'da otomatik aç
    // Bu sayede fonksiyon herhangi bir noktada çıksa bile kilit açılır
    std::lock_guard<std::mutex> lock(mutex_);
    
    task.id = nextId_++;        // ID ata ve sayacı artır
    tasks_.push_back(task);     // Görevi listenin sonuna ekle
    return task.id;             // Atanan ID'yi döndür
}

/**
 * ----------------------------------------------------------------------------
 * getTask() - ID ile Görev Getir
 * ----------------------------------------------------------------------------
 * Verilen ID'ye sahip görevi arar ve bulursa döndürür.
 * 
 * std::optional KULLANIMI:
 * - Görev bulunursa: optional içinde Task döner
 * - Görev bulunamazsa: std::nullopt (boş değer) döner
 * - Bu yaklaşım nullptr ile uğraşmaktan daha güvenlidir
 * 
 * @param id Aranan görevin ID'si
 * @return Bulunan görev veya boş (nullopt)
 * 
 * const fonksiyon: Bu fonksiyon sınıfın durumunu değiştirmez
 */
std::optional<Task> TaskRegistry::getTask(uint64_t id) const {
    std::lock_guard<std::mutex> lock(mutex_);
    
    // Tüm görevleri dolaş ve ID'yi ara
    for (const auto& t : tasks_) {
        if (t.id == id) return t;  // Bulundu, görevi döndür
    }
    return std::nullopt;  // Bulunamadı, boş değer döndür
}

/**
 * ----------------------------------------------------------------------------
 * listTasks() - Tüm Görevleri Listele
 * ----------------------------------------------------------------------------
 * Kayıtlı tüm görevlerin bir kopyasını döndürür.
 * 
 * NEDEN KOPYA?
 * - Orijinal listeye doğrudan erişim thread-safe olmaz
 * - Çağıran kod listeyi istediği gibi kullanabilir
 * 
 * @return Tüm görevlerin kopyası
 */
std::vector<Task> TaskRegistry::listTasks() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return tasks_;  // Listenin kopyasını döndür
}

/**
 * ----------------------------------------------------------------------------
 * removeTask() - Görevi Sil
 * ----------------------------------------------------------------------------
 * Verilen ID'ye sahip görevi listeden kaldırır.
 * 
 * REMOVE-ERASE İDİOMU:
 * C++'ta vector'dan eleman silmek için iki aşamalı işlem gerekir:
 * 1. std::remove_if: Silinecek elemanları sona taşı
 * 2. erase: Taşınan elemanları gerçekten sil
 * 
 * Bu idiom performans açısından tek tek silmekten daha verimlidir.
 * 
 * @param id Silinecek görevin ID'si
 * @return true: Silindi, false: ID bulunamadı
 */
bool TaskRegistry::removeTask(uint64_t id) {
    std::lock_guard<std::mutex> lock(mutex_);

    // remove_if: Koşulu sağlayan elemanları sona taşır
    // Lambda fonksiyonu: [id] dış kapsamdan id değişkenini yakalar
    auto it = std::remove_if(
        tasks_.begin(),     // Arama başlangıcı
        tasks_.end(),       // Arama sonu
        [id](const Task& t) {  // Her eleman için çalışacak koşul fonksiyonu
            return t.id == id;  // Bu görev silinecek mi?
        }
    );

    // Eğer silinecek eleman bulunduysa
    if (it != tasks_.end()) {
        tasks_.erase(it, tasks_.end());  // Sona taşınan elemanları sil
        return true;  // Başarılı
    }
    return false;  // ID bulunamadı
}


/**
 * ----------------------------------------------------------------------------
 * count() - Görev Sayısını Döndür
 * ----------------------------------------------------------------------------
 * Kayıtlı görev adedini döndürür.
 * 
 * @return Listedeki görev sayısı
 */
size_t TaskRegistry::count() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return tasks_.size();  // Vector'ün eleman sayısı
}

/**
 * ----------------------------------------------------------------------------
 * clear() - Tüm Görevleri Temizle
 * ----------------------------------------------------------------------------
 * Listeyi tamamen boşaltır. Dikkatli kullanılmalı!
 */
void TaskRegistry::clear() {
    std::lock_guard<std::mutex> lock(mutex_);
    tasks_.clear();  // Tüm elemanları sil
}

} // namespace jts
