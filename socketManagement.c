/****************************
 socketManagement.c

 Módulo con operaciones sobre
 los sockets, para escribir
 y leer datos
 
 Andrea Balbás        09-10076
 Gustavo El Khoury    10-10226     
 
 Septiembre - Diciembre 2013
 ****************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "socketManagement.h"
#include "errors.h"

void copyDataToFD(int from, int to) {
  char buffer[1024];
  int amount;

  while ((amount = read(from, buffer, sizeof(buffer))) > 0){
    if (write(to, buffer, amount) != amount) {
      fatalError("Falló la escritura al socket");
      return;
    }
  }
  
  if (amount < 0) fatalError("Falló la escritura al socket");

}

void copyDataToArray(int from, char *to) {
  int amount;
  while ((amount = read(from, to, 170*sizeof(char))) > 0);
  return;

}


// Implementa nuestro protocolo de comunicacion, que sigue estos pasos:
// -)El cliente se conecta y envia el comando
// -)El servidor lee el comando, y le avisa al cliente
// -)El cliente envia la longitud del parametro que sigue
// -)El servidor recibe la señal, y le responde al cliente
// -)El cliente envia el parametro

int readCommandFromSocket(int socketFD, commandPacket *receivedCommand){
  int nBytesRead = 0;
  int n = 1;
  char command[4];
  int argLength;
  int response = 0;

  // El cliente manda el comando, y el servidor lo lee
  n = read(socketFD,&receivedCommand->command,4);
  if (n!=4) fatalError("Server protocol error 1");
  puts(receivedCommand->command);

  // El servidor responde, para que el cliente mande la longitud del parametro
  if (write(socketFD,&response,4) == -1) fatalError("Server protocol error 2");

  // El servidor lee la longitud
  n = read(socketFD,&argLength,4);
  if (n!=4) fatalError("Server protocol error 3");
  printf("%d\n",argLength);

  // El servidor responde, para que el cliente mande el parametro
  if (write(socketFD,&response,4) == -1) fatalError("Server protocol error 4");

  // Si el parametro no es vacio, el servidor lee y responde
  if (argLength != 0){
    receivedCommand->argument = (char *) malloc(argLength*sizeof(char)+1);
    n = read(socketFD,receivedCommand->argument,4);
    write(socketFD,&response,4);
  }

  //n = read(socketFD,&argLength,4);
  printf("%s %d\n",receivedCommand->argument,argLength);

  return 0;
}

int sendCommandToSocket(int socketFD, char command[4], char *argument){
  int serverResponse = 0;
  int argLength = strlen(argument);
  int n = 0;

  // El cliente escribe el comando
  puts(command);
  if (write(socketFD,command,4) == -1) fatalError("Client protocol error 1");

  // El cliente espera la respuesta del servidor, para mandar
  // la longitud del parametro
  n = read(socketFD,&serverResponse,4);
  if (n!=4 || serverResponse != 0) fatalError("Client protocol error 2");

  // El cliente manda la longitud del parametro
  if (write(socketFD,&argLength,4) == -1) fatalError("Client protocol error 3");

  // El cliente espera la respuesta del servidor, para mandar el parametro
  n = read(socketFD,&serverResponse,4);
  if (n!=4 || serverResponse != 0) fatalError("Client protocol error 4");

  if (argLength != 0){
    if (write(socketFD,argument,argLength+1) == -1) fatalError("Client protocol error 5");

    // El cliente recibe respuesta final del servidor
    n = read(socketFD,&serverResponse,4);
    if (n!=4 || serverResponse != 0) fatalError("Client protocol error 6");
  }

  return 0;
}
