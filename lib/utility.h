#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ANSI_COLOR_RESET        "\x1b[0m"
#define ANSI_COLOR_BLACK        "\x1b[30m"
#define ANSI_COLOR_DARKRED      "\x1b[31m"
#define ANSI_COLOR_FORESTGREEN  "\x1b[32m"
#define ANSI_COLOR_BROWN        "\x1b[33m"
#define ANSI_COLOR_BLUE         "\x1b[34m"
#define ANSI_COLOR_MAGENTA      "\x1b[35m"
#define ANSI_COLOR_WHITE        "\x1b[37m"
#define ANSI_COLOR_GRAY         "\x1b[90m"
#define ANSI_COLOR_REDP         "\x1b[91m"
#define ANSI_COLOR_GREEN        "\x1b[92m"
#define ANSI_COLOR_YELLOW       "\x1b[93m"
#define ANSI_COLOR_PURPLE       "\x1b[94m"
#define ANSI_COLOR_PINK         "\x1b[95m"
#define ANSI_COLOR_CYAN         "\x1b[96m"

#define MAX_FILE_LENGTH 500
#define NUM_IS_DIGIT 10
#define NUM_RESERVED_WORDS 14
#define NUM_SPECIAL_SYMBOLS 16
#define INVALID_NUM 100000

// Errors
#define NUM_ERROR_TYPES 30
#define MAX_VARIABLE_LENGTH 11
#define ERROR_EXCEED_MAX_DIGITS 999999

typedef enum{
    nulsym = 1, identsym, numbersym, plussym, minussym, multsym, slashsym, oddsym,
    eqlsym, neqsym, lessym, leqsym, gtrsym, geqsym, lparentsym, rparentsym, commasym,
    semicolonsym, periodsym, becomessym, beginsym, endsym, ifsym, thensym, whilesym,
    dosym, callsym, constsym, varsym, procsym, writesym, readsym, elsesym
} tokenType;

typedef struct token {
    int id;
    char name[20];
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
// Stores all known tokens
token tokenStorage[MAX_FILE_LENGTH];

// Counter to move along the input file's contents, character by character
int counter;
int tokenCount = 0;
int error = 0;

// Function Prototypes
// Initialize data: known digits, reserved words, and symbols
void init();
void initDigits();
void initReservedWords();
void initSpecialSymbols();
void initTokenStorage();

// Reads raw input file, deletes comments, and removes whitespace
void read();
void readRawFile();
void deleteComments();
void removeWhiteSpace();
void writeLexemeTable();
void writeLexemeList();

// Scan to understand the code (previously known as covertascii)
void scan();
struct token* isIdentifier(struct token* t, int inputPosition);
char* isId(int inputPosition, char* string, int length);
struct token* isReservedWord(struct token* t, int inputPosition);
int isReserved(int inputPosition, char* string, int length);
struct token* isSpecialSymbol(struct token* t, int inputPosition);
int isSpecial(int inputPosition, char* string, int length);
struct token* isNumber(struct token* t, int inputPosition);
int isDigit(int inputPosition, int length, int total);

// Helper functions for readability and common use
int isWhiteSpace(char c);
int getLength(char* string);
const char* getErrorMessage(int id);

// Functions

void init() {
  initDigits();
  initReservedWords();
  initSpecialSymbols();
  initTokenStorage();
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

void initDigits() {
    // Zero
    digit[0].id = 3;
    strcpy(digit[0].name, "0");

    // One
    digit[1].id = 3;
    strcpy(digit[1].name, "1");

    // Two
    digit[2].id = 3;
    strcpy(digit[2].name, "2");

    // Three
    digit[3].id = 3;
    strcpy(digit[3].name, "3");

    // Four
    digit[4].id = 3;
    strcpy(digit[4].name, "4");

    // Five
    digit[5].id = 3;
    strcpy(digit[5].name, "5");

    // Six
    digit[6].id = 3;
    strcpy(digit[6].name, "6");

    // Seven
    digit[7].id = 3;
    strcpy(digit[7].name, "7");

    // Eight
    digit[8].id = 3;
    strcpy(digit[8].name, "8");

    // Nine
    digit[9].id = 3;
    strcpy(digit[9].name, "9");
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
    {
        tokenStorage[i].id = 0;
        memset(&tokenStorage[i].name[0], 0, sizeof(tokenStorage[i].name));
    }
}

const char* getErrorMessage(int id) {
  switch (id)
  {
    // No error - should *not* need this, but just in case
    case 0:
      return "";

    // Assigned a value to a variable using =.
    case 1:
      return "Use = instead of :=.";

    // Syntax error near constant declarations or in a conditional expression
    case 2:
      return "= must be followed by a number.";

    // Syntax error near constant declarations.
    case 3:
      return "Identifier must be followed by =.";

    // Syntax error near constant, variable, or procedure declarations.
    case 4:
      return "const, int, procedure must be followed by identifier.";

    // You missed a semicolon or a comma somewhere. Also check that you aren’t
    // adding extra semicolons to if-then-else and while-do’s.
    case 5:
      return "Semicolon or comma missing.";

    // Not used
    case 6:
      return "";

    // Not used
    case 7:
      return "";

    // Not used
    case 8:
      return "";

    // Missing a period at the end of the program.
    case 9:
      return "Period missing.";

    // Except for the last one in a block, every statement needs to end with a semicolon.
    case 10:
      return "Semicolon between statements missing.";

    // You tried to use an undeclared constant, variable, or procedure, or you tried to
    // access something that is outside of your scope.
    case 11:
      return "Undeclared identifier.";

    // You tried to assign a value to a constant or a procedure. Check your variable names.
    case 12:
      return "Assignment to constant or procedure is not allowed.";

    // You began a statement with an identifier,   but it wasn’t followed by an assignment
    // operator (:=).
    case 13:
      return "Assignment operator expected.";

    // You used call, but you didn’t include the procedure name.
    case 14:
      return "Call must be followed by an identifier.";

    // You tried to call a constant or a variable, which is meaningless.
    case 15:
      return "Call of a constant or variable is meaningless.";

    // if [condition] must be followed by then [statement].
    case 16:
      return "then expected.";

    // Not used
    case 17:
      return "";

    // while [condition] must be followed by do [statement].
    case 18:
      return "do expected.";

    // Not used
    case 19:
      return "";

    // In a conditional expression, you are missing a relational operator.
    case 20:
      return "Relational operator expected.";

    // You cannot use procedures in expressions (since they do not return or represent
    // values).
    case 21:
      return "Expression must not contain a procedure identifier.";

    // Missing the right parenthesis at the end of a factor.
    case 22:
      return "Right parenthesis missing.";

    // There is something wrong with a factor used in an expression.
    case 23:
      return "The preceding factor cannot begin with this symbol.";

    // Not used
    case 24:
      return "";

    // Code generator exceeded the maximum number of lines of code.
    case 25:
      return "This number is too large.";

    // You used out, but didn’t specify anything to output.
    case 26:
      return "out must be followed by an expression.";

    // You used in, but you didn’t specify what variable to assign it to.
    case 27:
      return "in must be followed by an identifier.";

    // Not used
    case 28:
      return "";

    // Constants cannot be redefined.
    case 29:
      return "Cannot redefine constants.";

    // Unknown error thrown
    default:
      return "";
  }
}

void read() {
  readRawFile();
  deleteComments();
  // removeWhiteSpace();
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

void removeWhiteSpace() {
    int i;
    for (i = 0; i < MAX_FILE_LENGTH - 1; i++)
    {
        //Ignore spaces
        if (cleanInput[i] == ' ')
            continue;
        //Ignore  newline feeds
        if (cleanInput[i] == '\n')
            continue;
        //Ignore horizontal tabs
        if (cleanInput[i] == '\t')
            continue;
        //Ignore carriage return
        if (cleanInput[i] == '\r')
            continue;

        // Otherwise, send the current character to the character only array
        textInput[i] = cleanInput[i];
    }

    // Debug: Show the file contents without white space
    printf(ANSI_COLOR_MAGENTA"File contents without white space\n"ANSI_COLOR_RESET);
    for (i = 0; i < MAX_FILE_LENGTH - 1; i++)
    {
        printf("%c", textInput[i]);
    }
    printf("\n");
}

struct token* isReservedWord(struct token* t, int inputPosition) {
  // Create a temporary string to test the current possible token
  char string[20];
  // Initialize the tempString to be empty
  memset(&string[0], 0, sizeof(string));

  // Get the return value of the check if it is a reserved word
  int value = isReserved(inputPosition, string, 0);

  // If the token was not a valid reserved word, return null (false condition for
  // the scan() function.)
  if ( value == 0 )
  {
    t->type = nulsym;
    return t;
  }

  // Valid token was found, set its values
  t->id = value;

  // Get the name of the reserved word
  int i;
  for ( i = 0; i < NUM_RESERVED_WORDS; i++ )
  {
    // If the reserved word is found, copy its name over to the struct token
    if ( reservedWords[i].id == value )
    {
      strcpy(t->name, reservedWords[i].name);
      t->type = reservedWords[i].type;
    }
  }

  // Otherwise return the address of the struct that contains all the data about
  // the valid token
  return t;
}

int isReserved(int inputPosition, char* string, int length) {
  // Length of string exceeds the max length of all known reserved words
  // or is an empty string, return false, as it is not a reserved word.
  if (length > 10)
    return 0;

  // Affix the next letter to the string
  string[length] = cleanInput[inputPosition];

  // If the string doesn't start with a letter or underscore, return false
  // *only check when the string has one character
  if ( length == 0 )
    // A - Z || a - z || underscore
    if ( !((string[0] >= 65 && string[0] <= 90) || ( string[0] >= 97 && string[0] <= 122) || (string[0] == 95)) )
      return 0;

  // If the current letter is not a letter, underscore, or number, return false
  // A - Z || 0 - 9 || a - z || underscore
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

struct token* isNumber(struct token* t, int inputPosition) {
  // Get the return value of the check if it is a reserved word
  // Also, assume that the number is invalid, so if the string
  // is empty, the first digit will replace the string.
  int value = isDigit(inputPosition, 0, INVALID_NUM);

  // If the token was not a valid number word, return the placeholder token
  // with type NULL
  if ( value == INVALID_NUM )
  {
    t->type = nulsym;
    return t;
  }

  // Valid token was found, set its values
  // A number is stored with an token type of 3
  t->id = 3;

  // Set token's type to be a number
  t->type = numbersym;

  // Its name is its value as a string
  // There's not a great way to convert an integer to a string, but
  // apparently there are a few available ones out there like sprintf
  // and snprintf
  //     (dest,format,value)
  sprintf(t->name, "%d", value);

  // Otherwise return the address of the struct that contains all the data about
  // the valid token
  return t;
}

int isDigit(int inputPosition, int length, int total) {
  /*// Numbers cannot exceed five digits in length
  if ( length > 5 )
    return INVALID_NUM;*/

  // If it's not a number, return the value that's already known
  if ( !(cleanInput[inputPosition] >= 48 && cleanInput[inputPosition] <= 57) )
  {
    // We've reached the end of a number, so skip forward along the clean input array
    if ( total == INVALID_NUM )
      return total;
    else
      return total / 10;
  }

  // If the starting total is invalid, set it to the first digit of the number
  if ( total == INVALID_NUM )
    total = cleanInput[inputPosition] - '0';
  else
    total += cleanInput[inputPosition] - '0';

  total *= 10;

  return isDigit(inputPosition + 1, length + 1, total);
}

struct token* isSpecialSymbol(struct token* t, int inputPosition) {
  // Create a temporary string to test the current possible token
  char string[20];
  // Initialize the tempString to be empty
  memset(&string[0], 0, sizeof(string));

  // Get the return value of the check if it is a reserved word
  int value = isSpecial(inputPosition, string, 0);

  // If the token was not a valid special symbol word, return null (false condition for
  // the scan() function.)
  if ( value == 0 )
  {
    t->type = nulsym;
    return t;
  }

  // Valid token was found, set its values
  t->id = value;

  // Get the name of the reserved word
  int i;
  for ( i = 0; i < NUM_SPECIAL_SYMBOLS; i++ )
  {
    // If the reserved word is found, copy its name over to the struct token
    if ( specialSymbols[i].id == value )
    {
      strcpy(t->name, specialSymbols[i].name);
      t->type = specialSymbols[i].type;
    }
  }

  // Otherwise return the address of the struct that contains all the data about
  // the valid token
  return t;
}

int isSpecial(int inputPosition, char* string, int length) {
  // Length of string exceeds the max length of all known reserved words
  // or is an empty string, return false, as it is not a reserved word.
  if (length > 2)
  return 0;

  // Affix the next letter to the string
  string[length] = cleanInput[inputPosition];

  // If the string doesn't start with a special character, return false
  // *only check when the string has one character
  if ( length == 0 )
    // ( ) * + ,  - . / OR : ; < = >
    if ( !( (string[0] >= 40 && string[0] <= 47) || ( string[0] >= 58 && string[0] <= 62) ) )
      return 0;

  // If the current character is NOT a special character
  // ( ) * + ,  - . / OR : ; < = >
  if ( !( (string[length] >= 40 && string[length] <= 47) || (string[length] >= 58 && string[length] <= 62) ) )
    return 0;

  // Check the current string against all known reserved words
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

struct token* isIdentifier(struct token* t, int inputPosition) {
  // Create a temporary string to test the current possible token
  char string[20];
  // Initialize the tempString to be empty
  memset(&string[0], 0, sizeof(string));

  // Get the return value of the check if it is a reserved word
  char* value = isId(inputPosition, string, 0);

  // If the token was not a valid reserved word, return null (false condition for
  // the scan() function.)
  if ( strcmp(value, "") == 0 )
  {
    t->type = nulsym;
    return t;
  }

  // Valid token was found, set its values
  // All identifiers have a token type of two
  t->id = 2;

  // Set type
  t->type = identsym;

  // Set the name to the identifier's value
  strcpy(t->name, value);

  // Otherwise return the address of the struct that contains all the data about
  // the valid token
  return t;
}

char* isId(int inputPosition, char* string, int length) {
  // Bug fix: Invalid character (ignores leading zeros)
  if (cleanInput[inputPosition] == 0)
    return string;

  // Affix the next letter to the string
  string[length] = cleanInput[inputPosition];

    // Length of string exceeds the max length allotted to identifiers
    if (length > MAX_VARIABLE_LENGTH)
      return (char*)"";

  // If the string doesn't start with a letter or underscore, return false
  // *only check when the string has one character
  if ( length == 1 )
    // A - Z || a - z || underscore
    if ( !((string[0] >= 65 && string[0] <= 90) || ( string[0] >= 97 && string[0] <= 122) || (string[0] == 95)) )
      return (char*)"";

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
        string[length] = '\0';
        return string;
      }

  // Otherwise, add the next letter to the string and try again
  return isId(inputPosition + 1, string, length + 1);
}

int getLength(char* string) {
    // Get length of the string
    // Until we reach the end of the name/value of the token,
    // keep a count of the number of letters so far until
    // we reach the terminating character \0
    int i = 0;
    do
        i++;
    while ( string[i] != '\0');
    return i;
}

void writeLexemeTable() {
    int i;
    lexemeTable = fopen("lexemetable.txt", "w");
    fprintf(lexemeTable, "%-15s%s", "Lexeme\t", "Token Type\n");
    for (i = 0; i < counter; i++)
    {
        if ( strcmp(tokenStorage[i].name, "") == 0 )
            continue;
        printf("%-15s\t%-d\n", tokenStorage[i].name, tokenStorage[i].id);
        fprintf(lexemeTable, "%-15s\t%-d\n", tokenStorage[i].name, tokenStorage[i].id);
    }
    fclose(lexemeTable);
}

void writeLexemeList() {
    int i;
    lexemeList = fopen("lexemelist.txt", "w");
    for(i = 0; i < counter; i++) {
        if ( strcmp(tokenStorage[i].name, "") == 0 )
            continue;
        if(tokenStorage[i].id == 2) {
            fprintf(lexemeList, "%d %s ", tokenStorage[i].id, tokenStorage[i].name);
        }
        else if(tokenStorage[i].id == 3) {
            fprintf(lexemeList, "%d %s ", tokenStorage[i].id, tokenStorage[i].name);
        }
        else
            fprintf(lexemeList, "%d ", tokenStorage[i].id);
    }
    fclose(lexemeList);
}

int isWhiteSpace(char c) {
    if ( (c == 0) || (c >= 9 && c <= 13) || (c == 32) )
        return 1;
    return 0;
}

#endif