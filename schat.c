/**
 * schat.c
 *
 * Septiembre - Diciembre 2013
 *
 * Manejo del servidor que maneja las solicitudes
 * de los clientes conectados al chat
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
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>

#include "errors.h"
#include "socketManagement.h"
#include "serverCommands.h"
#ifndef lta
  #include "lista.h"
#endif

#define PORT 20226
#define QUEUELENGTH 10


// Lista de todos los usuarios del servidor
userList globalUserList;

// Lista de salas en el servidor
chatRoomList chatRoomsList;

// Mutex de la lista global de usuarios
pthread_mutex_t globalUserListMutex = PTHREAD_MUTEX_INITIALIZER;

// Mutex de la lista de salas
pthread_mutex_t chatRoomsListMutex = PTHREAD_MUTEX_INITIALIZER;

// Arreglo de TIDs. Debe ser UNA LISTA
pthread_t tidList[512];

 /**
  * Crea la sala de chat a la que por defecto se van a conectar los usuarios.
  * Abre el socket y lo asocia al socket.
  * 
  * @param serverPort Numero de puerto al que se asociará el socket.
  * @param serverQueueLength Longitud de la cola del servidor.
  * @param defaultChatName Nombre de la sala por defecto.
  * @return Entero correspondiente al file descriptor del socket del servidor.
  * 
  */
int initializeServer(int serverPort,int serverQueueLength, char *defaultChatName){
  int serverSocketFD, newClientSocketFD;  
  struct sockaddr_in serverAddress;
  
  // Creo la sala por defecto
  initializeCRList(&chatRoomsList);
  addChatRoom(&chatRoomsList,defaultChatName, "admin",-1);

  
  /*Abrir el socket*/
  serverSocketFD = socket(PF_INET, SOCK_STREAM,0);
  int optval = 1;
  setsockopt(serverSocketFD, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);
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

 /**
  * Manejo de las peticiones de los clientes conectados al chat.
  * Inicializa el mutex del usuario al que atenderá.
  * Verifica que el cliente esté listo para enviar solicitudes y lo agrega
  * a la sala por defecto.
  * Lee del socket los comandos enviados por el cliente y los ejecuta.
  * 
  * @param args Parámetros que necesita el servidor para atender al cliente.
  *             args[0] -> Direccion del cliente.
  *             args[1] -> Apuntador al file descriptor del socket del cliente.
  *             args[2] -> Nombre del usuario que se va a atender.
  * 
  */
void * serveClient(void *args){
  // Argumentos
  void **argArray = (void **) args;
  struct sockaddr_in *clientAddress = (struct sockaddr_in *)(argArray[0]);
  int *newClientSocketFDPointer = (int *)argArray[1];
  int newClientSocketFD = *newClientSocketFDPointer;

  char *username = (char *)argArray[2];

  //Inicializamos el mutex del usuario
  pthread_mutex_t *myMutex = getMutex(globalUserList,username);
  if (myMutex == NULL) fatalError("No puedo obtener el mutex del usuario");
  else pthread_mutex_init(myMutex,NULL);
  
  char buffer[170];
  char command[4];
  command[3]='\0';
  int heartBeat;
  int operationComplete;
  
  char *response;

  // Se agrega el usuario a la sala por defecto
  pthread_mutex_lock(&chatRoomsListMutex);
  addUserToCRList(&chatRoomsList,"actual", username, newClientSocketFD);
  pthread_mutex_unlock(&chatRoomsListMutex);

  while( read(newClientSocketFD,&heartBeat,4)>0 ){
    commandPacket myCommand;
    pthread_mutex_lock(myMutex);
    readCommandFromSocket(newClientSocketFD,&myCommand);
    pthread_mutex_unlock(myMutex);
    
    strncpy(command,myCommand.command,3);

    // Definición de los comandos
    if ( strcmp(command,"sal") == 0 ){
    	printf("Mandaste sal\n");
    	pthread_mutex_lock(&chatRoomsListMutex);
      response = listChatrooms(&chatRoomsList,newClientSocketFD);
      pthread_mutex_unlock(&chatRoomsListMutex);

      pthread_mutex_lock(myMutex);
      sendResponseToClient(newClientSocketFD,response);
      pthread_mutex_unlock(myMutex);
    }
    else if ( strcmp(command,"usu") == 0 ){
    	printf("Mandaste usu\n");

      pthread_mutex_lock(&globalUserListMutex);
    	response = listUsers(&globalUserList,newClientSocketFD);
      pthread_mutex_unlock(&globalUserListMutex);

      pthread_mutex_lock(myMutex);
      sendResponseToClient(newClientSocketFD,response);
      pthread_mutex_unlock(myMutex);
    }

    else if ( strcmp(command,"men") == 0 ){
    	printf("Mandaste men\n");
      sendMessageToChatRooms(&chatRoomsList, username, myCommand.argument);
      
      pthread_mutex_lock(myMutex);
      sendResponseToClient(newClientSocketFD,"Tu mensaje fue enviado");
      pthread_mutex_unlock(myMutex);
      puts("Saliendo de men");
    }

    else if ( strcmp(command,"sus") == 0 ){
    	printf("Mandaste sus\n");
      pthread_mutex_lock(&chatRoomsListMutex);
      operationComplete = subscribeUser(&chatRoomsList,myCommand.argument, username,newClientSocketFD);
      pthread_mutex_unlock(&chatRoomsListMutex);
      
      pthread_mutex_lock(myMutex);
      if(operationComplete == 0) sendResponseToClient(
        newClientSocketFD,"La suscripción se ha realizado con éxito");
      else if (operationComplete == -1) sendResponseToClient(
        newClientSocketFD,"Ya estás suscrito a la sala");
      else sendResponseToClient(
        newClientSocketFD,"La sala no existe. Puedes crearla usando el comando cre");
      pthread_mutex_unlock(myMutex);
    }

    else if ( strcmp(command,"des") == 0 ){
      printf("Mandaste des\n");
      pthread_mutex_lock(&chatRoomsListMutex);
      unsubscribeUser(&chatRoomsList, username,newClientSocketFD);
      pthread_mutex_unlock(&chatRoomsListMutex);

      pthread_mutex_lock(myMutex);
      sendResponseToClient(
        newClientSocketFD,"Te has desuscrito de todas las salas");
      pthread_mutex_unlock(myMutex);
    }

    else if ( strcmp(command,"cre") == 0 ){
    	printf("Mandaste cre con argumento %s\n",myCommand.argument);
      pthread_mutex_lock(&chatRoomsListMutex);
      operationComplete = createChatroom(&chatRoomsList,myCommand.argument, username, newClientSocketFD);
      pthread_mutex_unlock(&chatRoomsListMutex);

      pthread_mutex_lock(myMutex);
      if(operationComplete == 0) sendResponseToClient(
        newClientSocketFD,"La sala fue creada exitosamente");
      else sendResponseToClient(
        newClientSocketFD,"Ya existe una sala con ese nombre");
      pthread_mutex_unlock(myMutex);
    }

    else if ( strcmp(command,"eli") == 0 ){
    	printf("Mandaste eli\n");
    	pthread_mutex_lock(&chatRoomsListMutex);
      operationComplete = deleteChatroom(&chatRoomsList,myCommand.argument, username);
      pthread_mutex_unlock(&chatRoomsListMutex);

      pthread_mutex_lock(myMutex);
      if(operationComplete == 0) sendResponseToClient(
        newClientSocketFD,"La sala fue eliminada exitosamente");
      else if (operationComplete == -1) sendResponseToClient(
        newClientSocketFD,"No se puede eliminar la sala por defecto");
      else sendResponseToClient(
        newClientSocketFD,"Para eliminar la sala debes ser el dueño");  
      pthread_mutex_unlock(myMutex);
    }

    else if ( strcmp(command,"fue") == 0 ){
      printf("Mandaste fue\n");
      pthread_mutex_lock(&chatRoomsListMutex);
      unsubscribeUser(&chatRoomsList, username,newClientSocketFD);
      pthread_mutex_unlock(&chatRoomsListMutex);

      pthread_mutex_lock(&globalUserListMutex);
      removeUser(&globalUserList,username);
      pthread_mutex_unlock(&globalUserListMutex);
      pthread_exit(NULL);
    }

    else{
    	printf("Comando erroneo: ");
    	puts(command);
    }
  }
  return;
}

 /**
  * Metodo del servidor que espera a que lleguen nuevas conexiones al chat.
  * Inicializa la estructura de datos que se utilizará para atender las 
  * peticiones del cliente.
  * Crea el hilo que se dedicará a atender las peticiones del cliente.
  * 
  * @param serverSocketFD Socket del servidor.
  * 
  */
void waitForConnections(int serverSocketFD){
  struct sockaddr_in clientAddress;
  int clientAddresslength;
  int newClientSocketFD;
  char *username;

  /*Servidor*/
    
    // Se crea la estructura de argumentos para los threads
    void **argList;
    argList = (void *) malloc(3*sizeof(int));
    if (argList==NULL) 
        perror("malloc");

    // Cantidad de clientes
    int userCount = 0;
  
    while((newClientSocketFD = accept(serverSocketFD, (struct sockaddr *) &clientAddress, &clientAddresslength)) >= 0) {
        // Se lee el nombre de usuario
        receiveHello(newClientSocketFD,&username);
        //Se agrega el usuario a la lista de usuarios del chat
        addUser(&globalUserList, username, newClientSocketFD);
        printf("Se agrega el usuario %s con FD %d\n",username,newClientSocketFD);
        
        argList[0] = &clientAddress;
        argList[1] = &newClientSocketFD;
        argList[2] = (void *)username;

        
        pthread_create(&tidList[userCount++],NULL,&serveClient,argList);
        
        printf("...Done\n");
    }
}

 /**
  * Programa principal de manejo del servidor.
  * Maneja los argumentos recibidos en el momento en que se ejecutó el programa
  * del servidor, es decir, el puerto y el nombre de la sala por defecto.
  * Inicializa la lista de usuarios, inicializa el servidor, y espera a 
  * que lleguen nuevas conexiones.
  * 
  */
main(int argc, char *argv[]){

  int index;
  int flag;

  // Variables de ejecución del cliente, que se setean según CLI
  int port;
  char chatRoom[100] = "actual";


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
  
  initialize(&globalUserList);
  int serverSocketFD = initializeServer(port,QUEUELENGTH,chatRoom);

  waitForConnections(serverSocketFD);
  return 0;
}
