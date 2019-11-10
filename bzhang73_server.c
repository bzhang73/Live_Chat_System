#include <stdlib.h>
#include <stdio.h>    /* needed for os x */
#include <errno.h>  /* defines ERESTART, EINTR */
#include <string.h>    /* for memset */
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>    /* for printing an internet address in a user-friendly way */
#include <pthread.h>
#include <sys/wait.h>     /* defines WNOHANG, for wait() */
#define PORT 15636
void *thread(void *vargp);
void *threadsend(void *vargp); //function thread to send message
void *threadrecv(void *vargp); //function thread to receive message

    /* main server function */
int main()
{
    /* listen the port 15636 */
    /* get a tcp/ip socket */
    /*   AF_INET is the Internet address (protocol) family  */
    /*   with SOCK_STREAM we ask for a sequenced, reliable, two-way */
    /*   conenction based on byte streams.  With IP, this means that */
    /*   TCP will be used */
    
    int listenfd = socket(AF_INET, SOCK_STREAM,0);
    if(listenfd < 0){
        perror("socket");
        exit(1);
    }
//    struct hostent *hp;

    
    /* set up our address */
    /* htons converts a short integer into the network representation */
    /* htonl converts a long integer into the network representation */
    /* INADDR_ANY is the special IP address 0.0.0.0 which binds the */
    /* transport endpoint to all IP addresses on the machine. */
    struct sockaddr_in serveraddr;
    bzero((char *)&serveraddr,sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    /* we use setsockopt to set SO_REUSEADDR. This allows us */
    /* to reuse the port immediately as soon as the service exits. */
    /* Some operating systems will not allow immediate reuse */
    /* on the chance that some packets may still be en route */
    /* to the port. */
    
    serveraddr.sin_port = htons(PORT);
    
    /* bind to the address to which the service will be offered */
    if(bind(listenfd,(struct sockaddr *)&serveraddr,sizeof(serveraddr)) < 0){
        perror("connect");
        exit(1);
    }
    
    /* set up the socket for listening with a queue length of 1024 */
    if(listen(listenfd,1024) < 0){
        perror("listen error");
        exit(1);
    }
    //char temp[100];
    struct sockaddr_in clientaddr;
    int clientlen, *connfdp;
    clientlen = sizeof(clientaddr); /* length of address */
    
    /* loop forever - wait for connection requests and perform the service */
    while(1){
        connfdp = (int *)malloc(sizeof(int));
        *connfdp = accept(listenfd,(struct sockaddr *)&clientaddr, &clientlen);
        pthread_t tid;
        printf("Accepted!\n");
        pthread_create(&tid,NULL,thread,connfdp);
    }
    EXIT_SUCCESS;
}

//create the thread to use two thread to transfer and receive message
void *thread(void *vargp)
{
    pthread_t tid1,tid2;
    int connfd = *((int *)vargp);
    int idata;
    char temp[100];
    pthread_create(&tid1,NULL,threadsend,vargp);
    pthread_create(&tid2,NULL,threadrecv,vargp);
    return NULL;
}

//function thread to send message
void *threadsend(void * vargp)
{
    int connfd = *((int *)vargp);
    int idata;
    char temp[100];
    while(1){
        //printf("server input:  ");
        fgets(temp,100,stdin);
        send(connfd,temp,100,0);
        printf("        server send OK\n");
    }
    return NULL;
}

//function thread to receive the message
void *threadrecv(void *vargp)
{
    char temp[100];
    int connfd = *((int *)vargp);
    while(1){
        int idata = 0;
        idata = recv(connfd,temp,100,0);
        if(idata > 0){
            printf("client :\n%s\n",temp);
        }
    }
    return NULL;
}
