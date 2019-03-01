#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char ** argv)
{
	int c = 0;
	int cocuk = fork();
	c ++;
	if(cocuk == 0)//child proses
	{	
		printf("CHILD : MYPID:%d MY-PARENT-PID:%d MY-C:%d\n",getpid(),getppid(),c);
		cocuk = fork();
		c += 10;
		if(cocuk) c = c*5;
		if(cocuk==0)
		{	//child proses again
			printf("NEW CHILD IN CHILD: MYPID:%d MY-PARENT-PID:%d MY-C:%d\n",getpid(),getppid(),c);		
		}else
		{	//parent proses again
			printf("NEW PARENT IN CHILD: MYPID:%d MY-CHILD-PID:%d MY-C:%d\n",getpid(),cocuk,c);
		}

	}
	else	//parent proses
	{	
		printf("PARENT : MYPID:%d MY-CHILD-PID:%d MY-C:%d\n",getpid(),cocuk,c);
		c += 5;
		if(fork()==0)
		{ //new child proses
			printf("NEW CHILD IN PARENT: MYPID:%d MY-PARENT-PID:%d MY-C:%d\n",getpid(),getppid(),c);	
		}else
		{//new parent process
			printf("NEW PARENT IN PARENT: MYPID:%d  MY-C:%d\n",getpid(),c);
		}
	}
	sleep(1);
	return 0;
}

