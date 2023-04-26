all: project3

project3: main.o linkedlist.o
	gcc main.o linkedlist.o -o project3 -ggdb

main.o: main.c linkedlist.h
	gcc -c main.c -ggdb

linkedlist.o: linkedlist.c linkedlist.h
	gcc -c linkedlist.c -ggdb

.PHONY: clean
clean:
	rm *.o
	rm project3

.PHONY: project
project: project3
	@echo "First test"
	./test_list 1 2 3 4 5 6 7 8 9 10
	@echo "second test"
	./test_list 10 53 24 1 15 89 63 49 78
