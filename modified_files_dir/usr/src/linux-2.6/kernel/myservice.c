#include <linux/kernel.h>

asmlinkage int sys_myservice (char* arg1){
	printk("%s", arg1);
	return 1;
}
