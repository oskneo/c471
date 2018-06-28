#include<stdio.h>
#include<sys/socket.h> 
#include<arpa/inet.h>
#include <string.h>
 
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
    
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port= htons(33445);
    
    if(connect(socket_desc, (struct sockaddr *)&server, sizeof(server))<0){
        printf("Connection fail.\n");
        return 1;
    }
    else{
        printf("Connect successfully.\n");
    }
    
    char* msg="Hiiiiiii!\n";
    // write(newsocket,nmsg,strlen(nmsg));
    
    
    // int sock = *(int*)socket_desc;
    // write(sock,msg,strlen(msg));
    
    if(send(socket_desc,msg,strlen(msg),0)<0){
        printf("Sending fail.\n");
        return 1;
    }
    else{
        printf("Sending successfully.\n");
    }
    
    char rmsg[1024];
    if(recv(socket_desc,rmsg,1024,0)<0){
        printf("Receiving fail.\n");
    }
    else{
        printf("Receiving successfully.\n");
        printf("%s\n",rmsg);
    }
    
     
    return 0;
}