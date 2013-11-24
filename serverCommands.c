#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "serverCommands.h"


char *listUsers(userList *globalUserList, int clientSocketFD){
	char *totalString;

	if (globalUserList->size == 0) {
		char *mensaje = "No hay usuarios conectados\n";
		totalString = (char *) malloc(strlen(mensaje)*sizeof(char)+1);
		strcpy(totalString,mensaje);
		return totalString;
	}

	// Longitud total del string
	int totalLength = 0;

	// Itero sobre la lista de usuarios, para determinar la suma
	// de las longitudes de los nombre
	userBox *act = globalUserList->head;
	while (act != NULL){
		totalLength += strlen(act->username)+1;
		act = act->sig;
	}
	totalString = (char *) malloc(totalLength *sizeof(char));

	// Ahora se itera otra vez, concatenando los strings
	act = globalUserList -> head;
	int currentLength = strlen(act->username);
	strcpy(totalString,act->username);
	totalString[currentLength]='\n';
	totalString[currentLength+1]='\0';
	act = act->sig;
		
	char *currentString;

	while (act != NULL){
		currentString = act->username;
		currentLength += strlen(currentString)+1;
		strcat(totalString,currentString);
		totalString[currentLength]='\n';
		act = act->sig;
	}

	return totalString;
}

char *listChatrooms(chatRoomList *chatRoomsList, int clientSocketFD){
	char *totalString;

	if (chatRoomsList->size == 0) {
		char *mensaje = "No hay salas creadas\n";
		totalString = (char *) malloc(strlen(mensaje)*sizeof(char)+1);
		strcpy(totalString,mensaje);
		return totalString;
	}

	// Longitud total del string
	int totalLength = 0;

	// Itero sobre la lista de salas de chat, para determinar la suma
	// de las longitudes de los nombres
	chatRooms *act =chatRoomsList->head;
	while (act != NULL){
		totalLength += strlen(act->chatRoomName)+1;
		act = act->next;
	}
	totalString = (char *) malloc(totalLength *sizeof(char));

	// Ahora se itera otra vez, concatenando los strings
	act = chatRoomsList-> head;
	int currentLength = strlen(act->chatRoomName);
	strcpy(totalString,act->chatRoomName);
	totalString[currentLength]='\n';
	totalString[currentLength+1]='\0';
	act = act->next;
	
	char *currentString;

	while (act != NULL){
		currentString = act->chatRoomName;
		currentLength += strlen(currentString)+1;
		strcat(totalString,currentString);
		totalString[currentLength]='\n';
		act = act->next;
	}

	return totalString;
}

int createChatroom(chatRoomList *chatList, char *roomName, char *owner, int ownerSocketFD){
	chatRooms *currentChat;
	for(currentChat=chatList->head;currentChat!=NULL;currentChat=currentChat->next){
		if(strcmp(currentChat->chatRoomName,roomName)==0) return -1;
	}
	addChatRoom(chatList,roomName);
	addUserToCRList(chatList,roomName,owner,ownerSocketFD);
	return 0;
}

int deleteChatroom(chatRoomList *chatList, char *roomName, char *username){
	return removeChatRoom(chatList,roomName, username);
}

int subscribeUser(chatRoomList *list, char *chatRoom, char * newUser, int clientSocketFD){
	int operationComplete = addUserToCRList(list,chatRoom,newUser,clientSocketFD);	

	if(operationComplete == 0) sendResponseToClient(
        clientSocketFD,"La suscripción se ha realizado con éxito");
      else if (operationComplete == -1) sendResponseToClient(
        clientSocketFD,"Ya estás suscrito a la sala");
      else sendResponseToClient(
        clientSocketFD,"La sala no existe. Puedes crearla usando el comando cre");
}

int unsubscribeUser(chatRoomList *list, char * username,int clientSocketFD){
	removeUserFromCRList(list,username);
	sendResponseToClient(
        clientSocketFD,"Te has desuscrito de todas las salas");
}