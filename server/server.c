#include<stdio.h>
#include<sys/socket.h> 
#include<arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include<sys/wait.h>
#include<sys/types.h>  


#define queue 20
#define port4 33455
#define port6 33446
#define buffersize6 1280
#define buffersize4 1440
#define fn "fileToTransfer"


int sendfile(int nsocket,char * msg,int bs){
    
    FILE *file=fopen(msg,"r");
    int filesize=0;
    if(file==NULL){
        filesize=-1;
        write(nsocket,&filesize,4);
        return -1;
    }
    
    
    
    fseek(file, 0L, SEEK_END);
    filesize = ftell(file);
    rewind(file);
    
    
    write(nsocket,&filesize,4);
    
    char buf[bs];
    bzero(&buf,bs);
    while(filesize>0){
        fread(buf,1,bs,file);
        
        if(send(nsocket,buf,bs,0)<0){
            printf("Sending file failed.\n");
            fclose(file);
            return 1;
        }
        filesize-=bs;
        bzero(&buf,bs);
    }
    fclose(file);
    
    printf("Sending successfully.\n");
    return 0;
    
    
    
    
    
    
}


int connection46(int port, int buffer,int type){
    
    int socket_desc;
    struct sockaddr_in server, client;
    struct sockaddr_in6 server6, client6;
    
    void * sv;
    void * cl;
    
    if(type==0){
        socket_desc = socket(AF_INET , SOCK_STREAM , 0);
        
        bzero(&server,sizeof(server));
        server.sin_addr.s_addr = INADDR_ANY;
        server.sin_family = AF_INET;
        server.sin_port= htons(port);
        
        sv=&server;
        
        
    }
    else{
        socket_desc = socket(AF_INET6 , SOCK_STREAM , 0);
        
        int on=1;
        if (setsockopt(socket_desc, IPPROTO_IPV6, IPV6_V6ONLY, &on, sizeof(on)) < 0) {
            perror("IPV6_V6ONLY setting failed.");
            exit(1);
        }

        int mtu=700;
        if (setsockopt(socket_desc, IPPROTO_IPV6, IPV6_MTU, &mtu, sizeof(mtu)) < 0) {
            perror("IPV6_MTU setting failed.");
            exit(1);
        }
        bzero(&server6,sizeof(server6));
        server6.sin6_addr = in6addr_any;
        server6.sin6_family = AF_INET6;
        server6.sin6_port= htons(port);
        
        sv=&server6;
        
    }
    
    
     
    if (socket_desc == -1)
    {
        printf("Could not create socket\n");
        
        return -1;
    }
    else{
        printf("Create socket successfully.\n");   
    }
    
    
    if(bind(socket_desc, type==0 ? (struct sockaddr *)&server : (struct sockaddr *)&server6, type==0 ? sizeof(server) : sizeof(server6))<0){
        printf("Bind fail.\n");
        return 1;
    }
    else{
        printf("Bind successfully.\n");
    }
    
    listen(socket_desc, queue);
    printf("Waiting for IPv%d connection at port %d...\n",type==0 ? 4:6, port);
    int c;
    c = type==0 ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6);
    int newsocket;
    while(newsocket = accept(socket_desc, type==0 ? (struct sockaddr *)&client : (struct sockaddr *)&client6, (socklen_t*)&c)){
        pid_t f=fork();
        if(f==0){
            puts(type==0 ? "This is a IPv4 connection." : "This is a IPv6 connection.");
            if (newsocket<0){
                perror("Accept failed.\n");
                return -1;
            }
            else{
                printf("Accept successfully.\n");
            }
    
    
    
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
    
    
    int i;
    for(i=1;i<argc;i++){
        if(strcmp(argv[i],"-p6")==0){
            pt6=atoi(argv[i+1]);
        }
        if(strcmp(argv[i],"-p4")==0){
            pt4=atoi(argv[i+1]);
        }
        if(strcmp(argv[i],"-b")==0){
            bfa[0]=atoi(argv[i+1]);
            bfa[1]=atoi(argv[i+1]);
        }
    }
    
    int pt[2]={pt4,pt6};
    
    
    // int socket_desc;
    // struct sockaddr_in server, client;
    
    // socket_desc = socket(AF_INET , SOCK_STREAM , 0);
     
    // if (socket_desc == -1)
    // {
    //     printf("Could not create socket\n");
    // }
    // else{
    //     printf("Create socket successfully.\n");   
    // }
    
    // server.sin_addr.s_addr = INADDR_ANY;
    // server.sin_family = AF_INET;
    // server.sin_port= htons(pt);
    
    // if(bind(socket_desc, (struct sockaddr *)&server, sizeof(server))<0){
    //     printf("Bind fail.\n");
    //     return 1;
    // }
    // else{
    //     printf("Bind successfully.\n");
    // }
    
    // listen(socket_desc, queue);
    // printf("Waiting...\n");
    // int c = sizeof(struct sockaddr_in);
    // int newsocket;
    // while(newsocket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)){
    //     int f=fork();
    //     if(f==0){
    //         if (newsocket<0){
    //             perror("Accept failed.\n");
    //             return -1;
    //         }
    //         else{
    //             printf("Accept successfully.\n");
    //         }
    
    
    
    //         char msg[30];
    //         if(read(newsocket,msg,30)<0){
    //             printf("Receiving fail.\n");
    //             return -1;
    //         }
    //         else{
    //             printf("Receiving successfully.\n");
    //             printf("%s\n",msg);
    //         }
    
    
    //         sendfile(newsocket,msg,buffer);
    //     }
    //     else if(f<0){
    //         printf("Child process creating failed.\n");
    //         return -1;
    //     }
    
    // }
    
    
     
    // return 0;
    
    int processId[2],pid;
    for(i=0;i<2;i++){
        pid=(int)fork();
        if(pid==0){
            
            // printf("%d\n",getpid());
            int buffer=bfa[i];
            if(connection46(pt[i],buffer,i)!=0){
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
        // switch(status){
            
        // }
    }
    
    // int pc6;
    // int pc4=fork();
    // if(pc4==0){
    //     if(connection4(pt,buffer)!=0){
    //         exit(1);
    //     }
    // }
    // else if(pc4<0){
    //     puts("The process to build ipv4 connection generated failed.");
    // }
    // else{
    //     // pc6=fork();
    //     // if(pc6==0){
    //     //     if(connection6(pt,buffer)!=0){
    //     //         exit(1);
    //     //     }
    //     // }
    //     // else if(pc6<0){
    //     //     puts("The process to build ipv6 connection generated failed.");
    //     // }
    //     // else{
        
    //     // }
    // }
    
    printf("%d\n",processId[0]);
    
    return 0;
}


