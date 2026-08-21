#ifndef AST_H
#define AST_H

#include "token.h"
#include "value.h"

typedef enum {
  AST_NUMBER,
  AST_STRING,
  AST_UNARY_OP,
  AST_BINARY_OP,
  AST_IDENTIFIER,
  AST_VAR_DECL,
  AST_ASSIGNMENT,
  AST_IF,
  AST_BLOCK,
  AST_WHILE,
  AST_FUNCTION_DECL,
  AST_CALL,
  AST_RETURN,
} ASTNodeType;

typedef struct {
  char name[64];
  char type[64];
} FuncParam;

typedef struct ASTNode {
  ASTNodeType type;
  union {
    // AST_NUMBER
    int number_val;
    // AST_STRING
    char *string;

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

    struct {
      struct ASTNode *condition;
      struct ASTNode *body;
    } while_loop;

    struct {
      char *func_name;      // function name
      FuncParam params[16]; // function args
      int params_count;
      struct ASTNode *body;
    } function_decl;

    struct {
      char *func_name; // func name
      FuncParam arguments[16];
      int arg_count;
    } call;
  };
} ASTNode;

ASTNode *astnode_create_num(int val);
ASTNode *astnode_create_string(char *string);
ASTNode *astnode_create_unaryop(TokenType op, ASTNode *operand);
ASTNode *astnode_create_binop(TokenType op, ASTNode *left, ASTNode *right);
ASTNode *astnode_create_var_decl(const char *var_name, ASTNode *expr);
ASTNode *astnode_create_assignment(const char *var_name, ASTNode *expr);
ASTNode *astnode_create_identifier(const char *id_name);
ASTNode *astnode_create_block(ASTNode **stmts, int count);
ASTNode *astnode_create_if(ASTNode *condition, ASTNode *then_branch,
                           ASTNode *else_branch);
ASTNode *astnode_create_while(ASTNode *condition, ASTNode *body);
ASTNode *astnode_create_function_decl(char *func_name, FuncParam params[16],
                                      int param_count, struct ASTNode *body);

void astnode_free(ASTNode *node);

#endif // AST_H
