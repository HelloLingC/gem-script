#ifndef VALUE_H
#define VALUE_H

#include <stdbool.h>

typedef enum {
  VAL_NIL,
  VAL_NUMBER,
  VAL_BOOL,
  VAL_STRING,
} ValueType;

// Value should be only used in runtime
// not compile-time(AST...)
typedef struct {
  ValueType type;
  union {
    double number;
    bool boolean;
    char *string;
  };
} Value;

typedef struct {
  Value *values;
  int count;
  int capacity;
} ValueArray;

void valuearray_init(ValueArray *va);
void valuearray_write(ValueArray *va, Value value);
void valuearray_free(ValueArray *va);

Value value_number(double number);
Value value_bool(bool boolean);
Value value_string(char *string);

#endif // VALUE_H
