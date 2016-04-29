#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "config.h"
#include "util.h"
#include "pmachine.h"
#include "lexicalanalyzer.h"
#include "parser.h"

int main() {
  lexicalanalyzer();
  parser();
  pmachine();
  return 0;
}
