#include <fcntl.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define PORT 12345
#define MAX_BUFFER_SIZE 1024

void SDF(void);
void ICTM(void);
void ECS(void);
void FICM(void);
int FAULT(void);

struct FICM {
  int fuel;
  char time[30];
};
typedef struct FICM *ficm;
struct SDF {
    float temperature;
    float speed;
    float pressure;
    float fused;
};
typedef struct SDF *SDFP;

struct FAULT
{
	int pid;
  	int fuelq;
};
struct message{
  //8 bit register  0-key 1-battery 2-armature 3-seat_belt 4-door_lock 5-hand_break
  	uint8_t reg; 
  //hand break flag
  	bool h_break;
  	bool key; 
  	bool seat; 
  	bool door; 
};
struct message msg;
