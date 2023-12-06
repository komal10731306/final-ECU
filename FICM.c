#include "ficm.h"
int fuel;
char* f_time;

int main()
{
	struct FICM  *shared_memory;
  	struct SDF *memory;
 	int shmid,key_t=23452,speed,pos,temp;
  	shmid=shmget((key_t),1024,0666| IPC_CREAT);
  	printf("KEY OF THESHARED MEMORY IS %d\n",shmid);
  	shared_memory =(struct FICM*)shmat(shmid,NULL,0);
    if(shmid==-1)
    {
      	printf("SHARED MEMORY NOT ASSIGNED");
    	exit(1);
    }
    memory=(struct SDF*)shmat(shmid,NULL,0);
    printf("\nTEMPERATURE READ FROM DATA FUSION MODEL : %.2f\n",memory->temperature);
    if (memory->temperature>220)
    {
      	printf("\nENGINE TEMPERATURE IS TOO HIGH , FUEL INJECTION TUBE WILL NOT WORK UNTIL TEMPERATURE FALLS BELOW 220 CELSIUS\n");
	    int pid,shmid2, key=11111;
	    pid=getpid();
	    struct FAULT *smem_fault;
	    shmid2=shmget(key,1024,0666| IPC_CREAT);
    	smem_fault=(struct FAULT *)shmat(shmid2,NULL,0);
      	smem_fault->pid=pid;
    	printf("process ID FICM %d\n",smem_fault->pid);
    	sleep(15);
      	system("./FAULT");
    }
	printf("\nSPEED \t \tTHROTTLE POSITION\n0-499 \t \t 00-19\n500-999 \t ""20-39\n1000-1999 \t 40-59\n2000-2999\t 60-79\n3000-4000 \t 80-100");
	printf("\n______________________________________________\n");
    speed=random_input(0,499); pos=random_input(0,19);
    printf("\n\nSPEED : %d\tTHROTTLE POSITION : %d\n",speed,pos);
    calculate(speed,pos);
    printf("\n______________________________________________\n");
    speed=random_input(3000,4000); pos=random_input(20,40); //invalid tet case
    printf("\nSPEED : %d\tTHROTTLE POSITION : %d\n",speed,pos);
    calculate(speed,pos);
    printf("\n______________________________________________\n");
    speed=random_input(500,999); pos=random_input(20,39); 
    printf("\nSPEED : %d\tTHROTTLE POSITION : %d\n",speed,pos);
    calculate(speed,pos);

    printf("\n______________________________________________\n");
    speed=random_input(1000,1999);pos=random_input(40,59); 
    printf("\nSPEED : %d\tTHROTTLE POSITION : %d\n",speed,pos);
    calculate(speed,pos);

    printf("\n______________________________________________\n");
    speed=random_input(2000,2999);pos=random_input(60,79); 
    printf("\nSPEED : %d\tTHROTTLE POSITION : %d\n",speed,pos);
    calculate(speed,pos);

    printf("\n______________________________________________\n");
    speed=random_input(3000,4000);pos=random_input(80,100); 
    printf("SPEED : %d\tTHROTTLE POSITION : %d\n",speed,pos);
    calculate(speed,pos);

    shared_memory->fuel=fuel;
    strcpy(shared_memory->time,f_time);
    printf("FUEL INJECTED : %d\tTIME OF INJECTION:%s\n",shared_memory->fuel,shared_memory->time);

}
int random_input(int min,int max)
{
	srand(time(0));
	return rand() % (max-min+1)+min;
}

int calculate(int speed,int t_pos)
{
	fuel =0;
	if((speed >=0 && speed<500) && (t_pos>=0 && t_pos <20))
  	{
    	fuel=12;
    }
    else if ((speed >=500 && speed<1000) && (t_pos>=20 && t_pos <40))
    {
    	fuel=18;
    }
    else if ((speed >=1000 && speed<2000) && (t_pos>=40 && t_pos <60))
    {
    	fuel=24;
    }
    else if ((speed >=2000 && speed <3000) && (t_pos>=60 && t_pos <80))
    {
    	fuel=30;
    }
    else if ((speed >=3000 && speed <=4000) && (t_pos>=80 && t_pos <=100))
    {
    	fuel=36;
    }
    else
    {
    	printf("\nTHROTTLE POSITON AND RPM IS INVALID\n");
        return 1;
    }
    f_time=__TIME__;
  	printf("\n FUEL INJECTED:%dml\t TIME OF INJECTION:%s\n",fuel,f_time);
}
