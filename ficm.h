#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/ipc.h>  
#include<sys/shm.h>
#include<stdint.h>
#include<time.h>
struct FICM
{
  	int fuel;
  	char time[30];
};
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
int calculate(int ,int );
int random_input(int ,int );
