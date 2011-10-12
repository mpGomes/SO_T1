#include <sys/lfsyscall.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char buffer[1024];

void send(char* msg)
    {
    int rv;
    int size= strlen(msg) +1;
    printf("SEND: sending %d bytes of '%s' \n", size, msg);
    rv= lfsyscall(0, msg, size);
    printf("SEND: returned %d \n", rv);
    }

void receive(int size)
    {
    int rv;
    printf("RECEIVE: receiving\n");
    rv=lfsyscall(1, buffer, size);
    printf("RECEIVE: returned %d \n", rv);
    printf("RECEIVE: received '%s' \n", buffer);
    }
void main()
{   
    send("bla");
    receive(5);
}
