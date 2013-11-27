#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>

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
	

	char *mensaje = "La lista de usuarios es:\n";
	int currentLength = strlen(mensaje);
	totalString = (char *) malloc((currentLength+totalLength)*sizeof(char));
	strcpy(totalString,mensaje);
	

	// Ahora se itera otra vez, concatenando los strings
	act = globalUserList -> head;
	// strcpy(totalString,act->username);
	// totalString[currentLength]='\n';
	// totalString[currentLength+1]='\0';
	// act = act->sig;
		
	char *currentString;

	while (act != NULL){
		currentString = act->username;
		currentLength += strlen(currentString)+1;
		strcat(totalString,currentString);
		totalString[currentLength-2]='\n';
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

	char *mensaje = "La lista de salas es:\n";
	int currentLength = strlen(mensaje);
	totalString = (char *) malloc((currentLength+totalLength)*sizeof(char));
	strcpy(totalString,mensaje);

	// Ahora se itera otra vez, concatenando los strings
	act = chatRoomsList-> head;
	
	char *currentString;

	while (act != NULL){
		currentString = act->chatRoomName;
		currentLength += strlen(currentString)+1;
		strcat(totalString,currentString);
		totalString[currentLength-1]='\n';
		//totalString[currentLength+1]='\0';
		act = act->next;
	}

	return totalString;
}

int createChatroom(chatRoomList *chatList, char *roomName, char *owner, int ownerSocketFD){
    return addChatRoom(chatList, roomName, owner, ownerSocketFD);
}

int deleteChatroom(chatRoomList *chatList, char *roomName, char *username){
	return removeChatRoom(chatList,roomName, username);
}

int subscribeUser(chatRoomList *list, char *chatRoom, char * newUser, int clientSocketFD){
	return addUserToCRList(list,chatRoom,newUser,clientSocketFD);	
}

void unsubscribeUser(chatRoomList *list, char * username,int clientSocketFD){
	removeUserFromCRList(list,username);
}

void sendMessageToChatRooms(chatRoomList *list, char * username, char *message){
	chatRooms *act;
	act = list->head;
	userList *currentUsers;
	while (act != NULL){
		currentUsers = &act->users;
		if (isIn(*currentUsers,username))
			sendMessageToUsers(currentUsers, message);
		act = act->next;
	}
}

void sendMessageToUsers(userList *destinations, char *message){
	userBox *act;
	act = destinations->head;
	while(act != NULL){
		//printf("A traves de %d\n",act->clientSocketFD);
		//printf("Le estoy mandando a ");
		//puts(act->username);
		//puts("jaja");
		pthread_mutex_lock(&act->userMutex);
		//puts("jaja2");
		if (act->clientSocketFD != -1) sendMessagesToClient(act->clientSocketFD, message);
		//puts("Ya le mande");
		pthread_mutex_unlock(&act->userMutex);
		act = act -> sig;
	}
	
}