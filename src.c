#include "lib/utility.h"


int main() {
  printf("Initialize data: ");
  init();
  printf("done\n\n");
  printf("Read file contents: ");
  read();
  printf("done\n\n");
  printf("Scanning input\n__________________________\n");
  scan();
  printf("__________________________\n\n");

  printf("Lexeme Table\n__________________________\n");
  writeLexemeTable();
  writeLexemeList();
  return 0;
}

void scan() {
  // For each character in the input array (cleaninput.txt)
  for( counter = 0; counter < MAX_FILE_LENGTH - 1; counter++ )
  {
    if ( !isWhiteSpace(cleanInput[counter]) )
    {
      error = 0;
      // printf("Check: %c\n", cleanInput[counter]);
      // Assume it is an invalid token type
      struct token* t = (struct token*)malloc(sizeof(struct token));

      t = isReservedWord(t, counter);
      if ( t->type != nulsym )
      {
        printf(ANSI_COLOR_GREEN"%s\n"ANSI_COLOR_RESET, t->name);
        tokenStorage[tokenCount++] = *t;
        counter += getLength(t->name) - 1;
        free(t);
        continue;
      }

      t = isIdentifier(t, counter);
      if ( t->type != nulsym )
      {
        if ( t->type == numbersym )
          printf(ANSI_COLOR_REDP"%s\n"ANSI_COLOR_RESET, t->name);
        else if (t->type == identsym )
          printf(ANSI_COLOR_CYAN"%s\n"ANSI_COLOR_RESET, t->name);

        tokenStorage[tokenCount++] = *t;
        counter += getLength(t->name) - 1;
        free(t);
        continue;
      }

      t = isSpecialSymbol(t, counter);
      if ( t->type != nulsym )
      {
        printf(ANSI_COLOR_YELLOW"%s\n"ANSI_COLOR_RESET, t->name);
        tokenStorage[tokenCount++] = *t;
        counter += getLength(t->name) - 1;
        free(t);
        continue;
      }

      t = isNumber(t, counter);
      if ( t->type != nulsym)
      {
        printf(ANSI_COLOR_PINK"%s\n"ANSI_COLOR_RESET, t->name);
        tokenStorage[tokenCount++] = *t;
        counter += getLength(t->name) - 1;
        free(t);
        continue;
      }


    }
  }
}
