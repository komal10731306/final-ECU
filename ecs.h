#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include<time.h>

#define PORT 12345
#define MAX_BUFFER_SIZE 1024
//handle_client used to send info from server to client 
void handle_client(int );
//adjusting to minimum value (range)
uint16_t getupdatetominimum(uint16_t ,char *);
