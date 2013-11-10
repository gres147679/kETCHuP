/****************************
lista.h
Lista enlazada simple de usuarios
 Andrea Balbás        09-10076
 Gustavo El Khoury    10-10226     
 
 Septiembre - Diciembre 2013
 
 ****************************/
 
#include <stdio.h>
#include <stdlib.h>

struct usuarios{
    char *username;
    usuarios *sig;
}

struct usuarios *cabeza, *cola;

void adduser(char *newuser){
    struct usuarios *newu;
    newu = (struct usuarios *) malloc (sizeof(struct usuarios));
    if (newu==NULL) 
      perror("malloc");

    newu->username = newuser;
    newu->sig = NULL;

    if (cabeza==NULL){
        cabeza=newu;
        cola=newu;
    }else{
        cola->sig = newu;
        cola = newu;
    }
}

/*
void removeuser(char *username){
    usuarios *act;
    act = cabeza;
    while (*act != NULL){
        if (*act->username==*username) break;
            act = act->sig;
        }

        if (*act!=NULL){

        }
    }

}*/















