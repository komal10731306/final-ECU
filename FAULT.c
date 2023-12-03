#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include<sys/ipc.h>  
#include<sys/shm.h>
#include<stdint.h>
#include<time.h>
#include<mqueue.h>

// Define signals for faults
#define TEMP_FAULT_SIGNAL SIGUSR1
#define SPEED_FAULT_SIGNAL SIGUSR2
#define FUEL_FAULT_SIGNAL SIGINT

struct FAULT
{
	int pid;
	int fuelq;
};

struct SDF{
    float temperature;
    float speed;
    float pressure;
    float fused;
};
struct SDF *memory;
// Signal handlers
void temperatureFaultHandler(int signo) {
    printf("Temperature is too high. Fault detected!\n");
    exit(EXIT_FAILURE);
}

/*void speedFaultHandler(int signo) {
    printf("High speed detected. Fault detected!\n");
    exit(EXIT_FAILURE);
}*/

void fuelLeakageHandler(int signo) {
    printf("Fuel leakage detected. Fault detected!\n");
    exit(EXIT_FAILURE);
}
int main() 
{
	int shmid,key_t=23452;
	struct FICM  *shared_memory;
	shmid=shmget((key_t),1024,0666| IPC_CREAT);
	//printf("KEY OF THESHARED MEMORY IS %d\n",shmid);
	shared_memory =(struct FICM*)shmat(shmid,NULL,0);
  	if(shmid==-1)
  	{
  		printf("SHARED MEMORY NOT ASSIGNED");
  		exit(1);
  	}
  	memory=(struct SDF*)shmat(shmid,NULL,0);
  	//printf("\nTEMPERATURE READ FROM  : %.2f\n",memory->temperature);
    int shmidnew,key_t1=11111;
    struct FAULT *smem_fault;
    shmidnew=shmget((key_t1),1024,0666| IPC_CREAT);
    smem_fault=(struct FAULT *)shmat(shmidnew,NULL,0);
    
       signal(TEMP_FAULT_SIGNAL, temperatureFaultHandler);
    //signal(SPEED_FAULT_SIGNAL, speedFaultHandler);
    signal(FUEL_FAULT_SIGNAL, fuelLeakageHandler);
    
    // Register signal handlers

    // Simulate faults by sending signals
    if (memory->temperature > 220.0) 
    {
    printf("FICM pid %d\n",smem_fault->pid);
    printf("Temperature is too high. Fault detected!\n");
        kill(smem_fault->pid,TEMP_FAULT_SIGNAL);
    }

    else if (smem_fault->fuelq==1) 
    {
    printf("ECU pid %d\n",smem_fault->pid);
    	printf("Fuel leakage detected. Fault detected!\n");
        kill(smem_fault->pid,FUEL_FAULT_SIGNAL);
    }
	else
	{
    // Continue with normal program execution if no faults are detected
    printf("No faults detected. Continuing normal execution.\n");
}
   return 0;
}
