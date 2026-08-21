CC = clang
CFLAGS = -Wall -Wextra -std=c17
TARGET = gem

SRCS = main.c env.c ast.c lexer.c parser.c vm.c value.c compiler.c
OBJS = $(SRCS:.c=.o)
HEADERS = value.h env.h token.h ast.h lexer.h parser.h vm.h

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
