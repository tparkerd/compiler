#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "color.h"
#include "pmachine.h"
#include "lexicalanalyzer.h"
#include "parser.h"


//void readTokenList();

int main() {
  pmachine();
  //lexicalanalyzer();
  //readTokenList();
  //parser();
  return 0;
}

//void readTokenList() {
//  int i;
//  for (i = 0; i < counter; i++)
//  {
//    if (strcmp(tokenStorage[i].name, "") == 0 || tokenStorage[i].id == 0)
//      continue;
//
//    printf(ANSI_COLOR_CYAN"%-15s\t\t%-d\t(%s)\n"ANSI_COLOR_RESET, tokenStorage[i].name, tokenStorage[i].id, translate(tokenStorage[i].id));
//  }
//}
