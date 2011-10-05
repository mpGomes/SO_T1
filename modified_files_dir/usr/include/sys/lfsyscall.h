#include <unistd.h>

#include <sys/syscall.h>

#define __NR_lfsyscall		326
#define lfsyscall(X, Y, Z)		syscall(__NR_lfsyscall, X, Y, Z);
