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
int lookUp(const char* name, int level);
void insertSymbol(int kind, const char* name, int val, int level, int addr);


void parser() {

  memset(symbolList, 0, sizeof(symbol) * MAX_SYMBOL_TABLE_SIZE);
  readTokenList();
  countValidTokens();
  displayTokenList();
  program();
  createSymbolList();
}

void program() {
  (DEBUG) ? printf(ANSI_COLOR_CYAN"program()\n"ANSI_COLOR_RESET) : printf(" ");
  getNextToken();
  block();
  if (t.type != periodsym)
    error(9); // expected period

  getNextToken();
  if ( t.id != 0 )
  {
    error(29); // no token should follow a period
  }
  FILE* parserLog = fopen(PARSER_LOG, "a");
  fprintf(parserLog, "\nInput program is syntactically correct.\n");
  fclose(parserLog);

  // Why does this get inserted into the symbol table?
  gen(11, 0, 3);
  // fclose(codeGenOutput);
  displayCodeGen();
}

void block() {
  level++;
  (DEBUG) ? printf(ANSI_COLOR_CYAN"block()\n"ANSI_COLOR_RESET) : printf(" ");
  struct symbol tmp;
  int space = 4;
  int jmpAddr = gen(7, 0, 0);
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
      insertSymbol(1, tmp.name, tmp.val, 0, 0);

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
      printf(ANSI_COLOR_PURPLE"declare identifier (%d, %s, %d, %d)\n"ANSI_COLOR_RESET, 2, tmp.name, tmp.val, level);
      insertSymbol(2, tmp.name, tmp.val, level, 0);
      space++;

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
      error(4); // incorrect symbol after procedure

    // Valid id found, so assign its Name
    strcpy(tmp.name, t.name);

    getNextToken();
    if ( t.type != semicolonsym )
      error(5); // expected semicolon

    // Set a dummy value for the offset, so we can figure it out later
    tmp.val = 0;
    printf(ANSI_COLOR_REDP"declare procedure (%d, %s, %d, %d)\n"ANSI_COLOR_RESET, 3, tmp.name, tmp.val, level);
    // Using asm_line as the addr may not be the best option
    insertSymbol(3, tmp.name, tmp.val, level, asm_line);

    // Increase the level by one because anything after the proc was declared
    // with be at a higher level, but not including the proc itself
    getNextToken();
    block();

    if ( t.type != semicolonsym )
      error(5); // expected semicolon

    getNextToken();

  } // end procedure declaration

  // Here is where the address for JMP is changed to the correct address
  asm_code[jmpAddr].m = asm_line;
  gen(6, 0, space); // INC, 0, space (reserve space)
  statement();
  gen(2, 0, 0); // OPR, 0, 0 (return)
  level--;
}

void statement() {
  (DEBUG) ? printf(ANSI_COLOR_CYAN"statement()\n"ANSI_COLOR_RESET) : printf(" ");
  // If an identifier is found
  if ( t.type == identsym )
  {
    // Check if the idenfier has already been declared
    // Assume it has not been declared
    int declared = 0;
    int i;
    // Search the symbolList so far for the symbol
    for (i = 0; i < tokenCount; i++)
    {
      // See if the symbol exists
      (DEBUG) ? printf(ANSI_COLOR_CYAN"strcmp(%s, %s) && %d <= %d )\n"ANSI_COLOR_RESET, symbolList[i].name, t.name, symbolList[i].level, level ) : printf(" ");
      // There is a chance that the 'kind' to 'type' comparison won't work.
      // in fact, you may want to return the namespace, but then check its type
      // separately to entire a typing error
      if ( strcmp(symbolList[i].name, t.name) == 0 && symbolList[i].level <= level && symbolList[i].kind == t.type)
      {
        declared = 1;
        break;
      }
    }
    // If the variable was not declared, throw and error
    if (!declared)
      error(11); // undeclared var found


    getNextToken();
    if ( t.type != becomessym )
      error(26); // expected becomes

    getNextToken();
    expression();
  }
  // If a call is found instead
  else if ( t.type == callsym )
  {
    struct token tmpToken = t;
    getNextToken();
    if ( t.type != identsym )
      error(14); // identifier expected

    // Code gen for callsym
    // For some reason it is looking for this symbo to see if it is declared
    // oh, gotta check that the procedure was actually declared
    // insertSymbol(1, tmp.name, tmp.val, 0);
    // lookUp(int kind, const char* name, int val, int level)
    (DEBUG) ? printf(ANSI_COLOR_REDP"lookUp(3, %s, %d, %d)\n"ANSI_COLOR_RESET, t.name, atoi(t.name), level) : printf(" ");
    int tmpIndex = lookUp(t.name, level);
    if (tmpIndex == -1)
      error(11); // make need new error to state that the procedure is undeclared

    // Valid call was made, generate the code for it (what is the address though?)
    gen(5, level - symbolList[tmpIndex].level, symbolList[tmpIndex].addr ); // we need this after the procedure is parsered

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
  else if ( t.type == ifsym )
  {
    getNextToken();
    condition();
    if ( t.type != thensym ){
      error(16); // then expected
    }

    getNextToken();
    statement();
    if(t.type != semicolonsym){
      error(5);
    }
    getNextToken();


    if( t.type == elsesym )
    {
      getNextToken();
      statement();
    }
    getNextToken();
  }

  else if ( t.type == whilesym )
  {
    getNextToken();
    condition();
    if ( t.type != dosym )
      error(18); // do expected

    getNextToken();
    statement();
  }
  // If a read is found instead of identifier or call
  else if ( t.type == readsym )
  {
    getNextToken();
    if ( t.type != identsym )
      error(14); // identifier expected

    getNextToken();

  }
  // If a write is found instead of identifier or call
  else if ( t.type == writesym )
  {
    getNextToken();
    if ( t.type != identsym )
      error(14); // identifier expected

    // Look for the symbol in the symbol table to make sure it is declared
    // STARTED THIS FOR LIT, BUT NOT FINISHED
    // symbolIndex = lookUp(t.name, level);
    // if (symbolIndex == -1)
    //   error()

    getNextToken();
  }
}

void condition() {
  (DEBUG) ? printf(ANSI_COLOR_CYAN"condition()\n"ANSI_COLOR_RESET) : printf(" ");
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
  (DEBUG) ? printf(ANSI_COLOR_CYAN"expression()\n"ANSI_COLOR_RESET) : printf(" ");
  // Trying to do an op code gen
  tokenType tmpOp;

  if ( t.type == plussym || t.type == minussym )
  {
    tmpOp = t.type;
    if ( tmpOp == minussym )
    {
      getNextToken();
      term();
      gen(2, 0, 1);
    }

  }
  else // not sure why this would need to be else, but it might get an extra w/o being extra
    term();
  while ( t.type == plussym || t.type == minussym )
  {
    tmpOp = t.type;
    getNextToken();
    term();

    if ( tmpOp == plussym )
      gen(2, 0, 2);
    else
      gen(2, 0, 3);

  }
}

void term() {
  (DEBUG) ? printf(ANSI_COLOR_CYAN"term()\n"ANSI_COLOR_RESET) : printf(" ");
  factor();
  while ( t.type == multsym || t.type == slashsym )
  {
    getNextToken();
    factor();
  }
}

void factor() {
  (DEBUG) ? printf(ANSI_COLOR_CYAN"factor()\n"ANSI_COLOR_RESET) : printf(" ");

  int symbolIndex;

  if ( t.type == identsym )
  {
    symbolIndex = lookUp(t.name, level);


    getNextToken();
  }





  else if ( t.type == numbersym )
    getNextToken();

  else if ( t.type == lparentsym )
  {
    getNextToken();
    expression();
    if( t.type != rparentsym )
      error(22); // expected closing parenthesis

    getNextToken();
  }
  else
    error(27); // expected id, num, or opening parenthesis
}

void getNextToken() {
  if ( tokenCounter > tokenCount )
  {
    exit(0);
  }
  t = lexList[tokenCounter];
  tokenCounter++;
  (DEBUG) ? printf(ANSI_COLOR_GREEN"[%d] %s (%s)\n"ANSI_COLOR_RESET, t.id, translate(t.id), t.name) : printf(" ");
}

void error(int e) {
  FILE* lexemeOutput = fopen("parseoutput.txt", "a");
  fprintf(lexemeOutput, "\n");
  switch(e) {
    case 1:
      fprintf(lexemeOutput, "%s", "Use ""="", not "":=""\n");
      printf("Use ""="", not "":=""\n");
      break;
    case 2:
      fprintf(lexemeOutput, "%s", """="" must be followed by a number.\n");
      printf("""="" must be followed by a number.\n");
      break;
    case 3:
      fprintf(lexemeOutput, "%s", "Identifier must be followed by ""="".\n");
      printf("Identifier must be followed by ""="".\n");
      break;
    case 4:
      fprintf(lexemeOutput, "%s", "const, var, procedure must be followed by identifier.\n");
      printf("const, var, procedure must be followed by identifier.\n");
      break;
    case 5:
      fprintf(lexemeOutput, "%s", "Semicolon or comma missing.\n");
      printf("Semicolon or comma missing.\n");
      break;
    case 6:
      fprintf(lexemeOutput, "%s", "Incorrect symbol after procedure declaration.\n");
      printf("Incorrect symbol after procedure declaration.\n");
      break;
    case 7:
      fprintf(lexemeOutput, "%s", "Statement expected.\n");
      printf("Statement expected.\n");
      break;
    case 8:
      fprintf(lexemeOutput, "%s", "Incorrect symbol after statement part in block.\n");
      printf("Incorrect symbol after statement part in block.\n");
      break;
    case 9:
      fprintf(lexemeOutput, "%s", "Period expected.\n");
      printf("Period expected.\n");
      break;
    case 10:
      fprintf(lexemeOutput, "%s", "Semicolon between statements missing.\n");
      printf("Semicolon between statements missing.\n");
      break;
    case 11:
      fprintf(lexemeOutput, "%s", "Undeclared identifier.\n");
      printf("Undeclared identifier.\n");
      break;
    case 12:
      fprintf(lexemeOutput, "%s", "Assignment to constant or procedure is not allowed\n");
      printf("Assignment to constant or procedure is not allowed\n");
      break;
    case 13:
      fprintf(lexemeOutput, "%s", "Assignment operator expected.\n");
      printf("Assignment operator expected.\n");
      break;
    case 14:
      fprintf(lexemeOutput, "%s", "call must be followed by an identifier.\n");
      printf("call must be followed by an identifier.\n");
      break;
    case 15:
      fprintf(lexemeOutput, "%s", "Call of a constant or variable is meaningless.\n");
      printf("Call of a constant or variable is meaningless.\n");
      break;
    case 16:
      fprintf(lexemeOutput, "%s", "then expected.\n");
      printf("then expected.\n");
      break;
    case 17:
      fprintf(lexemeOutput, "%s", "Semicolon or } expected.\n");
      printf("Semicolon or } expected.\n");
      break;
    case 18:
      fprintf(lexemeOutput, "%s", "do expected.\n");
      printf("do expected.\n");
      break;
    case 19:
      fprintf(lexemeOutput, "%s", "Incorrect symbol following statement.\n");
      printf("Incorrect symbol following statement.\n");
      break;
    case 20:
      fprintf(lexemeOutput, "%s", "Relational operator expected.\n");
      printf("Relational operator expected.\n");
      break;
    case 21:
      fprintf(lexemeOutput, "%s", "Expression must not contain a procedure identifier.\n");
      printf("Expression must not contain a procedure identifier.\n");
      break;
    case 22:
      fprintf(lexemeOutput, "%s", "Right parenthesis missing.\n");
      printf("Right parenthesis missing.\n");
      break;
    case 23:
      fprintf(lexemeOutput, "%s", "The preceding factor cannot begin with this symbol.\n");
      printf("The preceding factor cannot begin with this symbol.\n");
      break;
    case 24:
      fprintf(lexemeOutput, "%s", "An expression cannot begin with this symbol.\n");
      printf("An expression cannot begin with this symbol.\n");
      break;
    case 25:
      fprintf(lexemeOutput, "%s", "This number is too large.\n");
      printf("This number is too large.\n");
      break;

    // Adding addition error messages for things that weren't specificed
    case 26:
      fprintf(lexemeOutput, "%s", "Expected a becomessym\n");
      printf("Expected a becomessym\n");
      break;
    case 27:
      fprintf(lexemeOutput, "%s", "Expected identifier, number, or opening parenthesis in factor.\n");
      printf("Expected identifier, number, or opening parenthesis in factor.\n");
      break;
    case 28:
      fprintf(lexemeOutput, "%s", "Expected else.\n");
      printf("Expected else.\n");
      break;
    case 29:
      fprintf(lexemeOutput, "%s", "No symbol should follow the final period.");
      printf("No symbol should follow the final period.\n");
      break;
    default:
      fprintf(lexemeOutput, "%s", "An error has occurred.\n");
      printf("An error has occurred.\n");
    }

  fclose(lexemeOutput);
  // Halt the parser
  exit(0);
}

void readTokenList() {
 int i = 0;

  FILE* parserInput = fopen("lexemelist.txt", "r");
  FILE* lexemeOutput = fopen("parseoutput.txt", "w");

  int tmp;
  while ( fscanf(parserInput, "%d", &tmp) != EOF && tmp > 0 && tmp < 34) {
    {
      fprintf(lexemeOutput, "%d ", tmp);
      char buffer[20];
      if ( tmp == 2 || tmp == 3)
      {
        fscanf(parserInput, "%s", buffer);
        strcpy(lexList[i].name, buffer);
        fprintf(lexemeOutput, "%s ", buffer);
      }
      lexList[i].id = tmp;
      lexList[i].type = (tokenType)tmp;
      i++;
    }
  }
  fclose(lexemeOutput);
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
  FILE* ofp = fopen(PARSER_OUTPUT_SYMLIST, "w");
  // Header
  fprintf(ofp, "%-15s\t%-15s\t%-15s\t%-15s\n", "Name", "Type", "Level", "Value");
  for(c = 0; c < symbolCounter; c++)
  {
    if (symbolList[c].kind == 0)
      continue;
    fprintf(ofp, "%-15s\t%-15s\t%-15d\t%-15d\n", symbolList[c].name, kindToType(symbolList[c].kind), symbolList[c].level, symbolList[c].val);
  }
  fclose(ofp);
}

// Looks in the symbol table to see if a symbol already exists
// Level should be the current level for the parser, so if we
// declare "var i" in a two separa... something something
// returns either the location of the existing sym, or the next
// empty location in "symbol memory": the symbol list array.
int lookUp( const char* name, int level) {
  // Gotta do it backwards instead!
  int i;
  for ( i = symbolCounter; i >= 0; i-- )
  {
    // We've got a match!
    // Check if...
    // Subprocedure (greater level) or equal to current level (may need to just be greater so sibling procs don't try to redeclare variables)
    // Same name
    if ( symbolList[i].level >= level && (strcmp(symbolList[i].name, name) == 0) )
      return i;
  }

  if ( symbolCounter + 1 < MAX_SYMBOL_TABLE_SIZE )
  {
    printf(ANSI_COLOR_DARKRED"%d\n"ANSI_COLOR_RESET, symbolCounter);
    return symbolCounter + 1;
  }
  else
  {
    printf(ANSI_COLOR_DARKRED"%d\n"ANSI_COLOR_RESET, 0);
    return -1;
  }

}

// If the symbol does not exist yet,
void insertSymbol(int kind, const char* name, int val, int level, int addr) {
  // Find the location in the symbol list, or find an empty slot for it
  int location = lookUp(name, level);
  symbolCounter++;

  symbolList[location].kind = kind;
  strcpy(symbolList[location].name, name);
  symbolList[location].val = val;
  symbolList[location].level = level;
  symbolList[location].addr = addr;
}
