CC = clang
CFLAGS = -Wall -Wextra -std=c17 -Isrc -MMD -MP
TARGET = gem

BUILD_DIR = build
SRC_DIR = src

# Automatically find all .c files in src/
SRCS = $(wildcard $(SRC_DIR)/*.c)
# Map src/foo.c -> build/foo.o
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))
# Track generated .d dependency files
DEPS = $(OBJS:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Pattern rule: compile src/%.c to build/%.o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Include auto-generated header dependency rules
-include $(DEPS)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean
