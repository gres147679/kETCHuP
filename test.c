#include "lista.h"

int main(){
	listaUsuarios blah;
	initialize(&blah);
	addUser(&blah,"Jajaj1");
	addUser(&blah,"Jajaj2");
	addUser(&blah,"Jajaj3");
	addUser(&blah,"Jajaj4");
	printLista(blah);
	return 0;	
}