/**
 * ============================================================================
 * TASK.CPP - GÖREV SINIFI UYGULAMA DOSYASI
 * ============================================================================
 * 
 * Bu dosya, Task yapısının fonksiyonlarının gerçek uygulamalarını içerir.
 * Başlık dosyasında (.hpp) sadece tanımlar vardı, burada kodlar yazılıdır.
 * 
 * İÇERİK:
 * - Yapıcı fonksiyonlar (constructors)
 * - serialize(): JSON formatına çevirme
 * - summary(): Özet metin oluşturma
 * - isValid(): Geçerlilik kontrolü
 * ============================================================================
 */

#include "task.hpp"    // Task yapısının tanımları
#include <sstream>     // std::ostringstream - metin akışı oluşturmak için

namespace jts {

/**
 * ----------------------------------------------------------------------------
 * Task::Task() - Varsayılan Yapıcı (Default Constructor)
 * ----------------------------------------------------------------------------
 * Parametresiz çağrıldığında boş/varsayılan değerlerle görev oluşturur.
 * 
 * BAŞLATICI LİSTESİ (Initializer List) KULLANIMI:
 * : ile başlayan kısım, üye değişkenleri doğrudan başlatır.
 * Bu yöntem, fonksiyon gövdesinde atama yapmaktan daha verimlidir.
 */
Task::Task()
    : id(0)              // ID henüz atanmadı (registry atayacak)
    , name("unnamed")    // Varsayılan isim
    , type(TaskType::CPU) // Varsayılan tür
    , priority(5)        // Orta öncelik
    , realtime(false)    // Gerçek zamanlı değil
    , cpu_cores{}        // Boş çekirdek listesi
    , work(nullptr)      // Henüz iş fonksiyonu yok
{}

/**
 * ----------------------------------------------------------------------------
 * Task::Task(id, name, type, priority) - Parametreli Yapıcı
 * ----------------------------------------------------------------------------
 * Temel değerlerle yeni görev oluşturur.
 * 
 * @param id Görev kimliği
 * @param name Görev adı (const reference ile alınır - kopyalama maliyeti yok)
 * @param type Görev türü
 * @param priority Öncelik seviyesi
 */
Task::Task(uint64_t id, const std::string& name, TaskType type, int priority)
    : id(id)
    , name(name)
    , type(type)
    , priority(priority)
    , realtime(false)    // Varsayılan: gerçek zamanlı değil
    , cpu_cores{}        // Varsayılan: boş liste
    , work(nullptr)      // Varsayılan: iş fonksiyonu yok
{}

/**
 * ----------------------------------------------------------------------------
 * Task::serialize() - JSON Formatına Çevirme
 * ----------------------------------------------------------------------------
 * Görevi JSON formatında bir metin olarak döndürür.
 * Bu format veri kaydetme, ağ iletişimi veya loglama için kullanılır.
 * 
 * ÇIKTI ÖRNEĞİ:
 * {"id":1,"name":"camera","type":"CPU","priority":7,"realtime":false,"cpu_cores":[0,1]}
 * 
 * MANTIK:
 * - ostringstream: Metin parçalarını birleştirmek için kullanılır
 * - Her alan JSON formatına uygun şekilde eklenir
 * - Diziler (cpu_cores) köşeli parantez içinde virgülle ayrılır
 */
std::string Task::serialize() const {
    std::ostringstream oss;  // Çıktı akışı oluştur
    
    oss << "{";  // JSON nesnesi başlangıcı
    
    // Her alanı JSON formatında ekle
    oss << "\"id\":" << id << ",";
    oss << "\"name\":\"" << name << "\",";
    oss << "\"type\":\"" << taskTypeToString(type) << "\",";
    oss << "\"priority\":" << priority << ",";
    oss << "\"realtime\":" << (realtime ? "true" : "false") << ",";
    
    // cpu_cores dizisini JSON dizisi olarak yaz
    oss << "\"cpu_cores\":[";
    for (size_t i = 0; i < cpu_cores.size(); ++i) {
        oss << cpu_cores[i];
        // Son elemandan sonra virgül koyma
        if (i < cpu_cores.size() - 1) oss << ",";
    }
    oss << "]}";  // Dizi ve nesne sonu
    
    return oss.str();  // Akışı string'e çevir ve döndür
}

/**
 * ----------------------------------------------------------------------------
 * Task::summary() - Özet Bilgi Metni
 * ----------------------------------------------------------------------------
 * Görevin kısa özetini okunabilir formatta döndürür.
 * Konsol çıktısı veya log kayıtları için idealdir.
 * 
 * ÇIKTI ÖRNEĞİ:
 * [Task#1] camera_capture | CPU | pri=7 | REALTIME
 */
std::string Task::summary() const {
    std::ostringstream oss;
    
    // Temel bilgileri formatla
    oss << "[Task#" << id << "] " << name 
        << " | " << taskTypeToString(type) 
        << " | pri=" << priority;
    
    // Gerçek zamanlı ise etiket ekle
    if (realtime) oss << " | REALTIME";
    
    return oss.str();
}

/**
 * ----------------------------------------------------------------------------
 * Task::isValid() - Geçerlilik Kontrolü
 * ----------------------------------------------------------------------------
 * Görev verilerinin mantıklı ve kullanılabilir olup olmadığını kontrol eder.
 * 
 * KONTROL EDİLEN KURALLAR:
 * 1. Öncelik 0-10 arasında olmalı
 * 2. İsim boş olmamalı
 * 3. CPU çekirdek numaraları 0-5 arasında olmalı (Jetson sınırı)
 * 
 * @return true: Tüm kurallar sağlanıyor, false: En az bir kural ihlali var
 */
bool Task::isValid() const {
    // Kural 1: Öncelik aralık kontrolü
    if (priority < 0 || priority > 10) return false;
    
    // Kural 2: İsim boş olmamalı
    if (name.empty()) return false;
    
    // Kural 3: Her çekirdek numarası geçerli aralıkta olmalı
    for (int core : cpu_cores) {
        if (core < 0 || core > 5) return false;  // Jetson'da 6 çekirdek var (0-5)
    }
    
    return true;  // Tüm kontrollerden geçti
}

} // namespace jts