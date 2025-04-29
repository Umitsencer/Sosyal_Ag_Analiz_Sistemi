#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "social_network.h"

void ornekVeriOlustur(SosyalAg* ag, int kullaniciSayisi, int ortArkadas) {
    char kullaniciAdi[50];
    for (int i = 1; i <= kullaniciSayisi; i++) {
        sprintf(kullaniciAdi, "Kullanici%d", i);
        kullaniciEkle(ag, i, kullaniciAdi);
    }

    // Rastgele arkadaşlık ilişkileri oluştur
    for (int i = 1; i <= kullaniciSayisi; i++) {
        int arkadasSayisi = rand() % (ortArkadas * 2) + 1;  // 1 ile 2*ortArkadas arası
        for (int j = 0; j < arkadasSayisi; j++) {
            int arkadasId = rand() % kullaniciSayisi + 1;
            if (arkadasId != i) {
                arkadaslikEkle(ag, i, arkadasId);
            }
        }
    }
}

void veriSetiniKaydet(SosyalAg* ag, const char* dosyaAdi) {
    FILE* dosya = fopen(dosyaAdi, "w");
    if (!dosya) {
        printf("Veri seti dosyası oluşturulamadı!\n");
        return;
    }

    // Kullanıcıları kaydet
    KSDugum* simdiki = ag->kullanicilar;
    while (simdiki != NULL) {
        fprintf(dosya, "USER %d %s\n", simdiki->kullanici_id, simdiki->kullanici->kullaniciAdi);
        simdiki = simdiki->sag;
    }

    // Arkadaşlık ilişkilerini kaydet
    simdiki = ag->kullanicilar;
    while (simdiki != NULL) {
        for (int i = 0; i < simdiki->kullanici->arkadasSayisi; i++) {
            if (simdiki->kullanici_id < simdiki->kullanici->arkadaslar[i]->id) {
                fprintf(dosya, "FRIEND %s %s\n", 
                        simdiki->kullanici->kullaniciAdi,
                        simdiki->kullanici->arkadaslar[i]->kullaniciAdi);
            }
        }
        simdiki = simdiki->sag;
    }

    fclose(dosya);
    printf("Veri seti %s dosyasına kaydedildi.\n", dosyaAdi);
}

int main() {
    setlocale(LC_ALL, "");
    srand(time(NULL));  // Rastgele sayı üreteci için seed

    // Sosyal ağ oluştur
    SosyalAg* ag = sosyalAgOlustur();
    if (!ag) {
        printf("Sosyal ağ oluşturulamadı!\n");
        return 1;
    }

    // Örnek veri oluştur
    printf("Örnek veri oluşturuluyor...\n");
    ornekVeriOlustur(ag, 20, 5);
    
    // Veri setini kaydet
    veriSetiniKaydet(ag, "veriseti.txt");
    
    // 1. İlişki Ağacı ve DFS ile Arkadaş Arama
    printf("\n1. İlişki Ağacı ve DFS ile Arkadaş Arama:\n");
    printf("2 mesafedeki arkadaşlar (Kullanici1 için):\n");
    belirliMesafedekiArkadaslariBul(ag, 1, 2);
    
    // 2. Ortak Arkadaş Analizi
    printf("\n2. Ortak Arkadaş Analizi:\n");
    printf("Kullanici1 ve Kullanici4 arasındaki ortak arkadaşlar:\n");
    ortakArkadaslariBul(ag, 1, 4);
    
    // 3. Topluluk Tespiti
    printf("\n3. Topluluk Tespiti:\n");
    topluluklariBul(ag);
    
    // 4. Etki Alanı Hesaplama
    printf("\n4. Etki Alanı Hesaplama:\n");
    printf("Kullanıcı etki puanları:\n");
    for (int i = 1; i <= 5; i++) {
        float puan = etkiPuaniHesapla(ag, i);
        printf("Kullanici%d'nin etki puanı: %.2f\n", i, puan);
    }
    
    // 5. En Etkili Kullanıcıları Bul
    printf("\n5. En Etkili Kullanıcılar:\n");
    enEtkiliKullanicilariBul(ag, 5);
    
    // 6. Görselleştirmeler
    printf("\n6. Görselleştirmeler oluşturuluyor...\n");
    agiGorsellestir(ag);
    kirmiziSiyahAgaciGorsellestir(ag);
    topluluklariGorsellestir(ag);
    etkiPuanlariniGorsellestir(ag);
    
    printf("\nTüm işlemler tamamlandı!\n");
    printf("Oluşturulan görselleştirmeler:\n");
    printf("- ag.png: Sosyal ağ görselleştirmesi\n");
    printf("- agac.png: Kırmızı-Siyah ağaç görselleştirmesi\n");
    printf("- topluluklar.png: Topluluk görselleştirmesi\n");
    printf("- etki.png: Etki puanları görselleştirmesi\n");
    
    return 0;
} 