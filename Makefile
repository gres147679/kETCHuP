CC = gcc
CFLAGS = -g -L /usr/lib/happycoders
LIBS = -lsocket -lnsl


all : sclient cclient 

errors.o : errors.c errors.h
	$(CC) $(CFLAGS) -c errors.c

sockutil.o : sockutil.c sockutil.h
	$(CC) $(CFLAGS) -c sockutil.c	

sclient : schat.c sockutil.o errors.o 
	$(CC) $(CFLAGS) -o schat schat.c errors.o sockutil.o $(LIBS)

cclient : cchat.c errors.o
	$(CC) $(CFLAGS) -o cchat cchat.c errors.o $(LIBS)

