/**
 * serverCommands.h
 *
 * Septiembre - Diciembre 2013
 *
 * Encabezado del manejo de los comandos que son enviados al servidor.
 *
 * @author Andrea Balbás        09-10076
 * @author Gustavo El Khoury    10-10226
 */
#define lta
#include "lista.h"

 /**
  * Lista todos los usuarios conectados al servidor de chat.
  * 
  * @param globalUserList Lista de usuarios del servidor.
  * @param clientSocketFD Socket del usuario que envió el comando.
  * @return String con los nombres de los usuarios conectados, incluido
  *         el que solicitó que se listaran los usuarios.
  * 
  */
char *listUsers(userList *globalUserList, int clientSocketFD);

 /**
  * Lista todas las salas existentes en el servidor
  * 
  * @param chatRoomList Lista de salas del servidor
  * @param clientSocketFD Socket del usuario que envió el comando.
  * @return String con los nombres de las salas existentes en el servidor
  * 
  */
char *listChatrooms(chatRoomList *chatList, int clientSocketFD);

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
int createChatroom(chatRoomList *chatList, char *roomName, char *owner, int ownerSocketFD);

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
int deleteChatroom(chatRoomList *chatList, char *roomName, char *username);

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
int subscribeUser(chatRoomList *list, char *chatRoom, char * newUser, int clientSocketFD);

 /**
  * Desuscribe a un usuario de todas las salas de chat.
  * 
  * @param list Lista de salas existentes en el servidor.
  * @param username Nombre de usuario del cliente que desea desuscribirse.
  * @param clientSocketFD Socket del cliente que solicitó desuscribirse.
  * 
  */
void unsubscribeUser(chatRoomList *list, char * username, int clientSocketFD);

 /**
  * Para cada sala existente en el servidor, si el cliente está suscrito,
  * se envía el mensaje a todos los usuarios presentes, incluido él mismo.
  * 
  * @param list Lista de salas existentes en el servidor.
  * @param username Nombre de usuario del cliente que envía el mensaje
  * @param message Mensaje que está enviando el usuario
  * 
  */
void sendMessageToChatRooms(chatRoomList *list, char * username, char *message);

 /**
  * Envío de un mensaje a los usuarios pertenecientes a una lista.
  * 
  * @param userList Usuarios a los que se enviará el mensaje.
  * @param message Mensaje que se envía.
  * 
  */
void sendMessageToUsers(userList *destinations, char *message);
// sendMessage