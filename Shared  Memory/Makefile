CC     = gcc
CFLAGS = -Wall -Wextra -O3

all: Manager palindrome

Manager: Manager.c
	$(CC) $(CFLAGS) -o Manager Manager.c

palindrome: Palindrome.c
	$(CC) $(CFLAGS) -o Palindrome Palindrome.c

clean:
	rm -f Manager Palindrome *.o
