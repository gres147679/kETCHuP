/****************************
 socketManagement.c

 Módulo con operaciones sobre
 los sockets, para escribir
 y leer datos
 
 Andrea Balbás        09-10076
 Gustavo El Khoury    10-10226     
 
 Septiembre - Diciembre 2013
 ****************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "socketManagement.h"
 #include "errors.h"

void copyDataToFD(int from, int to) {
  char buf[1024];
  int amount;

 
  while ((amount = read(from, buf, sizeof(buf))) > 0){
    if (write(to, buf, amount) != amount) {
      fatalError("Socket write failed");
      return;
    }
  }
  
  if (amount < 0) fatalError("Socket write failed");

}

void copyDataToArray(int from, char *to) {
  int amount;
  while ((amount = read(from, to, 170*sizeof(char))) > 0);
  return;

}