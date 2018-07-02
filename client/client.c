#include<stdio.h>
#include<sys/socket.h> 
#include<arpa/inet.h>
#include <string.h>
#include<netdb.h>
#include <unistd.h>
#include <stdlib.h>

#define port4 33455
#define port6 33446
#define buffersize6 1280
#define buffersize4 1440
#define fn "fileToTransfer"
#define address "127.0.0.1"


int receivefile(int nsocket,char * msg,int bs){
    
    int reno=0;
    
    if(recv(nsocket,&reno,4,0)<0){
        printf("Receiving fail.\n");
    }
    else{
        printf("Receiving successfully.\n");
        // printf("%s\n",rmsg);
        // char nmsg[100];
        // strcpy(nmsg,"The size of file is ");
        // strcat(nmsg, itoa(filesize));
        // strcat(nmsg, ".\n");
        
        if(reno==-1){
            printf("COULD NOT OPEN REQUESTED FILE!\n");
            return -1;
            
        }
        
        
        printf("The size of file is %d.\n",reno);
    }
    
    FILE *file=fopen(msg,"a");
    char buf[bs];
    bzero(&buf,bs);
    int received=bs;
    
    while(reno>0){
        
        if(reno<bs){
            received=reno;
        }
        recv(nsocket,buf,received,0);
        // printf("%d\n",reno);
        if(received<0){
            printf("Receiving fail.\n");
            fclose(file);
            return 1;
        }
        // else if(received==0){
        //     break;
        // }
        else{
            
            fwrite(buf,1,received,file);
            reno-=received;
            bzero(&buf,bs);
            
        }
    }
    fclose(file);
    printf("Receiving successfully.\n");
    
    return 0;
    
    
    
}


int sock(char * hn, int pt[2]){

    struct addrinfo *rs;
    struct addrinfo indication;
    // int family;


    int socket_desc,errno=0;


    bzero(&indication,sizeof(struct addrinfo));
    if( getaddrinfo(hn, NULL, &indication, &rs) != 0){
        printf("Cannot get the ip address from the host name.");
        return -1;
    }

    switch(rs->ai_family){
        case AF_INET:{
            puts("This is an IPv4 address.");

            struct sockaddr_in *server4;
            socket_desc = socket(AF_INET , SOCK_STREAM , 0);
     
            if (socket_desc == -1)
            {
                printf("Could not create socket\n");
            }
            else{
                printf("Create socket successfully.\n");   
            }
    //server.sin_addr.s_addr = inet_addr(inet_ntoa(*((struct in_addr *)adr->h_addr_list[0])));
            // server4.sin_addr.s_addr = inet_addr("::1");
    
            // server4.sin_family = AF_INET;
            // server4.sin_port= htons(pt[0]);
            // puts("gggggg");
            server4 = (struct sockaddr_in *)rs->ai_addr;
            // server4->sin_addr.s_addr=inet_addr(rs->ai_addr->sa_data);
            server4->sin_port=htons(pt[0]);
            // server4->sin_family=AF_INET;
            // puts("gggggg1");

            errno=connect(socket_desc, (struct sockaddr *)server4, sizeof(*server4));
            // errno=connect(socket_desc, rs->ai_addr, rs->ai_addrlen);
            
            // puts("gggggg2");
            

            break;
        }
        case AF_INET6:{
            puts("This is an IPv6 address.");

            struct sockaddr_in6 *server6;
            socket_desc = socket(AF_INET6 , SOCK_STREAM , 0);
     
            if (socket_desc == -1)
            {
                printf("Could not create socket\n");
            }
            else{
                printf("Create socket successfully.\n");   
            }
    //server.sin_addr.s_addr = inet_addr(inet_ntoa(*((struct in_addr *)adr->h_addr_list[0])));
            // server4.sin_addr.s_addr = inet_addr("::1");
    
            // server4.sin_family = AF_INET;
            // server4.sin_port= htons(pt[0]);

            int on=1;
            if (setsockopt(socket_desc, IPPROTO_IPV6, IPV6_V6ONLY, &on, sizeof(on)) < 0) {
                perror("IPV6_V6ONLY setting failed.");
                return -1;
            }
            // puts("ggggg");
            // server6=(struct sockaddr_in6*)malloc(sizeof(struct sockaddr_in6));
            // bzero(server6,sizeof(*server6));
            // memcpy(&server6, rs->ai_addr, rs->ai_addrlen);
            // puts("ggggg1");
            server6 = (struct sockaddr_in6 *)rs->ai_addr;
            server6->sin6_port=htons(pt[1]);
            server6->sin6_family=AF_INET6;
            // puts("ggggg2");
            errno=connect(socket_desc, (struct sockaddr *)server6, sizeof(*server6));
            // puts("ggggg3");

        }

        
    }
    if(errno<0){
        printf("Connection failed.\n");
        return -1;
    }
    else{
        printf("Connect successfully.\n");
    }




    return socket_desc;
}
 
int main(int argc , char *argv[])
{
    
    int pt6=port6,
    pt4=port4,
    pt[2],
    buffer=buffersize6;
    
    char hn[50]=address;
    
    char msg[30];
    bzero(msg,30);
    strcpy(msg,fn);
    
    int i;
    for(i=1;i<argc;i++){
        if(strcmp(argv[i],"-f")==0){
            bzero(msg,30);
            // memset(msg,0,30);
            strcpy(msg,argv[i+1]);
            // memcpy(msg,argv[i+1],strlen(argv[i+1]));
        }
        else if(strcmp(argv[i],"-p6")==0){
            pt6=atoi(argv[i+1]);
        }
        else if(strcmp(argv[i],"-p4")==0){
            pt4=atoi(argv[i+1]);
        }
        else if(strcmp(argv[i],"-b")==0){
            buffer=atoi(argv[i+1]);
        }
        else if(strcmp(argv[i],"-a")==0){
            bzero(hn,50);
            strcpy(hn,argv[i+1]);
        }
    }
    
    pt[0]=pt4;
    pt[1]=pt6;


    
    
    
    // struct hostent *adr=gethostbyname(hn);
    // if(adr==NULL){
    //     herror("gethostbyname");
    //     return 1;
    // }
    
    
    int socket_desc;
    // struct sockaddr_in server;

    socket_desc = sock(hn,pt);
    
    // socket_desc = socket(AF_INET , SOCK_STREAM , 0);
     
    if (socket_desc == -1)
    {
        printf("Could not create socket\n");
        return -1;
    }
    else{
        printf("Create socket successfully.\n");   
    }
    // //server.sin_addr.s_addr = inet_addr(inet_ntoa(*((struct in_addr *)adr->h_addr_list[0])));
    // server.sin_addr.s_addr = inet_addr("::1");
    
    // server.sin_family = AF_INET;
    // server.sin_port= htons(pt);
    
    // if(connect(socket_desc, (struct sockaddr *)&server, sizeof(server))<0){
    //     printf("Connection failed.\n");
    //     return 1;
    // }
    // else{
    //     printf("Connect successfully.\n");
    // }
    
    
    // printf("%lu\n",strlen(msg));
    
    if(send(socket_desc,msg,strlen(msg),0)<0){
        printf("Sending fail.\n");
        return 1;
    }
    else{
        printf("Sending successfully.\n");
    }
    
    // char rmsg[1024];
    // recv(socket_desc,rmsg,1024,0
    
    
    receivefile(socket_desc,msg, buffer);
    close(socket_desc);
    
     
    return 0;
}


