#include "ast.h"
#include <stdlib.h>
#include <string.h>

ASTNode *astnode_create_num(int val) {
  ASTNode *node = malloc(sizeof(ASTNode));
  node->type = AST_NUMBER;
  node->number_val = val;
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
 * @brief Build AST node represents `name = expression`
 *
 * @param var_name
 * @param expr
 * @return ASTNode*
 */
ASTNode *astnode_create_assignment(const char *var_name, ASTNode *expr) {
  ASTNode *node = malloc(sizeof(ASTNode));
  node->type = AST_ASSIGNMENT;
  node->assignment.expr = expr;
  strncpy(node->assignment.name, var_name, 64);
  return node;
}

ASTNode *astnode_create_identifier(const char *id_name) {
  ASTNode *node = malloc(sizeof(ASTNode));
  node->type = AST_IDENTIFIER;
  strncpy(node->identifier_name, id_name, 64);
  return node;
}

void astnode_free(ASTNode *node) {
  if (!node) {
    return;
  }
  if (node->type == AST_BINARY_OP) {
    astnode_free(node->binary_op.left);
    astnode_free(node->binary_op.right);
  }
  free(node);
}
