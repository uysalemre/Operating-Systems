/*
@AUTHOR : EMRE UYSAL
@PERSONAL_WEBSITE: uysalem16_itu2016.pythonanywhere.com
@COPYRIGHT: YOU CAN COPY AND USE THIS CODE WITHOUT PERMISSION.
*/
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>//fork()
#include <sys/wait.h>//pause()
#include <time.h>//srand() 
//semaphore and shared memory and signal handling//
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <signal.h>


#define KeySem1 2000 //semaphore identifier 1
#define KeySem2 2001 //semaphore identifier 2
#define KeyShm1 2002 //shared mem identifier 1
#define KeyShm2 2003 //shared mem identifier 2
#define KeyShm3 2004 //shared mem identifier 3
#define KeyShm4 2005 //shared mem identifier 4
#define KeySem3 2006 //semaphore identifier 3
 
//semaphore increment operation
void sem_signal(int semid,int val){
	struct sembuf semaphore;
	semaphore.sem_num=0;
	semaphore.sem_op=val;
	semaphore.sem_flg=1;
	semop(semid,&semaphore,1);
}

//semaphore decrement operation
void sem_wait(int semid,int val){
	struct sembuf semaphore;
	semaphore.sem_num=0;
	semaphore.sem_op=(-1*val);
	semaphore.sem_flg=1;
	semop(semid,&semaphore,1);
}
//signal handling
void mysignal(int signum){
}

void mysigset(int num){
	struct sigaction mysigaction;
	mysigaction.sa_handler=(void *)mysignal;
	mysigaction.sa_flags=0;
	sigaction(num,&mysigaction,NULL);
}

int main(void){
	mysigset(12);//signal handling with num=12
	srand(time(NULL));//creating random numbers
	int shdmem1=0,shdmem2=0,shdmem3=0,shdmem4=0;//shared memory ids
	int *N=NULL;//number of the array 
	int termSem=0,lock=0,lock1=0; //semaphore identifiers
	int i,myorder,f=1,cntrl=0,rand_num;
  	//array pointers for General array,First Partition Array,Second Partition Array 
	int *G_array=NULL,*F_array=NULL,*S_array=NULL;
	int child[3];//getting the pids of child processes
	//creating 3 child process	
	for(i=0;i<3;i++){
		if(f>0){
			f=fork();
		}
		if(f==-1){
			printf("Fork Hatasi..\n");
			exit(1);		
		}
		if(f==0){
			break;
		}
		else{
			child[i]=f;//getting pids of childs
			//control for childs if you want remove comment
			//printf("%d inci %d myparent:%d\n",i,child[i],getppid());	
		}
	}
	//parent process			
	if(f>0){
		//for synchronization I created 1 semaphore
		termSem=semget(KeySem1,1,0700|IPC_CREAT);
		semctl(termSem,0,SETVAL,0);
		//for mutual exclusion I created 2 semaphores
		lock=semget(KeySem2,1,0700|IPC_CREAT);
		semctl(lock,0,SETVAL,1);
		lock1=semget(KeySem3,1,0700|IPC_CREAT);
		semctl(lock,0,SETVAL,1);
		//for N: creation of the shared memory id and attaching address space
		shdmem1=shmget(KeyShm1,sizeof(int),0700|IPC_CREAT);
		N=(int *)shmat(shdmem1,0,0);
		//getting and controlling the N value from user
		while(cntrl==0)
		{
			printf("Diziye cift sayida eleman giriniz(N):");
			scanf("%d",N);
			if((*N)%2!=0){
				printf("Girilen sayi cift degil..!\n");	
			}
			else{	
				if((*N)==0){
					printf("0 girilemez!\n"); cntrl=0;
				}
				else{
					printf("Girilen sayi:%d\n",*N);
					cntrl=1;
				}	
			}		
		}
		//taking memory for General array  and attaching it		
		shdmem2=shmget(KeyShm2,sizeof(int),0700|IPC_CREAT);
		G_array=(int *)shmat(shdmem2,0,0);
		//taking memory for First array and attaching it
		shdmem3=shmget(KeyShm3,sizeof(int),0700|IPC_CREAT);
		F_array=(int *)shmat(shdmem3,0,0);
		//taking memory for Second array and attaching it
		shdmem4=shmget(KeyShm4,sizeof(int),0700|IPC_CREAT);
		S_array=(int *)shmat(shdmem4,0,0);
		///creating the array of random number between 1 and 999
		for(i=0;i<(*N);i++){
			G_array[i] = rand() % 999;
		}
		//printing the shared memory area with N and G_array
		printf("Paylasilan Bellek Alani(N,G_ARRAY):");
		for(i=0;i<(*N);i++){
			if(i==0)
			{
				printf("[%d|",(*N));			
			}
			if(i!=(*N)-1){
				printf("%d,",G_array[i]);			
			}
			else{
				printf("%d]\n",G_array[i]);
			}	
		}
		sleep(2);//sleeping 2 seconds
		//sending signal to all childs
		for(i=0;i<3;i++)
		{
			kill(child[i],12);
		}
		sem_wait(termSem,2);//wait for child processes
		printf("Tum cocuklar islerini bitirdi!... \n en kucuk :%d en buyuk:%d\n",G_array[0],G_array[(*N)-1]);
		//give back the resources of semaphores and shared memories		
		shmdt(N);
		shmdt(G_array);
		shmdt(F_array);
		shmdt(S_array);
		semctl(termSem, 0, IPC_RMID, 0);
		semctl(lock, 0, IPC_RMID, 0);
		shmctl(shdmem1, IPC_RMID, 0);
		shmctl(shdmem2, IPC_RMID, 0);
		shmctl(shdmem3, IPC_RMID, 0);
		shmctl(shdmem4, IPC_RMID, 0);
		exit(0);
	}	
	//child processes
	else{
		pause();//wait for parent 
		myorder=i;//detection of the order of the child

		//semaphores
		termSem=semget(KeySem1,1,0);
		lock=semget(KeySem2,1,0);
		lock1=semget(KeySem3,1,0);
		
		//returning ids and reading the General Array and N		
		shdmem1 = shmget(KeyShm1, sizeof(int), 0);
		N = (int *) shmat(shdmem1, 0, 0);
		shdmem2 = shmget(KeyShm2, sizeof(int), 0);
		G_array = (int *) shmat(shdmem2, 0, 0);

		//for First part array and Second part array
		shdmem3 = shmget(KeyShm3, sizeof(int), 0);
		F_array = (int *) shmat(shdmem3, 0, 0);
		shdmem4 = shmget(KeyShm4, sizeof(int), 0);
		S_array = (int *) shmat(shdmem4, 0, 0);
		

		if(myorder==0)//for first child
		{	//take the first part and sort it
			//critical session
			sem_wait(lock,1); 
			printf("COCUK %d CALISIYOR..\n",myorder);
			printf("uzerinde calistigi elemanlar:{");
			int f_begin=0, f_end=((*N)/2)-1;

			for(f_begin=0;f_begin<=f_end;f_begin++)
			{
				if(f_begin>=0 && f_begin<f_end){	
					printf("%d,",G_array[f_begin]);
				}
				if(f_begin==f_end){
					printf("%d}\n",G_array[f_begin]);
				}
				F_array[f_begin]=G_array[f_begin];
		
			}
			//start and end adress of the array
			printf("Ilk array adresler baslangic:%lu bitis:%lu\n", (unsigned long)G_array,(unsigned long)(G_array+(*N/2)));
			//bubble sort for sorting the array 
			int i, j,temp;
  			for (i = 0; i < f_end; i++){ 
       				for (j = 0; j < f_end-i; j++){ 
           				if (F_array[j] > F_array[j+1]){
						temp=F_array[j];
						F_array[j]=F_array[j+1];
						F_array[j+1]=temp;
				 	}
				}
			}
			//sorted 
			printf("Siralanmis hal:{");
			for(f_begin=0;f_begin<=f_end;f_begin++)
			{
				if(f_begin>=0 && f_begin<f_end){	
					printf("%d,",F_array[f_begin]);
				}
				if(f_begin==f_end){
					printf("%d}\n",F_array[f_begin]);
				}
		
			}
			//give memory back
			shmdt(N);
			shmdt(G_array);
			shmdt(F_array);
			shmdt(S_array);
			sem_signal(lock,1);
			sem_signal(lock1,1);
			exit(1);
		}
		if(myorder==1)//for second child 
		{	
			//second part of the array and sort it
			//critical session
			sem_wait(lock1,1);
			printf("COCUK %d CALISIYOR..\n",myorder);
			printf("Uzerinde calistigi elemanlar:{");
			int bas=(*N)/2, son=(*N)-1;
			int s=0;
			for(bas=(*N)/2;bas<=son;bas++)
			{
				if(bas>=(*N)/2 && bas<son){	
					printf("%d,",G_array[bas]);
				}
				if(bas==son){
					printf("%d}\n",G_array[bas]);
				}
				S_array[s]=G_array[bas];
				s++;
		
			}
			//start and end adress of the array
			printf("ikinci array adresler baslangic:%lu bitis:%lu\n", (unsigned long)(G_array+(*N/2)),(unsigned long)(G_array+(*N)));

			//bubble sort for sorting the array 
			int i, j,temp;
  			for (i = 0; i < son; i++){ 
       				for (j = 0; j < son-i; j++){ 
           				if (S_array[j] > S_array[j+1]){
						temp=S_array[j];
						S_array[j]=S_array[j+1];
						S_array[j+1]=temp;
				 	}
				}
			}
			//sorted 
			printf("Siralanmis hal:{");
			for(bas=(*N)/2;bas<=son;bas++)
			{
				if(bas>=(*N)/2 && bas<son){	
					printf("%d,",S_array[bas]);
				}
				if(bas==son){
					printf("%d}\n",S_array[bas]);
				}
			}
			shmdt(N);
			shmdt(G_array);
			shmdt(F_array);
			shmdt(S_array);
			sem_signal(lock,1);
			sem_signal(termSem,1);
			exit(1);
		}
		
		if(myorder==2)//for third child
		{	//combine(merge) F and S arrays together
			sem_wait(lock,2);
			int i,j,k;
			printf("COCUK %d CALISIYOR..Diziler kaynastiriliyor.\n",myorder);
			for(i=0;i<(*N); i++)
        		{
            			G_array[i] = F_array[i];
        		}
     			for(j=(*N)/2;j<(*N); j++)
        		{	
            			G_array[j] =S_array[j];
            			i++; 
       			}
			//////////
			for (i = 0; i < (*N); ++i) 
			{
				for (j = i + 1; j < (*N); ++j)
				{
					if (G_array[i] > G_array[j]) 
					{
						k =  G_array[i];
						G_array[i] = G_array[j];
						G_array[j] = k;
					}
				}
			}
			//////
			sleep(1);
			printf("Siralanmis Son hali:");
			for(i=0;i<(*N);i++){
				printf("-%d-",G_array[i]);
			}
			printf("\n");
			shmdt(N);
			shmdt(G_array);
			shmdt(F_array);
			shmdt(S_array);
			sem_signal(termSem,1);
			exit(1);
		}
	
	}
return 0;
}

