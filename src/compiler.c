#include "compiler.h"
#include "ast.h"
#include "token.h"
#include "value.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// emit jump instruction with two placeholder bytes into a chunk
// returns the offset of the placeholder
int emitJump(Chunk *chunk, uint8_t instruction) {
  chunk_write(chunk, instruction);
  int jumpOffset = chunk->count;
  chunk_write(chunk, 0xff);
  chunk_write(chunk, 0xff);
  return jumpOffset;
}

void patchJump(Chunk *chunk, int offset) {
  int distance = chunk->count - offset - 2;
  chunk->code[offset] = (distance >> 8) & 0xff;
  chunk->code[offset + 1] = distance & 0xff;
}

// recursively walk the AST
void compile(ASTNode *node, Chunk *chunk) {
  if (!node) {
    exit(1);
  }

  if (node->type == AST_VAR_DECL) {
    compile(node->assignment.expr, chunk);
    size_t nameIdx =
        chunk_constant_add(chunk, value_string(node->assignment.name));
    chunk_write(chunk, OP_DEFINE_GLOBAL);
    chunk_write(chunk, nameIdx);
  }

  if (node->type == AST_ASSIGNMENT) {
    compile(node->assignment.expr, chunk);
    size_t nameIdx =
        chunk_constant_add(chunk, value_string(node->assignment.name));
    chunk_write(chunk, OP_SET_GLOBAL);
    chunk_write(chunk, nameIdx);
  }

  if (node->type == AST_IDENTIFIER) {
    size_t nameIdx =
        chunk_constant_add(chunk, value_string(node->identifier_name));
    chunk_write(chunk, OP_GET_GLOBAL);
    chunk_write(chunk, nameIdx);
  }

  if (node->type == AST_NUMBER) {
    size_t constIdx = chunk_constant_add(chunk, value_number(node->number_val));
    chunk_write(chunk, OP_CONSTANT);
    chunk_write(chunk, constIdx);
  }

  if (node->type == AST_BLOCK) {
    for (int i = 0; i < node->block.count; i++) {
      compile(node->block.stmts[i], chunk);
    }
  }

  if (node->type == AST_IF) {
    // Push the condition result into the stack first
    compile(node->if_stmt.condition, chunk);
    // Fill jump offset with placeholders
    int thenJumpOffset = emitJump(chunk, OP_JUMP_IF_FALSE);
    compile(node->if_stmt.then_branch, chunk);

    if (node->if_stmt.else_branch) {
      int JumpOffset = emitJump(chunk, OP_JUMP);
      // backpatch thenJumpOffset after OP_JUMP
      patchJump(chunk, thenJumpOffset);
      compile(node->if_stmt.else_branch, chunk);
      patchJump(chunk, JumpOffset);
    } else {
      patchJump(chunk, thenJumpOffset);
    }
  }

  if (node->type == AST_WHILE) {
    int loopStart = chunk->count;
    compile(node->while_loop.condition, chunk);
    int offset = emitJump(chunk, OP_JUMP_IF_FALSE);
    compile(node->while_loop.body, chunk);

    // emit OP_LOOP with 16bits jumping backward to loopStart
    int loopOffset = emitJump(chunk, OP_LOOP);
    int distance = chunk->count - loopStart;
    chunk->code[loopOffset] = (distance >> 8) & 0xff;
    chunk->code[loopOffset + 1] = distance & 0xff;

    patchJump(chunk, offset);
  }

  if (node->type == AST_BINARY_OP) {
    compile(node->binary_op.left, chunk);
    compile(node->binary_op.right, chunk);
    uint8_t op;
    switch (node->binary_op.op) {
    case TOKEN_PLUS: {
      op = OP_ADD;
      break;
    }
    case TOKEN_MINUS: {
      op = OP_SUB;
      break;
    }
    case TOKEN_STAR: {
      op = OP_MUL;
      break;
    }
    case TOKEN_SLASH: {
      op = OP_DIV;
      break;
    }
    case TOKEN_GREATER_THAN: {
      op = OP_GREATER;
      break;
    }
    case TOKEN_EQUAL: {
      op = OP_EQUAL;
      break;
    }
    case TOKEN_LESS_THAN: {
      op = OP_LESS;
      break;
    }
    default: {
      fprintf(stderr, "Runtime Error: VM: Unrecognized token.");
      exit(1);
    }
    }
    chunk_write(chunk, op);
  }

  if (node->type == AST_UNARY_OP) {
    compile(node->unary_op.operand, chunk);
    chunk_write(chunk, OP_NAGTIVE);
  }
}
