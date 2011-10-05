#include <linux/kernel.h>
#include <linux/linkage.h>
#include <linux/module.h>

int (* lf_impl) (int, const void*, int);

EXPORT_SYMBOL(lf_impl);

asmlinkage int sys_lfsyscall(int send_or_receive, const void *msg, int size) {
	if (lf_impl == 0)
		return -1;
	else
		return lf_impl(send_or_receive, msg, size);
}
