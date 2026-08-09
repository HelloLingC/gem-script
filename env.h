#ifndef ENV_H
#define ENV_H

#include "value.h"
#include <stdbool.h>

typedef struct {
  char name[64];
  Value val;
} Symbol;

typedef struct Environment {
  Symbol symbols[100];
  int count;
  struct Environment *pareantEnv; // For nested variable scope
} Environment;

void env_set(Environment *env, char *name, Value val);
bool env_get(Environment *env, char name[64], Value *ret_val);

#endif // ENV_H
