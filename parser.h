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
void createSymbolList();
int lookUp(const char* name, int level);
int gen(int instruction, int l, int m);
void insertSymbol(int kind, const char* name, int val, int level, int addr);
void displayCodeGen();


void parser() {

  memset(symbolList, 0, sizeof(symbol) * MAX_SYMBOL_TABLE_SIZE);
  readTokenList();
  countValidTokens();
  if (DEBUG) displayTokenList();
  program();
  createSymbolList();
}

void program() {
  if (DEBUG) printf(ANSI_COLOR_CYAN"program()\n"ANSI_COLOR_RESET);
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

  if (DEBUG) printf(ANSI_COLOR_CYAN"exit_program()\n"ANSI_COLOR_RESET);
  gen(HALT, 0, 3);
  // fclose(codeGenOutput);
  displayCodeGen();
}

void block() {
  level++;
  if (DEBUG) printf(ANSI_COLOR_CYAN"block()\n"ANSI_COLOR_RESET);
  struct symbol tmp;
  int space = 4;
  int tmpBPos = asm_line;
  gen(JMP, 0, 0);
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
      insertSymbol(2, tmp.name, space, level, space);
      space++;

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

    // Add the procedure to the symbol table
    insertSymbol(3, tmp.name, asm_line, level, asm_line);

    // Implicit/Explicit declaration of the return value for a procedure
    // Every procedure will have its own implicit variable ‘return’, which is
    // inserted into the symbol table immediately after the procedure declaration
    // has been parsed.
    insertSymbol(2, "return", 0, level + 1, 0);

    // Increase the level by one because anything after the proc was declared
    // with be at a higher level, but not including the proc itself
    getNextToken();
    block();

    if ( t.type != semicolonsym )
      error(5); // expected semicolon

    getNextToken();
  } // end procedure declaration

  // Here is where the address for JMP is changed to the correct address
  asm_code[tmpBPos].m = asm_line;
  gen(INC, 0, space); // INC, 0, space (reserve space)
  statement();
  // Only return if it is not the end of the file (so when the t.type != period)
  // a procedure end will end with a semicolon
  if (t.type == semicolonsym && t.type != periodsym)
    gen(OPR, 0, 0); // OPR, 0, 0 (return)


  // ###############################################################
  // ###############################################################
  // #######           I think this is where we              #######
  // #######                                                 #######
  // #######           should place any code to              #######
  // #######           delete a symbol (local variable)      #######
  // #######           from the symbol list.                 #######
  // #######                                                 #######
  // #######                                                 #######
  // #######                                                 #######
  // ###############################################################
  // ###############################################################

  level--;
  if (DEBUG) printf(ANSI_COLOR_CYAN"exit_block()\n"ANSI_COLOR_RESET);
}

void statement() {
  if (DEBUG) printf(ANSI_COLOR_CYAN"statement()\n"ANSI_COLOR_RESET);
  // If an identifier is found
  int identIndex, symIndex, tmpIndex, tmpIndex2, tmpBlockIndex, tmpBlockIndex2, identOffset;
  if ( t.type == identsym )
  {
  	int symIndex = lookUp(t.name, level);
    // Check if the idenfier has already been declared
    // If the variable was not declared, throw and error
    if (symIndex == -1)
      error(11); // undeclared var found

    // Also check if it is not a var
    else if ( symbolList[symIndex].kind != 2 )
      error(30); // assignment to const/proc not valid

    if (DEBUG) printf("symIndex: %d, symbolList[%d]: %d\n", symIndex, symIndex, symbolList[symIndex].val);
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

    // Store the value at the offset of the starting identifier
    gen(STO, level - symbolList[symIndex].level, identOffset);
  }
  // If a call is found instead
  else if ( t.type == callsym )
  {
    // struct token tmpToken = t;
    getNextToken();
    if ( t.type != identsym )
      error(14); // identifier expected

    symIndex = lookUp(t.name, level);

    // Code gen for callsym
    if ( symIndex == -1)
      error(36); // procedure undeclared

    getNextToken();

    // Valid call was made, generate the code for it (what is the address though?)
    gen(CAL, level - symbolList[symIndex].level, symbolList[symIndex].addr );

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
    gen(JPC, 0, 0); // JPC, 0, ?
    statement();
    tmpBlockIndex2 = asm_line;
    gen(JMP, 0, 0); // JMP, 0, ?
    asm_code[tmpBlockIndex].m = asm_line;

    getNextToken();

    if ( t.type != elsesym )
    {
      tokenCounter--;
      tokenCounter--;
      t = lexList[tokenCounter];
      tokenCounter++;
    }
    if( t.type == elsesym )
    {
      getNextToken();
      statement();
    }
    asm_code[tmpBlockIndex2].m = asm_line;
  }

  else if ( t.type == whilesym )
  {
  	tmpIndex = asm_line;
    getNextToken();
    condition();
    tmpIndex2 = asm_line;
    gen(JPC, 0, 0);

    if ( t.type != dosym )
      error(18); // do expected

    getNextToken();
    statement();
    gen(JMP, 0, tmpIndex);
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

    gen(READ, 0, 2);

    gen(STO, 0, symbolList[symIndex].addr);
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
        error(15); // undeclared symbol

      // If it's a const, the actual value will be in its val field
      if(symbolList[symIndex].kind == 1)
        gen(LIT, 0, symbolList[symIndex].val);
      // Otherwise it must be a var, so the value will need to be loaded
      // based on its offset value (also in its val field)
      else
        gen(LOD, 0, symbolList[symIndex].val);

      gen(WRITE, 0, 1);

      getNextToken();
    }
  }
  if (DEBUG) printf(ANSI_COLOR_CYAN"exit_statement()\n"ANSI_COLOR_RESET);
}

void condition() {
	int someOp;
  if (DEBUG) printf(ANSI_COLOR_CYAN"condition()\n"ANSI_COLOR_RESET);
  if ( t.type == oddsym )
  {
  	gen(OPR, 0, 6);
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
        gen(OPR, 0, 8);
        break;
      case neqsym:
        gen(OPR, 0, 9);
        break;
      case lessym:
        gen(OPR, 0, 10);
        break;
      case leqsym:
        gen(OPR, 0, 11);
        break;
      case gtrsym:
        gen(OPR, 0, 12);
        break;
      case geqsym:
        gen(OPR, 0, 13);
        break;
    }
  }
  if (DEBUG) printf(ANSI_COLOR_CYAN"exit_condition()\n"ANSI_COLOR_RESET);
}

void expression() {
  if (DEBUG) printf(ANSI_COLOR_CYAN"expression()\n"ANSI_COLOR_RESET);
  // Trying to do an op code gen
  tokenType tmpOp;

  if ( t.type == plussym || t.type == minussym )
  {
    tmpOp = t.type;
    if ( tmpOp == minussym )
    {
      getNextToken();
      term();
      gen(OPR, 0, 1);
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
      gen(OPR, 0, 2);
    else
      gen(OPR, 0, 3);

  }
  if (DEBUG) printf(ANSI_COLOR_CYAN"exit_expression()\n"ANSI_COLOR_RESET);
}

void term() {
  if (DEBUG) printf(ANSI_COLOR_CYAN"term()\n"ANSI_COLOR_RESET);

  tokenType tmpOp;

  factor();
  while ( t.type == multsym || t.type == slashsym )
  {
  	tmpOp = t.type;
    getNextToken();
    factor();
    if(tmpOp == multsym)
    	gen(OPR, 0, 4);//edit this to correct one
    else
    	gen(OPR, 0, 5);//edit this to correct one
  }
  if (DEBUG) printf(ANSI_COLOR_CYAN"exit_term()\n"ANSI_COLOR_RESET);
}

void factor() {
  if (DEBUG) printf(ANSI_COLOR_CYAN"factor()\n"ANSI_COLOR_RESET);

  int symIndex;

  if ( t.type == identsym )
  {
    symIndex = lookUp(t.name, level);

    if (symIndex == -1)
      error(11); // undeclared identifier

    if(symbolList[symIndex].kind == 1)
    	gen(LIT, 0, symbolList[symIndex].val);
    else
    	gen(LOD, level - symbolList[symIndex].level, symbolList[symIndex].val);

    getNextToken();
	}

  else if ( t.type == numbersym )
  {
  	gen(LIT, 0, atoi(t.name));
    getNextToken();
  }

  else if ( t.type == callsym )
  {
    getNextToken();
    if ( t.type != identsym )
      error(4); // expected identifier

    // Check if the identifier is actually declared & a procedure
    symIndex = lookUp(t.name, level);
    // We shouldn't have to worry about this being out of bounds because of shortcircuiting
    if ( symIndex == -1 && symbolList[symIndex].kind != 3)
      error(36); // procedure undeclared
    else
    {
      // Will need to be changed for the parameter list implementation
      gen(CAL, level - symbolList[symIndex].level, symbolList[symIndex].addr );
      // recover the return value from the FV (offset zero) of the previous's activation record
      gen(INC, 0, 1);
    }

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

  if (DEBUG) printf(ANSI_COLOR_CYAN"exit_factor()\n"ANSI_COLOR_RESET);
}

void getNextToken() {
  if ( tokenCounter > tokenCount )
  {
    exit(0);
  }
  t = lexList[tokenCounter];
  tokenCounter++;
  if ( t.id != 0 )
    if (DEBUG) printf(ANSI_COLOR_GREEN"[%d] %s (%s)\n"ANSI_COLOR_RESET, t.id, symbolToString(t.id), t.name);
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
    case 36:
      fprintf(lexemeOutput, "%s", "Type error: CAL calls a undeclared procedure.");
      printf("Type error: CAL calls a undeclared procedure.\n");
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

void createSymbolList(){
  int c;
  FILE* ofp = fopen(PARSER_OUTPUT_SYMLIST, "w");
  // Header
  fprintf(ofp, "%-15s\t%-15s\t%-15s\t%-15s\n", "Name", "Type", "Level", "Value");
  for(c = 0; c < symbolCounter + 1; c++)
  {
    if (symbolList[c].kind == 0)
      continue;
    fprintf(ofp, "%-15s\t%-15s\t%-15d\t%-15d\n", symbolList[c].name, kindToString(symbolList[c].kind), symbolList[c].level, symbolList[c].val);
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
        if (DEBUG) printf(ANSI_COLOR_REDP"lookUp(name, level) = lookUp(%s, %d)\n"ANSI_COLOR_RESET, t.name, level);
        if (DEBUG) printf(ANSI_COLOR_CYAN"[name, level, value, addr]: (%s, %d, %d, %d)\n"ANSI_COLOR_RESET, symbolList[i].name, symbolList[i].level, symbolList[i].val, symbolList[i].addr);
        return i;
      }
    }
    level--;
  }
  return -1;
}

// If the symbol does not exist yet,
void insertSymbol(int kind, const char* name, int val, int level, int addr) {
  if (DEBUG) printf(ANSI_COLOR_PURPLE"insertSymbol(%s, %s, %d, %d)\n"ANSI_COLOR_RESET, kindToString(kind), name, level, val);

  // Find the location in the symbol list, or find an empty slot for it
  int location = symbolCounter++;

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
  if (DEBUG) printf(ANSI_COLOR_YELLOW"gen(%s, %d, %d)\n"ANSI_COLOR_RESET, instructionToString(instruction), l, m);
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
  if (DEBUG) printf("Number of asm_lines: %d\n%*s%*s%*s%*s%*s\n____________________________________________\n", asm_line, 7, "Line", 15, "Instruction", 4, "L", 5,"M", 11, "OPR Code");
  for (i = 0; i < asm_line; i++)
  {
    if ( asm_code[i].instruction == 2) // if OPR
    {
      if (DEBUG) printf("%*d %*s %*d %*d %*s\n", 5, asm_code[i].addr, 9, instructionToString(asm_code[i].instruction), 10, asm_code[i].l, 4, asm_code[i].m, 7, operationToString(asm_code[i].m));
    }
    else
      if (DEBUG) printf("%*d %*s %*d %*d\n", 5, asm_code[i].addr, 9, instructionToString(asm_code[i].instruction), 10, asm_code[i].l, 4, asm_code[i].m);

    // Why does the gen function seem to insert anything into the asm_code, but should be the symbol table?
    fprintf(ofp, "%d %d %d\n", asm_code[i].instruction, asm_code[i].l, asm_code[i].m);
  }
  fclose(ofp);
}
