#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/msg.h>
#include <stdbool.h>

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

int main(){

	//meassage queue key
	key_t key=12345;
	int msgid=msgget(key,0666|IPC_CREAT);
	
	if(msgid==-1)
	{
		perror("\nMESSAGE QUEUE NOT CREATED OR NOT FOUND\n");
		exit(1);
	}

	while(1)
	{
		
		msgrcv(msgid,&msg,sizeof(struct message),0,0);
		printf("_________________________________________________");
		
		// if block is executed when key flag is 0
		if(!msg.key)
		{
			printf("\nKEY IS NOT ON\n");
		}
		
		if(!msg.seat)
		{
			printf("\nSEAT BELTS ARE NOT WORN\n");
		}
		
		if(!msg.door)
		{
			printf("\nDOORS ARE NOT LOCKED\n");
		}
		
		if(!msg.h_break)
		{
			printf("\nHANDBREAKS ARE NOT APPLIED\n");
		}

		// when all the flags are set to 1 car gets on
		if(msg.key && msg.seat && msg.door && msg.h_break)
		{	
			printf("\nSIGNAL IS SENT TO THE IGNITION COIL\n");
			sleep(2);
			printf("\nVEHICLE IS ON RIDE SAFE\n");
			break;
		}
	}
	return 0;
}
