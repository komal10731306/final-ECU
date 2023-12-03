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
uint8_t val;

void k_on();
void s_belt();
void d_lock();
void hand_break();
void input();
void a_input(int msgid);

int main(){
	
	msg.reg=0;
	msg.key=0;
	msg.seat=0;
	msg.door=0;
	msg.h_break=0;
	
	uint8_t choice;
	// message queue key
	key_t key=12345;
	//creation of message queue
	int msgid=msgget(key,0666|IPC_CREAT);
	printf("%d",msgid);

	if(msgid==-1)
	{
		perror("\nMESSAGE QUEUE NOT CREATED OR NOT FOUND\n");
		exit(1);
	}
	
	ch:
	printf("\n1)MANUAL INPUT\t2)AUTOMATED INPUT\nENTER THE CHOICE\n");
	scanf("%hhd",&choice);
	
	if(choice==1)
	{
	
		do{
			input();
			
			//sending msg to message queue and error handling
			msgsnd(msgid,&msg,sizeof(msg),0);
			
			/*
			if((msgsnd(msgid,&msg,sizeof(msg),0)==-1));
			{
				perror("\nMESSAGE NOT SENT\n");
				exit(1);
			}
			*/
		}while(msg.reg!=63);
	}
	
	else if(choice==2)
	{
		a_input(msgid);
	}
	
	else
	{
		printf("\nINVALID CHOICE\n");
		goto ch;
	}	

}

/*automated_input:to send 3 differnt use cases*/
void a_input(int msgid){
	
	struct message u_case1,u_case2,u_case3;
	
	//1st use case: key=1 seat_belt=0 door=1 h_break=0
	u_case1.reg=u_case1.reg|(7|1<<4); u_case1.key=1; u_case1.seat=0; u_case1.door=1; u_case1.h_break=0;
	int a=msgsnd(msgid,&u_case1,sizeof(u_case1),0);
	printf("\n%d",a);
	//sleep(2);
	
	//2nd use case: key=1 seat_belt=0 door=1 h_break=1
	u_case2.reg=u_case2.reg|(7|1<<4|1<<5); u_case2.key=1; u_case2.seat=0; u_case2.door=1; u_case2.h_break=1;
	int b=msgsnd(msgid,&u_case2,sizeof(u_case2),0);
	printf("\n%d",b);
	//sleep(2);
	
	//3nd use case: key=1 seat_belt=1 door=1 h_break=1
	u_case3.reg=u_case3.reg|63; u_case3.key=1; u_case3.seat=1; u_case3.door=1; u_case3.h_break=1;
	int c=msgsnd(msgid,&u_case3,sizeof(u_case3),0);
	printf("\n%d",c);
	//sleep(2);
}

/*input:to accept all input parameters from user*/
void input(){
	printf("__________________________________________________________________");
            
	//if block is executed when key flag is 0 until set to 1
	if(!msg.key)
		k_on();

	if(!msg.seat)
		s_belt();

	if(!msg.door)
		d_lock();

	if(!msg.h_break)
		hand_break();
}

/* key_on : to accept key parameter from the user*/
void k_on(){
	
	//key label
	k:
	printf("\nPLEASE TURN ON THE KEY : 0 for NO 1 for YES\n");
	scanf("%hhd",&val);
	
	// error checking
	if(val>1 || val<0)
	{
		printf("\nInvalid input\n");
		goto k;
	}

	if(val==1)	
	{
		//setting 0,1,2 bits of reg to 1
		msg.reg=msg.reg|7;
		//setting key flag to 1
		msg.key=1;
	}
}

/* seat_belt:to accept seatbelt parameter*/
void s_belt(){
	
    belt:
	printf("\nPLEASE WEAR ON THE SEAT BELTS : 0 for NO 1 for YES\n");
	scanf("%hhd",&val);
	
	if(val>1 || val<0)
	{
		printf("\nINVALID INPUT\n");
		goto belt;
	}
	
	if(val==1)
	{
		// setting 4th bit of reg
		msg.reg=msg.reg|(1<<3);
		msg.seat=1;
	}
}

/* door_lock:to accept door lock parameter*/
void d_lock(){

	lock:
	printf("\nPLEASE LOCK THE DOORS : 0 for NO 1 for YES\n");
	scanf("%hhd",&val);
	
	if(val>1 || val<0)
	{
		printf("\nINVALID INPUT\n");
		goto lock;
	}
	
	if(val==1)
	{
		//setting 5th bit of reg
		msg.reg=msg.reg|(1<<4);
		msg.door=1;
	}
}

/* hand_break:to accept hand break parametr*/
void hand_break(){

	brk:
	printf("\nPLEASE APLLY HANDBREAKS : 0 for NO 1 for YES\n");
	scanf("%hhd",&val);
	
	if(val>1 || val<0)
	{
		printf("\nINVALID INPUT\n");
		goto brk;
	}

	if(val==1)
	{
		//setting 6th bit of reg
		msg.reg=msg.reg|(1<<5);
		msg.h_break=1;
	}
}
