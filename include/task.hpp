/**
 * ============================================================================
 * TASK.HPP - GÖREV BAŞLIK DOSYASI
 * ============================================================================
 * 
 * Bu dosya, Jetson Task Scheduler sisteminin temel görev (Task) yapısını
 * tanımlar. Bir "görev" sistemde çalıştırılacak bir iş birimini temsil eder.
 * 
 * MANTIK:
 * - Jetson kartlarında CPU, GPU ve I/O kaynakları sınırlıdır
 * - Her görev bir kaynak türüne (CPU/GPU/IO) aittir
 * - Zamanlayıcı bu bilgiyi kullanarak kaynakları verimli dağıtır
 * - Öncelik sistemi kritik görevlerin önce çalışmasını sağlar
 * ============================================================================
 */

#ifndef TASK_HPP  // Include guard: Dosyanın birden fazla dahil edilmesini önler
#define TASK_HPP

#include <string>      // std::string için - metin/yazı işlemleri
#include <vector>      // std::vector için - dinamik boyutlu dizi
#include <cstdint>     // uint64_t için - 64-bit tamsayı türü
#include <functional>  // std::function için - fonksiyon nesnesi sarmalayıcı

namespace jts {  // jts = Jetson Task Scheduler, tüm kodlar bu ad alanında

/**
 * ----------------------------------------------------------------------------
 * TaskType enum - Görev Türleri
 * ----------------------------------------------------------------------------
 * Bir görevin hangi donanım kaynağını yoğun kullanacağını belirtir.
 * Bu bilgi zamanlayıcının doğru kaynak ataması yapmasını sağlar.
 * 
 * CPU: İşlemci yoğun görevler (veri analizi, hesaplamalar)
 * GPU: Grafik işlemci görevleri (AI çıkarımı, görüntü işleme)
 * IO:  Giriş/Çıkış görevleri (dosya okuma, sensör verisi alma)
 */
enum class TaskType {
    CPU,
    GPU,
    IO
};

/**
 * ----------------------------------------------------------------------------
 * taskTypeToString() - Enum'u Metne Çevir
 * ----------------------------------------------------------------------------
 * TaskType değerini okunabilir bir yazıya dönüştürür.
 * Log kayıtlarında ve kullanıcı arayüzünde gösterim için kullanılır.
 * 
 * inline: Fonksiyonun derleyici tarafından çağrı yerine doğrudan
 *         yerleştirilmesini önerir, küçük fonksiyonlarda performans artırır
 */
inline const char* taskTypeToString(TaskType type) {
    switch (type) {  // type değişkeninin değerine göre dallan
        case TaskType::CPU: return "CPU";
        case TaskType::GPU: return "GPU";
        case TaskType::IO:  return "IO";
        default:            return "UNKNOWN";  // Beklenmeyen değer için güvenlik
    }
}

/**
 * ----------------------------------------------------------------------------
 * Task struct - Görev Yapısı
 * ----------------------------------------------------------------------------
 * Sistemde çalıştırılacak bir görevi tanımlayan veri yapısı.
 * 
 * ÜYE DEĞİŞKENLER:
 * - id: Benzersiz kimlik numarası (TaskRegistry tarafından atanır)
 * - name: Görevin insan tarafından okunabilir adı
 * - type: Görev türü (CPU/GPU/IO)
 * - priority: Öncelik seviyesi (0=düşük, 10=yüksek)
 * - realtime: Gerçek zamanlı çalışma gereksinimi
 * - cpu_cores: Görevin sabitlenmesi gereken CPU çekirdekleri
 * - work: Görev çalıştırıldığında yürütülecek fonksiyon
 */
struct Task {
    uint64_t id;            // Görev benzersiz kimliği (0'dan büyük)
    std::string name;       // Görev adı (örn: "camera_capture")
    TaskType type;          // Görev türü
    int priority;           // Öncelik (0-10 arası)
    bool realtime;          // true ise sabit zamanlama garantisi gerekli
    std::vector<int> cpu_cores;  // Sabitlenecek çekirdek numaraları
    std::function<void()> work;  // Çalıştırılacak iş fonksiyonu

    // Yapıcı fonksiyonlar (Constructor)
    Task();  // Varsayılan yapıcı - boş görev oluşturur
    Task(uint64_t id, const std::string& name, TaskType type, int priority);

    // Üye fonksiyonlar (Methods)
    std::string serialize() const;  // JSON formatında çıktı üret
    std::string summary() const;    // Özet bilgi metni üret
    bool isValid() const;           // Görev verilerinin geçerliliğini kontrol et
};

} // namespace jts

#endif  // TASK_HPP include guard sonu