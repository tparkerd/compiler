

// Global Variables
int tokenCounter = 0;
struct token t;

void parser();
void getNextToken();
void block();
void statement();
void condition();
void expression();
void term();
void factor();
void ifelse(); // this may need to be split into two things
void error();
const char* errorMessage(int n);


void parser() {
  // Get the first token in the program
  getNextToken();
  block();
  if (t.type != periodsym)
    error(); // #9, period expected

}

void block() {
  if ( t.type == constsym )
  {
    while ( t.type != commasym )
    {
      getNextToken();
      if ( t.type != identsym )
        error(); // expected identifier

      getNextToken();
      if ( t.type != eqlsym )
        error(); // expected equal sign

      getNextToken();
      if ( t.type != numbersym )
        error(); // expected number

      getNextToken();
      if ( t.type != semicolonsym )
        error(); // expected semicolon

      getNextToken();

    }

    if ( t.type == varsym )
    {
      while ( t.type == commasym )
      {
        getNextToken();
        if ( t.type != identsym )
          error(); // expected comma
        getNextToken();
      }

      if ( t.type != semicolonsym )
        error(); // expected semicolon

      getNextToken();
      block();

      if ( t.type != semicolonsym )
        error(); // expected semicolon

      getNextToken();
    }

    statement();
  }
}

void statement() {

  // If an identifier is found
  if ( t.type == identsym )
  {
    getNextToken();
    if ( t.type != becomessym )
      error(); // expected becomes symbol
    getNextToken();
    expression();

  // If a call is found instead
  } else if ( t.type == callsym )
  {
    getNextToken();
    if ( t.type != identsym )
      error(); // expected identifier
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
      error(); // expected end symbol

    getNextToken();

  // Else if ifsym instead of begin, identifier, or call
  } else if ( t.type == ifsym )
  {
    getNextToken();
    condition();
    if ( t.type != thensym )
      error(); // expected then
    getNextToken();
    statement();

  } else if ( t.type == whilesym )
  {
    getNextToken();
    condition();
    if ( t.type != dosym )
      error(); // do expected

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
    // if (t.type != relation )
    //   error(); // expected a relational operator
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
      error(); // expected righthand parenthesis
    getNextToken();
  } else
    error(); // expected an identifier, numbers, or left parenthesis
}


void getNextToken() {
  t = tokenStorage[tokenCounter++];
}

void error() {
  printf(ANSI_COLOR_DARKRED"error\n"ANSI_COLOR_RESET);
}
