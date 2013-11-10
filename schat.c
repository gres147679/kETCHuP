/****************************
 schat.c
 
 Andrea Balbás        09-10076
 Gustavo El Khoury    10-10226     
 
 Septiembre - Diciembre 2013
 ****************************/
 
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include "errors.h"
#include "sockutil.h"

#define PORT 20226
#define QUEUELENGTH 10

main(int argc, char *argv[]){
  int sockfd, newsockfd;  
  struct sockaddr_in clientaddr,serveraddr;
  int clientaddrlength;
  //int pid;      LO PONGO COMENTADO PORQUE NO SE SI USAREMOS HILOS O PROCESOS
  
  /*Abrir el socket*/
  sockfd = socket(PF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
    fatalerror("No se puede abrir el socket");
  
  /*Asociar el socket a una direccion*/
  bzero(&serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = PF_INET;
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddr.sin_port = htons(PORT);
  if (bind(sockfd, (struct sockaddr *) &serveraddr,
           sizeof(serveraddr)) != 0)
    fatalerror("No se puede asociar a un socket");
  if (listen(sockfd, QUEUELENGTH) < 0)
    fatalerror("No se puede escuchar");
    
  /*Servidor*/
  while (1) {
    /* Wait for a connection. */
    clientaddrlength = sizeof(clientaddr);
    newsockfd = accept(sockfd, 
                       (struct sockaddr *) &clientaddr,
                       &clientaddrlength);
    while (1){
      copyData(newsockfd,1); 
    }
    
  }
  
}