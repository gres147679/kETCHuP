/**
 * cchat.c
 *
 * Septiembre - Diciembre 2013
 *
 * Manejo del cliente que se conecta al servidor
 * para utilizar el servicio de chat
 *
 * @author Andrea Balbás        09-10076
 * @author Gustavo El Khoury    10-10226
 */
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

pthread_mutex_t serverMutex = PTHREAD_MUTEX_INITIALIZER;

 /**
  * Dada una línea del archivo de entrada, se completan los datos de un
  * commandPacket con el comando leido y su argumento.
  * Si se trata de un comando válido, retorna 0.
  * De lo contrario, retorna -1
  * 
  * @param newCommand Comando que se construye según la linea del archivo leida
  * @param line Linea del archivo de entrada leida.
  * @return 0 si el comando leído es válido. -1 en caso contrario
  * 
  */
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
            if (newCommand->argument==NULL) 
                perror("malloc");
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
        if (newCommand->argument==NULL) 
            perror("malloc");
    	newCommand->argument[0]='\0';
    	return 0;
    } 
    return -1;    
    
}

 /**
  * Conecta el cliente al servidor y envía el nombre de usuario del cliente.
  * En caso de haber colocado un archivo de entrada de comandos, procesa el
  * archivo, envía los comandos al servidor y muestra por salida estándar
  * la respuesta obtenida. 
  * 
  * @param clientSocketFD Apuntador al socket del cliente
  * @param port Puerto del servidor al que quiere conectarse el cliente
  * @param host Nombre del servidor en el que desea conectarse el cliente
  * @param username Nombre de usuario del cliente que se conecta al servidor
  * @param filename Apuntador al nombre del archivo de entrada indicado
  * @return 1 si la conexión se realizó exitosamente.
  * 
  */
int initializeClient(int *clientSocketFD,int port,char *host,char *username, char *filename){

    struct sockaddr_in serverAddress;

    //Se obtiene la direccion del servidor
    bzero(&serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(host);
    serverAddress.sin_port = htons(port);

    //Se crea el socket
    *clientSocketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (*clientSocketFD < 0)
      fatalError("can't open socket");

    //Se conecta al servidor
    if (connect(*clientSocketFD, (struct sockaddr *) &serverAddress,
                sizeof(serverAddress)) < 0)
      fatalError("can't connect to server");

    // El cliente manda el nombre de usuario primero
    sayHello(*clientSocketFD,username);
    
    // Procesamiento de los comandos del archivo de entrada
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
            if (response != NULL) puts(response);
            printf("*********************************************************\n");
          }
            
          free(buffer);
          buffer=NULL;
        }
      }
      fclose(input);
    }
    return 1;
}

 /**
  * Manejo de la consola que permite al cliente enviar al servidor los
  * comandos del chat que desee ejecutar. Este metodo es manejado
  * mediante el uso de un hilo, razón por la cual su parámetro corresponde
  * a un apuntador a void.
  * 
  * @param args Apuntador al socket del cliente
  * 
  */
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

      if(strcmp("fue",newCommand.command)==0) {
        sendCommandToSocket(clientSocketFD,newCommand);
        puts("Hasta luego!");
        close(clientSocketFD);
        exit(0);
      }

      int returno = 1;
      while (returno != 0){
        returno = pthread_mutex_trylock(&serverMutex);
      }

      sendCommandToSocket(clientSocketFD,newCommand);
      while ((response = readResponseFromServer(clientSocketFD)) == NULL) ;
      pthread_mutex_unlock(&serverMutex);
      printf("*********************************************************\n");
      puts(response);
    }
    else{
      puts("Comando erroneo.");
    }
      
    free(buffer);
    buffer=NULL;
  }
  return;
}

 /**
  * Programa principal de manejo del cliente. 
  * Realiza el manejo de los flags colocados al invocar al programa cchat.
  * Si no se especifica un username, se genera uno aleatorio.
  * Crea el hilo encargado de la consola para el envio de comandos del chat,
  * y muestra en la salida estándar los mensajes que se reciben en la(s)
  * sala(s) de chat a la(s) que está suscrito el cliente.
  * 
  */
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
  printf("Mi usuario es %s\n",username);

  // Archivo de entrada. Puede ser vacío
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
  else return 0;

  char *response;
  while(1){
    pthread_mutex_lock(&serverMutex);
    response = fetchMessagesFromServer(clientSocketFD);
    if (pthread_mutex_unlock(&serverMutex) != 0) puts("no pude desbloquear");
    if (response != NULL) {
      printf("Ha recibido un nuevo mensaje:\n");
      puts(response);
      printf("$>");
    }
  
  }

}