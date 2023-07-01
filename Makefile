CFLAGS = -Wall -g

objs = mines.o pilha.o

all: mine

mine: $(objs)
		gcc $(objs) -o mine 
	
mines.o:	mines.c 
	cc -c mines.c $(CFLAGS)
pilha.o: pilha.c 
	cc -c pilha.c $(CFLAGS)
			
clean:
	-rm -f *.o *~ 
	
purge: clean
	-rm -f main