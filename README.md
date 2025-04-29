# Sosyal Ağ Analiz Sistemi

## 📝 Proje Hakkında

Bu proje, sosyal ağları analiz etmek için gelişmiş veri yapıları ve algoritmalar kullanan bir C uygulamasıdır. Kullanıcılar arasındaki ilişkileri modelleyerek, topluluk yapılarını analiz eder ve etki alanlarını hesaplar.

## ✨ Özellikler

- 🌳 İlişki ağacı oluşturma ve yönetme
- 🔍 Derinlik öncelikli arama ile arkadaş bulma
- 👥 Ortak arkadaş analizi
- 🏘️ Topluluk tespiti
- 📊 Etki alanı hesaplama
- ⚡ Kırmızı-siyah ağaç ile optimize edilmiş kullanıcı araması

## 🛠️ Gereksinimler

- GCC (GNU Compiler Collection)
- Graphviz (görselleştirme için)
- Make

## 🚀 Kurulum

1. Graphviz'i yükleyin:
   - Windows: [Graphviz İndirme Sayfası](https://graphviz.org/download/)
   - Linux: `sudo apt-get install graphviz`
   - macOS: `brew install graphviz`

2. Projeyi derleyin:
```bash
make
```

3. Programı çalıştırın:
```bash
./social_network
```

## 📁 Proje Yapısı

```
.
├── social_network.h    # Başlık dosyası
├── social_network.c    # Kaynak kod dosyası
├── main.c             # Ana program
└── Makefile           # Derleme yapılandırması
```

## 💻 Veri Yapıları

### 1. User (Kullanıcı)
- Kullanıcı ID
- İsim
- Arkadaş listesi
- Etki skoru

### 2. RBNode (Kırmızı-Siyah Ağaç Düğümü)
- Anahtar (key)
- Değer (value)
- Sol ve sağ çocuklar
- Renk bilgisi

### 3. SocialNetwork (Sosyal Ağ)
- Kullanıcı listesi
- Kullanıcı sayısı
- Kırmızı-Siyah Ağaç

## 📊 Örnek Kullanım

```c
// Ortak arkadaşları bulma
common_friends = network.find_common_friends(1, 2);

// Toplulukları tespit etme
communities = network.detect_communities();

// Etki skorunu hesaplama
influence = network.calculate_influence(1);
```

## 🛠️ Derleme Seçenekleri

- `make`: Projeyi derler
- `make clean`: Derleme dosyalarını temizler

## 💾 Çıktılar

Program çalıştırıldığında:
- Sosyal ağın görselleştirmesi (`social_network.png`)
- Örnek analiz sonuçları (konsol çıktısı)
- Oluşturulan veri seti (`social_network_data.txt`)

## 🔒 Bellek Yönetimi

Program, dinamik bellek yönetimi kullanır ve tüm kaynakları düzgün şekilde serbest bırakır:
- `malloc()` ve `realloc()` ile bellek tahsisi
- `free()` ile bellek temizliği
- Bellek sızıntılarını önleyen temizleme fonksiyonları

## 📝 Lisans

Eğitim amaçlı bir çalışmadır.



