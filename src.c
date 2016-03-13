#include "lib/utility.h"


int main() {
    printf("Initialize data: ");
    init();
    // printf("DONE\n");
    printf("Read file contents: ");
    read();
    printf(ANSI_COLOR_CYAN"DONE\n"ANSI_COLOR_RESET);
    printf(ANSI_COLOR_CYAN"Scanning input: \n"ANSI_COLOR_RESET);
    scan();
    printf(ANSI_COLOR_CYAN"DONE\n"ANSI_COLOR_RESET);
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

      t = isIdentifier(t, counter);
      if ( t->type != nulsym )
      {
        printf(ANSI_COLOR_CYAN"%s\n"ANSI_COLOR_RESET, t->name);
        tokenStorage[tokenCount++] = *t;
        counter += getLength(t->name) - 1;
        free(t);
        continue;
      }

    }
  }
}
