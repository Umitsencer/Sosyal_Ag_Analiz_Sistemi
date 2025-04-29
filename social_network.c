#include "social_network.h"
#include <math.h>

// Yardımcı fonksiyonlar
void solDonusum(SosyalAg* ag, KSDugum* x) {
    KSDugum* y = x->sag;
    x->sag = y->sol;
    
    if (y->sol != NULL)
        y->sol->ust = x;
    
    y->ust = x->ust;
    
    if (x->ust == NULL)
        ag->kullanicilar = y;
    else if (x == x->ust->sol)
        x->ust->sol = y;
    else
        x->ust->sag = y;
    
    y->sol = x;
    x->ust = y;
}

void sagDonusum(SosyalAg* ag, KSDugum* y) {
    KSDugum* x = y->sol;
    y->sol = x->sag;
    
    if (x->sag != NULL)
        x->sag->ust = y;
    
    x->ust = y->ust;
    
    if (y->ust == NULL)
        ag->kullanicilar = x;
    else if (y == y->ust->sag)
        y->ust->sag = x;
    else
        y->ust->sol = x;
    
    x->sag = y;
    y->ust = x;
}

void kirmiziSiyahAgaciniDengele(SosyalAg* ag, KSDugum* dugum) {
    KSDugum* amca;
    while (dugum != ag->kullanicilar && dugum->ust->renk == KIRMIZI) {
        if (dugum->ust == dugum->ust->ust->sag) {
            amca = dugum->ust->ust->sol;
            if (amca != NULL && amca->renk == KIRMIZI) {
                amca->renk = SIYAH;
                dugum->ust->renk = SIYAH;
                dugum->ust->ust->renk = KIRMIZI;
                dugum = dugum->ust->ust;
            } else {
                if (dugum == dugum->ust->sol) {
                    dugum = dugum->ust;
                    sagDonusum(ag, dugum);
                }
                dugum->ust->renk = SIYAH;
                dugum->ust->ust->renk = KIRMIZI;
                solDonusum(ag, dugum->ust->ust);
            }
        } else {
            // Benzer işlemler sağ taraf için
            amca = dugum->ust->ust->sag;
            if (amca != NULL && amca->renk == KIRMIZI) {
                amca->renk = SIYAH;
                dugum->ust->renk = SIYAH;
                dugum->ust->ust->renk = KIRMIZI;
                dugum = dugum->ust->ust;
            } else {
                if (dugum == dugum->ust->sag) {
                    dugum = dugum->ust;
                    solDonusum(ag, dugum);
                }
                dugum->ust->renk = SIYAH;
                dugum->ust->ust->renk = KIRMIZI;
                sagDonusum(ag, dugum->ust->ust);
            }
        }
    }
    ag->kullanicilar->renk = SIYAH;
}

// Ana fonksiyonlar
SosyalAg* sosyalAgOlustur() {
    SosyalAg* ag = (SosyalAg*)malloc(sizeof(SosyalAg));
    if (!ag) {
        printf("Bellek ayrılamadı!\n");
        return NULL;
    }

    ag->kullanicilar = NULL;
    ag->kullaniciSayisi = 0;
    ag->maksKullanici = MAKS_KULLANICI;
    return ag;
}

void kullaniciEkle(SosyalAg* ag, int kullaniciId, const char* kullaniciAdi) {
    if (ag->kullaniciSayisi >= ag->maksKullanici) {
        printf("Maksimum kullanıcı sayısına ulaşıldı!\n");
        return;
    }

    Kullanici* yeniKullanici = (Kullanici*)malloc(sizeof(Kullanici));
    if (!yeniKullanici) {
        printf("Bellek ayrılamadı!\n");
        return;
    }

    yeniKullanici->id = kullaniciId;
    strncpy(yeniKullanici->kullaniciAdi, kullaniciAdi, MAKS_KULLANICI_ADI_UZUNLUK - 1);
    yeniKullanici->kullaniciAdi[MAKS_KULLANICI_ADI_UZUNLUK - 1] = '\0';
    yeniKullanici->arkadasSayisi = 0;
    yeniKullanici->arkadaslar = (Kullanici**)malloc(MAKS_ARKADAS * sizeof(Kullanici*));

    KSDugum* yeniDugum = (KSDugum*)malloc(sizeof(KSDugum));
    if (!yeniDugum) {
        free(yeniKullanici->arkadaslar);
        free(yeniKullanici);
        printf("Bellek ayrılamadı!\n");
        return;
    }

    yeniDugum->kullanici = yeniKullanici;
    yeniDugum->kullanici_id = kullaniciId;
    yeniDugum->sol = NULL;
    yeniDugum->sag = NULL;
    yeniDugum->ust = NULL;
    yeniDugum->renk = KIRMIZI;

    // Kırmızı-Siyah ağaca ekleme
    KSDugum* simdiki = ag->kullanicilar;
    KSDugum* ust = NULL;

    while (simdiki != NULL) {
        ust = simdiki;
        if (kullaniciId < simdiki->kullanici_id)
            simdiki = simdiki->sol;
        else
            simdiki = simdiki->sag;
    }

    yeniDugum->ust = ust;
    if (ust == NULL)
        ag->kullanicilar = yeniDugum;
    else if (kullaniciId < ust->kullanici_id)
        ust->sol = yeniDugum;
    else
        ust->sag = yeniDugum;

    kirmiziSiyahAgaciniDengele(ag, yeniDugum);
    ag->kullaniciSayisi++;
    printf("Kullanıcı eklendi: %s (ID: %d)\n", kullaniciAdi, kullaniciId);
}

void arkadaslikEkle(SosyalAg* ag, int kullaniciId1, int kullaniciId2) {
    KSDugum* dugum1 = ag->kullanicilar;
    KSDugum* dugum2 = ag->kullanicilar;
    
    // Kullanıcıları bul
    while (dugum1 != NULL && dugum1->kullanici_id != kullaniciId1) {
        if (kullaniciId1 < dugum1->kullanici_id)
            dugum1 = dugum1->sol;
        else
            dugum1 = dugum1->sag;
    }
    
    while (dugum2 != NULL && dugum2->kullanici_id != kullaniciId2) {
        if (kullaniciId2 < dugum2->kullanici_id)
            dugum2 = dugum2->sol;
        else
            dugum2 = dugum2->sag;
    }

    if (!dugum1 || !dugum2) {
        printf("Kullanıcılar bulunamadı!\n");
        return;
    }

    Kullanici* kullanici1 = dugum1->kullanici;
    Kullanici* kullanici2 = dugum2->kullanici;

    // Arkadaşlık ilişkisi ekle
    if (kullanici1->arkadasSayisi < MAKS_ARKADAS && kullanici2->arkadasSayisi < MAKS_ARKADAS) {
        kullanici1->arkadaslar[kullanici1->arkadasSayisi++] = kullanici2;
        kullanici2->arkadaslar[kullanici2->arkadasSayisi++] = kullanici1;
        printf("Arkadaşlık eklendi: %s - %s\n", kullanici1->kullaniciAdi, kullanici2->kullaniciAdi);
    } else {
        printf("Maksimum arkadaş sayısına ulaşıldı!\n");
    }
}

// DFS yardımcı fonksiyonu
static void dfsYardimci(Kullanici* simdikiKullanici, int simdikiMesafe, int hedefMesafe, int kullaniciId, int* ziyaretEdildi, int* mesafeler) {
    if (simdikiMesafe > hedefMesafe) return;
    
    ziyaretEdildi[simdikiKullanici->id] = 1;
    mesafeler[simdikiKullanici->id] = simdikiMesafe;
    
    if (simdikiMesafe == hedefMesafe && simdikiKullanici->id != kullaniciId) {
        printf("%s ", simdikiKullanici->kullaniciAdi);
    }
    
    for (int i = 0; i < simdikiKullanici->arkadasSayisi; i++) {
        Kullanici* arkadas = simdikiKullanici->arkadaslar[i];
        if (!ziyaretEdildi[arkadas->id]) {
            dfsYardimci(arkadas, simdikiMesafe + 1, hedefMesafe, kullaniciId, ziyaretEdildi, mesafeler);
        }
    }
}

void belirliMesafedekiArkadaslariBul(SosyalAg* ag, int kullaniciId, int mesafe) {
    KSDugum* baslangicDugum = ag->kullanicilar;
    while (baslangicDugum != NULL && baslangicDugum->kullanici_id != kullaniciId) {
        if (kullaniciId < baslangicDugum->kullanici_id)
            baslangicDugum = baslangicDugum->sol;
        else
            baslangicDugum = baslangicDugum->sag;
    }

    if (!baslangicDugum) {
        printf("Kullanıcı bulunamadı!\n");
        return;
    }

    // DFS için gerekli yapılar
    int* ziyaretEdildi = (int*)calloc(ag->maksKullanici, sizeof(int));
    int* mesafeler = (int*)calloc(ag->maksKullanici, sizeof(int));

    // DFS başlat
    dfsYardimci(baslangicDugum->kullanici, 0, mesafe, kullaniciId, ziyaretEdildi, mesafeler);
    printf("\n");

    free(ziyaretEdildi);
    free(mesafeler);
}

void ortakArkadaslariBul(SosyalAg* ag, int kullaniciId1, int kullaniciId2) {
    KSDugum* dugum1 = ag->kullanicilar;
    KSDugum* dugum2 = ag->kullanicilar;

    // Kullanıcıları bul
    while (dugum1 != NULL && dugum1->kullanici_id != kullaniciId1) {
        if (kullaniciId1 < dugum1->kullanici_id)
            dugum1 = dugum1->sol;
        else
            dugum1 = dugum1->sag;
    }

    while (dugum2 != NULL && dugum2->kullanici_id != kullaniciId2) {
        if (kullaniciId2 < dugum2->kullanici_id)
            dugum2 = dugum2->sol;
        else
            dugum2 = dugum2->sag;
    }

    if (!dugum1 || !dugum2) {
        printf("Kullanıcılar bulunamadı!\n");
        return;
    }

    Kullanici* kullanici1 = dugum1->kullanici;
    Kullanici* kullanici2 = dugum2->kullanici;

    // Ortak arkadaşları bul
    printf("Ortak arkadaşlar:\n");
    for (int i = 0; i < kullanici1->arkadasSayisi; i++) {
        for (int j = 0; j < kullanici2->arkadasSayisi; j++) {
            if (kullanici1->arkadaslar[i]->id == kullanici2->arkadaslar[j]->id) {
                printf("%s ", kullanici1->arkadaslar[i]->kullaniciAdi);
                break;
            }
        }
    }
    printf("\n");
}

void topluluklariBul(SosyalAg* ag) {
    int* ziyaretEdildi = (int*)calloc(ag->maksKullanici, sizeof(int));
    int* topluluk = (int*)malloc(ag->maksKullanici * sizeof(int));
    int toplulukSayisi = 0;

    // Her kullanıcı için
    KSDugum* simdiki = ag->kullanicilar;
    while (simdiki != NULL) {
        int kullaniciId = simdiki->kullanici_id;
        if (!ziyaretEdildi[kullaniciId]) {
            toplulukSayisi++;
            printf("Topluluk %d: ", toplulukSayisi);

            // DFS için gerekli yapılar
            Kullanici** yigin = (Kullanici**)malloc(ag->maksKullanici * sizeof(Kullanici*));
            int yiginUst = 0;

            // Başlangıç kullanıcısını işle
            yigin[yiginUst++] = simdiki->kullanici;
            ziyaretEdildi[kullaniciId] = 1;
            topluluk[kullaniciId] = toplulukSayisi;
            printf("%s ", simdiki->kullanici->kullaniciAdi);

            // Topluluğu bul (DFS ile)
            while (yiginUst > 0) {
                Kullanici* simdikiKullanici = yigin[--yiginUst];
                
                for (int i = 0; i < simdikiKullanici->arkadasSayisi; i++) {
                    Kullanici* arkadas = simdikiKullanici->arkadaslar[i];
                    if (!ziyaretEdildi[arkadas->id]) {
                        ziyaretEdildi[arkadas->id] = 1;
                        topluluk[arkadas->id] = toplulukSayisi;
                        yigin[yiginUst++] = arkadas;
                        printf("%s ", arkadas->kullaniciAdi);
                    }
                }
            }
            printf("\n");
            free(yigin);
        }
        simdiki = simdiki->sag;
    }

    // Topluluk istatistiklerini hesapla
    int* toplulukBoyutlari = (int*)calloc(toplulukSayisi + 1, sizeof(int));
    float* toplulukYogunluklari = (float*)calloc(toplulukSayisi + 1, sizeof(float));
    
    // Topluluk boyutlarını hesapla
    for (int i = 1; i <= ag->maksKullanici; i++) {
        if (ziyaretEdildi[i]) {
            toplulukBoyutlari[topluluk[i]]++;
        }
    }
    
    // Topluluk yoğunluklarını hesapla
    simdiki = ag->kullanicilar;
    while (simdiki != NULL) {
        int toplulukNo = topluluk[simdiki->kullanici_id];
        int icBaglantilar = 0;
        
        // Topluluk içi bağlantıları say
        for (int i = 0; i < simdiki->kullanici->arkadasSayisi; i++) {
            if (topluluk[simdiki->kullanici->arkadaslar[i]->id] == toplulukNo) {
                icBaglantilar++;
            }
        }
        
        // Yoğunluğu hesapla
        int maksOlasiBaglantilar = (toplulukBoyutlari[toplulukNo] * (toplulukBoyutlari[toplulukNo] - 1)) / 2;
        if (maksOlasiBaglantilar > 0) {
            toplulukYogunluklari[toplulukNo] += (float)icBaglantilar / maksOlasiBaglantilar;
        }
        
        simdiki = simdiki->sag;
    }
    
    // Topluluk istatistiklerini yazdır
    printf("\nTopluluk İstatistikleri:\n");
    for (int i = 1; i <= toplulukSayisi; i++) {
        printf("Topluluk %d: %d kullanıcı, ortalama yoğunluk: %.2f\n",
               i, toplulukBoyutlari[i], toplulukYogunluklari[i] / toplulukBoyutlari[i]);
    }

    free(ziyaretEdildi);
    free(topluluk);
    free(toplulukBoyutlari);
    free(toplulukYogunluklari);
}

float etkiPuaniHesapla(SosyalAg* ag, int kullaniciId) {
    KSDugum* dugum = ag->kullanicilar;
    while (dugum != NULL && dugum->kullanici_id != kullaniciId) {
        if (kullaniciId < dugum->kullanici_id)
            dugum = dugum->sol;
        else
            dugum = dugum->sag;
    }

    if (!dugum) {
        printf("Kullanıcı bulunamadı!\n");
        return 0.0f;
    }

    Kullanici* kullanici = dugum->kullanici;
    
    // 1. Doğrudan arkadaş sayısı (ağırlık: 0.3)
    float dogrudan = (float)kullanici->arkadasSayisi;
    float maksArkadas = 0.0f;
    
    // Maksimum arkadaş sayısını bul
    KSDugum* gecici = ag->kullanicilar;
    while (gecici != NULL) {
        if (gecici->kullanici->arkadasSayisi > maksArkadas) {
            maksArkadas = (float)gecici->kullanici->arkadasSayisi;
        }
        gecici = gecici->sag;
    }
    
    float normalizeDogrudan = maksArkadas > 0 ? dogrudan / maksArkadas : 0.0f;
    
    // 2. İkinci derece bağlantılar (ağırlık: 0.2)
    int ikinciDereceBaglantilar = 0;
    int* ziyaretEdildi = (int*)calloc(MAKS_KULLANICI, sizeof(int));
    ziyaretEdildi[kullaniciId] = 1;
    
    for (int i = 0; i < kullanici->arkadasSayisi; i++) {
        Kullanici* arkadas = kullanici->arkadaslar[i];
        ziyaretEdildi[arkadas->id] = 1;
        
        for (int j = 0; j < arkadas->arkadasSayisi; j++) {
            if (!ziyaretEdildi[arkadas->arkadaslar[j]->id]) {
                ikinciDereceBaglantilar++;
                ziyaretEdildi[arkadas->arkadaslar[j]->id] = 1;
            }
        }
    }
    
    float normalizeIkinciDerece = ikinciDereceBaglantilar / (float)(MAKS_KULLANICI - 1);
    
    // 3. Topluluk içi etki (ağırlık: 0.3)
    int* topluluk = (int*)calloc(MAKS_KULLANICI, sizeof(int));
    int toplulukId = 1;
    topluluk[kullaniciId] = toplulukId;
    
    // Kullanıcının topluluğunu bul
    for (int i = 0; i < kullanici->arkadasSayisi; i++) {
        topluluk[kullanici->arkadaslar[i]->id] = toplulukId;
    }
    
    // Topluluk içi bağlantıları say
    int icBaglantilar = 0;
    for (int i = 0; i < kullanici->arkadasSayisi; i++) {
        Kullanici* arkadas = kullanici->arkadaslar[i];
        for (int j = 0; j < arkadas->arkadasSayisi; j++) {
            if (topluluk[arkadas->arkadaslar[j]->id] == toplulukId) {
                icBaglantilar++;
            }
        }
    }
    
    float toplulukEtkisi = (float)icBaglantilar / (kullanici->arkadasSayisi * (kullanici->arkadasSayisi + 1));
    
    // 4. Merkezilik (ağırlık: 0.2)
    float merkezilik = 0.0f;
    if (kullanici->arkadasSayisi > 0) {
        int toplamBaglantilar = 0;
        for (int i = 0; i < kullanici->arkadasSayisi; i++) {
            toplamBaglantilar += kullanici->arkadaslar[i]->arkadasSayisi;
        }
        merkezilik = (float)toplamBaglantilar / (kullanici->arkadasSayisi * MAKS_ARKADAS);
    }
    
    // Etki puanını hesapla
    float etkiPuani = (normalizeDogrudan * 0.3f) +
                      (normalizeIkinciDerece * 0.2f) +
                      (toplulukEtkisi * 0.3f) +
                      (merkezilik * 0.2f);
    
    free(ziyaretEdildi);
    free(topluluk);
    
    return etkiPuani;
}

void agiGorsellestir(SosyalAg* ag) {
    FILE* dosya = fopen("ag.dot", "w");
    if (!dosya) {
        printf("Dosya oluşturulamadı!\n");
        return;
    }

    fprintf(dosya, "graph SosyalAg {\n");
    fprintf(dosya, "    rankdir=LR;\n");
    fprintf(dosya, "    node [shape=circle, style=filled, fillcolor=lightblue, fontname=\"Arial\"];\n");
    fprintf(dosya, "    edge [color=gray, penwidth=1.5];\n\n");

    // Etki puanlarını hesapla
    float* etkiPuanlari = (float*)malloc(MAKS_KULLANICI * sizeof(float));
    int* kullaniciIdleri = (int*)malloc(MAKS_KULLANICI * sizeof(int));
    int kullaniciSayisi = 0;

    KSDugum* simdiki = ag->kullanicilar;
    while (simdiki != NULL) {
        etkiPuanlari[kullaniciSayisi] = etkiPuaniHesapla(ag, simdiki->kullanici_id);
        kullaniciIdleri[kullaniciSayisi++] = simdiki->kullanici_id;
        simdiki = simdiki->sag;
    }

    // Maksimum etki puanını bul
    float maksEtki = 0.0f;
    for (int i = 0; i < kullaniciSayisi; i++) {
        if (etkiPuanlari[i] > maksEtki) {
            maksEtki = etkiPuanlari[i];
        }
    }

    // Düğümleri yaz
    simdiki = ag->kullanicilar;
    while (simdiki != NULL) {
        float etki = etkiPuaniHesapla(ag, simdiki->kullanici_id);
        float normalBoyut = 0.5 + (etki / maksEtki) * 1.5;
        
        fprintf(dosya, "    %d [label=\"%s\\n(%.2f)\", width=%.2f, height=%.2f, fillcolor=\"%s\"];\n",
                simdiki->kullanici_id,
                simdiki->kullanici->kullaniciAdi,
                etki,
                normalBoyut,
                normalBoyut,
                etki > maksEtki/2 ? "lightgreen" : "lightblue");
        
        simdiki = simdiki->sag;
    }

    // Kenarları yaz
    simdiki = ag->kullanicilar;
    while (simdiki != NULL) {
        for (int i = 0; i < simdiki->kullanici->arkadasSayisi; i++) {
            if (simdiki->kullanici_id < simdiki->kullanici->arkadaslar[i]->id) {
                fprintf(dosya, "    %d -- %d;\n",
                        simdiki->kullanici_id,
                        simdiki->kullanici->arkadaslar[i]->id);
            }
        }
        simdiki = simdiki->sag;
    }

    fprintf(dosya, "}\n");
    fclose(dosya);

    // Görselleştirmeyi oluştur
    system("dot -Tpng ag.dot -o ag.png");
    printf("Sosyal ağ görselleştirmesi ag.png dosyasına kaydedildi.\n");

    free(etkiPuanlari);
    free(kullaniciIdleri);
}

void kirmiziSiyahAgaciGorsellestir(SosyalAg* ag) {
    FILE* dosya = fopen("agac.dot", "w");
    if (!dosya) {
        printf("Dosya oluşturulamadı!\n");
        return;
    }

    fprintf(dosya, "digraph KirmiziSiyahAgac {\n");
    fprintf(dosya, "    rankdir=TB;\n");
    fprintf(dosya, "    node [shape=circle, style=filled, fontname=\"Arial\"];\n");
    fprintf(dosya, "    edge [color=black, penwidth=1.5];\n\n");
    
    dugumYaz(dosya, ag->kullanicilar);
    
    fprintf(dosya, "}\n");
    fclose(dosya);

    // Görselleştirmeyi oluştur
    system("dot -Tpng agac.dot -o agac.png");
    printf("Kırmızı-Siyah ağaç görselleştirmesi agac.png dosyasına kaydedildi.\n");
}

void dugumYaz(FILE* f, KSDugum* dugum) {
    if (dugum == NULL) return;
    
    // Düğüm boyutunu arkadaş sayısına göre hesapla
    float boyut = 0.5 + (dugum->kullanici->arkadasSayisi / (float)MAKS_ARKADAS) * 1.5;
    
    fprintf(f, "    %d [label=\"%s\\n(%d arkadaş)\", width=%.2f, height=%.2f, fillcolor=%s, fontcolor=white];\n",
            dugum->kullanici_id,
            dugum->kullanici->kullaniciAdi,
            dugum->kullanici->arkadasSayisi,
            boyut,
            boyut,
            dugum->renk == KIRMIZI ? "red" : "black");
    
    if (dugum->sol) {
        fprintf(f, "    %d -> %d [color=black];\n", dugum->kullanici_id, dugum->sol->kullanici_id);
        dugumYaz(f, dugum->sol);
    }
    if (dugum->sag) {
        fprintf(f, "    %d -> %d [color=black];\n", dugum->kullanici_id, dugum->sag->kullanici_id);
        dugumYaz(f, dugum->sag);
    }
}

void veriSetiniOku(SosyalAg* ag, const char* dosyaAdi) {
    FILE* dosya = fopen(dosyaAdi, "r");
    if (!dosya) {
        printf("Veri seti dosyası açılamadı: %s\n", dosyaAdi);
        return;
    }

    char satir[256];
    char kullaniciAdi[50];
    int kullaniciId;

    while (fgets(satir, sizeof(satir), dosya)) {
        // Boş satırları ve yorumları atla
        if (satir[0] == '\n' || satir[0] == '#')
            continue;

        // Kullanıcı ekleme
        if (sscanf(satir, "USER %d", &kullaniciId) == 1) {
            sprintf(kullaniciAdi, "Kullanici%d", kullaniciId);
            kullaniciEkle(ag, kullaniciId, kullaniciAdi);
        }
        // Arkadaşlık ilişkisi ekleme
        else if (strncmp(satir, "FRIEND ", 7) == 0) {
            char kullanici1[50], kullanici2[50];
            if (sscanf(satir, "FRIEND %s %s", kullanici1, kullanici2) == 2) {
                // Kullanıcı adlarından ID'leri çıkar
                int id1, id2;
                if (sscanf(kullanici1, "Kullanici%d", &id1) == 1 && 
                    sscanf(kullanici2, "Kullanici%d", &id2) == 1) {
                    arkadaslikEkle(ag, id1, id2);
                }
            }
        }
    }

    fclose(dosya);
    printf("Veri seti başarıyla okundu: %s\n", dosyaAdi);
}

// Kullanıcı etki puanı yapısı
typedef struct {
    int kullaniciId;
    float etkiPuani;
} KullaniciEtki;

// Kırmızı-Siyah ağaçtaki tüm kullanıcıları dolaşan yardımcı fonksiyon
void kullanicilariDolasVeEtkiHesapla(SosyalAg* ag, KullaniciEtki* etkiler, int* kullaniciSayisi) {
    kullanicilariDolasVeEtkiHesaplaYardimci(ag, ag->kullanicilar, etkiler, kullaniciSayisi);
}

// Kırmızı-Siyah ağaçtaki tüm kullanıcıları dolaşan yardımcı fonksiyon (recursive)
void kullanicilariDolasVeEtkiHesaplaYardimci(SosyalAg* ag, KSDugum* dugum, KullaniciEtki* etkiler, int* kullaniciSayisi) {
    if (dugum == NULL) return;
    
    kullanicilariDolasVeEtkiHesaplaYardimci(ag, dugum->sol, etkiler, kullaniciSayisi);
    
    // Kullanıcının etki puanını hesapla
    float puan = etkiPuaniHesapla(ag, dugum->kullanici_id);
    etkiler[*kullaniciSayisi].kullaniciId = dugum->kullanici_id;
    etkiler[*kullaniciSayisi].etkiPuani = puan;
    (*kullaniciSayisi)++;
    
    kullanicilariDolasVeEtkiHesaplaYardimci(ag, dugum->sag, etkiler, kullaniciSayisi);
}

void enEtkiliKullanicilariBul(SosyalAg* ag, int sayi) {
    if (!ag || sayi <= 0) {
        printf("Geçersiz parametreler!\n");
        return;
    }

    // Tüm kullanıcıların etki puanlarını hesapla ve sakla
    KullaniciEtki* etkiler = (KullaniciEtki*)malloc(MAKS_KULLANICI * sizeof(KullaniciEtki));
    int kullaniciSayisi = 0;

    // Kırmızı-Siyah ağaçtaki tüm kullanıcıları dolaş
    kullanicilariDolasVeEtkiHesapla(ag, etkiler, &kullaniciSayisi);

    // Etki puanlarına göre sırala (bubble sort)
    for (int i = 0; i < kullaniciSayisi - 1; i++) {
        for (int j = 0; j < kullaniciSayisi - i - 1; j++) {
            if (etkiler[j].etkiPuani < etkiler[j + 1].etkiPuani) {
                KullaniciEtki gecici = etkiler[j];
                etkiler[j] = etkiler[j + 1];
                etkiler[j + 1] = gecici;
            }
        }
    }

    // En etkili kullanıcıları yazdır
    printf("\nEn etkili %d kullanıcı:\n", sayi);
    
    // Eğer toplam kullanıcı sayısı 5'ten fazlaysa, ilk 3 ve son 2 kullanıcıyı göster
    if (kullaniciSayisi > 5) {
        // İlk 3 kullanıcıyı göster
        for (int i = 0; i < 3 && i < kullaniciSayisi; i++) {
            KSDugum* dugum = ag->kullanicilar;
            while (dugum != NULL && dugum->kullanici_id != etkiler[i].kullaniciId) {
                if (etkiler[i].kullaniciId < dugum->kullanici_id)
                    dugum = dugum->sol;
                else
                    dugum = dugum->sag;
            }
            
            if (dugum != NULL) {
                printf("%d. %s (Etki Puanı: %.2f)\n", 
                       i + 1, 
                       dugum->kullanici->kullaniciAdi, 
                       etkiler[i].etkiPuani);
            }
        }
        
        // Aradaki kullanıcıları "..." ile göster
        printf("...\n");
        
        // Son 2 kullanıcıyı göster
        for (int i = kullaniciSayisi - 2; i < kullaniciSayisi; i++) {
            KSDugum* dugum = ag->kullanicilar;
            while (dugum != NULL && dugum->kullanici_id != etkiler[i].kullaniciId) {
                if (etkiler[i].kullaniciId < dugum->kullanici_id)
                    dugum = dugum->sol;
                else
                    dugum = dugum->sag;
            }
            
            if (dugum != NULL) {
                printf("%d. %s (Etki Puanı: %.2f)\n", 
                       i + 1, 
                       dugum->kullanici->kullaniciAdi, 
                       etkiler[i].etkiPuani);
            }
        }
    } else {
        // Toplam kullanıcı sayısı 5 veya daha azsa, tüm kullanıcıları göster
        for (int i = 0; i < kullaniciSayisi; i++) {
            KSDugum* dugum = ag->kullanicilar;
            while (dugum != NULL && dugum->kullanici_id != etkiler[i].kullaniciId) {
                if (etkiler[i].kullaniciId < dugum->kullanici_id)
                    dugum = dugum->sol;
                else
                    dugum = dugum->sag;
            }
            
            if (dugum != NULL) {
            printf("%d. %s (Etki Puanı: %.2f)\n", 
                   i + 1, 
                       dugum->kullanici->kullaniciAdi, 
                       etkiler[i].etkiPuani);
            }
        }
    }

    free(etkiler);
}

// Toplulukları görselleştirme fonksiyonu
void topluluklariGorsellestir(SosyalAg* ag) {
    FILE* dosya = fopen("topluluklar.dot", "w");
    if (!dosya) {
        printf("Dosya oluşturulamadı!\n");
        return;
    }

    fprintf(dosya, "graph Topluluklar {\n");
    fprintf(dosya, "    rankdir=LR;\n");
    fprintf(dosya, "    node [shape=circle, style=filled, fontname=\"Arial\"];\n");
    fprintf(dosya, "    edge [color=gray, penwidth=1.5];\n\n");

    // Farklı topluluklar için farklı renkler kullan
    const char* renkler[] = {"lightblue", "lightgreen", "lightpink", "lightyellow", "lightgray"};
    int renkIndeksi = 0;

    // Basit topluluk tespiti (daha gelişmiş algoritmalarla geliştirilebilir)
    KSDugum* simdiki = ag->kullanicilar;
    while (simdiki != NULL) {
        // Arkadaş sayısına göre renk ata (basit topluluk tespiti)
        renkIndeksi = (simdiki->kullanici->arkadasSayisi / 5) % 5;
        
        fprintf(dosya, "    %d [label=\"%s\", fillcolor=\"%s\"];\n",
                simdiki->kullanici_id,
                simdiki->kullanici->kullaniciAdi,
                renkler[renkIndeksi]);

        // Kenarları yaz
        for (int i = 0; i < simdiki->kullanici->arkadasSayisi; i++) {
            if (simdiki->kullanici_id < simdiki->kullanici->arkadaslar[i]->id) {
                fprintf(dosya, "    %d -- %d;\n",
                        simdiki->kullanici_id,
                        simdiki->kullanici->arkadaslar[i]->id);
            }
        }
        simdiki = simdiki->sag;
    }

    fprintf(dosya, "}\n");
    fclose(dosya);

    // Görselleştirmeyi oluştur
    system("dot -Tpng topluluklar.dot -o topluluklar.png");
    printf("Topluluk görselleştirmesi topluluklar.png dosyasına kaydedildi.\n");
}

void etkiPuanlariniGorsellestir(SosyalAg* ag) {
    FILE* dosya = fopen("etki.dot", "w");
    if (!dosya) {
        printf("Dosya oluşturulamadı!\n");
        return;
    }

    fprintf(dosya, "digraph EtkiPuanlari {\n");
    fprintf(dosya, "    rankdir=LR;\n");
    fprintf(dosya, "    node [shape=box, style=filled, fontname=\"Arial\"];\n");
    fprintf(dosya, "    edge [style=invis];\n\n");

    // Tüm kullanıcıların etki puanlarını hesapla
    float* etkiPuanlari = (float*)malloc(MAKS_KULLANICI * sizeof(float));
    char** kullaniciAdlari = (char**)malloc(MAKS_KULLANICI * sizeof(char*));
    int kullaniciSayisi = 0;

    // Ağaçtaki tüm kullanıcıları dolaş
    KSDugum* simdiki = ag->kullanicilar;
    while (simdiki != NULL) {
        etkiPuanlari[kullaniciSayisi] = etkiPuaniHesapla(ag, simdiki->kullanici_id);
        kullaniciAdlari[kullaniciSayisi] = simdiki->kullanici->kullaniciAdi;
        kullaniciSayisi++;
        simdiki = simdiki->sag;
    }

    // Maksimum etki puanını bul
    float maksEtki = 0.0f;
    for (int i = 0; i < kullaniciSayisi; i++) {
        if (etkiPuanlari[i] > maksEtki) {
            maksEtki = etkiPuanlari[i];
        }
    }

    // Düğümleri çubuk olarak yaz
    for (int i = 0; i < kullaniciSayisi; i++) {
        float normalYukseklik = 0.2 + (etkiPuanlari[i] / maksEtki) * 2.0;  // Yükseklik 0.2 ile 2.2 arasında
        fprintf(dosya, "    %d [label=\"%s\\n(%.2f)\", width=1.0, height=%.2f, fillcolor=\"%s\"];\n",
                i,
                kullaniciAdlari[i],
                etkiPuanlari[i],
                normalYukseklik,
                etkiPuanlari[i] > maksEtki/2 ? "lightgreen" : "lightblue");
    }

    // Sırayı korumak için görünmez kenarlar ekle
    for (int i = 0; i < kullaniciSayisi - 1; i++) {
        fprintf(dosya, "    %d -> %d;\n", i, i + 1);
    }

    fprintf(dosya, "}\n");
    fclose(dosya);

    // Görselleştirmeyi oluştur
    system("dot -Tpng etki.dot -o etki.png");
    printf("Etki puanları görselleştirmesi etki.png dosyasına kaydedildi.\n");

    free(etkiPuanlari);
    free(kullaniciAdlari);
} 