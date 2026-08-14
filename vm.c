#include "vm.h"
#include "value.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void chunk_init(Chunk *chunk) {
  chunk->count = 0;
  chunk->capacity = 16;
  chunk->code = malloc(sizeof(uint8_t) * chunk->capacity);
  if (!chunk->code) {
    fprintf(stderr, "Error: Cannot allocate memory for chunk");
    exit(1);
  }
  ValueArray valueArray;
  valuearray_init(&valueArray);
  chunk->constants = valueArray;
}

void chunk_free(Chunk *chunk) {
  valuearray_free(&chunk->constants);
  free(chunk->code);
  chunk->count = 0;
  chunk->capacity = 0;
}

void chunk_write(Chunk *chunk, uint8_t byte) {
  if (chunk->count >= chunk->capacity) {
    chunk->capacity *= 2;
    chunk->code = realloc(chunk->code, sizeof(uint8_t) * chunk->capacity);
  }
  chunk->code[chunk->count++] = byte;
}

size_t chunk_constant_add(Chunk *chunk, Value value) {
  size_t idx = chunk->constants.count;
  valuearray_write(&chunk->constants, value);
  return idx;
}

void chunk_instruction_disassemble(uint8_t *code, int *offset) {
  uint8_t op = code[*offset];
  switch (op) {
  case OP_RETURN: {
    printf("RETURN\n");
    *offset = *offset + 1;
    break;
  }
  case OP_CONSTANT: {
    uint8_t constant = code[*offset + 1];
    printf("CONSTANT: %c\n", constant);
    *offset += 2;
    break;
  }
  default: {
    printf("UNKNOWN OP: %c\n", op);
  }
  }
}

/**
 * Convert bytecode inside a chunk into human-readable output
 */
void chunk_bytecode_disassemble(Chunk *chunk) {
  int offset = 0;
  while (offset < chunk->count) {
    chunk_instruction_disassemble(chunk->code, &offset);
  }
}

void vm_init(VM *vm) {
  vm->stackTop = vm->stack;
  vm->chunkCount = 0;
}

void vm_interpret(VM *vm, Chunk *chunk) {
  vm->chunk = chunk;
  vm->ip = chunk->code;
  vm_run(vm);
}

void vm_push(VM *vm, Value value) {
  *vm->stackTop = value;
  vm->stackTop++;
}

Value vm_pop(VM *vm) {
  if (vm->stackTop - vm->stack <= 0) {
    fprintf(stderr, "Runtime Error: VM: pop failed\n");
    exit(1);
  }
  return *--vm->stackTop;
}

void vm_run(VM *vm) {
  for (;;) {
    uint8_t instruction = *vm->ip++;
    switch (instruction) {
    case OP_CONSTANT: {
      // Read the constant index from bytecode
      uint8_t idx = *vm->ip++;
      if (idx >= vm->chunk->constants.count) {
        fprintf(stderr, "Runtime Error: VM: constants overflow.\n");
      }
      Value value = *(vm->chunk->constants.values + idx);
      vm_push(vm, value);
      break;
    }
    case OP_ADD: {
      Value b = vm_pop(vm);
      Value a = vm_pop(vm);
      vm_push(vm, value_number(a.number + b.number));
      break;
    }
    case OP_SUB: {
      Value b = vm_pop(vm);
      Value a = vm_pop(vm);
      vm_push(vm, value_number(a.number - b.number));
      break;
    }
    case OP_DIV: {
      Value b = vm_pop(vm);
      Value a = vm_pop(vm);
      vm_push(vm, value_number(a.number / b.number));
      break;
    }
    case OP_MUL: {
      Value b = vm_pop(vm);
      Value a = vm_pop(vm);
      vm_push(vm, value_number(a.number * b.number));
      break;
    }
    case OP_NAGTIVE: {
      Value a = vm_pop(vm);
      vm_push(vm, value_number(-a.number));
      break;
    }
    case OP_RETURN: {
      Value pop = vm_pop(vm);
      printf("VM: POP: %f\n", pop.number);
      return;
    }
    }
  }
}
