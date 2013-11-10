#include "lista.h"

int main(){
	listaUsuarios blah;
	initialize(&blah);
	addUser(&blah,"Jajaj1");
	addUser(&blah,"Jajaj2");
	addUser(&blah,"Jajaj3");
	addUser(&blah,"Jajaj4");
	addUser(&blah,"Jajaj5");
	printList(blah);

	removeUser(&blah,"Jajaj1");
	printf("Luego de borrar 1\n");
	printList(blah);

	removeUser(&blah,"Jajaj5");
	printf("Luego de borrar 5\n");
	printList(blah);

	removeUser(&blah,"Jajaj3");
	printf("Luego de borrar 3\n");
	printList(blah);
	
	return 0;	
}