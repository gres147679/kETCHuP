#define lta
#include "lista.h"

char *listUsers(userList *globalUserList, int clientSocketFD);

char *listChatrooms(chatRoomList *chatList, int clientSocketFD);

void createChatroom(chatRoomList *chatList, char *roomName, char *owner, int ownerSocketFD);

int deleteChatroom(chatRoomList *chatList, char *roomName, char *username);

void subscribeUser(chatRoomList *list, char *chatRoom, char * newUser, int clientSocketFD);

int unsubscribeUser(chatRoomList *list, char * username, int clientSocketFD);

void sendMessageToChatRooms(chatRoomList *list, char * username, char *message);

void sendMessageToUsers(userList *destinations, char *message);
// sendMessage