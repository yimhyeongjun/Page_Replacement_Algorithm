a.out : main.o queue.o stack.o optimal.o fifo.o lifo.o lru.o lfu.o sc.o esc.o stream.o
	gcc -o a.out main.o queue.o stack.o optimal.o fifo.o lifo.o lru.o lfu.o sc.o esc.o stream.o

main.o : userlib.h main.c
	gcc -c -o main.o main.c 

queue.o : userlib.h queue.c
	gcc -c -o queue.o queue.c

stack.o : userlib.h stack.c
	gcc -c -o stack.o stack.c

optimal.o : userlib.h optimal.c
	gcc -c -o optimal.o optimal.c

fifo.o : userlib.h fifo.c
	gcc -c -o fifo.o fifo.c

lifo.o : userlib.h lifo.c
	gcc -c -o lifo.o lifo.c

lru.o : userlib.h lru.c
	gcc -c -o lru.o lru.c

lfu.o : userlib.h lfu.c
	gcc -c -o lfu.o lfu.c

sc.o : userlib.h sc.c
	gcc -c -o sc.o sc.c

esc.o : userlib.h esc.c
	gcc -c -o esc.o esc.c

stream.o : userlib.h stream.c
	gcc -c -o stream.o stream.c

clean :
	rm *.o

