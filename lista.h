/****************************
lista.h
Lista enlazada simple de userBox
 Andrea Balbás        09-10076
 Gustavo El Khoury    10-10226     
 
 Septiembre - Diciembre 2013
 
 ****************************/
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

//Estructuras para listas de usuarios
typedef struct userBox{
    char *username;
    int clientSocketFD;
    pthread_mutex_t userMutex;
    struct userBox *sig;
} userBox; 

typedef struct userList{
    int size;
    userBox *head;
    userBox *tail;
} userList;

//Estructuras para listas de salas de chat
typedef struct chatRooms{
    char *chatRoomName;
    userList users;
    struct chatRooms *next;
} chatRooms;

typedef struct chatRoomList{
    int size;
    chatRooms *head;
    chatRooms *tail;
}chatRoomList;

//Estructuras para listas de hilos
typedef struct threadBox{
    pthread_t tid;
    struct threadBox *next;
} threadBox; 

typedef struct threadList{
    int size;
    threadBox *head;
    threadBox *tail;
} threadList;

//Metodos de lista de usuarios
void initialize(userList *lista);

int addUser(userList *lista,char *nombreUsuario, int clientSocketFD);

void removeUser(userList *lista, char *username);

void printList(userList lista);

userBox *getItem(userList lista,int index);

int isIn(userList list, char *username);

//Metodos de lista de salas de chat

void initializeCRList(chatRoomList *list);

int addChatRoom(chatRoomList *list, char *nombre, char *owner, int ownerSocketFD);

int removeChatRoom(chatRoomList *list, char *chatRoom, char *username);

int addUserToCRList(chatRoomList *list, char *chatRoom, char * newUser, int clientSocketFD);

void removeUserFromCRList(chatRoomList *list, char* username);

userList *getDestinations(chatRooms *chatRoom, char *chatRoomName, char * username);

void printCRList(chatRoomList list);


//Metodos de lista de hilos
void addThread(threadList *list,pthread_t thread);

void removeThread(threadList *list, pthread_t thread);

void printThreadList(threadList list);