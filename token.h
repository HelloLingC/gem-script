#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
  TOKEN_NUMBER,
  TOKEN_PLUS,
  TOKEN_MINUS,
  TOKEN_STAR,
  TOKEN_SLASH,
  TOKEN_LPAREN,
  TOKEN_RPAREN,
  TOKEN_EOF,
  TOKEN_ASSIGN,
  TOKEN_IDENTIFIER,
  TOKEN_EQUAL,        // ==
  TOKEN_LESS_THAN,    // <
  TOKEN_GREATER_THAN, // >
  TOKEN_LBRACE,       // {
  TOKEN_RBRACE,       // }
  TOKEN_IF,           // if
  TOKEN_ELSE,         // else
  TOKEN_WHILE,
} TokenType;

typedef struct {
  int value;
  char string_val[64];
  TokenType type;
} Token;

#endif // TOKEN_H
