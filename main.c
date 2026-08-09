#include "ast.h"
#include "env.h"
#include "evaluator.h"
#include "lexer.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  Environment *env = malloc(sizeof(Environment));
  env->count = 0;
  env->pareantEnv = NULL;

  const char *src_ptr = "x = 10 if x > 5 { y = 100 } else { y = 0 } y";
  lexer_init(src_ptr);

  advance();
  do {
    ASTNode *node = astparse_expression();
    printf("%d\n", (int)evaluate(node, env).number);
    astnode_free(node);
  } while (current_token.type != TOKEN_EOF);

  free(env);
  return 0;
}