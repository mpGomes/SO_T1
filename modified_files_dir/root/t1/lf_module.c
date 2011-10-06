#include <asm/uaccess.h>
#include <linux/wait.h>
#include <linux/module.h>
#include <linux/gfp.h>

extern int (*lf_impl)(int, const void* , int);
int lf_impl_internal(int, const void* , int);

static int QUEUE_SIZE= 512;
static int MESSAGE_SIZE= 512;
char** queue;
char* message_storage;

static DECLARE_WAIT_QUEUE_HEAD(wait_queue);
char **head;
char **tail;

int init_module(void) {
    printk("lf_module loaded\n");
    queue= (char**) kmalloc( QUEUE_SIZE*sizeof(char*), GFP_KERNEL );
    if (queue==NULL)
        return -1;
    message_storage= (char*) kmalloc( QUEUE_SIZE*MESSAGE_SIZE*sizeof(char), GFP_KERNEL);
    if (message_storage==NULL)
        return -1;
    head= queue;
    tail= queue;
    lf_impl= lf_impl_internal;
    return 0;
}

void cleanup_module(void) {
    kfree(queue);
    kfree(message_storage);
    printk("lf_module unloaded\n");
}




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
    return 0;
    }
    
int lfreceive( const void *msg, int size)
    {
    return 0;
    } 
    
int lf_impl_internal( int send_or_receive, const void *msg, int size)
    {
    printk("123");
    if (send_or_receive==0)
        return lfsend( msg, size );
    if (send_or_receive==1)
        return lfsend( msg, size );
    return -1;
    }
            
