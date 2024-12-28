CC = gcc
CFLAGS = -Wall -Wextra -O2

objects = src/lexer.o src/main.o src/parser.o
headers = src/lexer.h src/token.h src/parser.h

all: program

program: $(objects)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c $(headers)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(objects) program
