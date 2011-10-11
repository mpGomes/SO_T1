#include <asm/uaccess.h>
#include <linux/wait.h>
#include <linux/module.h>
#include <linux/gfp.h>

extern int (*lf_impl)(int, const void* , int);
int lf_impl_internal(int, const void* , int);

#define QUEUE_SIZE 512
#define MESSAGE_SIZE 512

typedef struct {
		char msg[MESSAGE_SIZE];
		int size;
		int read_offset;
		} msg_obj;

typedef struct {
		msg_obj* queue[QUEUE_SIZE];
		int head, tail;
		} queue_obj;

queue_obj global_queue;
static DECLARE_WAIT_QUEUE_HEAD(wait_queue);

int init_module(void) {
    printk("lf_module loaded\n");
    lf_impl= lf_impl_internal;
    return 0;
}

void cleanup_module(void) {
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

int lfsend(queue_obj* q, const void *msg, int size)
    {
    int rear;
    msg_obj* x;
    msg_obj* new_message= (msg_obj*) kmalloc( sizeof(msg_obj), GFP_KERNEL);
    printk("LFSEND: enqueuing '%s', size %i", (char*)msg, size);
    
    new_message->size= size;
    copy_from_user( &(new_message->msg), msg, size);
    while (1)
        {
        rear= q->tail;
        x= q->queue[rear % QUEUE_SIZE];
        if (rear != q->tail)
            {
            continue;
            printk("LFSEND: inconsistent state, retrying");
            }
        if (rear == q->head + QUEUE_SIZE)
            {
            continue;
            printk("LFSEND: queue full, retrying");
            }

        if (x == NULL)
            {
            if ( compare_and_swap( (int*) q->queue[rear % QUEUE_SIZE], (int) NULL, (int) new_message) )
                {
                compare_and_swap( (int*) q->tail, (int) rear, (int) rear+1);
                printk("LFSEND: enqueue succeeded");
                return 0;
                }
            }
        else
            {
            printk("LFSEND: concurrent enqueue detected, incrementing rear and retrying");
            compare_and_swap( (int*) q->tail, rear, rear+1);
            continue;
            }
        }
    return -123;
    }


int lfreceive( const void *msg, int size)
    {
    return 0;
    } 
    
int lf_impl_internal( int send_or_receive, const void *msg, int size)
    {
    printk("123");
    if (send_or_receive==0)
        return lfsend( &global_queue, msg, size );
    if (send_or_receive==1)
        return lfreceive( msg, size );
    return -1;
    }
            
