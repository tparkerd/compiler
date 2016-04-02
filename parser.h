#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "color.h"

#define DEBUG 1

typedef struct token {
  int id;
  char name[MAX_TOKEN_SPACE];
  tokenType type;
} token;

// Global Variables
int tokenCounter = 0;
struct token t;

void parser();
void program();
void getNextToken();
void block();
void statement();
void condition();
void expression();
void term();
void factor();
void ifelse(); // this may need to be split into two things
void error();
const char* translate(int n);

void parser() {
  (DEBUG) ? printf(ANSI_COLOR_CYAN"Token count: %d\n"ANSI_COLOR_RESET, tokenCounter) : printf(" ");
  program();
}

void program() {
  (DEBUG) ? printf(ANSI_COLOR_CYAN"program()\n"ANSI_COLOR_RESET) : printf(" ");
  getNextToken();
  block();
  if (t.type != periodsym)
  {
    printf(ANSI_COLOR_DARKRED"period\n"ANSI_COLOR_RESET);
    exit(0);
  }

}

void block() {
  (DEBUG) ? printf(ANSI_COLOR_CYAN"Block()\n"ANSI_COLOR_RESET) : printf(" ");
  if ( t.type == constsym )
  {

    do
    {
      getNextToken();
      if ( t.type != identsym )
      {
        printf(ANSI_COLOR_DARKRED"identifier\n"ANSI_COLOR_RESET);
        exit(0);
      }

      getNextToken();
      if ( t.type != eqlsym )
      {
        printf(ANSI_COLOR_DARKRED"equal sign\n"ANSI_COLOR_RESET);
        exit(0);
      }

      getNextToken();
      if ( t.type != numbersym )
      {
        printf(ANSI_COLOR_DARKRED"number\n"ANSI_COLOR_RESET);
        exit(0);
      }
    } while ( t.type == commasym );

      getNextToken();
      if ( t.type != semicolonsym )
      {
        printf(ANSI_COLOR_DARKRED"semicolon\n"ANSI_COLOR_RESET);
        exit(0);
      }

      getNextToken();
    }

    if ( t.type == varsym )
    {
      while ( t.type == commasym )
      {
        getNextToken();
        if ( t.type != identsym )
        {
          printf(ANSI_COLOR_DARKRED"identifier\n"ANSI_COLOR_RESET);
          exit(0);
        }
        getNextToken();
      }

      if ( t.type != semicolonsym )
      {
        printf(ANSI_COLOR_DARKRED"semicolon\n"ANSI_COLOR_RESET);
        exit(0);
      }

      getNextToken();
      block();

      if ( t.type != semicolonsym )
      {
        printf(ANSI_COLOR_DARKRED"semicolon\n"ANSI_COLOR_RESET);
        exit(0);
      }

      getNextToken();
    }

    statement();
  }
}

void statement() {
  (DEBUG) ? printf(ANSI_COLOR_CYAN"statement()\n"ANSI_COLOR_RESET) : printf(" ");
  // If an identifier is found
  if ( t.type == identsym )
  {
    getNextToken();
    if ( t.type != becomessym )
    {
      printf(ANSI_COLOR_DARKRED"becomes\n"ANSI_COLOR_RESET);
      exit(0);
    }
    getNextToken();
    expression();

  // If a call is found instead
  } else if ( t.type == callsym )
  {
    getNextToken();
    if ( t.type != identsym )
    {
      printf(ANSI_COLOR_DARKRED"identifier\n"ANSI_COLOR_RESET);
      exit(0);
    }
    getNextToken();

  // If a begin is found instead of identifier or call
  } else if ( t.type == beginsym )
  {
    getNextToken();
    statement();
    while ( t.type == semicolonsym )
    {
      getNextToken();
      statement();
    }
    if ( t.type != endsym )
    {
      printf(ANSI_COLOR_DARKRED"end\n"ANSI_COLOR_RESET);
      exit(0);
    }

    getNextToken();

  // Else if ifsym instead of begin, identifier, or call
  } else if ( t.type == ifsym )
  {
    getNextToken();
    condition();
    if ( t.type != thensym )
    {
      printf(ANSI_COLOR_DARKRED"then\n"ANSI_COLOR_RESET);
    getNextToken();
    statement();

  } else if ( t.type == whilesym )
  {
    getNextToken();
    condition();
    if ( t.type != dosym )
    {
      printf(ANSI_COLOR_DARKRED"do\n"ANSI_COLOR_RESET);
      exit(0);
    }

    getNextToken();
    statement();
  }
  }
}

void condition() {
  if ( t.type == oddsym )
  {
    getNextToken();
    expression();
  } else
  {
    expression();
    if ( t.type != neqsym || t.type != lessym || t.type != leqsym || t.type != gtrsym || t.type != geqsym )
    {
      printf(ANSI_COLOR_DARKRED"relational operator\n"ANSI_COLOR_RESET);
      exit(0);
    }
    getNextToken();
    expression();
  }
}

void expression() {
  if ( t.type == plussym || t.type == minussym )
    getNextToken();
  term();
  while ( t.type == plussym || t.type == minussym )
  {
    getNextToken();
    term();
  }
}

void term() {
  factor();
  while ( t.type == multsym || t.type == slashsym )
  {
    getNextToken();
    factor();
  }
}

void factor() {
  if ( t.type == identsym )
    getNextToken();
  else if ( t.type == numbersym )
    getNextToken();
  else if ( t.type == lparentsym )
  {
    getNextToken();
    if ( t.type != rparentsym )
      printf(ANSI_COLOR_DARKRED")\n"ANSI_COLOR_RESET);
    getNextToken();
  } else
  {
    printf(ANSI_COLOR_DARKRED"identifier, number, or (\n"ANSI_COLOR_RESET);
    exit(0);
  }
}

void getNextToken() {
  (DEBUG) ? printf(ANSI_COLOR_GREEN"Token (%d) %s\n"ANSI_COLOR_RESET, t.id, translate(t.id)) : printf(" ");
  while ( strcmp(tokenStorage[tokenCounter].name, "") == 0 )
    tokenCounter++;
  t = tokenStorage[tokenCounter++];
}

void error() {
  printf(ANSI_COLOR_DARKRED"error\n"ANSI_COLOR_RESET);
}

const char* translate(int n) {
  switch (n)
  {
    case 0:
      return "error";
    case 1:
      return "nul";
    case 2:
      return "identsym";
    case 3:
      return "numbersym";
    case 4:
      return "plussym";
    case 5:
      return "minussym";
    case 6:
      return "mulsym";
    case 7:
      return "slashsym";
    case 8:
      return "oddsym";
    case 9:
      return "eqlsym";
    case 10:
      return "neqsym";
    case 11:
      return "lessym";
    case 12:
      return "leqsym";
    case 13:
      return "gtrsym";
    case 14:
      return "geqsym";
    case 15:
      return "lparentsym";
    case 16:
      return "rparentsym";
    case 17:
      return "commasym";
    case 18:
      return "semicolonsym";
    case 19:
      return "periodsym";
    case 20:
      return "becomessym";
    case 21:
      return "beginsym";
    case 22:
      return "endsym";
    case 23:
      return "ifsym";
    case 24:
      return "thensym";
    case 25:
      return "whilesym";
    case 26:
      return "dosym";
    case 27:
      return "callsym";
    case 28:
      return "constsym";
    case 29:
      return "varsym";
    case 30:
      return "procsym";
    case 31:
      return "writesym";
    case 32:
      return "readsym";
    case 33:
      return "elsesym";
    case 34:
      return "errsym";
    default:
      return "unknown sym";
  }
}
