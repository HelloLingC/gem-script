#include "ast.h"
#include <stdio.h>
#include <stdlib.h>

ASTNode *astnode_create_num(int val) {
  ASTNode *node = malloc(sizeof(ASTNode));
  node->type = AST_NUMBER;
  node->number_val = val;
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

size_t is_digit(char c) { return c >= '0' && c <= '9'; }

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

    char current = **scpy;
    (*scpy)++;

    switch (current) {
    case '+': {
      return (Token){.type = TOKEN_PLUS, .value = 0};
    }
    case '-': {
      return (Token){.type = TOKEN_MINUS, .value = 0};
    }
    case '*':
      return (Token){.type = TOKEN_STAR, .value = 0};
    case '/':
      return (Token){.type = TOKEN_SLASH, .value = 0};
    }
  }
}

static Token current_token;
static const char *src_ptr;
void advance() { current_token = get_next_token(&src_ptr); }

ASTNode *astparse_factor() {
  if (current_token.type == TOKEN_NUMBER) {
    ASTNode *node = astnode_create_num(current_token.value);
    advance();
    return node;
  }
  printf("Syntax Error: Expected number");
  exit(1);
}

ASTNode *astparse_term() {
  ASTNode *left = astparse_factor();
  if (current_token.type == TOKEN_STAR || current_token.type == TOKEN_SLASH) {
    TokenType op = current_token.type;
    advance(); // consume * or /
    ASTNode *right = astparse_factor();
    left = astnode_create_binop(op, left, right);
  }
  return left;
}

ASTNode *astparse_expression() {
  ASTNode *left = astparse_term();
  if (current_token.type == TOKEN_PLUS || current_token.type == TOKEN_MINUS) {
    TokenType op = current_token.type;
    advance();
    ASTNode *right = astparse_term();
    left = astnode_create_binop(op, left, right);
  }
  return left;
}

int main(void) {
  const char *msg = "432 + 2";
  Token token;
  do {
    token = get_next_token(&msg);
    printf("TOK t: %d, TOK v: %d\n", token.type, token.value);
  } while (token.type != TOKEN_EOF);
  return 0;
}
