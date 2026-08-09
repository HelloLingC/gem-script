#ifndef PARSER_H
#define PARSER_H

#include "ast.h"

ASTNode *astparse_expression(void);
ASTNode *astparse_term(void);
ASTNode *astparse_factor(void);
ASTNode *astparse_block(void);
ASTNode *astparse_if(void);
ASTNode *astparse_while(void);

#endif // PARSER_H
