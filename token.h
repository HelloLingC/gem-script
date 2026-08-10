#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
  TOKEN_NUMBER,
  TOKEN_TRUE,
  TOKEN_FALSE,
  TOKEN_STRING,
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

  /** string_val can be either Identifier(Variable name) or String literal
  when it's an identifier, the size is fixed 64-char array
  */
  char *string_val;
  TokenType type;
} Token;

#endif // TOKEN_H
