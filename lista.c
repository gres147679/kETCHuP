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
#include "lista.h"

//Metodos de la lista de usuarios
void initialize(userList *lista){
    lista->head = NULL;
    lista->tail = NULL;
    lista->size = 0;
}

int addUser(userList *lista,char *nombreUsuario, int clientSocketFD){
    // Creamos la nueva caja de la lista
    userBox *newu;
    newu = (userBox *) malloc (sizeof(userBox));
    if (newu==NULL) 
      perror("malloc");

    // Rellenamos la caja
    newu->username = (char *) malloc(strlen(nombreUsuario)*sizeof(char)+1);
    strcpy(newu->username,nombreUsuario);
    newu->clientSocketFD = clientSocketFD;
    newu->sig = NULL;

    // Si la lista está vacía
    if (lista->size==0){
        lista->head=newu;
        lista->tail=newu;
        ++lista->size;
        return 0;
    }
    else{
        userBox *actual = lista->head;
        while (actual != NULL){
            if (strcmp(actual->username,nombreUsuario) == 0) return -1;
            actual = actual->sig;
        }

        lista->tail->sig = newu;
        lista->tail = newu;
        // Incremento el tamaño de la lista
        ++lista->size;
        return 0;
    }
 
}

void removeUser(userList *lista, char *username){
    userBox *act;
    act = lista->head;

    if (act == NULL) return;
    
    if (strcmp(act->username,username) == 0){
        lista->head = lista->head->sig;
        --lista->size;
    }else{
        while (act->sig != NULL){
            char *usernameSiguiente = act->sig->username;
            puts(usernameSiguiente);
            if ( strcmp(usernameSiguiente,username)==0 ) break;
            act = act->sig;
        }

        if (act->sig != NULL){
            act->sig = act->sig->sig;
            --lista->size;
        }
    }
}


void printList(userList lista){
    //creo un elemento tmp para iterar
    userBox *tmp;
    tmp = (userBox *) malloc (sizeof(userBox));
    if (tmp==NULL)
        perror("malloc");
    
    tmp = lista.head;
    while (tmp != NULL){
        printf("Usuario: %s \n",tmp->username);
        tmp = tmp->sig;
    }
}

userBox getItem(userList lista,int index){
    userBox *act;
    act = lista.head;
    int i;
    for (i = 0; i < index && act != NULL; i++){
        act = act->sig;
    }

    return *act;
}

int isIn(userList list, char *username){
    userBox *act;
    act = list.head;
    while(act != NULL){
        if (strcmp(act->username, username)== 0) return 1;
        act = act->sig;
    }

    return 0;
}

//Metodos de la lista de salas de chat
void initializeCRList(chatRoomList *list){
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void addChatRoom(chatRoomList *list, char *nombre){
    chatRooms *newChatRoom = (chatRooms *) malloc(sizeof(chatRooms));
    if (newChatRoom==NULL)
        perror("malloc");

    newChatRoom->chatRoomName = nombre;
    initialize( (userList *)&(newChatRoom->users) );


    // Si la lista está vacía
    if (list->size==0){
        list->head=newChatRoom;
        list->tail=newChatRoom;
    }
    else{
        list->tail->next = newChatRoom;
        list->tail = newChatRoom;
    }

    // Incremento el tamaño de la lista
    ++list->size;
}

int removeChatRoom(chatRoomList *list, char *chatRoom, char *username){
    chatRooms *act;
    act = list->head;
     if (strcmp(chatRoom, "actual")== 0) return -1;

    if (strcmp(act->chatRoomName,chatRoom) == 0){
        list->head = list->head->next;
        //here
        --list->size;
    }else{
        while (act->next != NULL){
            char *nextChatRoom = act->next->chatRoomName;
            if ( strcmp(nextChatRoom,chatRoom)==0 ) break;
            act = act->next;
        }

        char *owner = act->next->users.head->username;
        if (strcmp(owner,username)== 0){
            act->next = act->next->next;
            //here
            --list->size;
            return 0;
        }else{
            return -2;
        }
    }

    return 0;
}

int addUserToCRList(chatRoomList *list, char *chatRoom, char * newUser, int clientSocketFD){
    chatRooms *act;
    act = list->head;
    while (act != NULL){
        if (strcmp(act->chatRoomName,chatRoom) == 0){
            return addUser(&(act->users),newUser, clientSocketFD);
        }else{
            act = act->next;
        }
    }
    return -2;
}

void removeUserFromCRList(chatRoomList *list, char* username){
    chatRooms *act;
    act = list->head;
    userList *actUsers;
    while (act != NULL){
        actUsers = &(act->users);
        removeUser(actUsers,username);
        act = act->next;
    }
}

userList *getDestinations(chatRooms *chatRoom, char *chatRoomName, char * username){
    userList *destinations = &chatRoom->users;
    if (isIn(*destinations, username)) return destinations;
    return NULL;
}

void printCRList(chatRoomList list){
    //creo un elemento tmp para iterar
    chatRooms *tmp;
    tmp = (chatRooms *) malloc (sizeof(chatRooms));
    if (tmp==NULL)
        perror("malloc");
    
    tmp = list.head;
    while (tmp != NULL){
        printf("Chat room: %s \n",tmp->chatRoomName);
        tmp = tmp->next;
    }
}

//Metodos de la lista de threads
void addThread(threadList *list,pthread_t thread){
    // Creamos la nueva caja de la lista
    threadBox *newt;
    newt = (threadBox *) malloc (sizeof(threadBox));
    if (newt==NULL) 
      perror("malloc");

    // Rellenamos la caja
    newt->tid = thread;
    newt->next = NULL;

    // Si la lista está vacía
    if (list->size==0){
        list->head=newt;
        list->tail=newt;
    }
    else{
        list->tail->next = newt;
        list->tail = newt;
    }

    // Incremento el tamaño de la lista
    ++list->size;
}

void removeThread(threadList *list, pthread_t thread){
    threadBox *act;
    act = list->head;
    
    if (act->tid == thread){
        list->head = list->head->next;
    }else{
        while (act->next != NULL){
            pthread_t nextTid = act->next->tid;
            if ( nextTid == thread ) break;
            act = act->next;
        }

        if (act->next != NULL){
            act->next = act->next->next;
        }
    }
}


void printThreadList(threadList list){
    //creo un elemento tmp para iterar
    threadBox *tmp;
    tmp = (threadBox *) malloc (sizeof(threadBox));
    if (tmp==NULL)
        perror("malloc");
    
    tmp = list.head;
    while (tmp != NULL){
        printf("Thread ID: %i \n",(int)tmp->tid);
        tmp = tmp->next;
    }
}