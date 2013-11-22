
# Makefile 
#Andrea Balbás        09-10076
#Gustavo El Khoury    10-10226
#
#Septiembre - Diciembre 2013\

CC = gcc
CFLAGS = -g -L /usr/lib/happycoders
LIBS = -lnsl


all : sclient cclient 

lista.o: lista.c lista.h
	$(CC) $(CFLAGS) -c lista.c

errors.o : errors.c errors.h
	$(CC) $(CFLAGS) -c errors.c

socketManagement.o : errors.o socketManagement.c socketManagement.h
	$(CC) $(CFLAGS) -c socketManagement.c errors.o

serverCommands.o: serverCommands.c serverCommands.h lista.o
	$(CC) $(CFLAGS) -c serverCommands.c lista.o

sclient : schat.c socketManagement.o errors.o serverCommands.o lista.o
	$(CC) $(CFLAGS) -o schat -pthread schat.c errors.o socketManagement.o serverCommands.o lista.o $(LIBS)

cclient : cchat.c errors.o
	$(CC) $(CFLAGS) -o cchat cchat.c errors.o socketManagement.o $(LIBS)

clean:
	rm *.o schat cchat