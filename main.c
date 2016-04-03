#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "color.h"
#include "pmachine.h"
#include "lexicalanalyzer.h"
#include "parser.h"

void readTokenList();

int main() {
  //pmachine();
  lexicalanalyzer();
  readTokenList();
  parser();
  return 0;
}

void readTokenList() {
 int i, j = 0;

  FILE* parserInput = fopen("lexemelist.txt", "r");

  char tmp;

  while ( fscanf(parserInput, "%c", &tmp) != EOF ) {
      lexTest[i] = tmp;
      i++;
  }
  fclose(parserInput);

  for(i = 0; i < MAX_FILE_LENGTH; i++)
  {
    lexOutput[i] = lexTest[i];
  }

  FILE* parserOutput = fopen("parseData.txt", "w");

  for(i = 0; i < MAX_FILE_LENGTH; i++)
  {

    if ( lexOutput[i] == 0 )
      continue;
    fprintf(parserOutput, "%c", lexOutput[i]);
  }
  fclose(parserOutput);

   //printf(ANSI_COLOR_CYAN"%-15s\t\t%-d\t(%s)\n"ANSI_COLOR_RESET, tokenStorage[i].name, tokenStorage[i].id, translate(tokenStorage[i].id));
 }
