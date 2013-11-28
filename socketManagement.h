/****************************
 socketManagement.h

 Módulo con operaciones sobre
 los sockets, para escribir
 y leer datos
 
 Andrea Balbás        09-10076
 Gustavo El Khoury    10-10226     
 
 Septiembre - Diciembre 2013
 ****************************/

/**
  * Estructura que almacena los datos de un comando
  * 
  * command -> Nombre del comando
  * argument -> Argumento del comando
  */
typedef struct commandPacket{
  char command[4];
  char *argument;
} commandPacket;

 /**
  * Protocolo de comunicación para que el servidor reciba del cliente
  * su nombre de usuario
  * 
  * @param socketFD Socket del cliente cuyo nombre de usuario se recibirá.
  * @param username Nombre de usuario del cliente.
  * @return 0 si se recibió correctamente el nombre de usuario.
  * 
  */
int receiveHello(int socketFD, char **username);

 /**
  * Protocolo de comunicación para que el cliente le envíe al servidor su 
  * nombre de usuario
  * 
  * @param socketFD Socket del servidor.
  * @param username Nombre de usuario del cliente.
  * @return 0 si se envió correctamente el nombre de usuario.
  * 
  */
int sayHello(int socketFD, char *username);

/** Las siguiente dos funciones corresponden al protocolo de comunicación
 * destinado a que el cliente envíe comandos al servidor y éste los reciba.
 * Los pasos del protocolo son los siguientes:
 *   El cliente se conecta y envía el comando
 *   El servidor lee el comando, y le avisa al cliente
 *   El cliente envía la longitud del parámetro que sigue
 *   El servidor recibe la señal, y le responde al cliente
 *   El cliente envía el parámetro.
 */

/**
  * Protocolo de comunicación para que el servidor reciba comandos del cliente.
  *
  * @param socketFD Socket del cliente que envía el comando.
  * @param receivedCommand Comando que se recbió del cliente.
  * @return 0 si se recibió correctamente el comando.
  * 
  */
int readCommandFromSocket(int socketFD, commandPacket *receivedCommand);

 /**
  * Protocolo de comunicación para que el cliente envíe comandos al servidor.
  * 
  * @param socketFD Socket del cliente que envía el comando.
  * @param newCommand Comando que se envía al servidor.
  * @return 0 si se envió correctamente el nombre de usuario.
  * 
  */
int sendCommandToSocket(int socketFD, commandPacket newCommand);

/** Las siguiente dos funciones corresponden al protocolo de comunicación
 * destinado a que el cliente reciba las respuestas enviadas por el servidor.
 * Los pasos del protocolo son:
 *    El servidor envíia la longitud del string de respuesa, y el cliente acepta
 *    El servidor recibe la señal, y envia el string. El cliente confirma que
 *    recibió la respuesta. 
 */

 /**
  * Protocolo de comunicación para que el cliente lea la respuesta enviada
  * por el servidor.
  * 
  * @param socketFD Socket del cliente que recibirá la respuesta.
  * @return String con la respuesta enviada por el cliente.
  *         NULL en caso de que no haya respuesta.
  * 
  */
char *readResponseFromServer(int socketFD);

 /**
  * Protocolo de comunicación para que el servidor envíe una respuesta
  * a un cliente.
  * 
  * @param socketFD Socket del cliente al que se le envía la respuesta.
  * @param answer Respuesta que se le envía al cliente.
  * @return 0 si se envió correctamente la respuesta.
  * 
  */
int sendResponseToClient(int socketFD, char *answer);

 /**
  * Protocolo de comunicación para que el cliente reciba los mensajes
  * de las salas a las que está suscrito.
  * 
  * @param socketFD Socket que se leerá para obtener el mensaje recibido.
  * @return String con el mensaje recibido.
  * 
  */
char *fetchMessagesFromServer(int socketFD);

 /**
  * Protocolo de comunicación para que el servidor envíe un mensaje
  * a un cliente.
  * 
  * @param socketFD Socket del cliente al que se enviará el mensaje.
  * @param answer Mensaje que se enviará al cliente.
  * @return 0 si el mensaje fue enviado correctamente.
  * 
  */
int sendMessagesToClient(int socketFD, char *answer);