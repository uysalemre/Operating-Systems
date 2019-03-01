/*
@AUTHOR : EMRE UYSAL
@PERSONAL_WEBSITE: uysalem16_itu2016.pythonanywhere.com
@COPYRIGHT: YOU CAN COPY AND USE THIS CODE WITHOUT PERMISSION.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <signal.h>

//------------shared memory ve semaforlar---------//
#define KeySem1 4375 //semaphore identifier 1
#define KeySem2 4376 //semaphore identifier 2
#define KeyShm1 4377 //shared mem identifier 1
#define KeyShm2 4378 //shared mem identifier 2
#define KeyShm3 4379 //shared mem identifier 3
#define KeySem3 4380 //semaphore identifier 3
#define KeySem4 4381 //semaphore identifier 4
#define KeySem5 4382 //semaphore identifier 5
#define KeySem6 4383 //semaphore identifier 6
#define KeySem7 4384 //semaphore identifier 7
#define KeySem8 4385 //semaphore identifier 8
#define KeySem9 4386 //semaphore identifier 7
#define KeySem10 4387 //semaphore identifier 8
//------------------------------------------------//

//------asansor yonleri----//
#define asagi 0
#define yukari 1
#define sabit 2
//-------------------------//

#define bos -1

int termSem=0, termSem1=0, termSem2=0, termSem3=0, lock=0, lock1=0, lock2=0, lock3=0, lock4=0, lock5=0;
int kat,kisi,sure,kapasite;
int kuyruk_deger=0;
int sayac=0;

/////////////////////////////verileri tuttugumuz struct yapıları ///////////////////////////////////////////////////////

typedef struct{
    int yon;
    int hedef_kat;  //kisinin gidecegi kat
    int simdiki_kat;//kisinin asansoru bekledigi kat
    int zaman;      //kisinin maks dolasma suresi
    int id;         //kisinin idsi P1 P2 P3 gbi
}kullanici;

kullanici person;

typedef struct{
    int toplam_kat;
    int zaman;      //kalan simulasyon zamanı
    int simdiki_kat;//asansorun bulundugu kat
    int hedef_kat;  //asansorun gidecegi kat
    int yon;        //asansorun gidecegi yon
    kullanici *person;//asansorun icindeki kisiler
    int kapasite;   //asansorun kapasitesi
}kutu;

kutu *asansor;

typedef struct{
    kullanici *person; //asansor bekleyen kisiler
}bekleyen;

bekleyen *kuyruk;


////////////////////////////////semaforlar ve sinyal /////////////////////////////////////////////////////////////////////

//semafor artırma
void sem_signal(int semid,int val){
	struct sembuf semaphore;
	semaphore.sem_num=0;
	semaphore.sem_op=val;
	semaphore.sem_flg=1;
	semop(semid,&semaphore,1);
}

//semafor azaltma
void sem_wait(int semid,int val){
	struct sembuf semaphore;
	semaphore.sem_num=0;
	semaphore.sem_op=(-1*val);
	semaphore.sem_flg=1;
	semop(semid,&semaphore,1);
}
//signal handling
void mysignal(int signum){}

void mysigset(int num){
	struct sigaction mysigaction;
	mysigaction.sa_handler=(void *)mysignal;
	mysigaction.sa_flags=0;
	sigaction(num,&mysigaction,NULL);
}



//////////asansor cagirma fonksiyonu////////////

void asansor_cagir(int simdiki_kat)
{
    if((asansor->yon == asagi && asansor->simdiki_kat > 0 ) || ((asansor->yon == yukari) &&(asansor->hedef_kat<=asansor->toplam_kat)))
    {
        printf("Asansor %d katinda iken P%d asansoru %d katina cagirdi.\n",asansor->simdiki_kat,person.id,simdiki_kat);
        if(asansor->simdiki_kat > simdiki_kat){
            kuyruk->person[kuyruk_deger].yon = asagi;
        }
        else if(asansor->simdiki_kat < simdiki_kat)
        {
            kuyruk->person[kuyruk_deger].yon = yukari;
        }
        kuyruk->person[kuyruk_deger].simdiki_kat = simdiki_kat;
        kuyruk->person[kuyruk_deger].id = person.id;
        kuyruk->person[kuyruk_deger].zaman = person.zaman;
        kuyruk_deger++;
        //asansor hareket halinde ise beklesin
        while(asansor->simdiki_kat != kuyruk->person[sayac].simdiki_kat)
        {
            sleep(1);
            kuyruk->person[sayac].zaman -=1;
        }
        if(asansor->simdiki_kat == kuyruk->person[sayac].simdiki_kat)
        {

            printf("Asansor %d. kattan P%d kisisini aldi\n",asansor->simdiki_kat,kuyruk->person[sayac].id);

             //asansore bindir//
            asansor->person[kuyruk->person[sayac].id] = kuyruk->person[sayac];
            asansor->kapasite--;
            asansor->yon=sabit;

            int i,j;
            //kuyrukdaki kisileri kaydırıyoruz.//
  			for (i = 0; i < kuyruk_deger; i++){
       				for (j = 0; j < kuyruk_deger-i; j++){

                        kuyruk->person[j] = kuyruk->person[j+1];

						kuyruk->person[j+1].simdiki_kat = bos;
                        kuyruk->person[j+1].id = bos;
                        kuyruk->person[j+1].zaman = bos;
                        kuyruk->person[j+1].hedef_kat = bos;
                        kuyruk->person[j+1].yon =  bos;

				}
			}
            return;
        }
    }
    else if(asansor->yon == sabit)
    {
        printf("P%d asansoru %d. kata cagirdi.\n",person.id,simdiki_kat);
        //asansor icin yön ve hedef belirle//
        if(simdiki_kat > asansor->simdiki_kat)
        {
            printf("Asansor yukari çıkıyor.\n");
            asansor->hedef_kat = simdiki_kat;
            asansor->yon = yukari;
            sem_signal(termSem1,1);

        }
        else if(simdiki_kat < asansor->simdiki_kat)
        {
            printf("Asansor asagi iniyor.\n");
            asansor->hedef_kat = simdiki_kat;
            asansor->yon = asagi;
            sem_signal(termSem1,1);
        }
        else
        {
           asansor->hedef_kat = simdiki_kat;
           asansor->yon = sabit;
           sem_signal(termSem1,1);
        }
        sem_wait(termSem2,1);
        if(asansor->simdiki_kat == asansor->hedef_kat)
        {

            if(asansor->kapasite!=0){
                printf("Asansor %d. kattan P%d i aldi.",asansor->simdiki_kat,person.id);
                person.zaman-=1;
                sleep(1);
                //asansore bindir//
                asansor->person[person.id].id = person.id;
                asansor->person[person.id].simdiki_kat = asansor->simdiki_kat;
                asansor->person[person.id].zaman = person.zaman;
                asansor->kapasite--;
                asansor->yon=sabit;

            }
            else
            {
                printf("Asansor kapasitesi dolu.\n");
                return;
            }

            return;
        }
    }

    }


///asansor git fonksiyonu//////
void asansor_git(int hedef_kat)
{
    if(asansor->yon == yukari || asansor->yon == asagi){
        //beklet
        while(asansor->yon != sabit){}
    }
    if(asansor->yon == sabit)
    {

        if(hedef_kat > asansor->simdiki_kat)
        {

                printf("P%d %d katina basti\n",person.id,hedef_kat);
                asansor->hedef_kat = hedef_kat;
                asansor->person[person.id].hedef_kat=hedef_kat;
                asansor->yon = yukari;
                sem_signal(termSem1,1);

        }
        else if(hedef_kat < asansor->simdiki_kat)
        {

                printf("P%d %d katina basti\n",person.id,hedef_kat);
                asansor->hedef_kat = hedef_kat;
                asansor->yon = asagi;
                asansor->person[person.id].hedef_kat=hedef_kat;
                sem_signal(termSem1,1);
        }
        else
        {

                printf("P%d %d katina basti\n",person.id,hedef_kat);
                asansor->hedef_kat = hedef_kat;
                asansor->yon = sabit;
                asansor->person[person.id].hedef_kat=hedef_kat;
                sem_signal(termSem1,1);
        }
    }
    sem_wait(termSem2,1);
    printf("Asansor %d katinda P%d i indirdi.\n",asansor->simdiki_kat,asansor->person[person.id].id);
    asansor->person[person.id].zaman-=1;
    person.zaman=asansor->person[person.id].zaman;
    person.simdiki_kat = asansor->simdiki_kat;
    asansor->person[person.id].id = bos;
    asansor->person[person.id].simdiki_kat = bos;
    asansor->person[person.id].zaman = bos;
    asansor->kapasite++;
    asansor->yon=sabit;


}


int main(int argc,char *argv[]){
	mysigset(12);//signal handling with num=12
	int shdmem1=0,shdmem2=0,shdmem3=0;
	int i,f=1;

	//----------------DEGERLER INT YAPILDI---------------------//
    kat=atoi(argv[1]);
    kapasite=atoi(argv[2]);
    kisi=atoi(argv[3]);
    sure=atoi(argv[4]);

    if(sure<=5){
        printf("Lütfen 20 sn den yüksek değer girin!!\n");
        exit(0);
    }

	printf("Binanin kat sayisi:%d\n",kat);
	printf("Asansor kapasitesi:%d\n",kapasite);
	printf("Binada yer alan maksimum kisi sayisi:%d\n",kisi);
	printf("Kisiler icin maksimum dolasma suresi:%d\n",sure);
	//--------------------------------------------------------//

    //--------shared memory ve semaforlar-----------------//
	shdmem1=shmget(KeyShm1,sizeof(int),0700|IPC_CREAT);
	asansor=(kutu *)shmat(shdmem1,0,0);

	shdmem2=shmget(KeyShm2,sizeof(int),0700|IPC_CREAT);
	kuyruk=(bekleyen *)shmat(shdmem2,0,0);

    asansor->person=(kullanici *)(asansor+2*sizeof(int)); //asansordeki insanlara yer

    kuyruk->person=(kullanici *)(asansor+ 2*sizeof(int)); //bekleyen kuyrukdaki insanlara yer



	//senkronizasyon semaforu//
	termSem=semget(KeySem1,1,0700|IPC_CREAT);
	semctl(termSem,0,SETVAL,0);

	//dışlama semaforu//
	lock=semget(KeySem2,1,0700|IPC_CREAT);
	semctl(lock,0,SETVAL,1);

	lock1=semget(KeySem6,1,0700|IPC_CREAT);
	semctl(lock1,0,SETVAL,1);

	lock2=semget(KeySem7,1,0700|IPC_CREAT);
	semctl(lock2,0,SETVAL,1);

	lock3=semget(KeySem8,1,0700|IPC_CREAT);
	semctl(lock3,0,SETVAL,1);

	lock4=semget(KeySem9,1,0700|IPC_CREAT);
	semctl(lock4,0,SETVAL,1);

    lock5=semget(KeySem10,1,0700|IPC_CREAT);
	semctl(lock5,0,SETVAL,1);

    termSem1=semget(KeySem3,1,0700|IPC_CREAT);
	semctl(termSem1,0,SETVAL,0);

    termSem2=semget(KeySem4,1,0700|IPC_CREAT);
	semctl(termSem2,0,SETVAL,0);

    termSem3=semget(KeySem5,1,0700|IPC_CREAT);
	semctl(termSem3,0,SETVAL,0);

    //-----------------------------------------------//

    //asansor baslangic bilgilerini girelim//
    asansor->hedef_kat = bos;
    asansor->simdiki_kat = 1;
    asansor->yon= sabit;
    asansor->zaman = 60;
    asansor->kapasite = kapasite;
    asansor->toplam_kat = kat;
    //------------------------------------//

    for(i=0;i<kisi;i++)
    {
        //asansorun icerisine insan sayısı kadar bos yer actık
        asansor->person[i].id = bos;
        asansor->person[i].hedef_kat = bos;
        asansor->person[i].zaman = sure;
       //kuyruga insan sayısı kadar bos yer actık
        kuyruk->person[i].hedef_kat=bos;
        kuyruk->person[i].id=bos;
        kuyruk->person[i].simdiki_kat=bos;
        kuyruk->person[i].zaman=bos;
        kuyruk->person[i].yon=bos;
    }

    int child[kisi];

	for(i=0;i<kisi;i++){
		if(f>0){
			f=fork();
		}
		if(f==-1){
			printf("Fork hatasi..\n");
			exit(1);
		}
		if(f==0){
            srand(time(NULL)^getpid()+i);
            person.id=i+1;
            person.simdiki_kat=2+rand() % kat-1;
            person.hedef_kat=person.simdiki_kat;
            person.zaman=sure;
            printf("P%d baslangic kati :%d\n",person.id,person.simdiki_kat);
			break;
		}
		else{
			child[i]=f;
		}
	}
	//asansor prosesi
	if(f>0)
	{
        printf("Asansorun baslangic kati:%d\n",asansor->simdiki_kat);
        while(asansor->zaman>0 )
        {
            sem_wait(termSem1,1);
            if(asansor->yon == yukari)
            {
                sem_wait(lock1,1);
                while(asansor->simdiki_kat!=asansor->hedef_kat)
                {
                    asansor->simdiki_kat++;
                    asansor->zaman-=1;
                }
                if(asansor->simdiki_kat == asansor->hedef_kat)
                {
                    printf("Asansor hedefe ulasti\n");
                    sem_signal(termSem2,1);
                    sem_signal(lock1,1);
                }
            }
            else if(asansor->yon == asagi)
            {
                sem_wait(lock2,1);
                while(asansor->simdiki_kat!=asansor->hedef_kat)
                {
                    asansor->simdiki_kat--;
                    asansor->zaman-=1;

                }
                if(asansor->simdiki_kat == asansor->hedef_kat)
                {
                    printf("Asansor hedefe ulasti\n");
                    sem_signal(termSem2,1);
                    sem_signal(lock2,1);
                }

            }
            else if(asansor->yon == sabit)//yon sabitse
            {
                sem_wait(lock3,1);
                if(asansor->simdiki_kat == asansor->hedef_kat)
                {
                    printf("Asansor hedefe ulasti\n");
                    sem_signal(termSem2,1);
                    sem_signal(lock3,1);
                }

            }

        }
        printf("Simulasyon suresi doldu \n");
        exit(0);

	}
	//kisiler prosesi ve meşgul bekleme
	else
	{
        time_t t;
		//unique random sayılar üretmek icin
        srand((int)time(&t)%getpid());
		//semaforlar ve shared memory//
		lock=semget(KeySem2,1,0);
		lock1=semget(KeySem6,1,0);
		lock2=semget(KeySem7,1,0);
		lock3=semget(KeySem8,1,0);
		lock4=semget(KeySem9,1,0);
		lock5=semget(KeySem10,1,0);
		termSem=semget(KeySem1,1,0);
		termSem1=semget(KeySem3,1,0);
		termSem2=semget(KeySem4,1,0);
		termSem3=semget(KeySem5,1,0);


		shdmem1 = shmget(KeyShm1, sizeof(int), 0);
		asansor = (kutu *) shmat(shdmem1, 0, 0);

		shdmem2 = shmget(KeyShm2, sizeof(int), 0);
		kuyruk = (bekleyen *) shmat(shdmem2, 0, 0);


		//------kisi mesgul bekleme fonksiyonu-------//

		while(person.zaman>0 && asansor->zaman > 0)
		{
			int sure = rand() % 15+1;
            sleep(sure);
			person.zaman-=sure;
			int hedef_kat = rand() % asansor->toplam_kat+1;
			asansor_cagir(person.simdiki_kat);
			asansor_git(hedef_kat);
		}
        printf("P%d Simulasyon dolasma suresi doldu\n",person.id);
        exit(0);
	}
    return 0;
}
