# gcc is the compiler
CC=gcc
# MySQL flags
SQLFLAGS=-I/usr/include/mysql -DBIG_JOINS=1 -fno-strict-aliasing -g
SQLLIBS=-L/usr/lib/arm-linux-gnueabihf -lmysqlclient -lpthread -lz -lm -lrt -ldl
# I/O flags
BCM2835=-l bcm2835
# Compiler flags
# -c only creates an object file
CFLAGS=-c

all: run

tempget.o: tempget.h dbmsg.o
	$(CC) $(CFLAGS) $(SQLFLAGS) tempget.h $(SQLLIBS)

dbmsg.o: dbmsg.h
	$(CC) $(CFLAGS) $(SQLFLAGS) dbmsg.h $(SQLLIBS)

run: control.c tempget.o
	$(CC) -o run $(SQLFLAGS) control.c dbmsg.h tempget.h $(SQLLIBS) $(BCM2835)

clean:
	$(RM) run *.o *~