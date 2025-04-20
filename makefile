CC=cc
CCFLAGS=-Wall -Wextra 
search.o:search.c
	$(CC) $(CCFLAGS) -g -c -O $< -o $@
main:main.c search.o
	$(CC) $(CCFLAGS) -g -O $^ -o $@

