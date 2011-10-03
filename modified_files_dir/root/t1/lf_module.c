#include <asm/uacces.h>
#include <linux/wait.h>

static DECLARE_WAIT_QUEUE_HEAD(wait_queue);
char *head;
char *tail;


int compare_and_swap (int *cell, int oldvalue, int newvalue)
    {
    char result;
    asm ("lock cmpxchg %2, (%1) ; setz %0"
    : "=r" (result)
    : "r"(cell), "r"(newvalue), "a"(oldvalue));
    return result;
    }

int lfsend( const void *msg, int size)
    {
    
    }
    
int lfreceive( const void *msg, int size)
    {
    return 0;
    } 
    
int lf_impl( int send_or_receive, const void *msg, int size)
    {
    if (send_or_receive==0)
        return lfsend( msg, size );
    if (send_or_receive==1)
        return lfsend( msg, size );
    return -1;
    }
            
