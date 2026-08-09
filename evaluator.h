#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "ast.h"
#include "env.h"

Value evaluate(ASTNode *node, Environment *env);

#endif // EVALUATOR_H
