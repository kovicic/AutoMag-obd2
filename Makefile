CC 			= gcc
CFLAGS 		= -I inc
LDFLAGS		= -pthread -ltelnet -Wl,-rpath,/usr/local/lib/libtelnet
SOURCE 		= src/*.c

all:
	$(CC) $(CFLAGS) main.c $(SOURCE) -o candemo $(LDFLAGS)
    
