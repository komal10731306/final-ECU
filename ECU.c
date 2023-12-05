#include "ecu.h"

int main() 
{
	for (;;) 
	{
  		menu:
    	printf("\n1)FUEL INJECTION CONTROL MODULE (FICM) \n2)IGNITION TIMING CONTROL MODULE (ICTM)\n3)EMISSIONS COONTROL SUBSYSTEM (ECS)\n4)SENSOR DATA "
        "FUSION (SDF)\n5)FAULT DETECTION AND REPORTING\n6)EXIT\n");
    	printf("\nENTER THE CHOICE\n");
    	scanf("%d", &choice);
		printf("CHOOSED MODULE %d\n", choice);
    	if (choice == 1) 
    	{
			int shmid;
			shmid=shmget((key_t)23452,1024,0666);
			if(shmid==-1)
			{
				perror("FIRST EXECUTE SENSOR DATA FUSION MODULE\n");
				goto menu;
			}
      		FICM();
   		} 
    	else if (choice == 2) 
    	{
    		ICTM();
   		} 
    	else if (choice == 3) 
    	{
    		ECS();
    	} 
    	else if (choice == 4) 
    	{
      		SDF();
    	} 
    	else if (choice == 5) 
    	{
    		FAULT();
    	} 
    	else if (choice == 6) 
    	{
      		printf("\nTHANK YOU GOOD BYE\n");
      		exit(1);
    	} 
    	else 
    	{
      		printf("\nINVALID CHOICE\n");
    	}

	} // end of for
 	return 0;
}
void SDF()
{
	SDFP shared_memory;
    int shmid , key_t=23452;
    shmid=shmget(key_t,sizeof(struct SDF),0666);
    if(shmid==-1)
    {
        printf("\nSHARED MEMORY NOT FOUND\n");
        exit(0);
    }
    shared_memory=(SDFP)shmat(shmid,NULL,0);
    printf("\n RECEIVED DATA\n Temperature:%.2f\n Speed:%.2f\n Pressure:%.2f\n Fused:%.2f\n",shared_memory->temperature,shared_memory->speed,shared_memory->pressure,shared_memory->fused);
}
void FICM()
{
	struct FICM *shared_memory;
	int shmid;
	shmid=shmget((key_t)23452,1024,0666| IPC_CREAT);
	shared_memory =shmat(shmid,NULL,0);
	printf("DATA READ FROM FICM IS FUEL :%d\t TIME %s\n",shared_memory->fuel  , shared_memory->time);
}
void ICTM()
{
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
}
void ECS(void)
{
	int client_socket,server_socket;
	bool flag=0;
	struct sockaddr_in server_addr;
    char buffer[MAX_BUFFER_SIZE];
    char updateBuffer[MAX_BUFFER_SIZE];
    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }
    // Set up server address struct
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);//binary byte order formet
    server_addr.sin_port = htons(PORT);
    // Connect to server
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error connecting to server");
        close(client_socket);
        exit(EXIT_FAILURE);
    }
    // Receive and print the three text messages
    for (uint8_t i = 0; i < 3; ++i) 
    {
    	flag=0;
    	while(flag==0)
    	{
    	    memset(buffer, 0, sizeof(buffer));
     	   	if(recv(client_socket, buffer, sizeof(buffer), 0)<0)
     	   	{
  	      		perror("Cannot receive from server\n");
   	   		}
    	   	printf("\nData Received from server: %s\n", buffer);
    	   	uint16_t intbuf=atoi(buffer);
        	bzero(buffer,strlen(buffer));
			if (intbuf>=1000 && intbuf<=2000)
			{
				strcpy(buffer,"OK: Quantity is within range");
				if(send(client_socket, buffer, strlen(buffer), 0)<0)
				{
					perror("Cannot send to server\n");
				}
				flag=1;
			}
			else if(intbuf<1000)
			{
				strcpy(buffer,"FAULT: Quantity is less then minimum range");
				if(send(client_socket,buffer,strlen(buffer),0)<0)
				{
					perror("Cannot send to server\n");
				}
		
			}
			else
			{
				strcpy(buffer,"FAULT: Quantity is more then minimum range");
				if(send(client_socket, buffer, strlen(buffer), 0)<0)
				{
					perror("Cannot send to server\n");
				}
		
			}
		}
    }
    // Close the socket
    close(client_socket);
}
void FAULT()
{
	bool f;
	printf("IS FUEL LEAKAGE PRESENT? (0 for NO 1 for YES)\n");
	scanf("%d",&f);
	if(f)
	{
 		int pid;
  		int shmid2;
		pid=getpid();
		struct FAULT *smem_fault;
  		int key=11111;
  		shmid2=shmget(key,1024,0666| IPC_CREAT);
  		smem_fault=(struct FAULT *)shmat(shmid2,NULL,0);
  		smem_fault->pid=pid;
  		smem_fault->fuelq=f;
		printf("process ID FICM %d\n",smem_fault->pid);
		sleep(15);
	}
}
/*int main() 
{
  uint8_t ch;
  system("./SDF");
  SDF();
  system("./FICM");
  FICM();
  system("./FAULT");
  while(1)
  {
  printf("ENTER THE CHOICE\n 1. ITCM\n 2. ECS\n 3. EXIT\n");
  scanf("%hhd",&ch);
  if (ch==1)
  {
    ICTM();
  }
  else if (ch==2)
  {
    ECS();
  }
  else
  {
     exit(1);   
  }
  //int a=FAULT();
  }
  return 0;
}*/

/*
void SDF()
{
  SDFP shared_memory;
    int shmid , key_t=23452;
    shmid=shmget(key_t,sizeof(struct SDF),0666);
    if(shmid==-1)
    {
        printf("\nSHARED MEMORY NOT FOUND\n");
        exit(0);
    }
    shared_memory=(SDFP)shmat(shmid,NULL,0);
    printf("\n RECEIVED DATA\n Temperature:%.2f\n Speed:%.2f\n Pressure:%.2f\n Fused:%.2f\n",shared_memory->temperature,shared_memory->speed,shared_memory->pressure,shared_memory->fused);
}
void FICM()
{
  struct FICM *shared_memory;
  int shmid;
  shmid=shmget((key_t)23452,1024,0666| IPC_CREAT);
  shared_memory =shmat(shmid,NULL,0);
  printf("DATA READ FROM FICM IS FUEL :%d\t TIME %s\n",shared_memory->fuel  , shared_memory->time);
}
void ICTM()
{
  //meassage queue key
  //system("xterm -w ./ITCM");
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
}
void ECS(void)
{
  int client_socket,server_socket;
  bool flag=0;
  struct sockaddr_in server_addr;
    char buffer[MAX_BUFFER_SIZE];
    char updateBuffer[MAX_BUFFER_SIZE];
    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }
    // Set up server address struct
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);//binary byte order formet
    server_addr.sin_port = htons(PORT);
    // Connect to server
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error connecting to server");
        close(client_socket);
        exit(EXIT_FAILURE);
    }
    // Receive and print the three text messages
    for (uint8_t i = 0; i < 3; ++i) 
    {
      flag=0;
      while(flag==0)
      {
          memset(buffer, 0, sizeof(buffer));
          if(recv(client_socket, buffer, sizeof(buffer), 0)<0)
          {
              perror("Cannot receive from server\n");
          }
          printf("\nData Received from server: %s\n", buffer);
          uint16_t intbuf=atoi(buffer);
          bzero(buffer,strlen(buffer));
      if (intbuf>=1000 && intbuf<=2000)
      {
        strcpy(buffer,"OK: Quantity is within range");
        if(send(client_socket, buffer, strlen(buffer), 0)<0)
        {
          perror("Cannot send to server\n");
        }
        flag=1;
      }
      else if(intbuf<1000)
      {
        strcpy(buffer,"FAULT: Quantity is less then minimum range");
        if(send(client_socket,buffer,strlen(buffer),0)<0)
        {
          perror("Cannot send to server\n");
        }

      }
      else
      {
        strcpy(buffer,"FAULT: Quantity is more then minimum range");
        if(send(client_socket, buffer, strlen(buffer), 0)<0)
        {
          perror("Cannot send to server\n");
        }

      }
    }
    }
    // Close the socket
    close(client_socket);
}
int FAULT()
{
  bool f;
  printf("IS FUEL LEAKAGE PRESENT? (0 for NO 1 for YES)\n");
  scanf("%d",&f);
  if(f)
  {
    int pid;
      int shmid2;
    pid=getpid();
    struct FAULT *smem_fault;
      int key=11111;
      shmid2=shmget(key,1024,0666| IPC_CREAT);
      smem_fault=(struct FAULT *)shmat(shmid2,NULL,0);
      smem_fault->pid=pid;
      smem_fault->fuelq=f;
    printf("process ID FICM %d\n",smem_fault->pid);
    system("./FAULT");
    //sleep(15);     
    return 0;
  }
  exit(0);
}*/
