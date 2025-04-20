CC=cc
CCFLAGS=-Wall -Wextra 
search.o:search.c
	$(CC) $(CCFLAGS) -g -c -O $< -o $@
backup_system.o:backup_system.c
	$(CC) $(CCFLAGS) -g -c -O $< -o $@	
main:main.c search.o backup_system.c
	$(CC) $(CCFLAGS) -g -O $^ -o $@

