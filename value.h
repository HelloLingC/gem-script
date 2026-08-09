#ifndef VALUE_H
#define VALUE_H

#include <stdbool.h>

typedef enum {
  VAL_NIL,
  VAL_NUMBER,
  VAL_BOOL
} ValueType;

typedef struct {
  ValueType type;
  union {
    double number;
    bool boolean;
  };
} Value;

Value value_number(double number);

#endif // VALUE_H
