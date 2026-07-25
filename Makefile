CC = clang
CFLAGS = -Wall -Wextra -std=c17
TARGET=compiler


OBJS = main.c
HEADERS = ast.h 

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^


# Rebuild if header files changes
%.o: %.c $(HEADERS) 
	$(CC) $(CFLAGS) -c $<-o $@

