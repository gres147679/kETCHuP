/**
 * lista.c
 *
 * Septiembre - Diciembre 2013
 *
 * Definición y manejo de listas.
 * Se manejan 3 tipos de listas en el proyecto: listas de usuarios, 
 * de salas y de threads.
 *
 * @author Andrea Balbás        09-10076
 * @author Gustavo El Khoury    10-10226
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "lista.h"

//Metodos de la lista de usuarios

 /**
  * Dada una lista de usuarios, la inicializa colocando NULL su cabeza y cola, 
  * y coloca 0 en su tamaño
  * 
  * @param lista Lista que desea inicializarse
  * 
  */
void initialize(userList *lista){
    lista->head = NULL;
    lista->tail = NULL;
    lista->size = 0;
}

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
int addUser(userList *lista,char *nombreUsuario, int clientSocketFD){
    // Creamos la nueva caja de la lista
    userBox *newu = (userBox *) malloc (sizeof(userBox));
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
        userBox *previo;
        while (actual != NULL){
            if (strcmp(actual->username,nombreUsuario) == 0) return -1;
            else{      
              previo = actual;        
              actual = actual->sig;
            }
            //actual = actual->sig;
        }

        previo->sig = newu;
        //lista->tail->sig = newu;
        lista->tail = newu;
        // Incremento el tamaño de la lista
        ++lista->size;

        return 0;
    }
 
}

 /**
  * Elimina un usuario de una lista de usuarios.
  * 
  * @param lista Lista de la que se desea eliminar al usuario
  * @param nombreUsuario Nombre del usuario que se desea eliminar de la lista
  * 
  */
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

 /**
  * Muestra por salida estándar los nombres de usuario de los usuarios
  * pertenecientes a una lista
  * 
  * @param lista Lista cuyos usuarios quieren ser consultados
  * 
  */
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

 /**
  * Obtiene el mutex asociado a un usuario
  * 
  * @param lista Lista en la que se desea consultar el mutex de un usuarop
  * @param username Nombre del usuario cuyo mutex asociado quiere obtenerse
  * @return Si el usuario está en la lista, se retorna su mutex
  * 
  */
pthread_mutex_t *getMutex(userList lista,char *username){
    userBox *act;
    act = lista.head;
    if (strcmp(username,act->username) == 0) return &act->userMutex;
    else act = act->sig;

    while(act != NULL){
        if (strcmp(act->username,username)==0) return &act->userMutex;
        else act = act->sig;
    }
}

 /**
  * Determina si un usuario pertenece a una lista.
  * 
  * @param lista Lista en la que se desea consultar si el usuario pertenece.
  * @param username Nombre del usuario que se desea determinar si está
  *                 en la lista.
  * @return 1 si el usuario pertenece a la lista, 0 en caso contrario
  * 
  */
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
 /**
  * Dada una lista de salas de chat, la inicializa colocando NULL su cabeza 
  * y cola, y coloca 0 en su tamaño
  * 
  * @param list Lista que desea inicializarse
  * 
  */
void initializeCRList(chatRoomList *list){
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

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
int addChatRoom(chatRoomList *list, char *nombre, char *owner, int ownerSocketFD){
    //Creamos la sala que se quiere agregar
    chatRooms *newChatRoom = (chatRooms *) malloc(sizeof(chatRooms));
    if (newChatRoom==NULL)
        perror("malloc");

    //Rellenamos los datos de la sala
    newChatRoom->chatRoomName = (char *) malloc(strlen(nombre)*sizeof(char)+1);
    strcpy(newChatRoom->chatRoomName,nombre);
    initialize( (userList *)&(newChatRoom->users) );
    int creator = addUser(&(newChatRoom->users), owner, ownerSocketFD);

    // Si la lista está vacía
    if (list->size==0){
        list->head=newChatRoom;
        list->tail=newChatRoom;
        ++list->size;
        return 0;
    }
    else{
        chatRooms*act = list->head;
        while (act != NULL){
            if (strcmp(act->chatRoomName,nombre)== 0) return -1;
            act = act->next;
        }
        
        list->tail->next = newChatRoom;
        list->tail = newChatRoom;
        ++list->size;
        return 0;
    }
}

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
int removeChatRoom(chatRoomList *list, char *chatRoom, char *username){
    chatRooms *act;
    act = list->head;
     if (strcmp(chatRoom, "actual")== 0) return -1;

    if (strcmp(act->chatRoomName,chatRoom) == 0){
        list->head = list->head->next;
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
            --list->size;
            return 0;
        }else{
            return -2;
        }
    }

    return 0;
}

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

 /**
  * Elimina un usuario de todas las salas de una lista de sala.
  * 
  * @param lista Lista de salas de las que se desea eliminar al usuario.
  * @param username Nombre del usuario que se desea eliminar de las salas.
  * 
  */
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
userList *getDestinations(chatRooms *chatRoom, char * username){
    userList *destinations = &chatRoom->users;
    if (isIn(*destinations, username)) return destinations;
    return NULL;
}

 /**
  * Muestra por salida estándar los nombres de las salas pertenecientes a
  * una lista de salas.
  * 
  * @param list Lista cuyas salas quieren ser consultadas
  * 
  */
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
 /**
  * Agrega un thread una lista de threads.
  * 
  * @param list Lista en la que se desea agregar el thread.
  * @param thread Hilo que desea agregarse a la lista
  * 
  */
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

 /**
  * Elimina un hilo de una lista de threads.
  * 
  * @param list Lista de la que se desea eliminar al hilo.
  * @param thread Hilo que se desea eliminar de la lista.
  * 
  */
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

 /**
  * Muestra por salida estándar los hilos pertenecientes a una lista de hilos.
  * 
  * @param list Lista cuyos hilos quieren ser consultados
  * 
  */
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