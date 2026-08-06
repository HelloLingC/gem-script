#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Value value_number(double number) {
  Value value = {.type = VAL_NUMBER, .number = number};
  return value;
}

void env_set(Environment *env, char *name, Value val) {
  Symbol symbol = {.val = val};
  strncpy(symbol.name, name, 64);
  env->symbols[env->count++] = symbol;
}

bool env_get(Environment *env, char name[64], Value *ret_val) {
  for (int i = 0; i < env->count; i++) {
    if (strcmp(env->symbols[i].name, name) == 0) {
      ret_val->type = env->symbols[i].val.type;
      ret_val->number = env->symbols[i].val.number;
      return true;
    }
  }
  return false;
}

size_t is_digit(char c) { return c >= '0' && c <= '9'; }
size_t is_vaild_identifier(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

// Lexer
Token get_next_token(const char **scpy) {
  while (1) {
    if (**scpy == ' ' || **scpy == '\n' || **scpy == '\t') {
      (*scpy)++;
      continue;
    }
    if (**scpy == '\0')
      return (Token){.type = TOKEN_EOF, .value = 0};

    // small loop for Number Token
    // may advance *scpy
    if (is_digit(**scpy)) {
      int val = 0;
      while (is_digit(**scpy)) {
        val = val * 10 + (**scpy - '0');
        (*scpy)++;
      }
      return (Token){.type = TOKEN_NUMBER, .value = val};
    }

    if (is_vaild_identifier(**scpy)) {
      char buf[64];
      size_t len = 0;
      while (is_vaild_identifier(**scpy)) {
        buf[len++] = **scpy;
        (*scpy)++;
      }
      buf[len] = '\0';
      Token token = {.type = TOKEN_IDENTIFIER};
      strncpy(token.string_val, buf, 64);
      return token;
    }

    char current = **scpy;
    (*scpy)++;

    switch (current) {
    case '=':
      return (Token){.type = TOKEN_ASSIGN, .value = 0};
    case '+':
      return (Token){.type = TOKEN_PLUS, .value = 0};
    case '-':
      return (Token){.type = TOKEN_MINUS, .value = 0};
    case '*':
      return (Token){.type = TOKEN_STAR, .value = 0};
    case '/':
      return (Token){.type = TOKEN_SLASH, .value = 0};
    case '(':
      return (Token){.type = TOKEN_LPAREN, .value = 0};
    case ')':
      return (Token){.type = TOKEN_RPAREN, .value = 0};
    }
  }
}

static Environment *env;
static Token current_token;
static const char *src_ptr = "1+3 5+8+99 a=5 a";
static size_t src_pos = 0;
void advance() {
  current_token = get_next_token(&src_ptr);
  src_pos++;
}

ASTNode *astparse_factor() {
  // Hanlde - and +
  if (current_token.type == TOKEN_MINUS || current_token.type == TOKEN_PLUS) {
    advance();
    ASTNode *val = astnode_create_num(current_token.value);
    astnode_create_unaryop(current_token.type, val);
  }

  if (current_token.type == TOKEN_NUMBER) {
    ASTNode *node = astnode_create_num(current_token.value);
    advance();
    return node;
  }

  if (current_token.type == TOKEN_LPAREN) {
    advance();
    ASTNode *node = astparse_expression();
    if (current_token.type == TOKEN_RPAREN) {
      advance();
      return node;
    }
    printf("Syteax Error: Expected ')'\n");
    exit(1);
  }

  if (current_token.type == TOKEN_IDENTIFIER) {
    char var_name[64];
    strncpy(var_name, current_token.string_val, 64);
    advance();
    if (current_token.type == TOKEN_ASSIGN) {
      advance();
      ASTNode *expr = astparse_expression();
      return astnode_create_assignment(var_name, expr);
    }
    return astnode_create_identifier(var_name);
  }

  printf("Syntax Error: Expected number at %zu\n", src_pos);
  exit(1);
}

ASTNode *astparse_term() {
  ASTNode *left = astparse_factor();
  while (current_token.type == TOKEN_STAR ||
         current_token.type == TOKEN_SLASH) {
    TokenType op = current_token.type;
    advance(); // consume * or /
    ASTNode *right = astparse_factor();
    left = astnode_create_binop(op, left, right);
  }
  return left;
}

ASTNode *astparse_expression() {
  ASTNode *left = astparse_term();
  while (current_token.type == TOKEN_PLUS ||
         current_token.type == TOKEN_MINUS) {
    TokenType op = current_token.type;
    advance();
    ASTNode *right = astparse_term();
    left = astnode_create_binop(op, left, right);
  }
  return left;
}

Value evaluate(ASTNode *node) {
  if (node == NULL)
    return value_number(0);

  if (node->type == AST_ASSIGNMENT) {
    Value val = evaluate(node->assignment.expr);
    env_set(env, node->assignment.name, val);
    return val;
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

  if (node->type == AST_BINARY_OP) {
    double left = evaluate(node->binary_op.left).number;
    double right = evaluate(node->binary_op.right).number;
    switch (node->binary_op.op) {
    case TOKEN_PLUS:
      return value_number(left + right);
    case TOKEN_MINUS:
      return value_number(left - right);
    case TOKEN_STAR:
      return value_number(left * right);
    case TOKEN_SLASH:
      if (right == 0) {
        printf("Runtime error: Division by zero");
        exit(0);
      }
      return value_number(left / right);
    default:
      printf("Error: Unknow operator when evaluating");
      exit(0);
    }
  }
}

int main(void) {
  env = malloc(sizeof(Environment));

  advance();
  do {
    ASTNode *node = astparse_expression();
    printf("%d\n", (int)evaluate(node).number);
  } while (current_token.type != TOKEN_EOF);
  return 0;
}
