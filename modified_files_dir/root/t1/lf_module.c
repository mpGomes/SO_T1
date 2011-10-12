#include <asm/uaccess.h>
#include <linux/wait.h>
#include <linux/module.h>
#include <linux/gfp.h>
#include <linux/sched.h>
#include <asm-x86/cmpxchg_32.h>

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

static DECLARE_WAIT_QUEUE_HEAD(write_wait_queue);
static DECLARE_WAIT_QUEUE_HEAD(read_wait_queue);

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
    unsigned long result= cmpxchg( cell, (unsigned long) oldvalue, (unsigned long) newvalue);
    if (result==oldvalue)
        return 1;
    else
        return 0;
    }

int lfsend(queue_obj* q, const void *msg, int size)
{
    int rear;
    msg_obj* x;
    msg_obj* new_message= (msg_obj*) kmalloc( sizeof(msg_obj), GFP_KERNEL);
    printk("LFSEND: enqueuing '%s', size %i", (char*)msg, size);
    
    new_message->size= size;
    copy_from_user( new_message->msg, msg, size);
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
            printk("LFSEND: queue full, sleeping");
            wait_event( write_wait_queue, rear!=q->head + QUEUE_SIZE );
            continue;
            }

        if (x == NULL)
            {
            if ( compare_and_swap( (int*) &(q->queue[rear % QUEUE_SIZE]), (int) NULL, (int) new_message) )
                {
                compare_and_swap( (int*) &(q->tail), (int) rear, (int) rear+1);
                wake_up( &read_wait_queue );
                printk("LFSEND: enqueue succeeded");
                return 0;
                }
            }
        else
            {
            printk("LFSEND: concurrent enqueue detected, incrementing rear and retrying");
            compare_and_swap( (int*) (&q->tail), rear, rear+1);
            continue;
            }
        }
    return -123;
}

int lfreceive(queue_obj* q, const void *msg, int size)
{
	int front, size_to_read, offset, left;	/* index of the front of the queue */
	msg_obj* x;
	while(1) {
		front = q->head;
		x = q->queue[front % QUEUE_SIZE];
		
		if (front != q->head){
			printk("LFRECIEVE: Concurrent access\n");
			continue;
		}
		if (front == q->tail){
			printk("LFRECIEVE: Empty queue. sleeping\n");
            wait_event( read_wait_queue, front!=q->tail );
			continue;
		}
		if (x != 0){
			offset = x->read_offset;
			left = x->size - offset;
			size_to_read =  (size > left ) ? left : size;
			if (size_to_read < size) {	/* there is still message left */
				if (compare_and_swap((int*)&(q->queue[front % QUEUE_SIZE]),(int)x,(int)x)) 
					if(compare_and_swap(&(x->read_offset), offset, offset+size_to_read)) {
						copy_to_user(msg,&(x->msg[offset]), size_to_read);
						printk("LFRRECIEVE:Message copied, size read : %d\n", size_to_read);
                        wake_up( &write_wait_queue );
						return	size_to_read;
				}
			}
			else if (compare_and_swap((int*)&(q->queue[front % QUEUE_SIZE]),(int)x,0)) {
				compare_and_swap((int*)&(q->head), (int)front, front+1);
				copy_to_user(msg,&(x->msg[offset]), size_to_read);
				printk("LFRECIEVE:Message read completely\n");
				return	size_to_read;
				}
			}
		else {
			printk("LFRECIEVE: Concurrent access - update front");
			compare_and_swap((int*)&(q->head),(int)front,front+1);
		}
	}
} 
 
int lf_impl_internal( int send_or_receive, const void *msg, int size)
    {
    if (send_or_receive==0)
        return lfsend( &global_queue, msg, size );
    if (send_or_receive==1)
        return lfreceive( &global_queue, msg, size );
    return -1;
    }
            
