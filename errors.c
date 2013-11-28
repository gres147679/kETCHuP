/**
 * errors.c
 *
 * Septiembre - Diciembre 2013
 *
 * Definición y manejo de los errores que pudiesen ocurrir
 *
 * @author Andrea Balbás        09-10076
 * @author Gustavo El Khoury    10-10226
 */

#include <stdio.h>
#include <stdlib.h>
#include "errors.h"

char *programname;

 /**
  * Error que requiere una finalización del programa
  * 
  * @param message Mensaje que será mostrado por salida estándar
  * 
  */
void fatalError(char *message) {
  fprintf(stderr, "%s: %s\n", programname, message);
  perror(programname);
  fflush(stdout);
  exit(1);
}

 /**
  * Error en el que sólo se muestra un mensaje por pantalla, y no requiere
  * que se finalice el programa en ejecución
  * 
  * @param message Mensaje que será mostrado por salida estándar
  * 
  */
void errorMessage(char *message) {
  fprintf(stderr, "%s: %s\n", programname, message);
  fflush(stdout);
}

