#include "lista.h"

int main(){
	listaUsuarios blah;
	initialize(&blah);
	addUser(&blah,"Jajaj");
	addUser(&blah,"Jajaj");
	addUser(&blah,"Jajaj");
	addUser(&blah,"Jajaj");
	printLista(blah);
	return 0;	
}