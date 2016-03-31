#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Limits
#define MAX_FILE_LENGTH 25000
#define MAX_VARIABLE_LENGTH 11
#define MAX_NUM_LENGTH 5
#define MAX_TOKEN_SPACE 50
#define NUM_RESERVED_WORDS 14
#define NUM_SPECIAL_SYMBOLS 16
#define NUM_ERROR_TYPES 4

typedef enum{
  nulsym = 1, identsym, numbersym, plussym, minussym, multsym, slashsym, oddsym,
  eqlsym, neqsym, lessym, leqsym, gtrsym, geqsym, lparentsym, rparentsym, commasym,
  semicolonsym, periodsym, becomessym, beginsym, endsym, ifsym, thensym, whilesym,
  dosym, callsym, constsym, varsym, procsym, writesym, readsym, elsesym, errsym
} tokenType;

typedef struct token {
    int id;
    char name[MAX_TOKEN_SPACE];
    tokenType type;
} token;

// Global Variables
// Input file: input.txt
char rawInput[MAX_FILE_LENGTH];
char cleanInput[MAX_FILE_LENGTH];
char textInput[MAX_FILE_LENGTH];

// Output file: cleaninput.txt
FILE* cleanInputOutput;
FILE* lexemeTable;
FILE* lexemeList;
// Known digits, reserved words, and special symbols
struct token digit[MAX_FILE_LENGTH];
struct token reservedWords[MAX_FILE_LENGTH];
struct token specialSymbols[MAX_FILE_LENGTH];
struct token isError[MAX_FILE_LENGTH];
// Stores all known tokens
token tokenStorage[MAX_FILE_LENGTH];

// Counter to move along the input file's contents, character by character
int counter;
// Number of known tokens
int tokenCount;

// Function Prototypes
// Initialize data: known reserved words, special symbols, and errors
void init();
void initReservedWords();
void initSpecialSymbols();
void initError();
// Empty out the token storage
void initTokenStorage();

// Reads raw input file, deletes comments, output lexeme data
void read();
void readRawFile();
void deleteComments();
void writeLexemeTable();
void writeLexemeList();

// Segment function
void lexicalanalyzer();

// Scan the input for tokens
void scan();
// Check if it is an identifier, wrapper function to isId
void isIdentifier(struct token* t, int inputPosition);
char* isId(int inputPosition, char* string, int length);
// Check if it is a reserved word, wrapper to isReserved
void isReservedWord(struct token* t, int inputPosition);
int isReserved(int inputPosition, char* string, int length);
// Check if it is a special symbol, wrapper to isSpecial
void isSpecialSymbol(struct token* t, int inputPosition);
int isSpecial(int inputPosition, char* string, int length);
// Check if it is a number, wrapper to isDigit
void isNumber(struct token* t, int inputPosition);
char* isDigit(int inputPosition, char* string, int length);

// Helper functions for readability and comparisons
int isWhiteSpace(char c);
int getLength(char* string);

void lexicalanalyzer() {
  init();
  read();
  scan();
  writeLexemeTable();
  writeLexemeList();
}

// Functions
void init() {
  initReservedWords();
  initSpecialSymbols();
  initTokenStorage();
  initError();
}

void initReservedWords() {
  // Const
  reservedWords[0].id = 28;
  reservedWords[0].type = constsym;
  strcpy(reservedWords[0].name, "const");

  // Var
  reservedWords[1].id = 29;
  reservedWords[1].type = varsym;
  strcpy(reservedWords[1].name, "var");

  // Procedure
  reservedWords[2].id = 30;
  reservedWords[2].type = procsym;
  strcpy(reservedWords[2].name, "procedure");

  // Call
  reservedWords[3].id = 27;
  reservedWords[3].type = callsym;
  strcpy(reservedWords[3].name, "call");

  // Begin
  reservedWords[4].id = 21;
  reservedWords[4].type = beginsym;
  strcpy(reservedWords[4].name, "begin");

  // End
  reservedWords[5].id = 22;
  reservedWords[5].type = endsym;
  strcpy(reservedWords[5].name, "end");

  // If
  reservedWords[6].id = 23;
  reservedWords[6].type = ifsym;
  strcpy(reservedWords[6].name, "if");

  // Then
  reservedWords[7].id = 24;
  reservedWords[7].type = thensym;
  strcpy(reservedWords[7].name, "then");

  // Else
  reservedWords[8].id = 33;
  reservedWords[8].type = elsesym;
  strcpy(reservedWords[8].name, "else");

  // While
  reservedWords[9].id = 25;
  reservedWords[9].type = whilesym;
  strcpy(reservedWords[9].name, "while");

  // Do
  reservedWords[10].id = 26;
  reservedWords[10].type = dosym;
  strcpy(reservedWords[10].name, "do");

  // Read
  reservedWords[11].id = 32;
  reservedWords[11].type = readsym;
  strcpy(reservedWords[11].name, "read");

  // Write
  reservedWords[12].id = 31;
  reservedWords[12].type = writesym;
  strcpy(reservedWords[12].name, "write");

  // Odd
  reservedWords[13].id = 8;
  reservedWords[13].type = oddsym;
  strcpy(reservedWords[13].name, "odd");
}

void initSpecialSymbols() {
  // plussym
  specialSymbols[0].id = 4;
  specialSymbols[0].type = plussym;
  strcpy(specialSymbols[0].name, "+");

  // minussym
  specialSymbols[1].id = 5;
  specialSymbols[1].type = minussym;
  strcpy(specialSymbols[1].name, "-");

  // multsym
  specialSymbols[2].id = 6;
  specialSymbols[2].type = multsym;
  strcpy(specialSymbols[2].name, "*");

  // slashsym
  specialSymbols[3].id = 7;
  specialSymbols[3].type = slashsym;
  strcpy(specialSymbols[3].name, "/");

  // eqlsym
  specialSymbols[4].id = 9;
  specialSymbols[4].type = eqlsym;
  strcpy(specialSymbols[4].name, "=");

  // neqsym
  specialSymbols[5].id = 10;
  specialSymbols[5].type = neqsym;
  strcpy(specialSymbols[5].name, "<>");

  // lessym
  specialSymbols[6].id = 11;
  specialSymbols[6].type = lessym;
  strcpy(specialSymbols[6].name, "<");

  // leqsym
  specialSymbols[7].id = 12;
  specialSymbols[7].type = leqsym;
  strcpy(specialSymbols[7].name, "<=");

  // gtrsym
  specialSymbols[8].id = 13;
  specialSymbols[8].type = gtrsym;
  strcpy(specialSymbols[8].name, ">");

  // geqsym
  specialSymbols[9].id = 14;
  specialSymbols[9].type = geqsym;
  strcpy(specialSymbols[9].name, ">=");

  // lparentsym
  specialSymbols[10].id = 15;
  specialSymbols[10].type = lparentsym;
  strcpy(specialSymbols[10].name, "(");

  // rparentsym
  specialSymbols[11].id = 16;
  specialSymbols[11].type = rparentsym;
  strcpy(specialSymbols[11].name, ")");

  // commasym
  specialSymbols[12].id = 17;
  specialSymbols[12].type = commasym;
  strcpy(specialSymbols[12].name, ",");

  // semicolonsym
  specialSymbols[13].id = 18;
  specialSymbols[13].type = semicolonsym;
  strcpy(specialSymbols[13].name, ";");

  // periodsym
  specialSymbols[14].id = 19;
  specialSymbols[14].type = periodsym;
  strcpy(specialSymbols[14].name, ".");

  // becomessym
  specialSymbols[15].id = 20;
  specialSymbols[15].type = becomessym;
  strcpy(specialSymbols[15].name, ":=");
}

void initTokenStorage() {
  int i;
  for (i = 0; i < MAX_FILE_LENGTH; i++)
    memset(&tokenStorage[i], 0, sizeof(tokenStorage[i]));
}

void initError() {
  isError[0].id = 34;
  isError[0].type = errsym;
  strcpy(isError[0].name, "Variable does not start with letter");

  isError[1].id = 35;
  isError[1].type = errsym;
  strcpy(isError[1].name, "Number should not exceed 5 digits in length");

  isError[2].id = 36;
  isError[2].type = errsym;
  strcpy(isError[2].name, "Identifier's name is too long");

  isError[3].id = 37;
  isError[3].type = errsym;
  strcpy(isError[3].name, "Invalid symbol found");
}

void read() {
  readRawFile();
  deleteComments();
}

void readRawFile() {
  // Read in contents of raw file
  FILE* ifp = fopen("input.txt", "r");
  // Starting index
  int i = 0;

  char tmp;
  // Read in all the contents of the file
  while ( fscanf(ifp, "%c", &tmp) != EOF ) {
      rawInput[i] = tmp;
      i++;
  }
  fclose(ifp);
}

void deleteComments(){
  int i, j;
  cleanInputOutput = fopen("cleaninput.txt", "w");

  // For each element in the array - 2
  for(i = 0; i < MAX_FILE_LENGTH; i++)
  {
    if(rawInput[i] == '/' && rawInput[i + 1] == '*')
    {
      j = i + 1;
      // while( !(rawInput[j] == '*' && rawInput[j + 1] == '/') )
      while( rawInput[j] != '*' || rawInput[j + 1] != '/' )
        j++;

      i = j + 1;
      continue;
    }
    cleanInput[i] = rawInput[i];
  }

  // Write file contents without comments to output file (cleaninput.txt)
  for(i = 0; i < MAX_FILE_LENGTH; i++)
  {
    // Bug fix: encoding issue where leading NULL characters would cause encoding errors
    if ( cleanInput[i] == 0 )
      continue;
    fprintf(cleanInputOutput, "%c", cleanInput[i]);
  }
  fclose(cleanInputOutput);
}

void scan() {
  // Start with an empty token list
  tokenCount = 0;
  // For each character in the input array (cleaninput.txt)
  for( counter = 0; counter < MAX_FILE_LENGTH - 1; counter++ )
  {
    if ( !isWhiteSpace(cleanInput[counter]) )
    {
      // Assume it is an invalid token type
      struct token t;

      // Check if the current character starts an identifier
      isIdentifier(&t, counter);
      if ( t.type != nulsym )
      {
        tokenStorage[tokenCount++] = t;
        counter += getLength(t.name) - 1;
        continue;
      }

      // Check if the current character starts a reserved word
      isReservedWord(&t, counter);
      if ( t.type != nulsym )
      {
        tokenStorage[tokenCount++] = t;
        counter += getLength(t.name) - 1;
        continue;
      }

      // Check if the current character starts a special symbol
      isSpecialSymbol(&t, counter);
      if ( t.type != nulsym )
      {
        tokenStorage[tokenCount++] = t;
        counter += getLength(t.name) - 1;
        continue;
      }

      // Check if the current character starts a number
      isNumber(&t, counter);
      if ( t.type != nulsym )
      {
        tokenStorage[tokenCount++] = t;
        counter += getLength(t.name) - 1;
        continue;
      }

      // Otherwise an invalid symbol was encountered
      memset(&t, 0, sizeof(t));
      t.name[0] = cleanInput[counter];
      t.name[1] = '\0';
      t.type = errsym;
      t.id = 37;
      tokenStorage[tokenCount++] = t;
    }
  }
}

void isIdentifier(struct token* t, int inputPosition) {
  // Create a temporary string to test the current possible token
  char string[MAX_TOKEN_SPACE];
  // Initialize the string to be empty
  memset(&string[0], 0, sizeof(string));

  // Get the return value of the check if it is a reserved word
  char* value = isId(inputPosition, string, 0);

  // If the token was not a valid idenfier, return a nulsym
  if ( strcmp(value, "") == 0 )
  {
    t->type = nulsym;
    return;
  }

  // A token was found, set its values; all identifiers have a token type of two
  t->id = 2;

  // Set type
  t->type = identsym;

  // Set the name to the identifier's value
  strcpy(t->name, value);

  // Check if the current identifier is actually a reserved word
  int i;
  for (i = 0; i < NUM_RESERVED_WORDS; i++)
  {
    // If the current reserved word matches the given value
    // Reset the type and id to match its respective reserved word
    if (strcmp(reservedWords[i].name, value) == 0)
    {
      t->type = reservedWords[i].type;
      t->id = reservedWords[i].id;
      return;
    }
  }

  // Create a temporary string to store what the token is as a number
  char* temp = (char*)malloc(MAX_TOKEN_SPACE * sizeof(char));
  temp = isDigit(inputPosition, temp, 0);
  // If the identifier is the same string as if it were a number, then
  // the token is actually a number, so classify it as a number
  if (strcmp(t->name, temp) == 0 )
  {
    // Set type to number
    t->type = numbersym;
    t->id = 3;
    // If the number exceeds five digits in length, throw an error
    if ( getLength(t->name) > MAX_NUM_LENGTH )
    {
      t->type = errsym;
      t->id = 35;
    }
    // Free up the memory for the temp string
    free(temp);
    return;
  }

  // If the first character of the possible identifier is not a letter,
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
        // Set the token as an error and its id
        t->type = errsym;
        t->id = 34;
        // Free up the memory for the temp string
        free(temp);
        return;
      }

  // If the identifier exceeds 11 characters in length, throw an error
  if ( getLength(t->name) > MAX_VARIABLE_LENGTH )
  {
    // Set the token type as an error and its id
    t->type = errsym;
    t->id = 36;
  }

  // Free up the memory for the temp string and return
  free(temp);
}

char* isId(int inputPosition, char* string, int length) {
  // Affix the next letter to the string
  string[length] = cleanInput[inputPosition];

  // If the current letter is not a letter, underscore, or number, we've reached the
  // end of the possible identifier.
  // A - Z || 0 - 9 || a - z || underscore
  if ( !( /* A - Z */
        (string[length] >= 65 && string[length] <= 90)
        || /* 0 - 9 */
        (string[length] >= 48 && string[length] <= 57)
        || /* a - z */
        ( string[length] >= 97 && string[length] <= 122)
        || /* underscore */
        (string[length] == 95)
        )
      )
      {
        // Explicitly end the string with '\0' as a precaution
        string[length] = '\0';
        // All the characters up until this point as a string
        return string;
      }

  // Otherwise, add the next letter to the string and try again
  return isId(inputPosition + 1, string, length + 1);
}

void isReservedWord(struct token* t, int inputPosition) {
  // Create a temporary string to test the current possible token
  char string[MAX_TOKEN_SPACE];
  // Initialize the string to be empty
  memset(&string, 0, sizeof(string));

  // Get the return value of the check if it is a reserved word
  int value = isReserved(inputPosition, string, 0);

  // If the token was not a valid reserved word, return it as a nulsym
  if ( value == 0 )
  {
    t->type = nulsym;
    return;
  }

  // Valid token was found, set its values
  int i;
  for ( i = 0; i < NUM_RESERVED_WORDS; i++ )
  {
    // If/when the reserved word is found, copy its name over to the struct token
    if ( reservedWords[i].id == value )
    {
      t->id = reservedWords[i].id;
      strcpy(t->name, reservedWords[i].name);
      t->type = reservedWords[i].type;
    }
  }
}

int isReserved(int inputPosition, char* string, int length) {
  // Length of string exceeds the max length of all known reserved words
  // or is an empty string, return false, as it is not a reserved word.
  // 10 is the max because procedure is the longest reserved word at 9
  // characters in length
  if (length > 10)
    return 0;

  // Affix the next letter to the string
  string[length] = cleanInput[inputPosition];

  // Check if the first character is lowercase and a letter
  // a - z
  if ( !( string[length] >= 97 && string[length] <= 122) )
    return 0;

  // Check the current string against all known reserved words
  int i;
  for (i = 0; i < NUM_RESERVED_WORDS; i++)
  {
    // If the current reserved word matches the given value
    if (strcmp(reservedWords[i].name, string) == 0)
      // Return the token type as int
      return reservedWords[i].id;
  }
  // Otherwise, add the next letter to the string and try again
  return isReserved(inputPosition + 1, string, length + 1);
}

void isSpecialSymbol(struct token* t, int inputPosition) {
  // Create a temporary string to test the current possible token
  char string[MAX_TOKEN_SPACE];
  // Initialize the string to be empty
  memset(&string, 0, sizeof(string));

  // Get the return value of the check if it is a special symbol
  int value = isSpecial(inputPosition, string, 0);

  // If the token was not a valid special symbol, return nulsym
  if ( value == 0 )
  {
    t->type = nulsym;
    return;
  }

  // Valid token was found, set its values
  t->id = value;

  // Get the name of the special symbol
  int i;
  for ( i = 0; i < NUM_SPECIAL_SYMBOLS; i++ )
  {
    // If/when the special symbol is found, copy its name over to the struct token
    if ( specialSymbols[i].id == value )
    {
      strcpy(t->name, specialSymbols[i].name);
      t->type = specialSymbols[i].type;
    }
  }
}

int isSpecial(int inputPosition, char* string, int length) {
  // Length of string exceeds the max length of all known special symbols
  // or is an empty string, return false, as it is not a special symbol.
  if (length > 2)
    return 0;

  // Affix the next letter to the string
  string[length] = cleanInput[inputPosition];

  // If the current character is NOT a special character
  // ( ) * + ,  - . / OR : ; < = >
  if ( !( (string[length] >= 40 && string[length] <= 47) || (string[length] >= 58 && string[length] <= 62) ) )
    return 0;

  // One character lookahead
  // If the next character happens to be '>' or '=' in case the symbol happens to be '<>', '<=', or '>='
  if ( (cleanInput[inputPosition + 1] == 61 || cleanInput[inputPosition + 1] == 62) )
    return isSpecial(inputPosition + 1, string, length + 1);

  // Check the current string against all known special symbols
  int i;
  for (i = 0; i < NUM_SPECIAL_SYMBOLS; i++)
  {
    // If the current special symbol matches the given value
    if (strcmp(specialSymbols[i].name, string) == 0)
      // Return the token type as int
      return specialSymbols[i].id;
  }
  // Otherwise, add the next letter to the string and try again
  return isSpecial(inputPosition + 1, string, length + 1);
}

void isNumber(struct token* t, int inputPosition) {
  // Create a temporary string to test the current possible token
  char string[MAX_TOKEN_SPACE];
  // Initialize the string to be empty
  memset(&string, 0, sizeof(string));

  char* value = isDigit(inputPosition, string, 0);

  // If the token was not a valid number, return nulsym
  if ( getLength(value) == 0 )
  {
    t->type = nulsym;
    return;
  }

  // Valid token was found, set its values
  // A number is stored with an token type of 3
  t->id = 3;

  // Set token's type to be a number
  t->type = numbersym;
}

char* isDigit(int inputPosition, char* string, int length) {
  // Affix the next letter to the string
  string[length] = cleanInput[inputPosition];

  // If it's not a number, return the value that's already known
  if ( !(cleanInput[inputPosition] >= 48 && cleanInput[inputPosition] <= 57) )
  {
    // Affix a null character to terminate the string, as a precaution
    string[length] = '\0';
    return string;
  }

  // Otherwise, add the next letter to the string and try again
  return isDigit(inputPosition + 1, string, length + 1);
}

void writeLexemeTable() {
  int i;
  // Create/open destination file: lexemetable.txt
  lexemeTable = fopen("lexemetable.txt", "w");

  // Header for valid tokens
  fprintf(lexemeTable, "%-15s%s%s", "Lexeme\t", "\tToken Type\n",
    "___________________________________________________________\n");

  // For each token...
  for (i = 0; i < counter; i++)
  {
    // If the current token is null, skip it
    if ( strcmp(tokenStorage[i].name, "") == 0 )
      continue;

    // If the current token is a valid token, display its values
    if(tokenStorage[i].id >= 0 && tokenStorage[i].id <= 33)
      fprintf(lexemeTable, "%-15s\t\t%-d\n", tokenStorage[i].name, tokenStorage[i].id);
  }

  // Header for error list
  fprintf(lexemeTable, "%-15s%s%s", "\n\nLexeme\t", "\t\tError Type\n",
   "___________________________________________________________\n");
   // For each token
  for(i = 0; i < counter; i++)
  {
    // If any error is found, display it at the end of the table
    if(tokenStorage[i].id == 34)
      fprintf(lexemeTable, "%-15s\t\t%s\n", tokenStorage[i].name, isError[0].name);
    else if(tokenStorage[i].id == 35)
      fprintf(lexemeTable, "%-15s\t\t%s\n", tokenStorage[i].name, isError[1].name);
    else if(tokenStorage[i].id == 36)
      fprintf(lexemeTable, "%-15s\t\t%s\n", tokenStorage[i].name, isError[2].name);
    else if(tokenStorage[i].id == 37)
      fprintf(lexemeTable, "%-15s\t\t%s\n", tokenStorage[i].name, isError[3].name);
  }
  // Close destination file
  fclose(lexemeTable);
}

void writeLexemeList() {
  int i;
  // Create/open destination file: lexemelist.txt
  lexemeList = fopen("lexemelist.txt", "w");

  // For each token...
  for(i = 0; i < counter; i++) {
    // If the current token is null, skip it
    if ( strcmp(tokenStorage[i].name, "") == 0 )
      continue;

    // Display each token with its appropriate values
    if(tokenStorage[i].id == 2) {
      fprintf(lexemeList, "%d %s ", tokenStorage[i].id, tokenStorage[i].name);
    }
    else if(tokenStorage[i].id == 3) {
      fprintf(lexemeList, "%d %s ", tokenStorage[i].id, tokenStorage[i].name);
    }
    else if(tokenStorage[i].id >= 0 && tokenStorage[i].id <= 33)
      fprintf(lexemeList, "%d ", tokenStorage[i].id);
  }

  // Header for error list
  fprintf(lexemeList, "%-15s%-5s%s", "\n\n\nLexeme", "\tError Type\n",
   "___________________________________________________\n");
  for(i = 0; i < counter; i++)
  {
  // If the current token threw an error, display it
  if(tokenStorage[i].id == 34)
    fprintf(lexemeList, "%-15s\t%s\n", tokenStorage[i].name, isError[0].name);
  else if(tokenStorage[i].id == 35)
    fprintf(lexemeList, "%-15s\t%s\n", tokenStorage[i].name, isError[1].name);
  else if(tokenStorage[i].id == 36)
    fprintf(lexemeList, "%-15s\t%s\n", tokenStorage[i].name, isError[2].name);
  else if(tokenStorage[i].id == 37)
    fprintf(lexemeList, "%-15s\t%s\n", tokenStorage[i].name, isError[3].name);
  }

  // Close destination file
  fclose(lexemeList);
}

int isWhiteSpace(char c) {
  // If null OR horizontal tab, new line, vertical tab, form feed, carriage return
  // or a space, return true
  if ( (c == 0) || (c >= 9 && c <= 13) || (c == 32) )
    return 1;
  // Otherwise return false
  return 0;
}

int getLength(char* string) {
  // Get length of the string
  // Until we reach the end of the name/value of the token,
  // keep a count of the number of letters so far until
  // we reach the terminating character \0
  int i = 0;
  while ( string[i] != '\0')
  {
      i++;
  }
  return i;
}
