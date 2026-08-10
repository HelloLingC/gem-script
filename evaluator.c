#include "evaluator.h"
#include <stdio.h>
#include <stdlib.h>

Value evaluate(ASTNode *node, Environment *env) {
  if (node == NULL)
    return value_number(0);

  if (node->type == AST_ASSIGNMENT) {
    Value val = evaluate(node->assignment.expr, env);
    env_set(env, node->assignment.name, val);
    return val;
  }

  if (node->type == AST_BLOCK) {
    int count = node->block.count;
    for (int i = 0; i < count; i++) {
      ASTNode *stmt = node->block.stmts[i];
      evaluate(stmt, env);
    }
    if (count > 0) {
      return evaluate(node->block.stmts[count - 1], env);
    }
    return (Value){.type = VAL_NIL};
  }

  if (node->type == AST_IF) {
    Value cond = evaluate(node->if_stmt.condition, env);
    bool isTrue = (cond.type == VAL_BOOL && cond.boolean) ||
                  (cond.type == VAL_NUMBER && cond.number != 0);
    if (isTrue) {
      return evaluate(node->if_stmt.then_branch, env);
    } else if (node->if_stmt.else_branch != NULL) {
      return evaluate(node->if_stmt.else_branch, env);
    }
    return (Value){.type = VAL_NIL};
  }

  if (node->type == AST_WHILE) {
    Value cond = evaluate(node->while_loop.condition, env);
    bool isTrue = (cond.type == VAL_BOOL && cond.boolean) ||
                  (cond.type == VAL_NUMBER && cond.number != 0);
    Value last_val = (Value){.type = VAL_NIL};
    while (1) {
      Value val = evaluate(node->while_loop.body, env);
      if (!isTrue) {
        last_val = val;
        break;
      }
    }
    return last_val;
  }

  if (node->type == AST_IDENTIFIER) {
    Value val;
    if (env_get(env, node->identifier_name, &val)) {
      return val;
    }
    printf("Runtime error: Undefined identifier: '%s'\n",
           node->identifier_name);
    exit(1);
  }

  if (node->type == AST_NUMBER) {
    return value_number(node->number_val);
  }

  if (node->type == AST_STRING) {
    return value_string(node->string);
  }

  if (node->type == AST_BINARY_OP) {
    double left = evaluate(node->binary_op.left, env).number;
    double right = evaluate(node->binary_op.right, env).number;
    switch (node->binary_op.op) {
    case TOKEN_PLUS:
      return value_number(left + right);
    case TOKEN_MINUS:
      return value_number(left - right);
    case TOKEN_STAR:
      return value_number(left * right);
    case TOKEN_SLASH:
      if (right == 0) {
        printf("Runtime error: Division by zero\n");
        exit(0);
      }
      return value_number(left / right);
    case TOKEN_LESS_THAN:
      return (Value){.type = VAL_BOOL, .boolean = left < right};
    case TOKEN_GREATER_THAN:
      return (Value){.type = VAL_BOOL, .boolean = left > right};
    case TOKEN_EQUAL:
      return (Value){.type = VAL_BOOL, .boolean = left == right};
    default:
      printf("Error: Unknown operator when evaluating\n");
      exit(0);
    }
  }

  return (Value){.type = VAL_NIL};
}
