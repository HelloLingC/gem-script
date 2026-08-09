#include "env.h"
#include <string.h>

Value value_number(double number) {
  Value value = {.type = VAL_NUMBER, .number = number};
  return value;
}

void env_set(Environment *env, char *name, Value val) {
  Symbol symbol = {.val = val};
  strncpy(symbol.name, name, 64);
  symbol.name[63] = '\0';
  env->symbols[env->count++] = symbol;
}

bool env_get(Environment *env, char name[64], Value *ret_val) {
  for (int i = 0; i < env->count; i++) {
    if (strcmp(env->symbols[i].name, name) == 0) {
      ret_val->type = env->symbols[i].val.type;
      ret_val->number = env->symbols[i].val.number;
      ret_val->boolean = env->symbols[i].val.boolean;
      return true;
    }
  }
  return false;
}
