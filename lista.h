/**
 * lista.h
 *
 * Septiembre - Diciembre 2013
 *
 * Encabezado para el manejo de listas
 *
 * @author Andrea Balbás        09-10076
 * @author Gustavo El Khoury    10-10226
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

//Estructuras para listas de usuarios
 /**
  * Estructura que almacena los datos de un usuario
  * perteneciente a una lista.
  * 
  * username -> Nombre del usuario
  * clientSocketFD -> Socket asociado al usuario
  * userMutex -> Mutex asociado al usuario
  * sig -> Apuntador al proximo elemento de la lista
  */
typedef struct userBox{
    char *username;
    int clientSocketFD;
    pthread_mutex_t userMutex;
    struct userBox *sig;
} userBox; 

 /**
  * Estructura que modela una lista de usuarios
  * 
  * size-> Cantidad de elementos de la lista
  * head -> Apuntador a la cabeza de la lista
  * tail -> Apuntador a la cola de la lista
  */
typedef struct userList{
    int size;
    userBox *head;
    userBox *tail;
} userList;

//Estructuras para listas de salas de chat
 /**
  * Estructura que almacena los datos de una sala
  * perteneciente a una lista.
  * 
  * chatRoomName -> Nombre de la sala
  * users -> Lista de usuarios suscritos a la sala.
  * next -> Apuntador al proximo elemento de la lista
  */
typedef struct chatRooms{
    char *chatRoomName;
    userList users;
    struct chatRooms *next;
} chatRooms;

 /**
  * Estructura que modela una lista de salas
  * 
  * size-> Cantidad de elementos de la lista
  * head -> Apuntador a la cabeza de la lista
  * tail -> Apuntador a la cola de la lista
  */
typedef struct chatRoomList{
    int size;
    chatRooms *head;
    chatRooms *tail;
}chatRoomList;

//Estructuras para listas de hilos
 /**
  * Estructura que almacena los datos de un hilo
  * perteneciente a una lista.
  * 
  * tid -> id del thread
  * next -> Apuntador al proximo elemento de la lista
  */
typedef struct threadBox{
    pthread_t tid;
    struct threadBox *next;
} threadBox; 

 /**
  * Estructura que modela una lista de hilos
  * 
  * size-> Cantidad de elementos de la lista
  * head -> Apuntador a la cabeza de la lista
  * tail -> Apuntador a la cola de la lista
  */
typedef struct threadList{
    int size;
    threadBox *head;
    threadBox *tail;
} threadList;

//Metodos de lista de usuarios
 /**
  * Dada una lista de usuarios, la inicializa colocando NULL su cabeza y cola, 
  * y coloca 0 en su tamaño
  * 
  * @param lista Lista que desea inicializarse
  * 
  */
void initialize(userList *lista);

 /**
  * Agrega un usuario a una lista de usuarios.
  * 
  * @param lista Lista en la que se desea agregar al usuario
  * @param nombreUsuario Nombre del usuario que se desea agregar a la lista
  * @param clientSocketFD File descriptor del socket asociado al usuario
  *                       que quiere agregarse en la lista
  * @return -1 si el usuario ya estaba en la lista, 0 si el usuario fue
  *         correctamente agregado.
  * 
  */
int addUser(userList *lista,char *nombreUsuario, int clientSocketFD);

 /**
  * Elimina un usuario de una lista de usuarios.
  * 
  * @param lista Lista de la que se desea eliminar al usuario
  * @param nombreUsuario Nombre del usuario que se desea eliminar de la lista
  * 
  */
void removeUser(userList *lista, char *username);

 /**
  * Muestra por salida estándar los nombres de usuario de los usuarios
  * pertenecientes a una lista
  * 
  * @param lista Lista cuyos usuarios quieren ser consultados
  * 
  */
void printList(userList lista);

 /**
  * Obtiene el mutex asociado a un usuario
  * 
  * @param lista Lista en la que se desea consultar el mutex de un usuarop
  * @param username Nombre del usuario cuyo mutex asociado quiere obtenerse
  * @return Si el usuario está en la lista, se retorna su mutex
  * 
  */
pthread_mutex_t *getMutex(userList lista,char *username);

 /**
  * Determina si un usuario pertenece a una lista.
  * 
  * @param lista Lista en la que se desea consultar si el usuario pertenece.
  * @param username Nombre del usuario que se desea determinar si está
  *                 en la lista.
  * @return 1 si el usuario pertenece a la lista, 0 en caso contrario
  * 
  */
int isIn(userList list, char *username);

//Metodos de lista de salas de chat
 /**
  * Dada una lista de salas de chat, la inicializa colocando NULL su cabeza 
  * y cola, y coloca 0 en su tamaño
  * 
  * @param list Lista que desea inicializarse
  * 
  */
void initializeCRList(chatRoomList *list);

/**
  * Agrega una sala a una lista de salas
  * 
  * @param list Lista en la que se desea agregar la sala
  * @param nombre Nombre de la sala que se desea agregar a la lista
  * @param owner Nombre del usuario que desea agregar la sala a la lista
  * @param ownerSocketFD Socket asociado al usuario que desea agregar la sala
  * @param clientSocketFD File descriptor del socket asociado al usuario
  *                       que quiere agregarse en la lista
  * @return -1 si ya existe una sala con ese nombre en la lista, 0 en caso de
  * agregar la sala satisfactoriamente.
  * 
  */
int addChatRoom(chatRoomList *list, char *nombre, char *owner, int ownerSocketFD);

 /**
  * Elimina una sala de una lista de salas.
  * 
  * @param lista Lista de la que se desea eliminar al usuario
  * @param chatRoom Nombre de la sala que se desea eliminar de la lista
  * @param username Nombre del usuario que desea eliminar la sala de la lista
  * 
  * @return -1 si se intenta eliminar la sala por defecto
  *         -2 si el usuario que desea eliminar la sala no es su dueño
  *          0 si la sala se elimina correctamente
  * 
  */
int removeChatRoom(chatRoomList *list, char *chatRoom, char *username);

 /**
  * Agrega un usuario a una sala de una lista de salas.
  * 
  * @param list Lista de salas en donse se ubica la sala en la que quiere ser
  *             agregado el usuario.
  * @param chatRoom Nombre de la sala en la que quiere ser agregado.
  * @param newUser Nombre del usuario que se desea agregar a la sala.
  * @param clientSocketFD File descriptor del socket asociado al usuario
  *                       que quiere agregarse en la sala.
  * @return -1 si el usuario ya estaba en la sala, 0 si el usuario fue
  *         correctamente agregado, -2 si no pudo agregarse el usuario.
  * 
  */
int addUserToCRList(chatRoomList *list, char *chatRoom, char * newUser, int clientSocketFD);

 /**
  * Elimina un usuario de todas las salas de una lista de sala.
  * 
  * @param lista Lista de salas de las que se desea eliminar al usuario.
  * @param username Nombre del usuario que se desea eliminar de las salas.
  * 
  */
void removeUserFromCRList(chatRoomList *list, char* username);

 /**
  * Devuelve la lista de usuarios de una sala en la que se encuentra
  * un determinado usuario
  * 
  * @param chatRoom Si el usuario pertenece a esta sala, su lista de usuarios
  *                 desea conocerse. 
  * @param username Nombre del usuario que determina si la lista de usuarios
  *                 de esa sala es de interes.
  * @return Si el usuario pertenece a la sala, se retorna la lista de usuarios
  *         de la sala. En caso contrario, se retorna NULL.
  * 
  */
userList *getDestinations(chatRooms *chatRoom, char * username);

 /**
  * Muestra por salida estándar los nombres de las salas pertenecientes a
  * una lista de salas.
  * 
  * @param list Lista cuyas salas quieren ser consultadas
  * 
  */
void printCRList(chatRoomList list);


//Metodos de lista de hilos
 /**
  * Agrega un thread una lista de threads.
  * 
  * @param list Lista en la que se desea agregar el thread.
  * @param thread Hilo que desea agregarse a la lista
  * 
  */
void addThread(threadList *list,pthread_t thread);

 /**
  * Elimina un hilo de una lista de threads.
  * 
  * @param list Lista de la que se desea eliminar al hilo.
  * @param thread Hilo que se desea eliminar de la lista.
  * 
  */
void removeThread(threadList *list, pthread_t thread);

 /**
  * Muestra por salida estándar los hilos pertenecientes a una lista de hilos.
  * 
  * @param list Lista cuyos hilos quieren ser consultados
  * 
  */
void printThreadList(threadList list);