#include <sys/lfsyscall.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

char buffer[1024];

void send(char* msg)
    {
    int rv;
    int size= strlen(msg) +1;
    fprintf(stderr, "SEND: sending %d bytes of '%s' \n", size, msg);
    rv= lfsyscall(0, msg, size);
    fprintf(stderr, "SEND: returned %d \n", rv);
    }

int receive(int size)
    {
    int rv;
    fprintf(stderr, "RECEIVE: receiving\n");
    rv=lfsyscall(1, buffer, size);
    fprintf(stderr, "RECEIVE: returned %d \n", rv);
    fprintf(stderr, "RECEIVE: received '%s' \n", buffer);
    return rv;
    }
    
void fixed_itoa(int n, char* str)
    {
    char result[10]= "000000000";
    char buffer[10];
    int len;
    sprintf(buffer,"%d",n);
    len= strlen(buffer);
    strcpy(&result[9-len], buffer);
    strcpy(str, result);
    }
    
void test_wait_queues()
    {
    int pid, i;
    int iterations= 5;
    char msg[10];
    msg[1]='\0';
    
    pid= fork();
    if (pid==0)
        {
        msg[0]= '0';
        for (i=0; i<iterations; i++)
            {
            msg[0]+=1;
            send(msg);
            printf("sent message\n");
            }
        }
    else
        {
        for (i=0; i<iterations; i++)
            {
            receive(1000);
            printf("received message\n");
            sleep(1);
            }
        }
        
    printf("-------------------------------\n");
    
    pid= fork();
    if (pid==0)
        {
        msg[0]= '0';
        for (i=0; i<iterations; i++)
            {
            msg[0]+=1;
            send(msg);
            printf("sent message\n");
            sleep(1);
            }
        }
    else
        {
        for (i=0; i<iterations; i++)
            {
            receive(1000);
            printf("received message\n");
            }
        }
        
        
    }
    
void test_concurrency()
    {
    int i;
    int iterations= 1000;
    char msg[10];
    

    if (fork()==0)
        {
        fixed_itoa(i, msg);
        for (i=0; i<iterations; i++)
            {
            send(msg);
            printf("%s sent\n", msg);
            }
        }
    if (fork()==0)
        {
        for (i=0; i<iterations; i++)
            {
            receive(10);
            printf("%s received\n", buffer);
            }
        }
    }

void main()
    {   
    //test_concurrency();
    test_wait_queues();
    }
