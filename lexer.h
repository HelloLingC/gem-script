#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <stddef.h>

extern Token current_token;
extern size_t src_pos;

void lexer_init(const char *src);
Token get_next_token(const char **scpy);
void advance(void);

#endif // LEXER_H
