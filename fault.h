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
