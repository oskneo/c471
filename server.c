#include<stdio.h>
#include<sys/socket.h> 
#include<arpa/inet.h>
#include <string.h>
 
int main(int argc , char *argv[])
{
    int socket_desc;
    struct sockaddr_in server, client;
    
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
     
    if (socket_desc == -1)
    {
        printf("Could not create socket\n");
    }
    else{
        printf("Create socket successfully.\n");   
    }
    
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;
    server.sin_port= htons(33445);
    
    if(bind(socket_desc, (struct sockaddr *)&server, sizeof(server))<0){
        printf("Bind fail.\n");
        return 1;
    }
    else{
        printf("Bind successfully.\n");
    }
    
    listen(socket_desc, 3);
    printf("Waiting...\n");
    int c = sizeof(struct sockaddr_in);
    int newsocket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (newsocket<0){
        perror("Accept failed.\n");
    }
    else{
        printf("Accept successfully.\n");
    }
    
    // char server_reply[2000];
    // read(socket_desc, server_reply , 2000);
    // printf("%s\n",server_reply);
    
    char* nmsg="hhhh\n";
    write(newsocket,nmsg,strlen(nmsg));
    char msg[1024];
    if(read(newsocket,msg,1024)<0){
        printf("Receiving fail.\n");
    }
    else{
        printf("Receiving successfully.\n");
        printf("%s\n",msg);
    }
    
    
     
    return 0;
}