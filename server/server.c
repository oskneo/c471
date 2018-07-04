#include<stdio.h>
#include<sys/socket.h> 
#include<arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include<sys/wait.h>
#include<sys/types.h>  
#include <sys/ioctl.h>
#include <net/if.h>



#define queue 20
#define port4 33455
#define port6 33446
#define buffersize6 1280
#define buffersize4 1440
#define fn "fileToTransfer"


int sendfile(int nsocket,char * msg,int bs){
    
    printf("%s will be sent...\n",msg);
    
    FILE *file=fopen(msg,"r");
    int filesize=0,sent=0;
    if(file==NULL){
        printf("COULD NOT OPEN REQUESTED FILE!\n");
        filesize=-1;
        write(nsocket,&filesize,4);
        return -1;
    }
    //This function will try to open the file depending on the file name sent from the client.
    //If not able to open the file, sent -1 to client.
    
    
    
    fseek(file, 0L, SEEK_END);
    filesize = ftell(file);
    rewind(file);
    //If able to open, check the file size and send to client.
    
    
    write(nsocket,&filesize,4);
    
    
    //Build a while loop to send fragments of the file.
    //Each fragment is equal or less than the size of the buffer. 
    char buf[bs];
    bzero(buf,bs);
    while(filesize>0){
        printf("filesize:%d\n",filesize);
        if(filesize<bs){
            bs=filesize;
        }
        //The loop will first read the size of buffer of the file to the buffer,
        //then send this content of this buffer to client.
        
        fread(buf,1,bs,file);
        sent=send(nsocket,buf,bs,0);
        printf("sent:%d\n",sent);
        if(sent<0){
            printf("Sending file failed.\n");
            fclose(file);
            return 1;
        }
        else if(sent==0){
            break;
        }
        filesize-=sent;
        bzero(buf,bs);
        //Then clear th buffer.
    }
    fclose(file);
    
    printf("Sending successfully.\n");
    return 0;
    
    
    
    
    
    
}


int connection46(int port, int buffer,int type){
    
    //Initiate socket file descripter, server and client struct for setting connections.
    
    int socket_desc;
    struct sockaddr_in server, client;
    struct sockaddr_in6 server6, client6;
    


    

    //The argument type indicates the type of IP connection.
    //0 is for IPv4. The other is for IPv6.
    if(type==0){
        //Create a TCP socket for IPv4.
        socket_desc = socket(AF_INET , SOCK_STREAM , 0);
        
        //Setting a IPv4 only connection.
        bzero(&server,sizeof(server));
        server.sin_addr.s_addr = INADDR_ANY;
        server.sin_family = AF_INET;
        server.sin_port= htons(port);
        
        
        
    }
    else{
        //Create a TCP socket for IPv6.
        socket_desc = socket(AF_INET6 , SOCK_STREAM , 0);
        
        int on=1;
        if (setsockopt(socket_desc, IPPROTO_IPV6, IPV6_V6ONLY, &on, sizeof(on)) < 0) {
            perror("IPV6_V6ONLY setting failed.");
            exit(1);
        }
        //Setting an IPv6 only connection.

        
        bzero(&server6,sizeof(server6));
        server6.sin6_addr = in6addr_any;
        server6.sin6_family = AF_INET6;
        server6.sin6_port= htons(port);
        
        
    }
    
    
     
    if (socket_desc == -1)
    {
        printf("Could not create socket\n");
        
        return -1;
    }
    else{
        printf("Create socket successfully.\n");   
    }
    
    //After creating a socket successfully, bind the sockets.
    if(bind(socket_desc, type==0 ? (struct sockaddr *)&server : (struct sockaddr *)&server6, type==0 ? sizeof(server) : sizeof(server6))<0){
        //Using ? : to discriminate IPv4 and IPv6.
        printf("Bind fail.\n");
        return 1;
    }
    else{
        printf("Bind successfully.\n");
    }
    
    //Begin to listen incoming connections.
    listen(socket_desc, queue);
    printf("Waiting for IPv%d connection at port %d...\n",type==0 ? 4:6, port);
    int c;
    c = type==0 ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6);
    int newsocket;
    while(newsocket = accept(socket_desc, type==0 ? (struct sockaddr *)&client : (struct sockaddr *)&client6, (socklen_t*)&c)){
        //When there is a incoming TCP request, accept the connection and build the TCP connection.
        //Here, the process for IPv4 only will accept IPv4 request. And the process for IPv6 only will accept IPv6 request.
        pid_t f=fork();
        //Build child processes to transfer files.
        if(f==0){
            
            if (newsocket<0){
                perror("Accept failed.\n");
                return -1;
            }
            else{
                printf("Accept successfully.\n");
            }
            
            puts(type==0 ? "This is a IPv4 connection." : "This is a IPv6 connection.");
            
            if(read(newsocket,&buffer,4)<0){
                printf("Receiving fail. Keep the original buffer size.\n");
            }
            else{
                printf("Receiving successfully.\n");
                printf("The present size of buffer is %d.\n",buffer);
            }
            
            //The process will first read the buffer size from client to set the new buffer size and MTU.
            if(type==1){
                
                
                int mtu=buffer+40;
                if(mtu>=1280){
                    if (setsockopt(socket_desc, IPPROTO_IPV6, IPV6_MTU, &mtu, sizeof(mtu)) < 0) {
                        perror("IPV6_MTU setting failed.");
                        exit(1);
                    }
                }
            }
    
            //Then receive the file name which is ready to send.
    
            char msg[30];
            bzero(msg,30);
            if(read(newsocket,msg,30)<0){
                printf("Receiving fail.\n");
                return -1;
            }
            else{
                printf("Receiving successfully.\n");
                printf("%s\n",msg);
            }
            
            
            
            
    
            //Then begin to send the file using a function by passing socket descripter, 
            //file name and buffer size.
            sendfile(newsocket,msg,buffer);
        }
        else if(f<0){
            printf("Child process creating failed.\n");
            exit(1);
        }
    
    }
    
    
     
    return 0;
    
}




 
int main(int argc , char *argv[])
{
    int pt6=port6,
    pt4=port4,
    bfa[2]={buffersize4,buffersize6};
    //Initiate variables for setting defaults.
    
    
    int i;
    for(i=1;i<argc;i++){
        if(i==2){
            pt6=atoi(argv[i]);
        }
        if(i==1){
            pt4=atoi(argv[i]);
        }
        if(i==3){
            bfa[0]=atoi(argv[i]);
            bfa[1]=atoi(argv[i]);
        }
    }
    //Receive arguments to change defaults.
    //An example of command of using this program:
    //./server.o 30001 30002 1460
    
    int pt[2]={pt4,pt6};
    
    
    
    int processId[2],pid;
    //Generate one process for listening IPv4 incoming connection and one other process for IPv6. 
    for(i=0;i<2;i++){
        pid=(int)fork();
        if(pid==0){
            int buffer=bfa[i];
            if(connection46(pt[i],buffer,i)!=0){
            //When i is 0, there is the process for listening IPv4.
            //When i is 1, it is for IPv6.
            //This function will use port number, 
            //buffer size and the type of IP connection to create connection. 
                exit(1);
            }
        }
        else if(pid<0){
            switch(i){
                case 0:
                    puts("The process to build ipv4 connection generated failed.");
                    break;
                case 1:
                    puts("The process to build ipv6 connection generated failed.");
            }
        }
        else{
            processId[i]=pid;
        }
    }
    
    int status;
    for(i=0;i<2;i++){
        waitpid(processId[i], &status, 0);
    }
    //Let the main process wait the above two processes.
    
    return 0;
}


