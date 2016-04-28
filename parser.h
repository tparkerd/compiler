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
int gen(int instruction, int l, int m);
const char* opTrans(int type);
void insertSymbol(int kind, const char* name, int val, int level, int addr);
void displayCodeGen();


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
  int tmpBPos = asm_line;
  gen(7, 0, 0);
  int tmpIndex;
  tmpM = 0;
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
      printf(ANSI_COLOR_PURPLE"Declare(const, %s, %d, %d, %d)\n"ANSI_COLOR_RESET, tmp.name, tmp.val, 0, 0);
      insertSymbol(1, tmp.name, tmp.val, 0, 0);

    } while ( t.type == commasym );

    getNextToken();

    // must end with a semicolon
    if ( t.type != semicolonsym )
      error(5); // semicolon

    getNextToken();
  } // end constant declaration

  tmpIndex = tmp.val;

  // Case: variable declaration
  if ( t.type == varsym )
  {
    do
    {
      getNextToken();
      if ( t.type != identsym )
        error(4); // expected identifier

      // Valid id found, so assign its Name
      strcpy(tmp.name, t.name);
      printf(ANSI_COLOR_PURPLE"declare identifier (var, %s, %d, %d, %d)\n"ANSI_COLOR_RESET, tmp.name, space, level, space);
      insertSymbol(2, tmp.name, space, level, space);
      space++;
      // (int kind, const char* name, int val, int level, int addr)
      // (int kind, Token t, int L, int M, int val)
      // pushSymTable(2, currentToken, lexLevel, currentM+4, 0);

      getNextToken();
    } while ( t.type == commasym );

    // must end with a semicolon
    if ( t.type != semicolonsym )
      error(5); // semicolon

    getNextToken();
  } // end variable declaration

  tmpIndex = tmpM;

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
    //tmp.val = 0;
    printf(ANSI_COLOR_REDP"declare procedure (proc, %s, %d, %d, %d)\n"ANSI_COLOR_RESET, tmp.name, asm_line, level, asm_line );
    // Using asm_line as the addr may not be the best option
    insertSymbol(3, tmp.name, asm_line, level, asm_line);

    // Increase the level by one because anything after the proc was declared
    // with be at a higher level, but not including the proc itself
    getNextToken();
    block();

    if ( t.type != semicolonsym )
      error(5); // expected semicolon
    // else
     	//gen(9, 0, 2);

    getNextToken();
  } // end procedure declaration

  tmp.val = tmp.val + 1;

  // Here is where the address for JMP is changed to the correct address
  asm_code[tmpBPos].m = asm_line;
  gen(6, 0, space); // INC, 0, space (reserve space)
  statement();
  // Only return if it is not the end of the file (so when the t.type != period)
  // a procedure end will end with a semicolon
  if (t.type == semicolonsym && t.type != periodsym)
    gen(2, 0, 0); // OPR, 0, 0 (return)
  level--;

  (DEBUG) ? printf(ANSI_COLOR_CYAN"exit_block()\n"ANSI_COLOR_RESET) : printf(" ");
}

void statement() {
  (DEBUG) ? printf(ANSI_COLOR_CYAN"statement()\n"ANSI_COLOR_RESET) : printf(" ");
  // If an identifier is found
  int identIndex, symIndex, tmpIndex, tmpIndex2, tmpBlockIndex, tmpBlockIndex2, identOffset;
  if ( t.type == identsym )
  {
  	int symIndex = lookUp(t.name, level);
    // Check if the idenfier has already been declared
    // Assume it has not been declared

    // If the variable was not declared, throw and error
    if (symIndex == -1)
      error(11); // undeclared var found
    else if ( symbolList[symIndex].kind == 1 )
      error(30); // assignment to const/proc not valid

    printf("symIndex: %d, symbolList[%d]: %d\n", symIndex, symIndex, symbolList[symIndex].val);
  	identOffset = symbolList[symIndex].addr;

    getNextToken();
    if ( t.type != becomessym )
    {
      if ( t. type == eqlsym )
        error(1); // use := instead of =
      else
        error(26); // expected becomes
    }

    getNextToken();
    expression();

    gen(4, level - symbolList[symIndex].level, identOffset); // may be wrong
  }
  // If a call is found instead
  else if ( t.type == callsym )
  {
    // struct token tmpToken = t;
    getNextToken();
    if ( t.type != identsym )
      error(14); // identifier expected

    // Code gen for callsym
    // For some reason it is looking for this symbo to see if it is declared
    // oh, gotta check that the procedure was actually declared
    symIndex = lookUp(t.name, level);

    if (symIndex == -1)
      error(11); // make need new error to state that the procedure is undeclared

    getNextToken();

    // Valid call was made, generate the code for it (what is the address though?)
    gen(5, level - symbolList[symIndex].level, symbolList[symIndex].addr ); // we need this after the procedure is parsered ( this may actually be just level, not the subtraction)

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
    if ( t.type != thensym )
      error(16); // then expected

    getNextToken();

    tmpBlockIndex = asm_line;
    //asm_code[tmpBlockIndex].m = asm_line;
    gen(8, 0, 0); // Why is this JPC and not a JMP?
    statement();
    tmpBlockIndex2 = asm_line;
    gen(7, 0, 0);
    asm_code[tmpBlockIndex].m = asm_line;
    //asm_code[tmpBlockIndex2].m = asm_line;

    getNextToken();

    if ( t.type != elsesym )
     {
       tokenCounter--;
       tokenCounter--;
       t = lexList[tokenCounter];
        tokenCounter++;
       if ( t.id != 0 )
         (DEBUG) ? printf(ANSI_COLOR_PURPLE"[%d] %s (%s)\n"ANSI_COLOR_RESET, t.id, translate(t.id), t.name) : printf(" ");
     }

    if( t.type == elsesym )
    {
    	//asm_code[tmpBlockIndex].m = asm_line;
    	//tmpBlockIndex = asm_line;
    	//gen(7, 0, 0);
      getNextToken();
      statement();
    }
    asm_code[tmpBlockIndex2].m = asm_line;

      //getNextToken();
  }

  else if ( t.type == whilesym )
  {
  	tmpIndex = asm_line;
    getNextToken();
    condition();
    tmpIndex2 = asm_line;
    gen(8, 0, 0);

    if ( t.type != dosym )
      error(18); // do expected

    getNextToken();
    statement();
    gen(7, 0, tmpIndex);
    asm_code[tmpIndex2].m = asm_line;
  }
  // If a read is found instead of identifier or call
  else if ( t.type == readsym )
  {
    getNextToken();
    if ( t.type != identsym )
      error(14); // identifier expected

    symIndex = lookUp(t.name, level);

    getNextToken();

    gen(10, 0, 2);

    gen(4, 0, symbolList[symIndex].addr);
  }
  // If a write is found instead of identifier or call
  else if ( t.type == writesym )
  {
    getNextToken();
    if ( t.type != identsym )
      error(14); // identifier expected
    else
    {
      symIndex = lookUp(t.name, level);
      if ( symIndex == -1 )
        error(15);

      getNextToken();
      if(symbolList[symIndex].kind == 1)
        gen(1, 0, symbolList[symIndex].val);
      else
        gen(3, 0, symbolList[symIndex].val);

      gen(9, 0, 1);
    }
  }
  (DEBUG) ? printf(ANSI_COLOR_CYAN"exit_statement()\n"ANSI_COLOR_RESET) : printf(" ");
}

void condition() {
	int someOp;
  (DEBUG) ? printf(ANSI_COLOR_CYAN"condition()\n"ANSI_COLOR_RESET) : printf(" ");
  if ( t.type == oddsym )
  {
  	gen(2, 0, 6);
    getNextToken();
    expression();
  } else
  {
    expression();
    someOp = t.type;
    if ( !(t.type == neqsym || t.type == lessym || t.type == leqsym || t.type == gtrsym || t.type == geqsym || t.type == eqlsym) )
      error(20); // relational operator expected

    getNextToken();
    expression();
    switch(someOp){
      case eqlsym:
        gen(2, 0, 8);
        break;
      case neqsym:
        gen(2, 0, 9);
        break;
      case lessym:
        gen(2, 0, 10);
        break;
      case leqsym:
        gen(2, 0, 11);
        break;
      case gtrsym:
        gen(2, 0, 12);
        break;
      case geqsym:
        gen(2, 0, 13);
        break;
    }
  }
  (DEBUG) ? printf(ANSI_COLOR_CYAN"exit_condition()\n"ANSI_COLOR_RESET) : printf(" ");
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
  (DEBUG) ? printf(ANSI_COLOR_CYAN"exit_expression()\n"ANSI_COLOR_RESET) : printf(" ");
}

    // case 4:
    //   return "plussym";
    // case 5:
    //   return "minussym";
    // case 6:
    //   return "mulsym";
    // case 7:
    //   return "slashsym";

void term() {
  (DEBUG) ? printf(ANSI_COLOR_CYAN"term()\n"ANSI_COLOR_RESET) : printf(" ");

  tokenType tmpOp;

  factor();
  while ( t.type == multsym || t.type == slashsym )
  {
  	tmpOp = t.type;
    getNextToken();
    factor();
    if(tmpOp == multsym)
    	gen(2, 0, 4);//edit this to correct one
    else
    	gen(2, 0, 5);//edit this to correct one
  }
  (DEBUG) ? printf(ANSI_COLOR_CYAN"exit_term()\n"ANSI_COLOR_RESET) : printf(" ");
}

void factor() {
  (DEBUG) ? printf(ANSI_COLOR_CYAN"factor()\n"ANSI_COLOR_RESET) : printf(" ");

  int symIndex;

  if ( t.type == identsym )
  {
    symIndex = lookUp(t.name, level);

    if (symIndex == -1)
      error(11); // undeclared identifier

    if(symbolList[symIndex].kind == 1)
    	gen(1, 0, symbolList[symIndex].val);
    else
    	gen(3, level - symbolList[symIndex].level, symbolList[symIndex].val);

    getNextToken();
	}

  else if ( t.type == numbersym )
  {
  	gen(1, 0, atoi(t.name));
    getNextToken();
  }

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

  (DEBUG) ? printf(ANSI_COLOR_CYAN"exit_factor()\n"ANSI_COLOR_RESET) : printf(" ");
}

void getNextToken() {
  if ( tokenCounter > tokenCount )
  {
    exit(0);
  }
  t = lexList[tokenCounter];
  tokenCounter++;
  if ( t.id != 0 )
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
    case 30:
      fprintf(lexemeOutput, "%s", "Assignment to const/proc not valid.");
      printf("Assignment to const/proc not valid.\n");
      break;
    case 35:
      fprintf(lexemeOutput, "%s", "Expected end.");
      printf("Expected end.\n");
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
  fprintf(ofp, "%-15s\t%-15s\t%-15s\t%-15s\t%-15s\n", "Name", "Type", "Level", "Value", "Addr");
  for(c = 0; c < symbolCounter + 1; c++)
  {
    if (symbolList[c].kind == 0)
      continue;
    fprintf(ofp, "%-15s\t%-15s\t%-15d\t%-15d\t%-15d\n", symbolList[c].name, kindToType(symbolList[c].kind), symbolList[c].level, symbolList[c].val, symbolList[c].addr);
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

  while ( level != -1 )
  {
    for ( i = symbolCounter - 1; i >= 0; i-- )
    {
      // We've got a match!
      // Check if...
      // Subprocedure (greater level) or equal to current level (may need to just be greater so sibling procs don't try to redeclare variables)
      // Same name
      // Also, make sure it is not a procedure
      if ( symbolList[i].level == level && (strcmp(symbolList[i].name, name) == 0) && ( symbolList[i].addr != -1) )
      {
        (DEBUG) ? printf(ANSI_COLOR_REDP"lookUp(name, level) = lookUp(%s, %d)\n"ANSI_COLOR_RESET, t.name, level) : printf(" ");
        (DEBUG) ? printf(ANSI_COLOR_CYAN"[name, level, value, addr]: (%s, %d, %d, %d)\n"ANSI_COLOR_RESET, symbolList[i].name, symbolList[i].level, symbolList[i].val, symbolList[i].addr) : printf(" ");
        return i;
      }
    }
    level--;
  }
  return -1;
}

// If the symbol does not exist yet,
void insertSymbol(int kind, const char* name, int val, int level, int addr) {
  // Find the location in the symbol list, or find an empty slot for it
  int location = symbolCounter;

  // Make sure that there was an available slot for the new symbol
  // This may need to be used to throw an error where there is a memory overflow
  if ( location != -1 )
    symbolCounter++;

  // If the symbol happens to be a var, we will need to increment the current M value
  // to maintain the correct JMP addresses
  if(symbolList[location].kind == 2)
    tmpM++;

  symbolList[location].kind = kind;
  strcpy(symbolList[location].name, name);
  symbolList[location].val = val;
  symbolList[location].level = level;
  symbolList[location].addr = addr;
}

int gen(int instruction, int l, int m) {
  ((DEBUG) ? printf(ANSI_COLOR_YELLOW"gen(%s, %d, %d)\n"ANSI_COLOR_RESET, opTrans(instruction), l, m) : printf(" "));
  asm_code[asm_line].addr = asm_line;
  asm_code[asm_line].instruction = instruction;
  asm_code[asm_line].l = l;
  asm_code[asm_line].m = m;
  asm_line++;
  return asm_line;
}

void displayCodeGen() {
  int i;
  FILE* ofp = fopen(PARSER_OUTPUT_ASMCODE, "w");
  printf("Number of asm_lines: %d\n", asm_line);
  for (i = 0; i < asm_line; i++)
  {
    printf("%*d %s %*d %*d\n", 2, asm_code[i].addr, opTrans(asm_code[i].instruction), 3, asm_code[i].l, 3, asm_code[i].m);
    // Why does the gen function seem to insert anything into the asm_code, but should be the symbol table?
    fprintf(ofp, "%d %d %d\n", asm_code[i].instruction, asm_code[i].l, asm_code[i].m);
  }
  fclose(ofp);
}

const char* opTrans(int type) {
  switch(type) {
    case 1:
      return "LIT";
    case 2:
      return "OPR";
    case 3:
      return "LOD";
    case 4:
      return "STO";
    case 5:
      return "CAL";
    case 6:
      return "INC";
    case 7:
      return "JMP";
    case 8:
      return "JPC";
    case 9:
    case 10:
    case 11:
      return "SIO";
    default:
      return "???";
  }
}
