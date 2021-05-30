all: part01 part01-sync
part01: part01.c
	gcc -pthread -o part01 part01.c
part01-sync: part02.c	
	gcc -pthread -Wall -D PTHREAD_SYNC -o part01-sync part02.c

clean:
	-rm -f *~ *.o *.dep part01 part01-sync
