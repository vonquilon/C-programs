COMPILER = gcc

all: cacheblock

cacheblock: cacheblock.o
	$(COMPILER) -o cacheblock cacheblock.o -lm

cacheblock.o: cacheblock.c
	$(COMPILER) -c -Os cacheblock.c

all:cachesize

cachesize: cachesize.o
	$(COMPILER) -o cachesize cachesize.o -lm

cachesize.o: cachesize.c
	$(COMPILER) -c cachesize.c

clean:
	rm -f cacheblock cachesize *.o
