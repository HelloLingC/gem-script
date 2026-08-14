#include "ast.h"
#include "compiler.h"
#include "env.h"
#include "lexer.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

char *read_file(const char *path) {
  FILE *file = fopen(path, "rb");
  if (!file) {
    fprintf(stderr, "Error: Cannot open file: %s\n", path);
    exit(74); // Standard exit code for input file error
  }
  fseek(file, 0, SEEK_END);
  long fileSize = ftell(file);
  rewind(file);

  char *buf = malloc(fileSize + 1); // plus a terminator
  if (!buf) {
    fprintf(stderr, "Error: Unable to malloc new memory when reading file\n");
    exit(1);
  }
  size_t byteRead = fread(buf, sizeof(char), fileSize, file);
  buf[byteRead] = '\0';

  fclose(file);
  return buf;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: gem <script.gem>\n");
    exit(1);
  }

  // read the argument for path for an input file
  if (argv[1] == NULL) {
    printf("Error: no input file\n");
    exit(1);
  }

  const char *code = read_file(argv[1]);

  Environment *env = malloc(sizeof(Environment));
  env->count = 0;
  env->pareantEnv = NULL;

  lexer_init(code);

  advance();
  do {
    ASTNode *node = astparse_expression();
    Chunk chunk;
    chunk_init(&chunk);
    compile(node, &chunk);
    chunk_write(&chunk, OP_RETURN);

    VM vm;
    vm_init(&vm);
    vm_interpret(&vm, &chunk);

    chunk_free(&chunk);
    astnode_free(node);
  } while (current_token.type != TOKEN_EOF);

  free((void *)code);
  free(env);

  return 0;
}
