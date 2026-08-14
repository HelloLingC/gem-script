
#include "value.h"
#include <stddef.h>
#include <stdint.h>

typedef enum {
  OP_CONSTANT,
  OP_ADD,
  OP_SUB,
  OP_DIV,
  OP_MUL,
  OP_NAGTIVE, // for unary -
  OP_RETURN,
} Opcode;

// Chunk is the sequence of bytecode in VM
typedef struct {
  int count;
  int capacity;
  uint8_t *code; // bytecode instruction
  ValueArray constants;
} Chunk;

void chunk_init(Chunk *chunk);
void chunk_free(Chunk *chunk);
void chunk_write(Chunk *chunk, uint8_t byte);
size_t chunk_constant_add(Chunk *chunk, Value value);

#define STACK_CAPACITY 256

typedef struct {
  Chunk *chunk;
  size_t chunkCount;
  uint8_t *ip; // instruction pointer
  Value stack[STACK_CAPACITY];
  Value *stackTop; // stack pointer
} VM;

void vm_init(VM *vm);
void vm_interpret(VM *vm, Chunk *chunk);
void vm_run(VM *vm);
void vm_free(VM *cm);
