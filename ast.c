#include "ast.h"
#include <stdlib.h>
#include <string.h>

ASTNode *astnode_create_num(int val) {
  ASTNode *node = malloc(sizeof(ASTNode));
  node->type = AST_NUMBER;
  node->number_val = val;
  return node;
}

ASTNode *astnode_create_string(char *string) {
  ASTNode *node = malloc(sizeof(ASTNode));
  node->type = AST_STRING;
  node->string = string;
  return node;
}

ASTNode *astnode_create_unaryop(TokenType op, ASTNode *operand) {
  ASTNode *node = malloc(sizeof(ASTNode));
  node->type = AST_UNARY_OP;
  node->unary_op.op = op;
  node->unary_op.operand = operand;
  return node;
}

ASTNode *astnode_create_binop(TokenType op, ASTNode *left, ASTNode *right) {
  ASTNode *node = malloc(sizeof(ASTNode));
  node->type = AST_BINARY_OP;
  node->binary_op.op = op;
  node->binary_op.left = left;
  node->binary_op.right = right;
  return node;
}

/**
 * @brief Build AST node representing `name = expression`
 */
ASTNode *astnode_create_assignment(const char *var_name, ASTNode *expr) {
  ASTNode *node = malloc(sizeof(ASTNode));
  node->type = AST_ASSIGNMENT;
  node->assignment.expr = expr;
  strncpy(node->assignment.name, var_name, 64);
  node->assignment.name[63] = '\0';
  return node;
}

ASTNode *astnode_create_identifier(const char *id_name) {
  ASTNode *node = malloc(sizeof(ASTNode));
  node->type = AST_IDENTIFIER;
  strncpy(node->identifier_name, id_name, 64);
  node->identifier_name[63] = '\0';
  return node;
}

ASTNode *astnode_create_block(ASTNode **stmts, int count) {
  ASTNode *node = malloc(sizeof(ASTNode));
  node->type = AST_BLOCK;
  node->block.stmts = stmts;
  node->block.count = count;
  return node;
}

ASTNode *astnode_create_if(ASTNode *condition, ASTNode *then_branch,
                           ASTNode *else_branch) {
  ASTNode *node = malloc(sizeof(ASTNode));
  node->type = AST_IF;
  node->if_stmt.condition = condition;
  node->if_stmt.then_branch = then_branch;
  node->if_stmt.else_branch = else_branch;
  return node;
}

ASTNode *astnode_create_while(ASTNode *condition, ASTNode *body) {
  ASTNode *node = malloc(sizeof(ASTNode));
  node->type = AST_WHILE;
  node->while_loop.condition = condition;
  node->while_loop.body = body;
  return node;
}

ASTNode *astnode_create_function_decl(char *func_name, FuncParam params[16],
                                      int param_count, struct ASTNode *body) {
  ASTNode *node = malloc(sizeof(ASTNode));
  node->type = AST_FUNCTION_DECL;
  node->function_decl.func_name = func_name;
  memcpy(node->function_decl.params, params, 16);
  node->function_decl.params_count = param_count;
  node->function_decl.body = body;
  return node;
}

void astnode_free(ASTNode *node) {
  if (!node) {
    return;
  }
  if (node->type == AST_BINARY_OP) {
    astnode_free(node->binary_op.left);
    astnode_free(node->binary_op.right);
  } else if (node->type == AST_UNARY_OP) {
    astnode_free(node->unary_op.operand);
  } else if (node->type == AST_ASSIGNMENT) {
    astnode_free(node->assignment.expr);
  } else if (node->type == AST_BLOCK) {
    for (int i = 0; i < node->block.count; i++) {
      astnode_free(node->block.stmts[i]);
    }
    free(node->block.stmts);
  } else if (node->type == AST_IF) {
    astnode_free(node->if_stmt.condition);
    astnode_free(node->if_stmt.then_branch);
    if (node->if_stmt.else_branch) {
      astnode_free(node->if_stmt.else_branch);
    }
  } else if (node->type == AST_WHILE) {
    astnode_free(node->while_loop.condition);
    astnode_free(node->while_loop.body);
  }
  free(node);
}
