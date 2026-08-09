#include "parser.h"
#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ASTNode *astparse_block(void) {
  advance(); // consume '{'
  ASTNode **stmts = malloc(sizeof(ASTNode *) * 64);
  int cnt = 0;
  while (current_token.type != TOKEN_RBRACE &&
         current_token.type != TOKEN_EOF) {
    stmts[cnt++] = astparse_expression();
  }
  advance(); // consume '}'
  return astnode_create_block(stmts, cnt);
}

ASTNode *astparse_if(void) {
  advance(); // consume 'if'
  ASTNode *condition = astparse_expression();
  ASTNode *then_branch = astparse_block();
  ASTNode *else_branch = NULL;
  if (current_token.type == TOKEN_ELSE) {
    advance(); // consume 'else'
    // handle 'else if'
    if (current_token.type == TOKEN_IF) {
      else_branch = astparse_if();
    } else {
      else_branch = astparse_block();
    }
  }
  return astnode_create_if(condition, then_branch, else_branch);
}

ASTNode *astparse_while(void) {
  advance(); // consume 'while'
  ASTNode *condition = astparse_expression();
  ASTNode *loop_body = astparse_block();
  return astnode_create_while(condition, loop_body);
}

ASTNode *astparse_factor(void) {
  if (current_token.type == TOKEN_IF) {
    return astparse_if();
  }

  if (current_token.type == TOKEN_WHILE) {
    return astparse_while();
  }

  // Handle - and +
  if (current_token.type == TOKEN_MINUS || current_token.type == TOKEN_PLUS) {
    advance();
    ASTNode *val = astnode_create_num(current_token.value);
    return astnode_create_unaryop(current_token.type, val);
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
    printf("Syntax Error: Expected ')'\n");
    exit(1);
  }

  if (current_token.type == TOKEN_IDENTIFIER) {
    char var_name[64];
    strncpy(var_name, current_token.string_val, 64);
    var_name[63] = '\0';
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

ASTNode *astparse_term(void) {
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

ASTNode *astparse_expression(void) {
  ASTNode *left = astparse_term();
  while (current_token.type == TOKEN_PLUS ||
         current_token.type == TOKEN_MINUS ||
         current_token.type == TOKEN_LESS_THAN ||
         current_token.type == TOKEN_GREATER_THAN ||
         current_token.type == TOKEN_EQUAL) {
    TokenType op = current_token.type;
    advance();
    ASTNode *right = astparse_term();
    left = astnode_create_binop(op, left, right);
  }

  return left;
}
