#include "lib/utility.h"


int main() {
  init();
  read();
  scan();
  writeLexemeTable();
  writeLexemeList();
  return 0;
}


void isIdentifier(struct token* t, int inputPosition) {
  // Create a temporary string to test the current possible token
  char string[20];
  // Initialize the tempString to be empty
  memset(&string[0], 0, sizeof(string));

  // Get the return value of the check if it is a reserved word
  char* value = isId(inputPosition, string, 0);

  // If the token was not a valid idenfier, return a null sym
  if ( strcmp(value, "") == 0 )
  {
    t->type = nulsym;
    return;
  }

  // Valid token was found, set its values
  // All identifiers have a token type of two
  t->id = 2;

  // Set type
  t->type = identsym;

  // Set the name to the identifier's value
  strcpy(t->name, value);

  // Check if the token is actually a number instead of identifier
  char* temp = (char*)malloc(20 * sizeof(char));
  temp = isDigit(inputPosition, temp, 0);
  // If the length of the string is the same as if it were made up of only
  // digits, it is safe to assume it is a number
  if (strcmp(t->name, temp) == 0 )
  {
    // Set type to number
    t->type = numbersym;
    // If the number exceeds five digits in length, throw an error
    if ( getLength(t->name) > 5 )
    {
        error = 1;
        printf("%s: ", getErrorMessage(error));
        t->type = nulsym;
    }
    // Free up the memory for the temp string
    free(temp);
    return;
  }

  // If the first character of the possible identifier is not a letter
  // throw an error that it is no longer a valid one
  if ( !( /* A - Z */
        (t->name[0] >= 65 && t->name[0] <= 90)
        || /* a - z */
        ( t->name[0] >= 97 && t->name[0] <= 122)
        || /* underscore */
        (t->name[0] == 95)
        )
      )
      {
        error = 0;
        printf("%s: %s\n", t->name, getErrorMessage(error));
        // Free up the memory for the temp string
        free(temp);
        return;
      }

  if ( getLength(t->name) > 11 )
  {
      error = 2;
      printf("%s: %s\n", t->name, getErrorMessage(error));
  }

  // Free up the memory for the temp string
  free(temp);

  // Otherwise return the address of the struct that contains all the data about
  // the valid token
  return;
}
