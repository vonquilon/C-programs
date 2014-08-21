all: sl

sl: main.o libsl.a
	gcc -o sl main.o ./libsl.a

libsl.a: sorted-list.o
	ar rcs libsl.a sorted-list.o

main.o: main.c sorted-list.h
	gcc -c -Wall main.c

sorted-list.o: sorted-list.c sorted-list.h
	gcc -c -Wall sorted-list.c

clean:
	rm -f sl
	rm -f libsl.a
	rm -f *.o
