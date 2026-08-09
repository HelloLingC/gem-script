CC = clang
CFLAGS = -Wall -Wextra -std=c17
TARGET = compiler

SRCS = main.c env.c ast.c lexer.c parser.c evaluator.c
OBJS = $(SRCS:.c=.o)
HEADERS = value.h env.h token.h ast.h lexer.h parser.h evaluator.h

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
