/**
 * serverCommands.c
 *
 * Septiembre - Diciembre 2013
 *
 * Manejo de los comandos que son enviados al servidor.
 *
 * @author Andrea Balbás        09-10076
 * @author Gustavo El Khoury    10-10226
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>

#include "serverCommands.h"

 /**
  * Lista todos los usuarios conectados al servidor de chat.
  * 
  * @param globalUserList Lista de usuarios del servidor.
  * @param clientSocketFD Socket del usuario que envió el comando.
  * @return String con los nombres de los usuarios conectados, incluido
  *         el que solicitó que se listaran los usuarios.
  * 
  */
char *listUsers(userList *globalUserList, int clientSocketFD){
	char *totalString;

	if (globalUserList->size == 0) {
		char *mensaje = "No hay usuarios conectados\n";
		totalString = (char *) malloc(strlen(mensaje)*sizeof(char)+1);
                if (totalString==NULL) 
                    perror("malloc");
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
	int currentLength = strlen(mensaje)+1;
	totalString = (char *) malloc((currentLength+totalLength)*sizeof(char));
        if (totalString==NULL) 
            perror("malloc");
	strcpy(totalString,mensaje);
	

	// Ahora se itera otra vez, concatenando los strings
	act = globalUserList -> head;
		
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

 /**
  * Lista todas las salas existentes en el servidor
  * 
  * @param chatRoomList Lista de salas del servidor
  * @param clientSocketFD Socket del usuario que envió el comando.
  * @return String con los nombres de las salas existentes en el servidor
  * 
  */
char *listChatrooms(chatRoomList *chatRoomsList, int clientSocketFD){
	char *totalString;

	if (chatRoomsList->size == 0) {
		char *mensaje = "No hay salas creadas\n";
		totalString = (char *) malloc(strlen(mensaje)*sizeof(char)+1);
                if (totalString==NULL) 
                    perror("malloc");
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
        if (totalString==NULL) 
            perror("malloc");
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

 /**
  * Añade una nueva sala de chat al servidor.
  * 
  * @param chatRoomList Lista de salas existentes antes de agregar la nueva.
  * @param roomName Nombre de la sala a agregar.
  * @param owner Nombre de usuario del cliente que creó la sala.
  * @param ownerSocketFD Socket del cliente que solcitó la creación de la sala
  * @return 0 si la sala fue creada correctamente, -1 si ya existe una sala
  *         con ese nombre.
  * 
  */
int createChatroom(chatRoomList *chatList, char *roomName, char *owner, int ownerSocketFD){
    return addChatRoom(chatList, roomName, owner, ownerSocketFD);
}

 /**
  * Elimina una sala del servidor. Para eliminar una sala, el cliente debe ser
  * su dueño, es decir, debe haberla creado. La sala por defecto no puede
  * ser eliminada por ningun usuario.
  * 
  * @param chatList Lista de salas existentes antes de la eliminación.
  * @param roomName Nombre de la sala a eliminar.
  * @param username Nombre de usuario del cliente que desea eliminar la sala.
  * @return -1 si se intenta eliminar la sala por defecto
  *         -2 si el usuario que desea eliminar la sala no es su dueño
  *          0 si la sala se elimina correctamente
  * 
  */
int deleteChatroom(chatRoomList *chatList, char *roomName, char *username){
	return removeChatRoom(chatList,roomName, username);
}

 /**
  * Agrega un usuario a la lista de usuarios de una sala.
  * 
  * @param list Lista de salas existentes en el servidor.
  * @param chatRoom Nombre de la sala a la que el usuario quiere suscribirse.
  * @param newUser Nombre de usuario del cliente que desea suscribirse.
  * @param clientSocketFD Socket del cliente que solicitó suscribirse a la sala.
  * @return -1 si el usuario ya estaba en la sala, 0 si el usuario fue
  *         correctamente agregado, -2 si no pudo agregarse el usuario.
  * 
  */
int subscribeUser(chatRoomList *list, char *chatRoom, char * newUser, int clientSocketFD){
	return addUserToCRList(list,chatRoom,newUser,clientSocketFD);	
}

 /**
  * Desuscribe a un usuario de todas las salas de chat.
  * 
  * @param list Lista de salas existentes en el servidor.
  * @param username Nombre de usuario del cliente que desea desuscribirse.
  * @param clientSocketFD Socket del cliente que solicitó desuscribirse.
  * 
  */
void unsubscribeUser(chatRoomList *list, char * username,int clientSocketFD){
	removeUserFromCRList(list,username);
}

 /**
  * Para cada sala existente en el servidor, si el cliente está suscrito,
  * se envía el mensaje a todos los usuarios presentes, incluido él mismo.
  * 
  * @param list Lista de salas existentes en el servidor.
  * @param username Nombre de usuario del cliente que envía el mensaje
  * @param message Mensaje que está enviando el usuario
  * 
  */
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

 /**
  * Envío de un mensaje a los usuarios pertenecientes a una lista.
  * 
  * @param userList Usuarios a los que se enviará el mensaje.
  * @param message Mensaje que se envía.
  * 
  */
void sendMessageToUsers(userList *destinations, char *message){
	userBox *act;
	act = destinations->head;
	while(act != NULL){
		pthread_mutex_lock(&act->userMutex);
		if (act->clientSocketFD != -1) sendMessagesToClient(act->clientSocketFD, message);
		pthread_mutex_unlock(&act->userMutex);
		act = act -> sig;
	}
	
}
