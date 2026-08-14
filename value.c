#include "value.h"
#include <stdio.h>
#include <stdlib.h>

#define VALUEARRAY_DEFAULT_CAPACITY 16

void valuearray_init(ValueArray *va) {
  va->capacity = VALUEARRAY_DEFAULT_CAPACITY;
  va->count = 0;
  va->values = malloc(sizeof(Value) * va->capacity);
  if (!va->values) {
    fprintf(stderr, "Error: Cannot allocate memory for chunk");
    exit(1);
  }
}
void valuearray_write(ValueArray *va, Value value) {
  if (va->count >= va->capacity) {
    va->capacity = va->capacity * 2;
    va->values = realloc(va->values, sizeof(value) * va->capacity);
  }
  va->values[va->count++] = value;
}

void valuearray_free(ValueArray *va) {
  free(va->values);
  va->values = NULL;
  va->count = 0;
  va->capacity = VALUEARRAY_DEFAULT_CAPACITY;
}