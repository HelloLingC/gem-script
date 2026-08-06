
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
} TokenType;

typedef struct {
  int value;
  TokenType type;
} Token;

typedef enum {
  AST_NUMBER,
  AST_UNARY_OP,
  AST_BINARY_OP,
} ASTNodeType;

typedef struct ASTNode {
  ASTNodeType type;
  union {
    // AST_NUMBER
    int number_val;

    // AST_UNARY_OP
    struct {
      TokenType op;
      struct ASTNode *operand;
    } unary_op;

    // AST_BINARY_OP
    struct {
      TokenType op;
      struct ASTNode *left;
      struct ASTNode *right;
    } binary_op;
  };
} ASTNode;

void astnode_free(ASTNode *node);
ASTNode *astparse_expression();
