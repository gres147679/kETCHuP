#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "serverCommands.h"


char *listUsers(userList *globalUserList, int clientSocketFD){
	// Longitud total del string
	int totalLength = 1;

	// Itero sobre la lista de usuarios, para determinar la suma
	// de las longitudes de los nombre
	userBox *act = globalUserList->head;
	while (act != NULL){
		totalLength += strlen(act->username)+1;
		act = act->sig;
	}
	char *totalString = (char *) malloc(totalLength *sizeof(char));

	// Ahora se itera otra vez, concatenando los strings
	act = globalUserList -> head;
	int currentLength = strlen(act->username);
	strcpy(totalString,act->username);
	totalString[currentLength]='\n';
	act = act->sig;

	
	char *currentString;

	while (act != NULL){
		currentString = act->username;
		currentLength = strlen(currentString);
		strcat(totalString,currentString);
		totalString[currentLength]='\n';
		act = act->sig;
	}

	puts(totalString);
	return totalString;
}

int listChatrooms(chatRoomList *chatList, int clientSocketFD);

int createChatroom(userList *globalUserList, chatRoomList *chatList, char *owner, char *roomName);

int deleteChatroom(chatRoomList *chatList, char *roomName, char *username);