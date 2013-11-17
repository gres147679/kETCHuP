/****************************
 cchat.c
 
 Andrea Balbás        09-10076
 Gustavo El Khoury    10-10226     
 
 Septiembre - Diciembre 2013
 ****************************/
#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <sysexits.h>
#include <netinet/in.h>
#include <string.h>
#include "errors.h"
#include "socketManagement.h"

#define PORT 20226

void copy(int sockfd) {
  int c;
  char outbuffer;
  char inbuffer;

  while ((c = getchar()) != EOF) {
    /* Write a character to the socket. */
    outbuffer = c;
    if (write(sockfd, &outbuffer, 1) != 1)
      fatalError("can't write to socket");
    /* Read the response and print it. */
    if (read(sockfd, &inbuffer, 1) != 1)
      fatalError("can't read from socket");
    putchar(inbuffer);
  }
}



int main (int argc, char **argv){
  int index;
  int flag;

  // Variables de ejecución del cliente, que se setean según CLI
  char host[16] = "127.0.0.1";
  int port;

  // Se genera un username random por defecto
  char username[20] = "usuario";
  int randNum = rand() % 100000;
  char rand[5];
  snprintf(rand, 5, "%d", randNum);
  strcat(username,rand);

  // Archivo de entrada. Puede ser vacío
  //NOTA: creo que podriamos poner filename[] y asi no limitar la extension
  char filename[100] = "";


  opterr = 0;

  while ((flag = getopt (argc, argv, "-h:-p:-n:-a:")) != -1)
   switch (flag)
     {
     case 'h':
       strcpy(host,optarg);
       break;
     case 'p':
       port = atoi(optarg);
       break;
     case 'n':
       strcpy(username,optarg);
       break;
     case 'a':
       strcpy(filename,optarg);
       printf("El archivo a abrir es %s", filename);
       break;
     case '?':
       if (optopt == 'h' || optopt == 'p' || optopt == 'n' || optopt == 'a')
         fprintf (stderr, "La opción -%c requiere un argumento.\n", optopt);
       else if (isprint(optopt))
         fprintf (stderr, "Opción inválida `-%c'.\n", optopt);
       else
         fprintf (stderr,
                  "Opción inválida `\\x%x'.\n",
                  optopt);
       return 1;
     default:
       abort ();
     }

  for (index = optind; index < argc; index++)
  printf ("No es una opción: %s\n", argv[index]);
  
  int clientSocketFD;
  struct sockaddr_in serverAddress;

  /* Remember the program name for error messages. */
  programname = argv[0];

  /* Get the address of the server. */
  bzero(&serverAddress, sizeof(serverAddress));
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = inet_addr(host);
  serverAddress.sin_port = htons(PORT);

  /* Open a socket. */
  clientSocketFD = socket(AF_INET, SOCK_STREAM, 0);
  if (clientSocketFD < 0)
    fatalError("can't open socket");

  /* Connect to the server. */
  if (connect(clientSocketFD, (struct sockaddr *) &serverAddress,
              sizeof(serverAddress)) < 0)
    fatalError("can't connect to server");

  /* Copy input to the server. */
  if (strlen(filename) != 0){
    FILE *input;
    input = fopen(filename,"r");
    if (input == NULL){
        perror("No se encuentra el archivo");
        exit(EX_NOINPUT);
    }else{
        char command[] = "";
        int n;
        while (feof(input) == 0){
            fgets(command, 170, input);
            n = write(clientSocketFD, command, sizeof(command));
        }
    }
    fclose(input);
  }
  // n = write(sockfd,,2);
  //copyDataToFD(0,clientSocketFD);
  close(clientSocketFD);

  exit(EXIT_SUCCESS);

}