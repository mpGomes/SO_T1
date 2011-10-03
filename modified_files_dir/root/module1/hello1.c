#include <linux/module.h> /* Needed by all modules */

int init_module(void) {
	printk("Hello world 1.\n");
	return 0; 		/* A non 0 return means init module failed;
				module can’t be loaded. */
}

void cleanup_module(void) {
	printk("Goodbyeworld 1.\n");
}
