#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "color.h"
#include "pmachine.h"
#include "lexicalanalyzer.h"
#include "parser.h"

void readTokenList();
void displayTokenList();
void countValidTokens();


int main() {
  pmachine();
  lexicalanalyzer();
  parser();
  return 0;
}
