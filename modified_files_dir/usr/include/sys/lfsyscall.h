#include <unistd.h>

#include <sys/syscall.h>

#define __NR_lfsyscall		326
#define lfsyscall(X)		syscall(__NR_lfsyscall, X);
