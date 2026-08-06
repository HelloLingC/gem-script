#include "ast.h"
#include <stdlib.h>

ASTNode *astnode_create_unary(TokenType op, ASTNode *operand) {}

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
