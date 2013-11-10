/****************************
 schat.c
 
 Andrea Balbás        09-10076
 Gustavo El Khoury    10-10226     
 
 Septiembre - Diciembre 2013
 ****************************/
#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "errors.h"
#include "socketManagement.h"

#define PORT 20226
#define QUEUELENGTH 10

int initializeServer(int serverPort,int serverQueueLength){
  int serverSocketFD, newClientSocketFD;  
  struct sockaddr_in serverAddress;
  
  //int pid;      LO PONGO COMENTADO PORQUE NO SE SI USAREMOS HILOS O PROCESOS
  
  /*Abrir el socket*/
  serverSocketFD = socket(PF_INET, SOCK_STREAM,0);
  if (serverSocketFD < 0)
    fatalError("No se puede abrir el socket");
  
  /*Asociar el socket a una direccion*/
  bzero(&serverAddress, sizeof(serverAddress));
  serverAddress.sin_family = PF_INET;
  serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
  serverAddress.sin_port = htons(serverPort);
  if (bind(serverSocketFD, (struct sockaddr *) &serverAddress,
           sizeof(serverAddress)) != 0)
    fatalError("No se puede asociar a un socket");
  if (listen(serverSocketFD, serverQueueLength) < 0)
    fatalError("No se puede escuchar");

  return serverSocketFD;
}

void serveClient(struct sockaddr_in clientAddress,int newClientSocketFD){
  char buffer[170];
  char command[4];
  command[3]='\0';

  puts("blah1");
  copyDataToFD(newClientSocketFD,1); 
  puts("blah");
  strncpy(command,buffer,3);

  // Definición de los comandos
  if ( strcmp(command,"sal") == 0 ){
    printf("Mandaste sal\n");
    //Llamada a implementación de sal
  }
  else if ( strcmp(command,"usu") == 0 ){
    printf("Mandaste usu\n");
    //Llamada a implementación de sal
  }
  else if ( strcmp(command,"men") == 0 ){
    printf("Mandaste men\n");
    //Llamada a implementación de sal
  }
  else if ( strcmp(command,"sus") == 0 ){
    printf("Mandaste sus\n");
    //Llamada a implementación de sal
  }
  else if ( strcmp(command,"des") == 0 ){
    printf("Mandaste des\n");
    //Llamada a implementación de sal
  }
  else if ( strcmp(command,"cre") == 0 ){
    printf("Mandaste cre\n");
    //Llamada a implementación de sal
  }
  else if ( strcmp(command,"eli") == 0 ){
    printf("Mandaste eli\n");
    //Llamada a implementación de sal
  }
  else if ( strcmp(command,"fue") == 0 ){
    printf("Mandaste fue\n");
    //Llamada a implementación de sal
  }
  return;
}

void waitForConnections(int serverSocketFD){
  struct sockaddr_in clientAddress;
  int clientAddresslength;
  int newClientSocketFD;

  /*Servidor*/
  while (1) {
    /* Wait for a connection. */
    clientAddresslength = sizeof(clientAddress);
    if (newClientSocketFD = accept(serverSocketFD, 
                       (struct sockaddr *) &clientAddress,
                       &clientAddresslength) == -1)
      fatalError("No se pueden aceptar conexiones");
    puts("Se conecto alguien;");
    serveClient(clientAddress,newClientSocketFD);
  }
}



  // while (1){
  //     copyData(newClientFD,1); 
  //   }


main(int argc, char *argv[]){

  int index;
  int flag;

  // Variables de ejecución del cliente, que se setean según CLI
  int port;
  char chatRoom[100] = "default";


  opterr = 0;

  while ((flag = getopt (argc, argv, "s:p:")) != -1)
   switch (flag){
     case 's':
       strcpy(chatRoom,optarg);
       break;
     case 'p':
       port = atoi(optarg);
       break;
     case '?':
       if (optopt == 's' || optopt == 'p')
         fprintf (stderr, "La opción -%c requiere un argumento.\n", optopt);
       else if (isprint(optopt))
         fprintf (stderr, "Opción inválida `-%c'.\n", optopt);
       else
         fprintf (stderr,
                  "Opción inválida `\\x%x'.\n",
                  optopt);
       return 1;
     default:
       abort ();
   }

  for (index = optind; index < argc; index++)
   printf ("No es una opción: %s\n", argv[index]);

  int serverSocketFD = initializeServer(PORT,QUEUELENGTH);

  waitForConnections(serverSocketFD);
  puts("Holla;");
  return 0;
}