/* Programa Servidor. ejemplo con sockets TCP/IP */
/* Ejemplo tomado del libro: Linux Application Development. Michael 
   K. Johnson y Erik Troan */

   

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
 
#include "socketManagement.h"

/* Espera por una conexi'on por el puerto 1236. Una vez que la conexion se 
   establece, se copian los datos enviados por el socket a la salida estandar y
   se espera por otra conexion */

void serveClient(struct sockaddr_in clientAddress,int newClientSocketFD){
  char buffer[170];
  char command[4];
  command[3]='\0';

  puts("blah1");
  copyDataToFD(newClientSocketFD,1); 
  puts("blah");
  strncpy(command,buffer,3);

  // Definición de los comandos
  if ( strcmp(command,"sal") == 0 ){
    printf("Mandaste sal\n");
    //Llamada a implementación de sal
  }
  else if ( strcmp(command,"usu") == 0 ){
    printf("Mandaste usu\n");
    //Llamada a implementación de sal
  }
  else if ( strcmp(command,"men") == 0 ){
    printf("Mandaste men\n");
    //Llamada a implementación de sal
  }
  else if ( strcmp(command,"sus") == 0 ){
    printf("Mandaste sus\n");
    //Llamada a implementación de sal
  }
  else if ( strcmp(command,"des") == 0 ){
    printf("Mandaste des\n");
    //Llamada a implementación de sal
  }
  else if ( strcmp(command,"cre") == 0 ){
    printf("Mandaste cre\n");
    //Llamada a implementación de sal
  }
  else if ( strcmp(command,"eli") == 0 ){
    printf("Mandaste eli\n");
    //Llamada a implementación de sal
  }
  else if ( strcmp(command,"fue") == 0 ){
    printf("Mandaste fue\n");
    //Llamada a implementación de sal
  }
  return;
}

int main(void)
{
        struct sockaddr_in address;
        int sock, conn, i;
        size_t addrLength = sizeof(struct sockaddr_in);


        if(( sock = socket (PF_INET, SOCK_STREAM, 0)) < 0)
                perror("socket");


        /* Permite reusar la direccion del socket en un periodo corto de tiempo */ 
       i = 1;
       setsockopt(sock,SOL_SOCKET, SO_REUSEADDR, &i, sizeof(i));
       

       address.sin_family = AF_INET;
       address.sin_port = htons(20226);
       memset(&address.sin_addr, 0, sizeof(address.sin_addr));

       if (bind(sock, (struct sockaddr *)&address, sizeof(address)))
                perror("bind");

       if (listen(sock, 5))
                perror("listen");
       while((conn = accept(sock, (struct sockaddr *) &address, &addrLength)) >= 0) {
                printf("conn = %d. sock = %d\n", conn, sock);
                printf("...getting data\n");
                copyDataToFD(conn,1);
                serveClient(address,conn);
                printf("...Done\n");
                close(conn);
     }

        
     close(sock);
     return 0;


}
