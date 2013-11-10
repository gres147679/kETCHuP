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
}

void removeUser(listaUsuarios *lista, char *username){
    usuarios *act;
    act = lista->cabeza;
    
    if (*act->username = username){
        lista->cabeza = lista->cabeza->sig;
    }else{
        while (act->sig != NULL){
            char *usernameSiguiente = act->sig->username;
            if ( strcmp(usernameSiguiente,username) ) break;
            act = act->sig;
        }

        if (act->sig != NULL){
            act->sig = act->sig->sig;
        }
    }
}


void printLista(listaUsuarios lista){
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