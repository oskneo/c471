#include<stdio.h>
#include<sys/socket.h> 
#include<arpa/inet.h>
#include <string.h>
#include<netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <net/if.h>


#define port4 33455
#define port6 33446
#define buffersize6 1280
#define buffersize4 1440
#define fn "fileToTransfer"
#define address "127.0.0.1"


int receivefile(int nsocket,char * msg,int bs){
    
    int reno=0;
    //Initiate the variable for the size of the requested file.
    
    if(recv(nsocket,&reno,4,0)<0){
        printf("Receiving fail.\n");
    }
    else{
        printf("Receiving successfully.\n");
        
        if(reno==-1){
            printf("COULD NOT OPEN REQUESTED FILE!\n");
            return -1;
            
        }
        
        
        printf("The size of file is %d.\n",reno);
    }
    //Assign the file size received from server to this variable.
    
    
    
    //Build a file to write data received from server on it.
    FILE *file=fopen(msg,"w");
    rewind(file);
    char buf[bs];
    bzero(buf,bs);
    int received=bs;
    
    //Use a while loop to receive fragments of the file and write them on the file built above.
    while(reno>0){
        
        printf("reno:%d\n",reno);
        received=recv(nsocket,buf,bs,0);
        //The fragments of the file will be received and written to buffer first. 
        
        if(received<0){
            printf("Receiving fail.\n");
            fclose(file);
            return 1;
        }
        else if(received==0){
            break;
        }
        else{
            if(reno<bs){
                received=reno;
            }
            printf("received:%d\n",received);
            //Then write to the file on disk.
            fwrite(buf,received,1,file);
            reno-=received;
            bzero(buf,bs);
            //After finishing writing one buffer,
            //clear the buffer and wait for receiving another fragment.
            
        }
    }
    fclose(file);
    printf("Receiving successfully.\n");
    
    return 0;
    
    
    
}


int sock(char * hn, int pt[2], int *buffer,int bfa[2]){

    struct addrinfo *rs;
    struct addrinfo indication;
    //Initiate socket file descripter, address info, server and client struct for setting connections.


    int socket_desc,errno=0;


    bzero(&indication,sizeof(struct addrinfo));
    if( getaddrinfo(hn, NULL, &indication, &rs) != 0){
        printf("Cannot get the ip address from the host name.");
        return -1;
    }
    //Get the address from host name. 
    //This address contains the type of IP connection.
    //Then the function can deal with IPv4 and IPv6 seperately.

    switch(rs->ai_family){
        //When ai_family is equal to AF_INET, it is IPv4.
        case AF_INET:{
            puts("This is an IPv4 address.");
            
            

            struct sockaddr_in *server4;
            socket_desc = socket(AF_INET , SOCK_STREAM , 0);
            //Create an IPv4 TCP socket.
     
            if (socket_desc == -1)
            {
                printf("Could not create socket\n");
            }
            else{
                printf("Create socket successfully.\n");   
            }
            
            //Use the address above to set the connection.
            
            
            
            
            server4 = (struct sockaddr_in *)rs->ai_addr;
            server4->sin_port=htons(pt[0]);
            *buffer=bfa[0];
            //Let buffer be the size set for IPv4.

            errno=connect(socket_desc, (struct sockaddr *)server4, sizeof(*server4));
            //Build the IPv4 connection to server using the settings.
            
            int socket_mtu=0;
            int sz=sizeof(socket_mtu);
            getsockopt(socket_desc,IPPROTO_IP,IP_MTU,(char *)&socket_mtu, &sz );
            printf("Original MTU is %d.\n",socket_mtu); 
            //Check the MTU of this connetion.
            
            
            
            

            break;
        }
        //When ai_family is equal to AF_INET6, it is IPv6.
        case AF_INET6:{
            puts("This is an IPv6 address.");

            

            struct sockaddr_in6 *server6;
            socket_desc = socket(AF_INET6 , SOCK_STREAM , 0);
            //Create an IPv6 TCP socket.
            
     
            if (socket_desc == -1)
            {
                printf("Could not create socket\n");
            }
            else{
                printf("Create socket successfully.\n");   
            }

            

            int on=1;
            if (setsockopt(socket_desc, IPPROTO_IPV6, IPV6_V6ONLY, &on, sizeof(on)) < 0) {
                perror("IPV6_V6ONLY setting failed.");
                return -1;
            }
            

            
            //Use the address above to set the connection.
            server6 = (struct sockaddr_in6 *)rs->ai_addr;
            server6->sin6_port=htons(pt[1]);
            server6->sin6_family=AF_INET6;
            *buffer=bfa[1];
            //Let buffer be the size set for IPv6.
            errno=connect(socket_desc, (struct sockaddr *)server6, sizeof(*server6));
            //Build the IPv4 connection to server using the settings.
            
            
            
            
            int socket_mtu=0;
            int sz=sizeof(socket_mtu);
            getsockopt(socket_desc,IPPROTO_IPV6,IPV6_MTU,(char *)&socket_mtu, &sz );
            printf("Original MTU is %d.\n",socket_mtu); 
            //Check the MTU of this connetion.
            
            int mtu=*buffer+40;
            if(mtu>=1280){
                if (setsockopt(socket_desc, IPPROTO_IPV6, IPV6_MTU, &mtu, sizeof(mtu)) < 0) {
                    perror("IPV6_MTU setting failed.");
                    exit(1);
                }
            }
            //Set the MTU using buffer size.
            

        }

        
    }
    if(errno<0){
        printf("Connection failed.\n");
        return -1;
    }
    else{
        printf("Connect successfully.\n");
    }
    
    if(send(socket_desc,buffer,4,0)<0){
        printf("Sending failed.\n");
    }
    else{
        printf("Sending new buffer size successfully.\n");
    }
    //Send the buffer size to server.



    return socket_desc;
}
 
int main(int argc , char *argv[])
{
    
    int pt6=port6,
    pt4=port4,
    pt[2],
    bfa[2]={buffersize4,buffersize6},
    buffer=0;
    //Initiate variables for setting defaults.
    
    char hn[50]=address;
    
    char msg[30];
    bzero(msg,30);
    strcpy(msg,fn);
    
    int i;
    for(i=1;i<argc;i++){
        if(i==4){
            bzero(msg,30);
            strcpy(msg,argv[i]);
        }
        else if(i==2){
            pt6=atoi(argv[i]);
            pt4=atoi(argv[i]);
        }
        else if(i==5){
            bfa[0]=atoi(argv[i]);
            bfa[1]=atoi(argv[i]);
        }
        else if(i==1){
            bzero(hn,50);
            strcpy(hn,argv[i]);
        }
    }
    
    //Receive arguments to change defaults.
    //An example of command of using this program:
    // ./client.o ::1 33446 localhost 1.txt 1460
    
    pt[0]=pt4;
    pt[1]=pt6;


    
    
    
    //Initiate socket file descripter for setting connections.
    int socket_desc;

    socket_desc = sock(hn,pt,&buffer,bfa);
    //It is a function to create socket connection and return a socket file descripter
    //passing host name, port numbers and buffer sizes.
    
     
    if (socket_desc == -1)
    {
        printf("Could not create socket\n");
        return -1;
    }
    else{
        printf("Create socket successfully.\n");   
    }
    
    //After creating connection successfully, send the file name to sevrer.
    if(send(socket_desc,msg,strlen(msg),0)<0){
        printf("Sending fail.\n");
        return 1;
    }
    else{
        printf("Sending successfully.\n");
    }
    
    
    //Then use a function to receive a file from server.
    receivefile(socket_desc,msg, buffer);
    close(socket_desc);
    
     
    return 0;
}


