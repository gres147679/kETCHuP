#define lta
#include "lista.h"

char *listUsers(userList *globalUserList, int clientSocketFD);

int listChatrooms(chatRoomList *chatList, int clientSocketFD);

int createChatroom(userList *globalUserList, chatRoomList *chatList, char *owner, char *roomName);

int deleteChatroom(chatRoomList *chatList, char *roomName, char *username);
// sendMessage
//         -subscribeUser
//         -desubscribeUser