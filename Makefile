
# Makefile 
#Andrea Balbás        09-10076
#Gustavo El Khoury    10-10226
#
#Septiembre - Diciembre 2013\

CC = gcc
CFLAGS = -g -L /usr/lib/happycoders
LIBS = -lnsl


all : sclient cclient 

errors.o : errors.c errors.h
	$(CC) $(CFLAGS) -c errors.c

socketManagement.o : errors.o socketManagement.c socketManagement.h
	$(CC) $(CFLAGS) -c socketManagement.c errors.o

sclient : schat.c socketManagement.o errors.o 
	$(CC) $(CFLAGS) -o schat schat.c errors.o socketManagement.o $(LIBS)

cclient : cchat.c errors.o
	$(CC) $(CFLAGS) -o cchat cchat.c errors.o socketManagement.o $(LIBS)

clean:
	rm *.o schat cchat