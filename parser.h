#define DEBUG 1
#define MAX_SYMBOL_TABLE_SIZE 100


typedef struct symbol {
  int kind;       // const = 1, var = 2, proc = 3
  char name[12];  // name up to 11 characters
  int val;        // number (ASCII value)
  int level;      // L level
  int addr;       // M address
} symbol;

// Global Variables
int tokenCounter = 0;
struct token t;
struct token lexList[MAX_FILE_LENGTH];
FILE* parserInput;
FILE* symbolTable;
struct symbol symbolList[MAX_SYMBOL_TABLE_SIZE];

void parser();
void program();
void block();
void statement();
void condition();
void expression();
void term();
void factor();
void getNextToken();
void error(int e);
void readTokenList();
void displayTokenList();
void countValidTokens();
const char* translate(int n);
const char* kindToType(int n);
void createSymbolList();
int findSym(int kind, const char* name, int val, int level);
void declareSym(int kind, const char* name, int val, int level);

void parser() {
  readTokenList();
  countValidTokens();
  // displayTokenList();
  program();
  createSymbolList();
}

void program() {
  (DEBUG) ? printf(ANSI_COLOR_CYAN"program()\n"ANSI_COLOR_RESET) : printf(" ");
  getNextToken();
  block();
  if (t.type != periodsym)
    error(9); // expected period

}

void block() {
  (DEBUG) ? printf(ANSI_COLOR_CYAN"Block()\n"ANSI_COLOR_RESET) : printf(" ");

  struct symbol tmp;
  // Case: constant declaration
  if ( t.type == constsym )
  {
    do
    {
      getNextToken();
      if ( t.type != identsym )
        error(4); // expected identifier

      // Valid id found, so assign its name
      strcpy(tmp.name, t.name);

      getNextToken();
      if ( t.type != eqlsym )
        error(3); // equal sign

      getNextToken();
      if ( t.type != numbersym )
        error(2); // number sym

      // Valid number found, so assign its value
      tmp.val = atoi(t.name);
      printf(ANSI_COLOR_PURPLE"Declare(%d, %s, %d, %d)\n"ANSI_COLOR_RESET, 1, tmp.name, tmp.val, 0);
      declareSym(1, tmp.name, tmp.val, 0);

    } while ( t.type == commasym );

    getNextToken();

    // must end with a semicolon
    if ( t.type != semicolonsym )
      error(5); // semicolon

    getNextToken();
  } // end constant declaration

  // Case: variable declaration
  if ( t.type == varsym )
  {
    int value = 4;
    do
    {
      getNextToken();
      if ( t.type != identsym )
        error(4); // expected identifier

      // Valid id found, so assign its Name
      strcpy(tmp.name, t.name);
      tmp.val = value++;
      printf(ANSI_COLOR_PURPLE"Declare(%d, %s, %d, %d)\n"ANSI_COLOR_RESET, 1, tmp.name, tmp.val, tmp.level);
      declareSym(2, tmp.name, tmp.val, tmp.level);

      getNextToken();
    } while ( t.type == commasym );

    // must end with a semicolon
    if ( t.type != semicolonsym )
      error(5); // semicolon

    getNextToken();
  } // end variable declaration

  // Case: procedure declaration
  while ( t.type == procsym )
  {
    getNextToken();
    if ( t.type != identsym )
      error(4); // expected identifier

    getNextToken();
    if ( t.type != semicolonsym )
      error(5); // expected semicolon

    getNextToken();
    block();
    // Why is there a second check for a semicolon?
    if ( t.type != semicolonsym )
      error(5); // expected semicolon

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
      error(26); // expected becomes

    getNextToken();
    expression();
  }
  // If a call is found instead
  else if ( t.type == callsym )
  {
    getNextToken();
    if ( t.type != identsym )
      error(14); // identifier expected

    getNextToken();
  }
  // If a begin is found instead of identifier or call
  else if ( t.type == beginsym )
  {
    getNextToken();
    statement();
    while ( t.type == semicolonsym )
    {
      getNextToken();
      statement();
    }
    if ( t.type != endsym )
      error(35); // expected endsym

    getNextToken();
  }

  /*############################################################################*/
  /*############################################################################*/
  /*############################################################################*/
  /*############################################################################*/
  /*############################################################################*/
  /*############################################################################*/
  /*############################################################################*/
  /*############################################################################*/
  /*############################################################################*/
  /*############################################################################*/
  /*############################################################################*/
  /*############################################################################*/
  /*############################################################################*/
  /*############################################################################*/
  /*############################################################################*/
  /*############################################################################*/
  /*############################################################################*/
  // If a read is found instead of identifier or call
  else if ( t.type == readsym )
  {
    getNextToken();
    while ( t.type != semicolonsym )
      getNextToken();

    // ########## WHY ARE WE RETURNING IF IT IS A SEMICOLON ??? ################## //
    if ( t.type == semicolonsym )
      return;

    getNextToken();
  }
  // If a write is found instead of identifier or call
  else if ( t.type == writesym )
  {
    getNextToken();
    while ( t.type != semicolonsym )
    {
      getNextToken();
      if(t.type == identsym)
        return;
    }
    if ( t.type == semicolonsym )
      return;

    getNextToken();

  // Else if ifsym instead of begin, identifier, or call
  } else if ( t.type == ifsym )
  {
    getNextToken();
    condition();
    if ( t.type != thensym )
    {
    //printf(ANSI_COLOR_DARKRED"then\n"ANSI_COLOR_RESET);
      error(16);
      exit(0);
    getNextToken();
    statement();
    }

    getNextToken();
  }
  /*############################################################################*/
  /*############################################################################*/
  /*############################################################################*/
  /*############################################################################*/
  /*############################################################################*/
  /*############################################################################*/
  /*############################################################################*/
  /*############################################################################*/
  /*############################################################################*/
  /*############################################################################*/
  /*############################################################################*/
  /*############################################################################*/
  /*############################################################################*/
  /*############################################################################*/
  /*############################################################################*/
  /*############################################################################*/
  /*############################################################################*/
  /*############################################################################*/
  /*############################################################################*/
  /*############################################################################*/
  /*############################################################################*/
  else if ( t.type == whilesym )
  {
    getNextToken();
    condition();
    if ( t.type != dosym )
      error(18); // do expected

    getNextToken();
    statement();
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
    if ( !(t.type == neqsym || t.type == lessym || t.type == leqsym || t.type == gtrsym || t.type == geqsym) )
      error(20); // relational operator expected

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

/// REVISIT FACTOR BECAUSE IT DOESN'T MATCH PSEUDOCODE
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
      error(22);

    else {
    //printf(ANSI_COLOR_DARKRED"identifier, number, or (\n"ANSI_COLOR_RESET);
    getNextToken();
    return;
    }
  }
}

void getNextToken() {
  if ( tokenCounter >= tokenCount)
  {
    exit(0);
  }
  t = lexList[tokenCounter];
  tokenCounter++;
  (DEBUG) ? printf(ANSI_COLOR_GREEN"Token (%d) %s\n"ANSI_COLOR_RESET, t.id, translate(t.id)) : printf(" ");
}

void error(int e) {
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

    // Adding addition error messages for things that weren't specificed
    case 26:
      printf("Expected a becomessym");
      break;
    case 27:
      printf("Expected identifier, number, or opening parenthesis in factor.\n");
      break;
    default:
      printf("An error has occurred.\n");
    }
  // Halt the parser
  exit(0);
}

void readTokenList() {
 int i = 0;

  FILE* parserInput = fopen("lexemelist.txt", "r");

  int tmp;
  while ( fscanf(parserInput, "%d", &tmp) != EOF && tmp > 0 && tmp < 34) {
    {
      char buffer[20];
      int intBuffer;
      if ( tmp == 2 || tmp == 3)
      {
        fscanf(parserInput, "%s", buffer);
        strcpy(lexList[i].name, buffer);
      }
      lexList[i].id = tmp;
      lexList[i].type = (tokenType)tmp;
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
    printf("\n");
}

void countValidTokens() {
  while ( lexList[tokenCount].id > 0 && lexList[tokenCount].id < 34 )
  {
    tokenCount++;
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

const char* kindToType(int n) {
  switch(n)
  {
    case 1:
      return "const";
    case 2:
      return "var";
    case 3:
      return "proc";
    default:
      return "";
  }
}

void createSymbolList(){
  int c;
  symbolTable = fopen("symboltable.txt", "w");
  // Header
  fprintf(symbolTable, "%-15s\t%-15s\t%-15s\t%-15s\n", "Name", "Type", "Level", "Value");
  for(c = 0; c < tokenCount; c++)
  {
    if (symbolList[c].kind == 0)
      break;
    fprintf(symbolTable, "%-15s\t%-15s\t%-15d\t%-15d\n", symbolList[c].name, kindToType(symbolList[c].kind), symbolList[c].level, symbolList[c].val);
  }
  fclose(symbolTable);
}

// Looks in the symbol table to see if a symbol already exists
// Level should be the current level for the parser, so if we
// declare "var i" in a two separa... something something
// returns either the location of the existing sym, or the next
// empty location in "symbol memory": the symbol list array.
int findSym(int kind, const char* name, int val, int level) {
  int i;
  for ( i = 0; i < MAX_SYMBOL_TABLE_SIZE; i++ )
  {
    // If we hit an empty block, the item doesn't exist, so
    // return the index of the symbol table to be used to insert a new
    // symbol
    if ( symbolList[i].kind == 0 )
      return i;

    // We've got a match!
    // Check if...
    // Same kind
    // Subprocedure (greater level) or equal to current level (may need to just be greater so sibling procs don't try to redeclare variables)
    // Same address (may be unnecessary), not sure how this works
    // Same name
    if ( symbolList[i].kind == kind && symbolList[i].level >= level && (strcmp(symbolList[i].name, name) == 0) )
      return i;
  }
  return 0;
}

// If the symbol does not exist yet,
void declareSym(int kind, const char* name, int val, int level) {
  // Find the location in the symbol list, or find an empty slot for it
  int location = findSym(kind, name, val, level);

  symbolList[location].kind = kind;
  strcpy(symbolList[location].name, name);
  symbolList[location].val = val;
  symbolList[location].level = level;
}
