#include "itcm.h"

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
	//error handling
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
	
		do
		{
			input();
			//sending msg to message queue 
			msgsnd(msgid,&msg,sizeof(msg),0);
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
	
	//1st use case: key=1, seat_belt=0, door=1, h_break=0
	u_case[0].reg=u_case[0].reg|(7|1<<4); 
	u_case[0].key=1; 
	u_case[0].seat=0; 
	u_case[0].door=1;
	u_case[0].h_break=0;
	
	//2nd use case: key=1, seat_belt=0, door=1, h_break=1
	u_case[1].reg=u_case[1].reg|(7|1<<4|1<<5); 
	u_case[1].key=1; 
	u_case[1].seat=0; 
	u_case[1].door=1; 
	u_case[1].h_break=1;
	
	//3nd use case: key=1, seat_belt=1, door=1, h_break=1
	u_case[2].key=1; 
	u_case[2].seat=1; 
	u_case[2].door=1; 
	u_case[2].h_break=1;
	
	for(int i=0;i<3;i++)
	{
		msgsnd(msgid,&u_case[i],sizeof(struct message),0);
		sleep(5);
	}
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
