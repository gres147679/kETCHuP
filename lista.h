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

typedef struct userBox{
    char *username;
    struct userBox *sig;
} userBox; 

typedef struct userList{
    int size;
    userBox *head;
    userBox *tail;
} userList;

void initialize(userList *lista){
    lista->head = NULL;
    lista->tail = NULL;
    lista->size = 0;
}

void addUser(userList *lista,char *nombreUsuario){
    // Creamos la nueva caja de la lista
    userBox *newu;
    newu = (userBox *) malloc (sizeof(userBox));
    if (newu==NULL) 
      perror("malloc");

    // Rellenamos la caja
    newu->username = nombreUsuario;
    newu->sig = NULL;

    // Si la lista está vacía
    if (lista->size==0){
        lista->head=newu;
        lista->tail=newu;
    }
    else{
        lista->tail->sig = newu;
        lista->tail = newu;
    }

    // Incremento el tamaño de la lista
    ++lista->size;
}

void removeUser(userList *lista, char *username){
    userBox *act;
    act = lista->head;
    
    if (strcmp(act->username,username) == 0){
        lista->head = lista->head->sig;
    }else{
        while (act->sig != NULL){
            char *usernameSiguiente = act->sig->username;
            if ( strcmp(usernameSiguiente,username)==0 ) break;
            act = act->sig;
        }

        if (act->sig != NULL){
            act->sig = act->sig->sig;
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

typedef struct chatRooms{
    char *chatRoomName;
    //pthread_t chatThread;
    userList users;
    struct chatRooms *next;
} chatRooms;

typedef struct chatRoomList{
    int size;
    chatRooms *head;
    chatRooms *tail;
}chatRoomList;

void initializeCRList(chatRoomList *list){
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void addChatRoom(chatRoomList *list, chatRooms *newChatRoom){
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

void removeChatRoom(chatRoomList *list, char *chatRoom){
    chatRooms *act;
    act = list->head;
    
    if (strcmp(act->chatRoomName,chatRoom) == 0){
        list->head = list->head->next;
    }else{
        while (act->next != NULL){
            char *nextChatRoom = act->next->chatRoomName;
            if ( strcmp(nextChatRoom,chatRoom)==0 ) break;
            act = act->next;
        }

        if (act->next != NULL){
            act->next = act->next->next;
        }
    }
}

void addUserToCRList(chatRoomList *list, char *chatRoom, char * newUser){
    chatRooms *act;
    act = list->head;
    while (act->next != NULL){
        if (strcmp(act->chatRoomName,chatRoom) == 0){
            addUser(&(act->users),newUser);
            break;
        }else{
            act = act->next;
        }
    }
}

void removeUserFromCRList(chatRoomList *list, char* username){
    chatRooms *act;
    act = list->head;
    userList actUsers;
    while (act->next != NULL){
        actUsers = act->users;
        removeUser(&actUsers,username);
    }
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