#include <unistd.h>

#include <sys/syscall.h>

#define __NR_myservice		325
#define myservice(X)		syscall(__NR_myservice, X);
