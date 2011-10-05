#include <linux/kernel.h>
#include <linux/linkage.h>
#include <linux/module.h>

int (* sys_lf_impl) (char *);

EXPORT_SYMBOL(sys_lf_impl);

asmlinkage int sys_lfsyscall(char* arg1) {
	if (sys_lf_impl == 0)
		return -1;
	else
		return sys_lf_impl(arg1);
}
