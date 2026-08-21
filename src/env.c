#include "env.h"
#include <string.h>

Value value_number(double number) {
  Value value = {.type = VAL_NUMBER, .number = number};
  return value;
}

Value value_bool(bool boolean) {
  Value value = {.type = VAL_BOOL, .boolean = boolean};
  return value;
}

Value value_string(char *string) {
  Value value = {.type = VAL_STRING, .string = string};
  return value;
}

// Declaration: Always adds a new variable to the current scope
void env_define(Environment *env, char *name, Value val) {
  Symbol symbol = {.val = val};
  strncpy(symbol.name, name, 64);
  symbol.name[63] = '\0';
  env->symbols[env->count++] = symbol;
}

// Assignment: Walks the scope chain looking for an existing variable
//    Returns true if found & updated, false if undefined
bool env_assign(Environment *env, char *name, Value val) {
  for (int i = 0; i < env->count; i++) {
    if (strcmp(env->symbols[i].name, name) == 0) {
      if (env->symbols[i].val.type != val.type) {
        // Todo:
      }
      env->symbols[i].val = val;
      return true;
    }
  }
  return false;
}

// Walks the scope to read a variable
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
