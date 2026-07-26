
typedef enum {
  TOKEN_NUMBER,
  TOKEN_PLUS,
  TOKEN_MINUS,
  TOKEN_STAR,
  TOKEN_SLASH,
  TOKEN_LPAREN,
  TOKEN_RPAREN,
  TOKEN_EOF,
} TokenType;

typedef struct {
  int value;
  TokenType type;
} Token;

typedef enum {
  AST_NUMBER,
  AST_BINARY_OP,
} ASTNodeType;

typedef struct ASTNode {
  ASTNodeType type;
  union {
    // AST_NUMBER
    int number_val;

    // AST_BINARY_OP
    struct {
      TokenType op;
      struct ASTNode *left;
      struct ASTNode *right;
    } binary_op;
  };
} ASTNode;

ASTNode *astparse_expression();
