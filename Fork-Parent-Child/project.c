/*
@AUTHOR : EMRE UYSAL
@PERSONAL_WEBSITE: uysalem16_itu2016.pythonanywhere.com
@COPYRIGHT: YOU CAN COPY AND USE THIS CODE WITHOUT PERMISSION.
*/

#include <stdio.h>//standart input output
#include <unistd.h>//fork

int main(int argc,char * argv[]){
	int first=fork(); //burada 1. seviyedeki anne ve 2. seviyede bir cocuk proses olusturuldu
	if(first==0)	  //2. seviyede sol taraftaki cocuk proses(kabul)
	{ 
		printf("2. SEVİYE ÇOCUK : MYPID:%d MY-PARENT-PID:%d \n",getpid(),getppid()); //cocuk prosesin pid si ve annesinin pid si
		int left_child=fork();		// mevcut cocuk proses den bir cocuk daha olusturuldu yani 3. seviye cocuk proses sol taraf(kabul)
		if(left_child==0)		
		{
			printf("3. SEVİYE SOL ÇOCUK : MYPID:%d MY-PARENT-PID:%d \n",getpid(),getppid());//cocuk proses pid ve annesinin pid si
		}
		else
		{
			printf("2. SEVİYE SOLUN ANNESİ : MYPID:%d MY-CHILD-PID:%d \n",getpid(),left_child); //cocugun annesinin(cocugu dogrulamak amacıyla)
		}
	}
	else //1. seviye anne proses
	{
		printf("1. SEVİYE ANNE : MYPID:%d MY-CHILD-PID:%d \n",getpid(),first); //1. seviyedeki anne prosesin pid si ve cocugunun pid si
		int second_child=fork(); // sag tarafta bir cocuk olusturmak amacıyla 1. seviye anneden cocuk olusturuldu
		if(second_child==0)//sag taraf cocugunun prosesi
		{
			printf("2. SEVİYE SAĞ ÇOCUK : MYPID:%d MY-PARENT-PID:%d \n",getpid(),getppid());//2. seviye sag taraftaki cocugun pid ve parent bilgileri
			int left_of_right=fork(); //2. seviye sag taraftaki cocuktan bir cocuk daha olusturulur (kabulen) 3. seviye sol cocuk olur.
			if(left_of_right==0)
			{
				printf("3. SEVİYE ,2.SEVİYE SAĞ ÇOCUĞUN SOLU: MYPID:%d MY-PARENT-PID:%d \n",getpid(),getppid());//cocugun bilgileri
				int child_of_left=fork(); //bu cocuktan bir cocuk daha olusturdugumuzda 3.seviye 2.seviye sağ cocugun solundakinin cocugu olur
				if(child_of_left==0)
				{
					printf("4. SEVİYE SOL ÇOCUK: MYPID:%d MY-PARENT-PID:%d \n",getpid(),getppid());//4. seviye sol cocuk bilgileri
				}
				else
				{
					printf("3. SEVİYE SOL ANNE: MYPID:%d MY-CHILD-PID:%d \n",getpid(),child_of_left);//annesinin bilgileri dogrulamak amaclı
				}
			}
			else //2.seviye sag tarafın olan cocugunun anne bilgileri
			{
				printf("2. SEVİYE SAĞ ANNE : MYPID:%d MY-CHILD-PID:%d \n",getpid(),left_of_right);
				int right_of_right=fork();// bu anneden bir cocuk daha oldugunda 3. seviye en sagdaki cocuk olur
				if(right_of_right==0)
				{
					printf("3. SEVİYE EN SAĞ ÇOCUK: MYPID:%d MY-PARENT-PID:%d \n",getpid(),getppid());//cocugun bilgileri
				}
				else
				{
					printf("2. SEVİYE SAĞ ANNE: MYPID:%d MY-CHILD-PID:%d \n",getpid(),right_of_right);//annesinin bilgileri
				}
			}
		}
		else //sag taraf cocugun anne bilgisi
		{
			printf("1. SEVİYE ANNE : MYPID:%d MY-CHILD-PID:%d \n",getpid(),second_child);// sag cocugun annesi yani 1. seviye anne pid ve cocugu pidsi
		}
	}
	sleep(2);/* tum anne ve cocukların kendı ve annesının pid degerlerini 
		    dogru bulmalarını saglamak ıcın proseslerin sonlanması 2 sn gecıktırılmıstır */
	return 0;
}
