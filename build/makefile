# makefile for my design.  --kylelee, 2014.3.9
CC=gcc
CD=cd
CFLAGS=-W -Wall -g
#three dirs.
VPATH = ../headers:../sources
TFLAG=-pthread
SERVER=server.bin
#HEADDIR=./headers
#BUILDDIR=./build
#SRCDIR=./sources
OBJ=queue.o server.o main.o
#SRC=/src
SERVER: $(OBJ)
	$(CC) $(CFLAGS) $(TFLAG)  $(OBJ) -o $(SERVER)
$(OBJ): %.o:%.c
	$(CC) -c $(CFLAGS) $< -o $@
.PHONY: clean
clean:
	rm -rf $(OBJ) $(SERVER)