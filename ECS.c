#include "ecs.h"
uint16_t Max=2100,Min=900;
int server_socket, client_socket;
int main() 
{
	//file creation to keep records for server data interaction 
	FILE *fd;
    fd=fopen("recordECSs.txt","a");
    if(fd==NULL)
    {
    	perror("FILE NOT EXIST\n");
        exit(0);
    }
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len=sizeof(client_addr);

    // Create socket
    server_socket=socket(AF_INET, SOCK_STREAM,0);
    if (server_socket==-1)
    {
    	perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Set up server address struct
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);//host to network byte 

    // Bind the socket socket_file_discripter ,(struct socaddr) typecasted server_addr and address length 
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) 
    {
    	perror("Error binding");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections Socket_fd and max len of q
    if (listen(server_socket,5)==-1) 
    {
    	perror("Error listening");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);
    fprintf(fd,"Server listening on port %d...\n", PORT);
    fclose(fd);

    while (1) 
    {
    	// Accept connection from client sock_fd struct sockaddr type addr len 
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_len);
        if (client_socket == -1) 
        {
     		perror("Error accepting connection");
        	close(server_socket);
            exit(EXIT_FAILURE);
        }
        //inet_ntoa convert host network bytes to string 
        //ntohs() convert network byte order to host byte order
        printf("Accepted connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Handle the client
        handle_client(client_socket);
        return 0;
	}    
}
//function to adjust values to range 
uint16_t getupdatetominimum(uint16_t quantity,char *gasname)
{
    printf("Adjusting the quantity of %s to minimum value\n",gasname);
    if(quantity<1000)//DECREMENTING  VALUE
    {
        while(quantity<1000)
        {
            printf("%dppm of %s \n",1000-quantity,gasname);
            quantity+=10;
        }
    }
    else//INCREMENTING  VALUE
    {
        while(quantity>2000)
        {
            printf("%dppm of %s \n",quantity-1000,gasname);
            quantity-=10;
        }	
    }
    return quantity;	
}
//to send data to client 
void handle_client(int client_socket) 
{
	char buffer[1024]; char *fault;
    // Send first text data
    FILE *fd;
    fd=fopen("recordECSs.txt","a");
    if(fd==NULL)
    {
	    perror("FILE NOT EXIST\n");
        exit(0);
    }
    uint16_t quantity_Carbon_monoxide,Nitrogen_oxide,Hydro_Carbons;
    bool flag=0;
    srand(time(0));
    //input using random function 
    quantity_Carbon_monoxide=rand()%(Max-Min+1)+Min;
    printf("Quantity of Carbon monoxide in %dppm\n",quantity_Carbon_monoxide);
	//input using random function 
    Nitrogen_oxide=rand()%(Max-Min+1)+Min;
    printf("Quantity of Carbon monoxide in %dppm\n",Nitrogen_oxide);
	//input using random function 
    Hydro_Carbons=rand()%(Max-Min+1)+Min;
    printf("Quantity of Carbon monoxide in %dppm\n",Hydro_Carbons);
    printf("\n");
    char text1[1024];
    char gasname[]="CarbonMonoxide";
    while(flag==0)
    {
    	sprintf(text1,"%d",quantity_Carbon_monoxide);
		//send data to client from server 
        if(send(client_socket, text1, strlen(text1),0)<0)
        {
    	    perror("Cannot send to client\n");
        }
        memset(buffer,'\0',sizeof(buffer));
		//recv ack from client 
        if(recv(client_socket, buffer, sizeof(buffer),0)<0)
        {
        	perror("Cannot receive from client\n");
        }
		printf("Received Acknoledgement FROM CLIENT :%s\n",buffer);
		fprintf(fd,"Received Acknoledgement FROM CLIENT :%s\n",buffer);

		fault=strtok(buffer,":");
		if(strcmp(fault,"OK")==0)
		{
			printf("QUANTITY OF Carbon monoxide IS MINIMUM\n");
			fprintf(fd,"QUANTITY OF Carbon monoxide IS MINIMUM\n");
			flag=1;
		}
		else
		{
			fprintf(fd,"Adjusting the quantity of Carbon monoxide to minimum value\n");
			quantity_Carbon_monoxide=getupdatetominimum(quantity_Carbon_monoxide,gasname);

		}
	}
	//send(client_socket, text1, strlen(text1), 0);
	printf("Sent Carbon monoxide to client.\n");
	fprintf(fd,"Sent Carbon monoxide to client.\n\n");
	printf("\n");

	// Send second text data
	sleep(1); // Introducing a delay for demonstration purposes
	char text2[1024];
	strcpy(gasname,"NitrogenOxide");
	flag=0;
	while(flag==0)
	{
		sprintf(text2,"%d",Nitrogen_oxide);
		//send data to client from server 
		if(send(client_socket, text2, strlen(text2),0)<0)
		{
			perror("Cannot send to client\n");
		} 

		memset(buffer,'\0',sizeof(buffer));
		//recv ack from client 
		if(recv(client_socket, buffer, sizeof(buffer),0)<0)
		{
			perror("cannot receive from client\n");
		}
		printf("Received Acknoledgement FROM CLIENT  :%s\n",buffer);
		fprintf(fd,"Received Acknoledgement FROM CLIENT  :%s\n",buffer);

		fault=strtok(buffer,":");
		if(strcmp(fault,"OK")==0)
		{
			printf("QUANTITY OF Nitrogen oxide IS MINIMUM\n");
			fprintf(fd,"QUANTITY OF Nitrogen oxide IS MINIMUM\n");
			flag=1;
		}
		else
		{
			fprintf(fd,"Adjusting the quantity of Nitrogen Oxide to minimum value\n");
			Nitrogen_oxide=getupdatetominimum(Nitrogen_oxide,gasname);
		}
	}
	printf("Sent NITROGEN OXIDE to client.\n");
	fprintf(fd,"Sent NITROGEN OXIDE to client.\n\n");
	printf("\n");

	// Send third text data
	sleep(1); // Introducing a delay for demonstration purposes
	char text3[1024];
	strcpy(gasname,"Hydrocarbons");
	flag=0;
	while(flag==0)
	{
		sprintf(text3,"%d",Hydro_Carbons);
		//send data to client from server 
		if(send(client_socket, text3, strlen(text3),0)<0)
		{
			perror("Cannot send to client\n");
		}		
		//clear buffer to NULL
		memset(buffer,'\0',sizeof(buffer));
		//recv ack from client 
		if(recv(client_socket, buffer, sizeof(buffer),0)<0)
		{
			perror("Cannot receive from client\n");
			exit(0);
		}
		printf("Received Acknoledgement from client:%s\n",buffer);
		fprintf(fd,"Received Acknoledgement from client:%s\n",buffer);

		fault=strtok(buffer,":");
		if(strcmp(buffer,"OK")==0)
		{
			printf("QUANTITY OF HYDROCARBONS IS MINIMUM\n");
			fprintf(fd,"\nQUANTITY OF HYDROCARBONS IS MINIMUM\n");
			flag=1;
		}
		else
		{
			fprintf(fd,"\nAdjusting the quantity of HydroCarbons to minimum value\n");
			Hydro_Carbons=getupdatetominimum(Hydro_Carbons,gasname);
		}
	}
	printf("Sent HYDROCARBONS DATA to client.\n");
	fprintf(fd,"Sent HYDROCARBONS DATA to client.\n\n");
	fclose(fd);
}
