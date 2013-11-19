/****************************
 socketManagement.h

 Módulo con operaciones sobre
 los sockets, para escribir
 y leer datos
 
 Andrea Balbás        09-10076
 Gustavo El Khoury    10-10226     
 
 Septiembre - Diciembre 2013
 ****************************/
typedef struct commandPacket{
  char command[4];
  char *argument;
} commandPacket;


void copyDataToFD(int, int);

void copyDataToArray(int from, char *to) ;

int readCommandFromSocket(int socketFD, commandPacket *receivedCommand);

int sendCommandToSocket(int socketFD, commandPacket newCommand);