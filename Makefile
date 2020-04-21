main: main.o scheduler.o process.o
	gcc -Wall -std=c99 main.o scheduler.o process.o -o main

main.o: main.c
	gcc -Wall -std=c99 main.c -c

scheduler.o: scheduler.c
	gcc -Wall -std=c99 scheduler.c -c

process.o: process.c
	gcc -Wall -std=c99 process.c -c

clean:
	rm -rf *o

run:
	sudo ./main