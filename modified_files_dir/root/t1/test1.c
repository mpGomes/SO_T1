#include <sys/lfsyscall.h> 
#include <stdio.h>
#include <stdlib.h>

void main()
{   
    int return_value;
    char* sent_message= "bla\0";
    char* received_message= malloc(10);
    
    printf("sending '%s' \n", sent_message);
    return_value= lfsyscall(0, sent_message, 4);
    printf("returned %d \n", return_value);
    
    printf("receiving\n");
    return_value=lfsyscall(1, received_message, 4);
    printf("returned %d \n", return_value);
    printf("received '%s' \n", received_message);
    
    printf("all done! \n");
}
