#include "compiler.h"
#include "ast.h"
#include "token.h"
#include "value.h"
#include <stdlib.h>

// recursively walk the AST
void compile(ASTNode *node, Chunk *chunk) {
  if (!node) {
    exit(1);
  }

  if (node->type == AST_NUMBER) {
    size_t constIdx = chunk_constant_add(chunk, value_number(node->number_val));
    chunk_write(chunk, OP_CONSTANT);
    chunk_write(chunk, constIdx);
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
    }
    chunk_write(chunk, op);
  }

  if (node->type == AST_UNARY_OP) {
    compile(node->unary_op.operand, chunk);
    chunk_write(chunk, OP_NAGTIVE);
  }
}
