#ifndef SOSYAL_AG_H
#define SOSYAL_AG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAKS_KULLANICI 100
#define MAKS_KULLANICI_ADI_UZUNLUK 50
#define MAKS_ARKADAS 50

// Renk tanımlamaları
#define KIRMIZI 0
#define SIYAH 1

// Kullanıcı yapısı
typedef struct Kullanici {
    int id;
    char kullaniciAdi[MAKS_KULLANICI_ADI_UZUNLUK];
    int arkadasSayisi;
    struct Kullanici** arkadaslar;
} Kullanici;

// Kırmızı-Siyah ağaç düğümü
typedef struct KSDugum {
    struct Kullanici* kullanici;
    int kullanici_id;
    struct KSDugum* sol;
    struct KSDugum* sag;
    struct KSDugum* ust;
    int renk;
} KSDugum;

// Sosyal ağ yapısı
typedef struct {
    KSDugum* kullanicilar;
    int kullaniciSayisi;
    int maksKullanici;
} SosyalAg;

// Fonksiyon prototipleri
SosyalAg* sosyalAgOlustur();
void kullaniciEkle(SosyalAg* ag, int kullaniciId, const char* kullaniciAdi);
void arkadaslikEkle(SosyalAg* ag, int kullaniciId1, int kullaniciId2);
void belirliMesafedekiArkadaslariBul(SosyalAg* ag, int kullaniciId, int mesafe);
void ortakArkadaslariBul(SosyalAg* ag, int kullaniciId1, int kullaniciId2);
void topluluklariBul(SosyalAg* ag);
float etkiPuaniHesapla(SosyalAg* ag, int kullaniciId);
void enEtkiliKullanicilariBul(SosyalAg* ag, int sayi);
void agiGorsellestir(SosyalAg* ag);
void kirmiziSiyahAgaciGorsellestir(SosyalAg* ag);
void veriSetiniOku(SosyalAg* ag, const char* dosyaAdi);

// Kırmızı-Siyah ağaç işlemleri
void kirmiziSiyahAgaciniDengele(SosyalAg* ag, KSDugum* dugum);
void solDonusum(SosyalAg* ag, KSDugum* x);
void sagDonusum(SosyalAg* ag, KSDugum* y);

#endif // SOSYAL_AG_H 