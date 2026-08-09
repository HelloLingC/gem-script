#include "lexer.h"
#include <stdio.h>
#include <string.h>

Token current_token;
size_t src_pos = 0;
static const char *src_ptr = NULL;

static size_t is_digit(char c) { return c >= '0' && c <= '9'; }
static size_t is_vaild_identifier(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

void lexer_init(const char *src) {
  src_ptr = src;
  src_pos = 0;
}

void advance(void) {
  current_token = get_next_token(&src_ptr);
  src_pos++;
}

Token get_next_token(const char **scpy) {
  while (1) {
    if (**scpy == ' ' || **scpy == '\n' || **scpy == '\t') {
      (*scpy)++;
      continue;
    }
    if (**scpy == '\0')
      return (Token){.type = TOKEN_EOF, .value = 0};

    // small loop for Number Token
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
      } else if (strcmp(buf, "while") == 0) {
        token.type = TOKEN_WHILE;
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
