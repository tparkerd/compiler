#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "color.h"
#include "pmachine.h"
#include "lexicalanalyzer.h"
#include "parser.h"

void readTokenList();
// void displayTokenList(int count);
void displayTokenList();
void countValidTokens();
// void cleanLexList(struct token* list, int size);
// struct token* lexList;


int main() {
  pmachine();
  lexicalanalyzer();
  parser();
  return 0;
}
