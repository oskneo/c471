#include<stdio.h>
#include<sys/socket.h> 
#include<arpa/inet.h>
 
int main(int argc , char *argv[])
{
    int socket_desc;
    struct sockaddr_in server;
    
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
     
    if (socket_desc == -1)
    {
        printf("Could not create socket\n");
    }
    else{
        printf("Create socket successfully.\n");   
    }
     
    return 0;
}