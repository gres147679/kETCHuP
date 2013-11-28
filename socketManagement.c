/**
 * socketManagement.c
 *
 * Septiembre - Diciembre 2013
 *
 * Funciones de lectura y escritura en sockets
 *
 * @author Andrea Balbás        09-10076
 * @author Gustavo El Khoury    10-10226
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "socketManagement.h"
#include "errors.h"

 /**
  * Protocolo de comunicación para que el servidor reciba del cliente
  * su nombre de usuario
  * 
  * @param socketFD Socket del cliente cuyo nombre de usuario se recibirá.
  * @param username Nombre de usuario del cliente.
  * @return 0 si se recibió correctamente el nombre de usuario.
  * 
  */
int receiveHello(int socketFD, char **username){
  int nBytesRead = 0;
  int n = 1;
  char command[4];
  int argLength;
  int response = 0;

  // El servidor lee el hola
  n = read(socketFD,&argLength,4);
  if (n!=4) fatalError("Server protocol error 0");  

  // El servidor responde, para que el cliente mande la longitud del parametro
  if (write(socketFD,&response,4) == -1) fatalError("Server protocol error 2");

  // El servidor lee la longitud
  n = read(socketFD,&argLength,4);
  if (n!=4) fatalError("Server protocol error 3");  

  // El servidor responde, para que el cliente mande el parametro
  if (write(socketFD,&response,4) == -1) fatalError("Server protocol error 4");

  // Si el parametro no es vacio, el servidor lee y responde
  if (argLength != 1){
    *username = (char *) malloc(argLength*sizeof(char)+1);
    if (*username==NULL) 
        perror("malloc");
    n = read(socketFD,*username,argLength);    
    write(socketFD,&response,4);
  }
  
  return 0;
}

 /**
  * Protocolo de comunicación para que el cliente le envíe al servidor su 
  * nombre de usuario
  * 
  * @param socketFD Socket del servidor.
  * @param username Nombre de usuario del cliente.
  * @return 0 si se envió correctamente el nombre de usuario.
  * 
  */
int sayHello(int socketFD, char *username){  
  int serverResponse = 0;
  int argLength = strlen(username)+1;
  int n = 0;

  // El cliente dice hola
  if (write(socketFD,&serverResponse,4) == -1) fatalError("Client protocol error 0");
  
  // El cliente espera la respuesta del servidor, para mandar
  // la longitud del nombre de usuario
  n = read(socketFD,&serverResponse,4);
  if (n!=4 || serverResponse != 0) {
      fatalError("Client protocol error 2");
  }

  // El cliente manda la longitud del nombre de usuario
  if (write(socketFD,&argLength,4) == -1) fatalError("Client protocol error 3");

  // El cliente espera la respuesta del servidor, para mandar el nombre de usuario
  n = read(socketFD,&serverResponse,4);
  if (n!=4 || serverResponse != 0) fatalError("Client protocol error 4");

  if (argLength != 1){
    if ( (n=write(socketFD,username,argLength)) == -1) fatalError("Client protocol error 5");
    // El cliente recibe respuesta final del servidor
    n = read(socketFD,&serverResponse,4);
    if (n!=4 || serverResponse != 0) fatalError("Client protocol error 6");
  }
    
  return 0;
}

/** Las siguiente dos funciones corresponden al protocolo de comunicación
 * destinado a que el cliente envíe comandos al servidor y éste los reciba.
 * Los pasos del protocolo son los siguientes:
 *   El cliente se conecta y envía el comando
 *   El servidor lee el comando, y le avisa al cliente
 *   El cliente envía la longitud del parámetro que sigue
 *   El servidor recibe la señal, y le responde al cliente
 *   El cliente envía el parámetro.
 */

/**
  * Protocolo de comunicación para que el servidor reciba comandos del cliente.
  *
  * @param socketFD Socket del cliente que envía el comando.
  * @param receivedCommand Comando que se recbió del cliente.
  * @return 0 si se recibió correctamente el comando.
  * 
  */
int readCommandFromSocket(int socketFD, commandPacket *receivedCommand){
  int nBytesRead = 0;
  int n = 1;
  char command[4];
  int argLength;
  int response = 0;

  // El servidor lee el hola

  // El cliente manda el comando, y el servidor lo lee
  n = read(socketFD,&receivedCommand->command,4);
  if (n!=4) {
    fatalError("Server protocol error 1");
  }

  // El servidor responde, para que el cliente mande la longitud del parametro
  if (write(socketFD,&response,4) == -1) fatalError("Server protocol error 2");

  // El servidor lee la longitud
  n = read(socketFD,&argLength,4);
  if (n!=4) fatalError("Server protocol error 3");  

  // El servidor responde, para que el cliente mande el parametro
  if (write(socketFD,&response,4) == -1) fatalError("Server protocol error 4");

  // Si el parametro no es vacio, el servidor lee y responde
  if (argLength != 1){
    receivedCommand->argument = (char *) malloc(argLength*sizeof(char)+1);
    if (receivedCommand->argument==NULL) 
        perror("malloc");
    n = read(socketFD,receivedCommand->argument,argLength);    
    write(socketFD,&response,4);
  }
  
  return 0;
}

 /**
  * Protocolo de comunicación para que el cliente envíe comandos al servidor.
  * 
  * @param socketFD Socket del cliente que envía el comando.
  * @param newCommand Comando que se envía al servidor.
  * @return 0 si se envió correctamente el nombre de usuario.
  * 
  */
int sendCommandToSocket(int socketFD, commandPacket newCommand){
  int serverResponse = 0;
  int argLength = strlen(newCommand.argument)+1;
  int n = 0;

  // El cliente dice hola
  if (write(socketFD,&serverResponse,4) == -1) fatalError("Client protocol error 0");
  
  // El cliente escribe el comando
  if (n=write(socketFD,newCommand.command,4) == -1) fatalError("Client protocol error 1");

  // El cliente espera la respuesta del servidor, para mandar
  // la longitud del parametro
  n = read(socketFD,&serverResponse,4);
  if (n!=4 || serverResponse != 0) {
      fatalError("Client protocol error 2");
  }

  // El cliente manda la longitud del parametro
  if (write(socketFD,&argLength,4) == -1) fatalError("Client protocol error 3");

  // El cliente espera la respuesta del servidor, para mandar el parametro
  n = read(socketFD,&serverResponse,4);
  if (n!=4 || serverResponse != 0) fatalError("Client protocol error 4");

  if (argLength != 1){
    if ( (n=write(socketFD,newCommand.argument,argLength)) == -1) fatalError("Client protocol error 5");
    // El cliente recibe respuesta final del servidor
    n = read(socketFD,&serverResponse,4);
    if (n!=4 || serverResponse != 0) fatalError("Client protocol error 6");
  }
  
  return 0;
}

/** Las siguiente dos funciones corresponden al protocolo de comunicación
 * destinado a que el cliente reciba las respuestas enviadas por el servidor.
 * Los pasos del protocolo son:
 *    El servidor envíia la longitud del string de respuesa, y el cliente acepta
 *    El servidor recibe la señal, y envia el string. El cliente confirma que
 *    recibió la respuesta. 
 */

 /**
  * Protocolo de comunicación para que el cliente lea la respuesta enviada
  * por el servidor.
  * 
  * @param socketFD Socket del cliente que recibirá la respuesta.
  * @return String con la respuesta enviada por el cliente.
  *         NULL en caso de que no haya respuesta.
  * 
  */
char *readResponseFromServer(int socketFD){

  int n = 1;
  int answerLength;
  int allGood = 0;
  char *answer;

  fd_set fdSet;
  struct timeval timeout;
  int rc, result;

  //Se determina el tiempo del timeout
  timeout.tv_sec = 1;
  timeout.tv_usec = 0;
  
  //Crea el file descriptor con el socket del cliente
  FD_ZERO(&fdSet);
  FD_SET(socketFD, &fdSet);
  rc = select(sizeof(fdSet)*4, &fdSet, NULL, NULL, &timeout);
  if (rc==-1) {
    fatalError("Error esperando mensajes");
    return NULL;
  }

  result = 0;
  if (rc >0 && FD_ISSET(socketFD, &fdSet)){
    // El cliente lee la longitud
    n = read(socketFD,&answerLength,4);
    if (n!=4) fatalError("Response reading error 1");  

    int b;
    // El cliente responde, para que el servidor mande el parametro
    if ((b=write(socketFD,&allGood,4)) == -1) fatalError("Response reading error 2");

    // Si la respuesta no es vacio, el cliente lee y responde
    if (answerLength != 0){
      answer = (char *) malloc(answerLength*sizeof(char));
      if (answer==NULL) 
          perror("malloc");
      n = read(socketFD,answer,answerLength);    
      write(socketFD,&allGood,4);
    }
    return answer;
  }
  else return NULL; 
  
}

 /**
  * Protocolo de comunicación para que el servidor envíe una respuesta
  * a un cliente.
  * 
  * @param socketFD Socket del cliente al que se le envía la respuesta.
  * @param answer Respuesta que se le envía al cliente.
  * @return 0 si se envió correctamente la respuesta.
  * 
  */
int sendResponseToClient(int socketFD, char *answer){
    
  int serverResponse = 0;
  int answerLength = strlen(answer)+1;
  int n = 0;

  // El servidor manda la longitud de la respuesta
  if (write(socketFD,&answerLength,4) == -1) fatalError("Response sending error 1");

  // El servidor espera la respuesta del cliente, para mandar la respuesta
  n = read(socketFD,&serverResponse,4);
  if (n!=4 || serverResponse != 0) fatalError("Response sending error 2");

  if (answerLength != 1){
    if ( (n=write(socketFD,answer,answerLength)) == -1) fatalError("Response sending error 3");
    // El cliente recibe respuesta final del servidor
    n = read(socketFD,&serverResponse,4);
    if (n!=4 || serverResponse != 0) fatalError("Response sending error 4");
  }
  
  return 0;
}

 /**
  * Protocolo de comunicación para que el cliente reciba los mensajes
  * de las salas a las que está suscrito.
  * 
  * @param socketFD Socket que se leerá para obtener el mensaje recibido.
  * @return String con el mensaje recibido.
  * 
  */
char *fetchMessagesFromServer(int socketFD){

  int n = 1;
  int answerLength;
  int allGood = 0;
  char *answer;

  fd_set fdSet;
  struct timeval timeout;
  int rc, result;

  //Se determina el tiempo del timeout
  timeout.tv_sec = 1;
  timeout.tv_usec = 0;
  //Crea el file descriptor con el socket del cliente
  FD_ZERO(&fdSet);
  FD_SET(socketFD, &fdSet);
  rc = select(sizeof(fdSet)*4, &fdSet, NULL, NULL, &timeout);
  if (rc==-1) {
    fatalError("Error esperando mensajes");
    return NULL;
  }

  result = 0;
  if (rc >0 && FD_ISSET(socketFD, &fdSet)){
    // El cliente lee la longitud
    n = read(socketFD,&answerLength,4);
    if (n!=4) fatalError("Response reading error 1");  

    // Si la respuesta no es vacio, el cliente lee y responde
    if (answerLength != 0){
      answer = (char *) malloc(answerLength*sizeof(char));
      if (answer==NULL) 
          perror("malloc");
      n = read(socketFD,answer,answerLength);    
    }
    return answer;
  }
  else return NULL;

}

 /**
  * Protocolo de comunicación para que el servidor envíe un mensaje
  * a un cliente.
  * 
  * @param socketFD Socket del cliente al que se enviará el mensaje.
  * @param answer Mensaje que se enviará al cliente.
  * @return 0 si el mensaje fue enviado correctamente.
  * 
  */
int sendMessagesToClient(int socketFD, char *answer){
  int serverResponse = 0;
  int answerLength = strlen(answer)+1;
  int n = 0;

  // El servidor manda la longitud de la respuesta
  if (write(socketFD,&answerLength,4) == -1) fatalError("Response sending error 1");

  if (answerLength != 1){
    if ( (n=write(socketFD,answer,answerLength)) == -1) fatalError("Response sending error 3");
  }
  
  return 0;
}