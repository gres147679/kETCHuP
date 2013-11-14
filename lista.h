/****************************
lista.h
Lista enlazada simple de usuarios
 Andrea Balbás        09-10076
 Gustavo El Khoury    10-10226     
 
 Septiembre - Diciembre 2013
 
 ****************************/
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

typedef struct usuarios{
    char *username;
    struct usuarios *sig;
} usuarios; 

typedef struct listaUsuarios{
    int size;
    usuarios *cabeza;
    usuarios *cola;
} listaUsuarios;

void initialize(listaUsuarios *lista){
    lista->cabeza = NULL;
    lista->cola = NULL;
    lista->size = 0;
}

void addUser(listaUsuarios *lista,char *nombreUsuario){
    // Creamos la nueva caja de la lista
    usuarios *newu;
    newu = (usuarios *) malloc (sizeof(usuarios));
    if (newu==NULL) 
      perror("malloc");

    // Rellenamos la caja
    newu->username = nombreUsuario;
    newu->sig = NULL;

    // Si la lista está vacía
    if (lista->size==0){
        lista->cabeza=newu;
        lista->cola=newu;
        printf("hola\n");
    }
    else{
        lista->cola->sig = newu;
        lista->cola = newu;
        printf("chao\n");
    }

    // Incremento el tamaño de la lista
    ++lista->size;
    /*******ESTO HAY QUE BORRARLO***********/
    printf("La cabeza ahora es %s\n",lista->cabeza->username);
    printf("La cola ahora es %s\n",lista->cola->username);
}

void removeUser(listaUsuarios *lista, char *username){
    usuarios *act;
    act = lista->cabeza;
    printf("%s\n",act->username);
    printf("%s\n",username);
    
    if (strcmp(act->username,username) == 0){

        printf("balsada"); 
        lista->cabeza = lista->cabeza->sig;
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


void printList(listaUsuarios lista){
    //creo un elemento tmp para iterar
    usuarios *tmp;
    tmp = (usuarios *) malloc (sizeof(usuarios));
    if (tmp==NULL)
        perror("malloc");
    
    tmp = lista.cabeza;
    while (tmp != NULL){
        printf("Usuario: %s \n",tmp->username);
        tmp = tmp->sig;
    }
}

typedef struct chatRooms{
    char *chatRoomName;
    //pthread_t chatThread;
    listaUsuarios users;
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
        /*******ESTO HAY QUE BORRARLO***********/
        printf("hola\n");
    }
    else{
        list->tail->next = newChatRoom;
        list->tail = newChatRoom;
        /*******ESTO HAY QUE BORRARLO***********/
        printf("chao\n");
    }

    // Incremento el tamaño de la lista
    ++list->size;
    /*******ESTO HAY QUE BORRARLO***********/
    printf("La cabeza ahora es %s\n",list->head->chatRoomName);
    printf("La cola ahora es %s\n",list->tail->chatRoomName);
}

void removeChatRoom(chatRoomList *list, char *chatRoom){
    chatRooms *act;
    act = list->head;
    /*******ESTO HAY QUE BORRARLO***********/
    printf("%s\n",act->chatRoomName);
    printf("%s\n",chatRoom);
    
    if (strcmp(act->chatRoomName,chatRoom) == 0){
        /*******ESTO HAY QUE BORRARLO***********/
        printf("balsada"); 
        
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