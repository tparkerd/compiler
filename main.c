#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "color.h"
#include "pmachine.h"
#include "lexicalanalyzer.h"
#include "parser.h"

int main() {
  lexicalanalyzer();
  parser();
  pmachine();
  return 0;
}
