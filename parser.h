#define DEBUG 1


// Global Variables
int tokenCounter = 0;
int tokenCount;
struct token t;
struct token lexList[MAX_FILE_LENGTH];
// struct token lexOutput[MAX_FILE_LENGTH];
FILE* parserInput;
// FILE* parserOutput;

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
void readTokenList();
void displayTokenList();
void countValidTokens();

void readTokenList() {
 int i = 0;

  FILE* parserInput = fopen("lexemelist.txt", "r");

  int tmp;
  while ( fscanf(parserInput, "%d", &tmp) != EOF && tmp > 0 && tmp < 34) {
    {
      char buffer[20];
      int intBuffer;
      if ( tmp == 2 )
      {
        fscanf(parserInput, "%s", buffer);
      } else if ( tmp == 3 )
      {
        fscanf(parserInput, "%d", &intBuffer);
      }
      lexList[i].id = tmp;
      i++;
    }
  }
  fclose(parserInput);
 }

void displayTokenList() {
    int i;
    for(i = 0; i < tokenCount; i++)
    {
      printf(ANSI_COLOR_BLUE"%d "ANSI_COLOR_RESET, lexList[i].id);
    }
}

void countValidTokens() {
  while ( lexList[tokenCount].id > 0 && lexList[tokenCount].id < 34 )
  {
    tokenCount++;
  }
}

void parser() {
  readTokenList();
  countValidTokens();
  displayTokenList();
  program();
}

void program() {
  (DEBUG) ? printf(ANSI_COLOR_CYAN"program()\n"ANSI_COLOR_RESET) : printf(" ");
  getNextToken();
  block();
  if (t.type != periodsym)
  {
    printf(ANSI_COLOR_DARKRED"period\n"ANSI_COLOR_RESET);
    return;
  }

}

void block() {
  (DEBUG) ? printf(ANSI_COLOR_CYAN"Block()\n"ANSI_COLOR_RESET) : printf(" ");

  // Case: constant declaration
  if ( t.type == constsym )
  {
    do
    {
      getNextToken();
      if ( t.type != identsym )
      {
        printf(ANSI_COLOR_DARKRED"identifier\n"ANSI_COLOR_RESET);
        return;
      }

      getNextToken();
      if ( t.type != eqlsym )
      {
        printf(ANSI_COLOR_DARKRED"equal sign\n"ANSI_COLOR_RESET);
        return;
      }

      getNextToken();
      if ( t.type != numbersym )
      {
        printf(ANSI_COLOR_DARKRED"number\n"ANSI_COLOR_RESET);
        return;
      }
    } while ( t.type == commasym );

    getNextToken();

    // must end with a semicolon
    if ( t.type != semicolonsym )
    {
      printf(ANSI_COLOR_DARKRED"semicolon\n"ANSI_COLOR_RESET);
      return;
    }

    getNextToken();
  } // end constant declaration

  // Case: variable declaration
  if ( t.type == varsym )
  {
    do
    {
      getNextToken();
      if ( t.type != identsym )
      {
        printf(ANSI_COLOR_DARKRED"identifier\n"ANSI_COLOR_RESET);
        return;
      }
      getNextToken();
    } while ( t.type == commasym );

    // must end with a semicolon
    if ( t.type != semicolonsym )
    {
      printf(ANSI_COLOR_DARKRED"semicolon\n"ANSI_COLOR_RESET);
      return;
    }
    getNextToken();
  } // end variable declaration

  // Case: procedure declaration
  while ( t.type == procsym )
  {
    getNextToken();
    if ( t.type != identsym )
    {
      printf(ANSI_COLOR_DARKRED"identifier\n"ANSI_COLOR_RESET);
      return;
    }
    getNextToken();
    if ( t.type != semicolonsym )
    {
      printf(ANSI_COLOR_DARKRED"semicolon\n"ANSI_COLOR_RESET);
      return;
    }
    getNextToken();
    block();

    if ( t.type != semicolonsym )
    {
      printf(ANSI_COLOR_DARKRED"semicolon\n"ANSI_COLOR_RESET);
      return;
    }
  } // end procedure declaration
  statement();
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
      return;
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
      return;
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
      return;
    }

    getNextToken();




  // If a read is found instead of identifier or call
  } else if ( t.type == readsym )
  {
    getNextToken();
    //statement();
    while ( t.type != semicolonsym )
    {
      getNextToken();
      //statement();
    }
    if ( t.type == semicolonsym )
    {
      //printf(ANSI_COLOR_DARKRED"end\n"ANSI_COLOR_RESET);
      return;
    }

    getNextToken();





      // If a write is found instead of identifier or call
  } else if ( t.type == writesym )
  {
    getNextToken();
    //statement();
    while ( t.type != semicolonsym )
    {
      getNextToken();
      //statement();
    }
    if ( t.type == semicolonsym )
    {
      //getNextToken();
      //printf(ANSI_COLOR_DARKRED"end\n"ANSI_COLOR_RESET);
      return;
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
    }

    getNextToken();

  } else if ( t.type == whilesym )
  {
    //printf("One\n");
    getNextToken();
    //printf("Two\n");
    condition();
    //printf("Three\n");
    if ( t.type != dosym )
    {
      getNextToken();
      statement();
      printf(ANSI_COLOR_DARKRED"do\n"ANSI_COLOR_RESET);
      return;
    }
  }
    getNextToken();
    statement();
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
      return;
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
    //condition();
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
    expression();
    if( t.type != rparentsym )
    {
      printf(ANSI_COLOR_DARKRED")\n"ANSI_COLOR_RESET);
    }
    else {
    //printf(ANSI_COLOR_DARKRED"identifier, number, or (\n"ANSI_COLOR_RESET);
    getNextToken();
    return;
    }
  }
}

void getNextToken() {
  if ( tokenCounter >= tokenCount + 1)
  {
    exit(0);
  }
  (DEBUG) ? printf(ANSI_COLOR_GREEN"Token (%d) %s\n"ANSI_COLOR_RESET, t.id, translate(t.id)) : printf(" ");
  t = lexList[tokenCounter++];
}

void error(int e) {
  //printf(ANSI_COLOR_DARKRED"error\n"ANSI_COLOR_RESET);
      switch(e){
        case 1:
            printf("Use ""="", not "":=""\n");
            break;
        case 2:
            printf("""="" must be followed by a number.\n");
            break;
        case 3:
            printf("Identifier must be followed by ""="".\n");
            break;
        case 4:
            printf("const, var, procedure must be followed by identifier.\n");
            break;
        case 5:
            printf("Semicolon or comma missing.\n");
            break;
        case 6:
            printf("Incorrect symbol after procedure declaration.\n");
            break;
        case 7:
            printf("Statement expected.\n");
            break;
        case 8:
            printf("Incorrect symbol after statement part in block.\n");
            break;
        case 9:
            printf("Period expected.\n");
            break;
        case 10:
            printf("Semicolon between statements missing.\n");
            break;
        case 11:
            printf("Undeclared identifier.\n");
            break;
        case 12:
            printf("Assignment to constant or procedure is not allowed\n");
            break;
        case 13:
            printf("Assignment operator expected.\n");
            break;
        case 14:
            printf("call must be followed by an identifier.\n");
            break;
        case 15:
            printf("Call of a constant or variable is meaningless.\n");
            break;
        case 16:
            printf("then expected.\n");
            break;
        case 17:
            printf("Semicolon or } expected.\n");
            break;
        case 18:
            printf("do expected.\n");
            break;
        case 19:
            printf("Incorrect symbol following statement.\n");
            break;
        case 20:
            printf("Relational operator expected.\n");
            break;
        case 21:
            printf("Expression must not contain a procedure identifier.\n");
            break;
        case 22:
            printf("Right parenthesis missing.\n");
            break;
        case 23:
            printf("The preceding factor cannot begin with this symbol.\n");
            break;
        case 24:
            printf("An expression cannot begin with this symbol.\n");
            break;
        case 25:
            printf("This number is too large.\n");
            break;
        default:
            printf("An error has occurred.\n");
        }
}

const char* translate(int n) {
  switch (n)
  {
    case 0:
      return "error";
    case 1:
      return "nulsym";
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
      return "unknownsym";
  }
}
