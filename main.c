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

void advance();

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
      Token token;
      // Keyword
      if (strcmp(buf, "if") == 0) {
        token.type = TOKEN_IF;
      } else if (strcmp(buf, "else") == 0) {
        token.type = TOKEN_ELSE;
      } else {
        token.type = TOKEN_IDENTIFIER;
        strncpy(token.string_val, buf, 64);
        token.string_val[63] = '\0';
      }

      return token;
    }

    char current = **scpy;
    (*scpy)++;

    switch (current) {
    case '=': {
      if (**scpy == '=') {
        advance();
        return (Token){.type = TOKEN_EQUAL};
      }
      return (Token){.type = TOKEN_ASSIGN, .value = 0};
    }
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
    case '<':
      return (Token){.type = TOKEN_LESS_THAN, .value = 0};
    case '>':
      return (Token){.type = TOKEN_GREATER_THAN, .value = 0};
    case '{':
      return (Token){.type = TOKEN_LBRACE, .value = 0};
    case '}':
      return (Token){.type = TOKEN_RBRACE, .value = 0};
    }
  }
}

static Environment *env;
static Token current_token;
// static const char *src_ptr = "1+3 5+8+99 a=5 a";
static const char *src_ptr = "x = 10 if x > 5 { y = 100 } else { y = 0 } y";
static size_t src_pos = 0;
void advance() {
  current_token = get_next_token(&src_ptr);
  src_pos++;
}

ASTNode *astparse_block() {
  advance(); // consume '{'
  ASTNode **stmts = malloc(sizeof(ASTNode) * 64);
  int cnt = 0;
  while (current_token.type != TOKEN_RBRACE &&
         current_token.type != TOKEN_EOF) {
    stmts[cnt++] = astparse_expression();
  }
  advance(); // consume '}'
  return astnode_create_block(stmts, cnt);
}

ASTNode *astparse_if() {
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

ASTNode *astparse_factor() {
  if (current_token.type == TOKEN_IF) {
    return astparse_if();
  }

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

Value evaluate(ASTNode *node) {
  if (node == NULL)
    return value_number(0);

  if (node->type == AST_ASSIGNMENT) {
    Value val = evaluate(node->assignment.expr);
    env_set(env, node->assignment.name, val);
    return val;
  }

  if (node->type == AST_BLOCK) {
    int count = node->block.count;
    for (int i = 0; i < count; i++) {
      ASTNode *stmt = node->block.stmts[i];
      evaluate(stmt);
    }
    return evaluate(node->block.stmts[count - 1]);
  }

  if (node->type == AST_IF) {
    Value cond = evaluate(node->if_stmt.condition);
    bool isTrue = (cond.type == VAL_BOOL && cond.boolean) ||
                  (cond.type == VAL_NUMBER && cond.number != 0);
    if (isTrue) {
      return evaluate(node->if_stmt.then_branch);
    } else if (node->if_stmt.else_branch != NULL) {
      return evaluate(node->if_stmt.else_branch);
    }
    return (Value){.type = VAL_NIL};
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
    case TOKEN_LESS_THAN:
      return (Value){.type = VAL_BOOL, .boolean = left < right};
    case TOKEN_GREATER_THAN:
      return (Value){.type = VAL_BOOL, .boolean = left > right};
    case TOKEN_EQUAL:
      return (Value){.type = VAL_BOOL, .boolean = left == right};
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