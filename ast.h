#include <stdbool.h>
#include <string.h>

typedef enum { VAL_NIL, VAL_NUMBER, VAL_BOOL } ValueType;

typedef struct {
  ValueType type;
  union {
    double number;
    bool boolean;
  };
} Value;

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
} TokenType;

typedef struct {
  int value;
  char string_val[64];
  TokenType type;
} Token;

typedef enum {
  AST_NUMBER,
  AST_UNARY_OP,
  AST_BINARY_OP,
  AST_IDENTIFIER,
  AST_ASSIGNMENT,
  AST_IF,
  AST_BLOCK
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

    // AST_IDENTIFIER
    char identifier_name[64];

    // AST_ASSIGNMENT
    struct {
      char name[64];
      struct ASTNode *expr;
    } assignment;

    // AST_BLOCK
    struct {
      struct ASTNode **stmts;
      int count;
    } block;

    // AST_IF
    struct {
      struct ASTNode *condition;
      struct ASTNode *then_branch;
      struct ASTNode *else_branch; // can be null if no else
    } if_stmt;
  };
} ASTNode;

typedef struct {
  char name[64];
  Value val;
} Symbol;

typedef struct Environment {
  Symbol symbols[100];
  int count;
  struct Environment *pareantEnv; // For nested variable scope
} Environment;

void env_set(Environment *env, char name[64], Value val);
bool env_get(Environment *env, char name[64], Value *ret_val);

ASTNode *astnode_create_num(int val);
ASTNode *astnode_create_unaryop(TokenType op, ASTNode *operand);
ASTNode *astnode_create_binop(TokenType op, ASTNode *left, ASTNode *right);
ASTNode *astnode_create_assignment(const char *var_name, ASTNode *expr);
ASTNode *astnode_create_identifier(const char *id_name);
ASTNode *astnode_create_block(ASTNode **stmts, int count);
ASTNode *astnode_create_if(ASTNode *condition, ASTNode *then_branch,
                           ASTNode *else_branch);

ASTNode *astparse_expression();
void astnode_free(ASTNode *node);