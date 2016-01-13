#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

struct Temsilci
{
  char isim[20];
  int mesgul; //Temsilcinin dolu olup olmadigi bilgisini verecek. 0 ise bos, herhangi bir sayi ise musteri isim'i tutuyordur.
};

struct Musteri
{
  int isim;
  int gelmeSuresi;
  int beklemeSuresi;
  int aktif; // 0 : false; 1 :true. Gelip gelmedigini kontrol amacli kullanilir.
  int eskiGelmeSuresi; // Istatistik icin tutulur. Bir onceki gelme suresini saklar.
};

typedef struct Dugum{ // Kuyruk icin kullanilacak dugum yapisi ve tip tanimlamalari yapilir.
    struct Musteri kMusteri; // Data olarak Musteri struct'indan bir yapi tutacak.
    struct Dugum* ileri;
} Dugum;

typedef struct Kuyruk {
    Dugum* ilk;
    Dugum* son;
    void (*kuyrugaEkle) (struct Kuyruk*, int);
    struct Musteri (*cikar) (struct Kuyruk*);
    // Hangi elemanin cikarildigi bilgisini verebilmek icin donus tipi Musteri yapildi. (Test amacli.)
    void (*yaz) (struct Kuyruk*);
    int kayit; // Kuyruktaki kayit sayisini tutan degisken.
    int maxKayit; // Kuyruktaki max kisi sayisini tutacak olan degisken.
} Kuyruk;

void kuyrugaEkle (Kuyruk* kuyruk, struct Musteri _musteri) {
    if(kuyruk->kayit > 100) {
    	printf("Kuyruk doldugu icin malesef daha fazla kisi eklenemiyor!\n");
    }
    else {
	    Dugum* d = (Dugum*) malloc (sizeof(Dugum)); // Yeni bir dugum icin yer ayrilir.
	    d->kMusteri = _musteri; // Gelen musteri datasi dugume eklendi.
	    d->ileri = NULL; // Bir sonraki gosterecegi eleman null yapildi.

	    if (kuyruk->ilk == NULL) { // Eger kuyrukta bir veri yoksa,
	        kuyruk->ilk = d; // Olusturulan dugum kuyrugun ilk elemani olarak atandi.
	    } else { // Eger kuyrukta bir veri varsa,
	        kuyruk->son->ileri = d; // Son elemanin bir ilerisine atandi.
	    }
	    kuyruk->son = d; // Gelen dugum son eleman olarak belirlendi.
	    kuyruk->kayit++; // Kuyruktaki kayit sayisi bir arttirildi.

	    // Kuyruktaki max kayit sayisini bulmak icin...
	    if(kuyruk->kayit > kuyruk->maxKayit) {
	    	kuyruk->maxKayit = kuyruk->kayit;
	    }
    }
}

struct Musteri cikar (Kuyruk* kuyruk) {
    Dugum* ilk = kuyruk->ilk; // Kuyrugun ilk elemani alindi.
    struct Musteri _m; // Verilerini yedeklemek icin bir Musteri degiskeni olusturuldu. (Test amacli)
    _m = ilk->kMusteri;
    kuyruk->ilk = ilk->ileri; // Cikarilacak olan elemanin bir ilerisindeki eleman kuyrugun ilk elemani olarak belirlendi.
    kuyruk->kayit--; // Kayit sayisi bir azaltildi.
    free(ilk); // Bellek alani serbest birakildi.
    return _m; // Cikartilan Musteri datalari donduruldu. (Test amacli.)
}

/* yaz() fonksiyonu test amacli olusturulmustur... */
void yaz (Kuyruk* kuyruk) {
    printf("\nMusteriler: ");
    if (kuyruk->kayit == 0)
        printf("Kuyrukta hic kayit yok.\n");
    else {
        Dugum* ilk = kuyruk->ilk;
        int i, kayit = kuyruk->kayit;
        printf("%d kayit bulundu:\n", kuyruk->kayit);
        for (i = 0; i < kayit; i++) {
            if (i > 0)
                printf(", ");
            printf("Musteri#%d", ilk->kMusteri.isim);
            ilk = ilk->ileri;
        }
    }
    printf("\n\n");
}
/* yaz() fonksiyonu test amacli olusturulmustur... */

Kuyruk kuyrukOlustur () {
    /* Olusturulacak olan yeni bir kuyruk yapisi icin ilk atamalar ve tanimlamalar yapiliyor. */
    Kuyruk kuyruk;
    kuyruk.kayit = 0;
    kuyruk.maxKayit = 0;
    kuyruk.ilk = NULL;
    kuyruk.son = NULL;
    kuyruk.kuyrugaEkle = &kuyrugaEkle;
    kuyruk.cikar = &cikar;
    kuyruk.yaz = &yaz;
    return kuyruk;
}

int main()
{
  Kuyruk kuyruk = kuyrukOlustur();
  //kuyruk.yaz(&kuyruk);
  struct Temsilci temsilci1, temsilci2, temsilci3;
  strcpy(temsilci1.isim, "Temsilci#1");
  temsilci1.mesgul = 0;
  strcpy(temsilci2.isim, "Temsilci#2");
  temsilci2.mesgul = 0;
  strcpy(temsilci3.isim, "Temsilci#3");
  temsilci3.mesgul = 0;
  //printf("Temsilci isimleri: %s-%s-%s \n", temsilci1.isim, temsilci2.isim, temsilci3.isim);

  int zaman = 0;
  int toplamSure;
  int cekirdek; //srand icin.

  printf("Sayi uretici icin cekirdegi giriniz : ");
  scanf("%d", &cekirdek);
  printf("Toplam simulasyon suresini giriniz : ");
  scanf("%d", &toplamSure);
  printf("Simulasyon basliyor...\n\n");

  struct Musteri musteriler[1000];
  int musterilerIndis = 0;
  int i;
  int mgs = 0; // mgs : musteri gelis suresi
  int mbs = 0; // mbs : musteri bekleme suresi
  int musteriUretme = 0;
  int tempToplamSure = toplamSure;

  srand(cekirdek);
  for(i = 0; i <= toplamSure; i++) {
    //printf("%d <= %d - i : %d\n", mgs, toplamSure, i);
    mgs = (rand() % 5) + mgs;
    if(mgs <= toplamSure &&  mgs < tempToplamSure) {
      mbs = (rand() % 7) + 3; // min : 3 & max : 10
      struct Musteri tempmusteri;
      tempmusteri.isim = musterilerIndis+1;
      tempmusteri.gelmeSuresi = mgs;
      tempmusteri.eskiGelmeSuresi = mgs;
      tempmusteri.beklemeSuresi = mbs;
      tempmusteri.aktif = 0;
      musteriler[musterilerIndis] = tempmusteri;


      if((musteriler[musterilerIndis].gelmeSuresi + musteriler[musterilerIndis].beklemeSuresi) > toplamSure) {
        // Kuyrukta bekleyen musterilerden dolayi arta kalan sure olursa buraya girecek ve toplam sureyi duzenleyecek.
        toplamSure = musteriler[musterilerIndis].gelmeSuresi + musteriler[musterilerIndis].beklemeSuresi;
      }

      //printf("Musteri#%d - %d - %d\n", musteriler[musterilerIndis].isim, musteriler[musterilerIndis].gelmeSuresi, musteriler[musterilerIndis].beklemeSuresi);
      /*
      printf("Gelme Suresi : %d\n", musteriler[musterilerIndis].gelmeSuresi);
      printf("Bekleme Suresi : %d\n", musteriler[musterilerIndis].beklemeSuresi);
      printf("Cikis Suresi : %d\n", musteriler[musterilerIndis].gelmeSuresi + musteriler[musterilerIndis].beklemeSuresi);
      */
      musterilerIndis++;
    }
  }
  double hizmet1=0, hizmet2=0, hizmet3=0;
  int gelen1=1, gelen2=1, gelen3=1;
  while(zaman <= toplamSure) {
  	for(i=0; i<musterilerIndis;i++) {
      if((musteriler[i].gelmeSuresi + musteriler[i].beklemeSuresi) > toplamSure) {
        // Kuyrukta bekleyen musterilerden dolayi arta kalan sure olursa buraya girecek ve toplam sureyi duzenleyecek.
        toplamSure = musteriler[i].gelmeSuresi + musteriler[i].beklemeSuresi;
      }
  	}
    for(i=0; i<musterilerIndis;i++) {
      if(musteriler[i].gelmeSuresi == zaman && musteriler[i].aktif == 0) { // Musteri gelir.
        printf("%d. dakikada Musteri#%d geldi.\n", zaman, musteriler[i].isim);
        // Kuyruga ekleme islemi yapilir.
        kuyrugaEkle(&kuyruk, musteriler[i]);
        //yaz(&kuyruk);
        if(temsilci1.mesgul == 0) { // Temsilci#1 bosta ise...
          gelen1 += 1;
          printf("%d. dakikada Musteri#%d'e Temsilci#1'de hizmet verilmeye baslandi.\n", zaman, musteriler[i].isim);
          hizmet1 += musteriler[i].beklemeSuresi;
          temsilci1.mesgul = musteriler[i].isim; // Temsilci#1 mesgul durumuna cekilir.
          // Kuyruktan cikarma islemi yapilir.
          musteriler[i].aktif = 1; // Artik geldigini biliyoruz.
          cikar(&kuyruk);
        }
        else if(temsilci2.mesgul == 0) { // Temsilci#2 bosta ise...
          gelen2 += 1;
          printf("%d. dakikada Musteri#%d'e Temsilci#2'de hizmet verilmeye baslandi.\n", zaman, musteriler[i].isim);
          hizmet2 += musteriler[i].beklemeSuresi;
          temsilci2.mesgul = musteriler[i].isim; // Temsilci#2 mesgul durumuna cekilir.           // Kuyruktan cikarma islemi yapilir.
          musteriler[i].aktif = 1; // Artik geldigini biliyoruz.
          cikar(&kuyruk);
        }
        else if(temsilci3.mesgul == 0) { // Temsilci#3 bosta ise...
          gelen3 += 1;
          printf("%d. dakikada Musteri#%d'e Temsilci#3'de hizmet verilmeye baslandi.\n", zaman, musteriler[i].isim);
          hizmet3 += musteriler[i].beklemeSuresi;
          temsilci3.mesgul = musteriler[i].isim; // Temsilci#3 mesgul durumuna cekilir.           // Kuyruktan cikarma islemi yapilir.
          musteriler[i].aktif = 1; // Artik geldigini biliyoruz.
          cikar(&kuyruk);
        }
        else { // Hicbir temsilci bosta degil ise buraya girecek.
          //printf("Tum temsilciler dolu, musteri kuyrukta bekleyecek.\n");
        }
      }
      else if(kuyruk.kayit > 0) { // Kuyrukta kayit varsa buraya girecek.
        if(temsilci1.mesgul == 0) { // Temsilci#1 bosta ise...
          struct Musteri _musteri = kuyruk.ilk->kMusteri;
          gelen1 += 1;
          printf("%d. dakikada Musteri#%d'e Temsilci#1'de hizmet verilmeye baslandi.\n", zaman, _musteri.isim);
          temsilci1.mesgul = _musteri.isim;
          hizmet1 += musteriler[i].beklemeSuresi;
          musteriler[_musteri.isim-1].eskiGelmeSuresi = musteriler[_musteri.isim-1].gelmeSuresi;
          musteriler[_musteri.isim-1].gelmeSuresi = zaman;
          musteriler[_musteri.isim-1].aktif = 1; // Artik geldigini biliyoruz.
          cikar(&kuyruk);
          //yaz(&kuyruk);
        }
        else if(temsilci2.mesgul == 0) { // Temsilci#2 bosta ise...
          struct Musteri _musteri = kuyruk.ilk->kMusteri;
          gelen2 += 1;
          printf("%d. dakikada Musteri#%d'e Temsilci#2'de hizmet verilmeye baslandi.\n", zaman, _musteri.isim);
          hizmet2 += musteriler[i].beklemeSuresi;
          temsilci2.mesgul = _musteri.isim;
          musteriler[_musteri.isim-1].eskiGelmeSuresi = musteriler[_musteri.isim-1].gelmeSuresi;
          musteriler[_musteri.isim-1].gelmeSuresi = zaman;
          musteriler[_musteri.isim-1].aktif = 1; // Artik geldigini biliyoruz.
          cikar(&kuyruk);
          //yaz(&kuyruk);
        }
        else if(temsilci3.mesgul == 0) { // Temsilci#3 bosta ise...
          struct Musteri _musteri = kuyruk.ilk->kMusteri;
          gelen3 += 1;
          printf("%d. dakikada Musteri#%d'e Temsilci#3'de hizmet verilmeye baslandi.\n", zaman, _musteri.isim);
          hizmet3 += musteriler[i].beklemeSuresi;
          temsilci3.mesgul = _musteri.isim;
          musteriler[_musteri.isim-1].eskiGelmeSuresi = musteriler[_musteri.isim-1].gelmeSuresi;
          musteriler[_musteri.isim-1].gelmeSuresi = zaman;
          musteriler[_musteri.isim-1].aktif = 1; // Artik geldigini biliyoruz.
          cikar(&kuyruk);
          //yaz(&kuyruk);
        }
      }

      if(temsilci1.mesgul != 0 && zaman == musteriler[temsilci1.mesgul-1].gelmeSuresi + musteriler[temsilci1.mesgul-1].beklemeSuresi) {
        // Temsilci#1 dolu ise ve Temsilci#1'de ki musterinin isi bitti ise buraya girecektir.
        printf("%d. dakikada Musteri#%d Temsilci#1 i terketti.\n", zaman, musteriler[temsilci1.mesgul-1].isim);
        temsilci1.mesgul = 0; // Temsilci#1 artik mesgul degildir.
      }
      else if(temsilci2.mesgul != 0 && zaman == musteriler[temsilci2.mesgul-1].gelmeSuresi + musteriler[temsilci2.mesgul-1].beklemeSuresi) {
        // Temsilci#2 dolu ise ve Temsilci#2'de ki musterinin isi bitti ise buraya girecektir.
        printf("%d. dakikada Musteri#%d Temsilci#2 i terketti.\n", zaman, musteriler[temsilci2.mesgul-1].isim);
        temsilci2.mesgul = 0; // Temsilci#2 artik mesgul degildir.
      }
      else if(temsilci3.mesgul != 0 && zaman == musteriler[temsilci3.mesgul-1].gelmeSuresi + musteriler[temsilci3.mesgul-1].beklemeSuresi) {
        // Temsilci#3 dolu ise ve Temsilci#3'de ki musterinin isi bitti ise buraya girecektir.
        printf("%d. dakikada Musteri#%d Temsilci#3 i terketti.\n", zaman, musteriler[temsilci3.mesgul-1].isim);
        temsilci3.mesgul = 0; // Temsilci#3 artik mesgul degildir.
      }
    }
    zaman++;
  }
  double ortHizmet1=0, ortHizmet2=0, ortHizmet3=0;
  ortHizmet1 = hizmet1 / gelen1;
  ortHizmet2 = hizmet2 / gelen2;
  ortHizmet3 = hizmet3 / gelen3;

  printf("\nSimulasyon bitti, sonuclar: \n\n");
  printf("Herhangi bir zamanda kuyrukta bulunan max kisi sayisi: %d\n", kuyruk.maxKayit);

  int max = 0; // Max bekleme suresi.
  double ort = 0; // Kuyruktaki ortalama bekleme suresi.
  for(i = 0; i < musterilerIndis; i++) {
  	//printf("Musteri#%d %d-%d----\n", i+1, musteriler[i].gelmeSuresi, musteriler[i].eskiGelmeSuresi);
  	if(max < (musteriler[i].gelmeSuresi - musteriler[i].eskiGelmeSuresi))
  		max = musteriler[i].gelmeSuresi - musteriler[i].eskiGelmeSuresi;
  	ort += musteriler[i].gelmeSuresi - musteriler[i].eskiGelmeSuresi;
  }

  ort = ort/musterilerIndis;

  printf("Kuyrukta en uzun sure bekleyen musterinin bekleme suresi: %d\n", max);
  printf("Bir musterinin kuyrukta ortalama bekleme suresi: %f\n\n", ort);
  printf("Temsilci      Toplam Musteri Sayisi    Ortalama Hizmet Verme Suresi\n");
  printf("--------      ---------------------    ----------------------------\n");
  printf("Temsilci#1            %d                          %f\n",gelen1,ortHizmet1);
  printf("Temsilci#2            %d                          %f\n",gelen2,ortHizmet2);
  printf("Temsilci#3            %d                          %f\n",gelen3,ortHizmet3);

}
