/**
 * errors.h
 *
 * Septiembre - Diciembre 2013
 *
 * Definicion y manejo de los errores que pudiesen ocurrir
 *
 * @author Andrea Balbás        09-10076
 * @author Gustavo El Khoury    10-10226
 */

#ifndef _ERRORS_H_
#define _ERROR_H_

extern char *programname;

 /**
  * Error que requiere una finalización del programa
  * 
  * @param message Mensaje que será mostrado por salida estándar
  * 
  */
extern void fatalError(char *message);

 /**
  * Error en el que sólo se muestra un mensaje por pantalla, y no requiere
  * que se finalice el programa en ejecución
  * 
  * @param message Mensaje que será mostrado por salida estándar
  * 
  */
extern void errorMessage(char *message);

#endif

