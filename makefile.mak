CC=gcc
CFLAGS=-Wall -I.
DEPS=
OBJS=myshell.o
EXE=myshell:
DEBUG=-g

myshell: $(OBJS)
	$(CC) -o $(EXE) $(OBJS) $(CFLAGS)

run:
	./$(EXE)

clean:
	rm -f $(OBJS)
	rm -f $(EXE)