#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "serverCommands.h"



int listUsers(userList *globalUserList, int clientSocketFD);

int listChatrooms(chatRoomList *chatList, int clientSocketFD);

int createChatroom(userList *globalUserList, chatRoomList *chatList, char *owner, char *roomName);

int deleteChatroom(chatRoomList *chatList, char *roomName, char *username);