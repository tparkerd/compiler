#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "color.h"
#include "pmachine.h"
#include "lexicalanalyzer.h"
#include "codegen.h"
#include "parser.h"

int main() {
  lexicalanalyzer();
  codegen();
  parser();
  pmachine();
  return 0;
}
