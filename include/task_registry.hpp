/**
 * ============================================================================
 * TASK_REGISTRY.HPP - GÖREV KAYIT DEFTERİ BAŞLIK DOSYASI
 * ============================================================================
 * 
 * Bu dosya, sistemdeki tüm görevleri yöneten TaskRegistry sınıfını tanımlar.
 * 
 * MANTIK:
 * - Tüm görevler merkezi bir kayıt defterinde tutulur
 * - Her görev benzersiz bir ID alır (otomatik artan)
 * - Thread-safe (iş parçacığı güvenli) tasarım: mutex ile korunur
 * - CRUD işlemleri: Create (ekle), Read (oku), Update, Delete (sil)
 * 
 * NEDEN TEKİL KAYIT (Registry)?
 * - Görevleri merkezi yönetim sağlar
 * - ID çakışmalarını önler
 * - Çoklu iş parçacığından güvenli erişim sağlar
 * ============================================================================
 */

#ifndef TASK_REGISTRY_HPP  // Include guard başlangıcı
#define TASK_REGISTRY_HPP

#include "task.hpp"    // Task yapısı için
#include <vector>      // std::vector - görev listesi için
#include <mutex>       // std::mutex - iş parçacığı senkronizasyonu için
#include <optional>    // std::optional - var/yok durumu için (C++17)

namespace jts {

/**
 * ----------------------------------------------------------------------------
 * TaskRegistry sınıfı - Görev Kayıt Defteri
 * ----------------------------------------------------------------------------
 * Sistemdeki tüm görevleri yönetir. Thread-safe yapıdadır.
 * 
 * KULLANIM ÖRNEĞİ:
 *   TaskRegistry registry;
 *   Task t1; t1.name = "örnek";
 *   uint64_t id = registry.registerTask(t1);  // Kaydet, ID al
 *   auto task = registry.getTask(id);         // ID ile bul
 *   registry.removeTask(id);                  // Sil
 */
class TaskRegistry {
public:
    /**
     * registerTask() - Yeni Görev Kaydet
     * Görevi listeye ekler ve otomatik bir ID atar.
     * @param task Kaydedilecek görev
     * @return Atanan benzersiz görev ID'si
     */
    uint64_t registerTask(Task task);
    
    /**
     * getTask() - ID ile Görev Getir
     * @param id Aranan görevin ID'si
     * @return Bulunan görev veya boş (std::nullopt)
     * 
     * std::optional kullanımı: Görev bulunamazsa nullptr yerine
     * güvenli bir "boş" değer döner
     */
    std::optional<Task> getTask(uint64_t id) const;
    
    /**
     * listTasks() - Tüm Görevleri Listele
     * @return Kayıtlı tüm görevlerin kopyası
     */
    std::vector<Task> listTasks() const;
    
    /**
     * removeTask() - Görevi Sil
     * @param id Silinecek görevin ID'si
     * @return true: başarıyla silindi, false: bulunamadı
     */
    bool removeTask(uint64_t id);
    
    /**
     * count() - Görev Sayısını Döndür
     * @return Kayıtlı görev adedi
     */
    size_t count() const;
    
    /**
     * clear() - Tüm Görevleri Temizle
     * Listeyi tamamen boşaltır
     */
    void clear();

private:
    // Özel üye değişkenler (dışarıdan erişilemez)
    
    std::vector<Task> tasks_;  // Görev listesi (dinamik dizi)
    
    /**
     * mutex_ - Karşılıklı Dışlama Kilidi
     * Birden fazla thread aynı anda listeye eriştiğinde veri bozulmasını önler.
     * "mutable" anahtar kelimesi: const fonksiyonlarda bile mutex kilitlenebilir
     */
    mutable std::mutex mutex_;
    
    uint64_t nextId_ = 1;  // Bir sonraki atanacak ID (1'den başlar)
};

} // namespace jts

#endif  // TASK_REGISTRY_HPP include guard sonu