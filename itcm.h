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
//use_case:Array of structures to store 3 test cases
struct message u_case[3];
uint8_t val;

void k_on();
void s_belt();
void d_lock();
void hand_break();
void input();
void a_input(int msgid);
