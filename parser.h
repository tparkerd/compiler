#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "color.h"

// Assume a token is always known

void parser();
void getNextToken();

const char* errorMessage(int n);

void parser() {
  // Get the first token in the program

  getNextToken();
  block();
  if (token.type != periodsym)
    error(9); // period expected

}

void block()
{
  if ( token.type == constsym )
  {
    while ( token.type != commasym )
    {
      getNextToken();
      if ( token.type != identsym )
        error(); // expected identifier

      getNextToken();
      if ( token.type != eqsym )
        error(); // expected equal sign

      getNextToken();
      if ( token.type != numbersym )
        error(); // expected number

      getNextToken();
      if ( token.type != semicolonsym )
        error(); // expected semicolon

      getNextToken();

    }

    if ( token.type == varsym )
    {
      while ( token.type == commasym )
      {
        getNextToken();
        if ( token.type != identsym )
          error(); // expected comma
        getNextToken();
      }

      if ( token.type != semicolonsym )
        error(); // expected semicolon

      getNextToken();
      block();

      if ( token.type != semicolonsym )
        error(); // expected semicolon

      getNextToken();
    }

    statement();
  }
}


void getNextToken() {

}

const char* errorMessage(int n) {
  switch(n)
  {
    case 0:
      break;
    case 1:
      return "Use = instead of :=.";
    case 2:
      return "= must be followed by a number.";
    case 3:
      return "Identifier ust be followed by =.";
    case 4:
      return "const, var, procedure must be followeed by identifier.";
    case 5:
      return "semicolon or comma missing.";
    case 6:
      return "Incorrect symbol after procedure declaration.";
    case 7:
      return "Statement expected.";
    case 8:
      return "Incorrect symbol after statement part in block.";
    case 9:
      return "Period expected.";
    case 10:
      return "Semicolon between statements missing.";
    case 11:
      return "Undeclared identifier.";
    case 12:
      return "Assignment to constant or procedure is not allowed.";
    case 13:
      return "Assignment operator expected.";
    case 14:
      return "call must be followed by an identifier.";
    case 15:
      return "Call of a constant or variable is meaningless.";
    case 16:
      return "then expected.";
    case 17:
      return "Semicolon or } expected.";
    case 18:
      return "do expected.";
    case 19:
      return "Incorrect symbol following statement.";
    case 20:
      return "Relational operator expected.";
    case 21:
      return "Expression must not contain a procedure identifier.";
    case 22:
      return "Right parenthesis missing.";
    case 23:
      return "The preceding factor cannot begin with this symbol.";
    case 24:
      return "An expression cannot begin with this symbol.";
    case 25:
      return "This number is too large.";
    default:
      return "Unknown error";
  }
}
