#include <stdlib.h>
#include <stdio.h> /* needed for os x*/
#include <errno.h>
#include <string.h>    /* for strlen */
#include <unistd.h>
#include <netdb.h>     /* for gethostbyname() */
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAXLINE 100;
void *threadsend(void *vargp); //function thread to send message
void *threadrecv(void *vargp); //function thread to send message

int main()
{
    
    int *clientfdp;    /* host information */
    clientfdp = (int *)malloc(sizeof(int));/* address length when we get the port number */
   
    /* get a tcp/ip socket */
    /* We do this as we did it for the server */
    /* request the Internet address protocol */
    /* and a reliable 2-way byte stream */
    
    *clientfdp = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in serveraddr;    /* server address */
    struct hostent *hp;
    bzero((char *)&serveraddr,sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(15636);    /* default: whatever is in port.h */
    serveraddr.sin_addr.s_addr = inet_addr("128.226.114.200");    /* default: this host */
   
    /* connect */
    if(connect(*clientfdp,(struct sockaddr *)&serveraddr,sizeof(serveraddr)) < 0){
        printf("connect error\n");
        exit(1);
    }
    pthread_t tid1,tid2;
    printf("connected\n");
    while(1){
        pthread_create(&tid1,NULL,threadsend,clientfdp); //function thread to send message
        pthread_create(&tid2,NULL,threadrecv,clientfdp); //function thread to send message
    }
    return EXIT_SUCCESS;
}

//function thread to send message
void *threadsend(void * vargp)
{
    //pthread_t tid2;
    int connfd = *((int *)vargp);
    int idata;
    char temp[100];
    while(1){
        //printf("me: \n ");
        fgets(temp,100,stdin);
        send(connfd,temp,100,0);
        printf("          client send OK\n");
    }
    printf("client send\n");
    return NULL;
}

 //function thread to send message
void *threadrecv(void *vargp)
{
    char temp[100];
    int connfd = *((int *)vargp);
    while(1){
        int idata = 0;
        idata = recv(connfd,temp,100,0);
        if(idata > 0){
            printf("server :\n%s\n",temp);
        }
    }
    
    
    return NULL;
}
