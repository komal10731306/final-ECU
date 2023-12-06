#include "ecu.h"

int main() 
{
    uint8_t ch;
    system("./SDF");
    SDF();
    system("./FICM");
    FICM();
    system("./FAULT");
    while(1)
    {
        printf("ENTER THE CHOICE\n 1. ITCM\n 2. ECS\n 3.FAULT\n 4.EXIT\n");
        scanf("%hhd",&ch);
        if (ch==1)
        {
            ICTM();
        }
        else if (ch==2)
        {
            ECS();
        }
        else if(ch==3)
        {
            FAULT(); 
        }
        else
        {
            exit(1);
        }
  //int a=FAULT();
    }
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
    printf("\nDATA READ FROM FICM SHARED MEMORY \n FUEL :%d\t TIME %s\n",shared_memory->fuel  , shared_memory->time);
}
void ICTM()
{
    
    //system("xterm -w ./ITCM");
    //meassage queue key
    key_t key=12345;
    //creation of message queue
    int msgid=msgget(key,0666|IPC_CREAT);
    //error handling
    if(msgid==-1)
    {
        perror("\nMESSAGE QUEUE NOT CREATED OR NOT FOUND\n");
        exit(1);
    }
    while(1)
    {
        //retrieves first message from the message queue
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
//emission control subsystem
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
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) 
    {
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
        printf("\nprocess ID  %d\n",smem_fault->pid);
        system("./FAULT");
        //sleep(15);     
        return 0;
    }
    printf("\nNO FAULT DETECTED\n");
    exit(0);
}
