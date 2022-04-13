#include "includes.h"


#define MAX_LINE 1024

char buff[MAX_LINE];
int socket_create(int *socketfd){
int valread;
    *socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if(*socketfd==-1){
        syslog(LOG_ERR,"socket creation failed ");
        CleanAll(1);
    }  
    syslog(LOG_INFO,"socket created successfully! ");  

}

void connection_test(int *socketfd, struct sockaddr_in servaddr){
    int rc,valread;
    int count=0;

    while(1){
        rc = connect(*socketfd , (struct sockaddr *)&servaddr, sizeof(servaddr));
        if(rc!=0){
            count++;
            if(count==5){
                syslog(LOG_ERR,"Failed to connect to server 5 time, program is closing...");
                CleanAll(1);
            }
            syslog(LOG_INFO,"Connection to server failed %d times, retrying....",count);
            syslog(LOG_ERR,"Error code: %s ",strerror(errno));
            printf("ERROR INFO: %s \n",strerror(errno));
            printf("ERROR CODE: %d \n",errno);
            sleep(1);
            
        }
        else
        {
            syslog(LOG_INFO,"Successfully connected to server!");
            break;
        }
    }
}

void socket_connect(int *socketfd, struct sockaddr_in servaddr, char *address, char *port)
{
    int PortNum;
    char *portchar;
    if((address==NULL)&&(port==NULL)){
        syslog(LOG_ERR,"ip address and port are not specified...");
        CleanAll(1);
    }
    PortNum = strtol(port,&portchar,10);
    if(portchar==port){
        syslog(LOG_ERR,"Port must be integer...");
        CleanAll(1);
    }
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(address);
    servaddr.sin_port = htons(PortNum);
    connection_test(socketfd, servaddr);
}

char *socket_message(int socketfd,char command[])
{
    //char message[1000] , server_reply[2000];

    char buff[MAX_LINE];
 

    struct pollfd fds[2];
    char *data= (char *)malloc(sizeof(char) * MAX_LINE);
    int ret;
    int bytes;
    struct pollfd sdf[2];
    bzero(buff, MAX_LINE);
    // while (1){
    //     if((recv(socketfd, buff, MAX_LINE, 0))< 0){
    //         break;
    //     }
    //     bzero(buff,MAX_LINE);
    // }

        bzero(buff,MAX_LINE);
        strncpy(buff, command, MAX_LINE -2);
        strcat(buff, "\n");

        write(socketfd, buff ,sizeof(buff));
        bzero(buff, MAX_LINE);

    while(1){
        fds[0].fd = socketfd;
        fds[0].events = POLLIN;
        ret = poll(fds, 2, 0);
        if(ret == 0){
            syslog(LOG_ERR,"Socket closed");
        }
        if(fds[0].revents &POLLIN){
          recv(socketfd,buff,MAX_LINE,0);
          strncpy(data,buff,MAX_LINE);
          return data;
        }
       

    }
}


int socket_init(char *address, char *port)
{
    int socketfd;
    struct sockaddr_in servaddr;
    socket_create(&socketfd);
    socket_connect(&socketfd, servaddr, address, port);

    return socketfd;
}