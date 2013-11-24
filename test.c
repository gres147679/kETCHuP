#include "lista.h"

int main(){
    userList blah;
    initialize(&blah);
    addUser(&blah,"Andrea",0);
    
    // chatRooms *sala1;
    // sala1 = (chatRooms *) malloc(sizeof(chatRooms));
    // sala1->chatRoomName = "sala1";
    // sala1->users = blah;
    // sala1->next = NULL;
    
    // chatRooms *sala2;
    // sala2 = (chatRooms *) malloc(sizeof(chatRooms));
    // sala2->chatRoomName = "sala2";
    // sala2->users = blah;
    // sala2->next = NULL;
    
    // chatRooms *sala3;
    // sala3 = (chatRooms *) malloc(sizeof(chatRooms));
    // sala3->chatRoomName = "sala3";
    // sala3->users = blah;
    // sala3->next = NULL;
    
    // chatRooms *sala4;
    // sala4 = (chatRooms *) malloc(sizeof(chatRooms));
    // sala4->chatRoomName = "sala4";
    // sala4->users = blah;
    // sala4->next = NULL;
    
    // chatRoomList list;
    // initializeCRList(&list);
    
    // addChatRoom(&list,sala1);
    // addChatRoom(&list,sala2);
    // addChatRoom(&list,sala3);
    // addChatRoom(&list,sala4);
    // printCRList(list);

    printList(blah);
     removeUser(&blah,"Andrea");
     printf("Luego de borrar 1\n");
     printList(blah);
// 
//     removeUser(&blah,"Jajaj5");
//     printf("Luego de borrar 5\n");
//     printList(blah);
// 
//     removeUser(&blah,"Jajaj3");
//     printf("Luego de borrar 3\n");
//     printList(blah);

    return 0;   
}