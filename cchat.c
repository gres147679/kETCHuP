/****************************
 cchat.c
 
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
#include <errno.h>
#include <sysexits.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>

#include "errors.h"
#include "socketManagement.h"

#define DEFAULTPORT 20226

// Dada una línea del archivo de entrada, la rutina rellena un commandPacket
// con el comando y su argumento, sólo si son comandos válidos. De lo contrario,
// retorna -1

int getCommandFromLine(commandPacket *newCommand,char *line){
    newCommand->command[3]='\0';
    size_t lineLength = strlen(line);
   
    // Comandos válidos con argumentos
    if (lineLength>3){
    	char backup = line[3];
    	line[3]='\0';
    	if ( !strcmp(line,"men") || !strcmp(line,"sus") || !strcmp(line,"cre")
    	|| !strcmp(line,"eli") ){    
  	    strncpy(newCommand->command,line,3);
  	    
  	    newCommand->argument = (char *) malloc ((lineLength-4)*sizeof(char));
  	    strncpy(newCommand->argument,line+4,lineLength-4);	    
  	    return 0;
      }
      else{
        line[3]=backup;
  	    return -1;
      }
  }
    
    // Comandos validos sin argumentos
    else if ( !strcmp(line,"sal") || !strcmp(line,"usu") || !strcmp(line,"des")
 	|| !strcmp(line,"fue") ){
    	strncpy(newCommand->command,line, 3);    
    	newCommand->argument = (char *) malloc (sizeof(char));
    	newCommand->argument[0]='\0';
    	return 0;
    } 
    return -1;    
    
}


int initializeClient(int *clientSocketFD,int port,char *host,char *username, char *filename){
    //int clientSocketFD;
    struct sockaddr_in serverAddress;

    /* Get the address of the server. */
    bzero(&serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(host);
    serverAddress.sin_port = htons(port);

    /* Open a socket. */
    *clientSocketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (*clientSocketFD < 0)
      fatalError("can't open socket");

    /* Connect to the server. */
    if (connect(*clientSocketFD, (struct sockaddr *) &serverAddress,
                sizeof(serverAddress)) < 0)
      fatalError("can't connect to server");

    // El cliente manda el nombre de usuario primero
    sayHello(*clientSocketFD,username);
    
    /* Copy input to the server. */
    if (strlen(filename) != 0){
      FILE *input;
      input = fopen(filename,"r");
      if (input == NULL){
          perror("No se encuentra el archivo");
          exit(EX_NOINPUT);
      }
      else{
        char *buffer = NULL;
        char command[4];
        size_t bufferSize = 0;
        int n;
        int lineLength;
        char *response;
        
        commandPacket newCommand;
        while ( !feof(input) ){
          getline(&buffer,&bufferSize,input);
          if (!feof(input)){
            lineLength = strlen(buffer);
            buffer[lineLength-1]='\0';
          }
            
          if (getCommandFromLine(&newCommand,buffer)!=-1){

            if(strcmp("fue",newCommand.command)==0)return 0;

            sendCommandToSocket(*clientSocketFD,newCommand);
            response = readResponseFromServer(*clientSocketFD);
            puts(response);
          }
            
          free(buffer);
          buffer=NULL;
        }
      }
      fclose(input);
    }
    return 1;
}

void * executeShell(void *args){
  int clientSocketFD = *(int*)(args);
  char *buffer = NULL;
  size_t bufferSize = 0;
  char *response;
  commandPacket newCommand;
  int lineLength;

  while ( 1 ){
    printf("$>");
    getline(&buffer,&bufferSize,stdin);

    lineLength = strlen(buffer);
    buffer[lineLength-1]='\0';


    if (getCommandFromLine(&newCommand,buffer)!=-1){

      if(strcmp("fue",newCommand.command)==0) break;

      sendCommandToSocket(clientSocketFD,newCommand);
      response = readResponseFromServer(clientSocketFD);
      puts(response);
    }
      
    free(buffer);
    buffer=NULL;
  }
  return;
}

int main (int argc, char **argv){
  int index;
  int flag;

  // Variables de ejecución del cliente, que se setean según CLI
  char host[16] = "127.0.0.1";
  int port = DEFAULTPORT;

  // Se genera un username random por defecto
  char username[20] = "usuario";
  srand(time(NULL));
  int randNum = rand() % 100000;
  char rand[5];
  snprintf(rand, 5, "%d", randNum);
  strcat(username,rand);

  // Archivo de entrada. Puede ser vacío
  //NOTA: creo que podriamos poner filename[] y asi no limitar la extension
  char filename[100] = "";


  opterr = 0;

  while ((flag = getopt (argc, argv, "-h:-p:-n:-a:")) != -1)
   switch (flag)
     {
     case 'h':
       strcpy(host,optarg);
       break;
     case 'p':
       port = atoi(optarg);
       break;
     case 'n':
       strcpy(username,optarg);
       break;
     case 'a':
       strcpy(filename,optarg);
       printf("El archivo a abrir es %s\n", filename);
       break;
     case '?':
       if (optopt == 'h' || optopt == 'p' || optopt == 'n' || optopt == 'a')
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

  int clientSocketFD;
  pthread_t serverThread;
  int getShell = initializeClient(&clientSocketFD,port,host,username,filename);
  

  // Se crea un thread que lee comandos por pantalla
  if (getShell) pthread_create(&serverThread,NULL,&executeShell,(void *)(&clientSocketFD));
  while(1);

}