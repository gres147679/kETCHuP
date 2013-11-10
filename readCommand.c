/****************************
readCommand.c
 
 Andrea Balbás        09-10076
 Gustavo El Khoury    10-10226     
 
 Septiembre - Diciembre 2013
 ****************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sysexits.h>
#include <string.h>

char command[170];

void readCmd(FILE *file){
    
    fgets(command, 170, file);
}

void main(int argc, char *argv[]) {
    FILE *input;
    input = fopen(argv[1], "r");
    if (input == NULL){
        perror("No se encuentra el archivo");
        exit(EX_NOINPUT);
    }
 
    char cmdname[4];
    cmdname[3]='\0';
    while (feof(input)==0){
        readCmd(input);
        printf("comando: %s \n",command);
        strncpy(cmdname, command, 3);
        printf("nombre: %s\n", cmdname);
    }
    fclose(input);
}