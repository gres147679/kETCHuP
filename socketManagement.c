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
  char buffer[1024];
  int amount;

  puts("testing before, jojojo");
  while ((amount = read(from, buffer, sizeof(buffer))) > 0){
    puts("testing");
    if (write(to, buffer, amount) != amount) {
      fatalError("Falló la escritura al socket");
      return;
    }
  }
  
  if (amount < 0) fatalError("Falló la escritura al socket");

}

void copyDataToArray(int from, char *to) {
  int amount;
  while ((amount = read(from, to, 170*sizeof(char))) > 0);
  return;

}