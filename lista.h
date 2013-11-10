/****************************
lista.h
Lista enlazada simple de usuarios
 Andrea Balbás        09-10076
 Gustavo El Khoury    10-10226     
 
 Septiembre - Diciembre 2013
 
 ****************************/
 
#include <stdio.h>
#include <stdlib.h>

typedef struct{
    char *username;
    struct usuarios *sig;
} usuarios; 

typedef struct{
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

void removeUser(char *username){
    usuarios *act;
    act = cabeza;
    
    if (*act->username = username){
        cabeza = *cabeza->sig;
    }else{
        while (*act->sig != NULL){
            if (*(*act->sig)->username==username) break;
            act = act->sig;
        }

        if (*act->sig!=NULL){
            act->sig = *(act->sig)->sig;
        }
    }
}

void printLista(){
    //creo un elemento tmp para iterar
    usuarios *tmp;
    tmp = (usuarios *) malloc (sizeof(usuarios));
    if (tmp==NULL)
        perror("malloc");
    
    tmp = cabeza;
    while (*act != NULL){
        printf("Usuario: " + *act->username + "\n");
        act = *act->sig;
    }
}
















>>>>>>> 50e56508471c3baa1248990389a689353e265608
