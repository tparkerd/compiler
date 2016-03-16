#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ANSI_COLOR_RESET         "\x1b[0m"
#define ANSI_COLOR_BLACK         "\x1b[30m"
#define ANSI_COLOR_DARKRED       "\x1b[31m"
#define ANSI_COLOR_FORESTGREEN   "\x1b[32m"
#define ANSI_COLOR_BROWN         "\x1b[33m"
#define ANSI_COLOR_BLUE          "\x1b[34m"
#define ANSI_COLOR_MAGENTA       "\x1b[35m"
#define ANSI_COLOR_WHITE         "\x1b[37m"
#define ANSI_COLOR_GRAY          "\x1b[90m"
#define ANSI_COLOR_REDP          "\x1b[91m"
#define ANSI_COLOR_GREEN         "\x1b[92m"
#define ANSI_COLOR_YELLOW        "\x1b[93m"
#define ANSI_COLOR_PURPLE        "\x1b[94m"
#define ANSI_COLOR_PINK          "\x1b[95m"
#define ANSI_COLOR_CYAN          "\x1b[96m"

#define MAX_FILE_LENGTH 500
#define NUM_IS_DIGIT 10
#define NUM_RESERVED_WORDS 14
#define NUM_SPECIAL_SYMBOLS 16

// Errors
#define NUM_ERROR_TYPES 32
#define MAX_VARIABLE_LENGTH 11
#define MAX_NUM_LENGTH 5

typedef enum{
  nulsym = 1, identsym, numbersym, plussym, minussym, multsym, slashsym, oddsym,
  eqlsym, neqsym, lessym, leqsym, gtrsym, geqsym, lparentsym, rparentsym, commasym,
  semicolonsym, periodsym, becomessym, beginsym, endsym, ifsym, thensym, whilesym,
  dosym, callsym, constsym, varsym, procsym, writesym, readsym, elsesym, errsym
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
// Number of known tokens
int tokenCount;
// Global flag for error handling
int error;

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
void isIdentifier(struct token* t, int inputPosition);
char* isId(int inputPosition, char* string, int length);
void isReservedWord(struct token* t, int inputPosition);
int isReserved(int inputPosition, char* string, int length);
void isSpecialSymbol(struct token* t, int inputPosition);
int isSpecial(int inputPosition, char* string, int length);
void isNumber(struct token* t, int inputPosition);
char* isDigit(int inputPosition, char* string, int length);

// Helper functions for readability and common use
int isWhiteSpace(char c);
int getLength(char* string);
const char* getErrorMessage(int id);


int main() {
  init();
  read();
  scan();
  writeLexemeTable();
  writeLexemeList();
  return 0;
}

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
    // Identifier starts with a number
    case 0:
      return "Variable does not start with letter";

    // Number exceeds five digits
    case 1:
      return "Number should not exceed 5 digits in length";

    // Identifier exceed 11 characters in length
    case 2:
        return "Identifier's name is too long";

    // Invalid symbol
    case 3:
      return "Invalid symbol found";

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

void scan() {
  // Start with an empty token list
  tokenCount = 0;
  // For each character in the input array (cleaninput.txt)
  for( counter = 0; counter < MAX_FILE_LENGTH - 1; counter++ )
  {
    if ( !isWhiteSpace(cleanInput[counter]) )
    {
      // Assume that there is no error until proven otherwise
      error = 0;
      // printf("Check: %c\n", cleanInput[counter]);
      // Assume it is an invalid token type
      struct token t;

      // Check if the current character starts a reserved word
      isReservedWord(&t, counter);
      if ( t.type != errsym )
      {
        printf(ANSI_COLOR_GREEN"%s\n"ANSI_COLOR_RESET, t.name);
        tokenStorage[tokenCount++] = t;
        counter += getLength(t.name) - 1;
        continue;
      }

      // Check if the current character starts an identifier
      isIdentifier(&t, counter);
      if ( t.type != errsym )
      {
        if ( t.type == numbersym )
          printf(ANSI_COLOR_PURPLE"%s\n"ANSI_COLOR_RESET, t.name);
        else
          printf(ANSI_COLOR_WHITE"%s\n"ANSI_COLOR_RESET, t.name);
        tokenStorage[tokenCount++] = t;
        counter += getLength(t.name) - 1;
        continue;
      }

      // Check if the current character starts a special symbol
      isSpecialSymbol(&t, counter);
      if ( t.type != errsym )
      {
        printf(ANSI_COLOR_YELLOW"%s\n"ANSI_COLOR_RESET, t.name);
        tokenStorage[tokenCount++] = t;
        counter += getLength(t.name) - 1;
        continue;
      }

      // Check if the current character starts a number
      isNumber(&t, counter);
      if ( t.type != errsym )
      {
        printf(ANSI_COLOR_PURPLE"%s\n"ANSI_COLOR_RESET, t.name);
        tokenStorage[tokenCount++] = t;
        counter += getLength(t.name) - 1;
        continue;
      }

      // Otherwise an invalid symbol was encountered
      error = 3;
      printf(ANSI_COLOR_DARKRED"%s: "ANSI_COLOR_RESET, getErrorMessage(error));
      printf(ANSI_COLOR_REDP"%c\n"ANSI_COLOR_RESET, cleanInput[counter]);
      t.name[0] = cleanInput[counter];
      t.type = errsym;
      t.id = 0;
      tokenStorage[tokenCount++] = t;

    }
  }
}

void isReservedWord(struct token* t, int inputPosition) {
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
    t->type = errsym;
    return;
  }

  // Valid token was found, set its values
  int i;
  for ( i = 0; i < NUM_RESERVED_WORDS; i++ )
  {
    // If the reserved word is found, copy its name over to the struct token
    if ( reservedWords[i].id == value )
    {
      t->id = reservedWords[i].id;
      strcpy(t->name, reservedWords[i].name);
      t->type = reservedWords[i].type;
    }
  }

  // Return the token with all its known values
  return;
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

void isNumber(struct token* t, int inputPosition) {
  // Create a temporary string to test the current possible token
  char string[20];
  // Initialize the tempString to be empty
  memset(&string[0], 0, sizeof(string));

  char* value = isDigit(inputPosition, string, 0);

  // If the token was not a valid number, return the placeholder token
  // with a errsym type
  if ( getLength(value) == 0 )
  {
    t->type = errsym;
    return;
  }

  // Valid token was found, set its values
  // A number is stored with an token type of 3
  t->id = 3;

  // Set token's type to be a number
  t->type = numbersym;

  // Otherwise return the address of the struct that contains all the data about
  // the valid token
  return;
}

char* isDigit(int inputPosition, char* string, int length) {
  // Affix the next letter to the string
  string[length] = cleanInput[inputPosition];

  // If it's not a number, return the value that's already known
  if ( !(cleanInput[inputPosition] >= 48 && cleanInput[inputPosition] <= 57) )
  {
    string[length] = '\0';
    return string;
  }

  // Otherwise, add the next letter to the string and try again
  return isDigit(inputPosition + 1, string, length + 1);
}

void isSpecialSymbol(struct token* t, int inputPosition) {
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
    t->type = errsym;
    return;
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
  return;
}

int isSpecial(int inputPosition, char* string, int length) {
  // Length of string exceeds the max length of all known reserved words
  // or is an empty string, return false, as it is not a reserved word.
  if (length > 2)
    return 0;

  // Affix the next letter to the string
  string[length] = cleanInput[inputPosition];

  // If the string doesn't start with a special character, return false
  // *only check when the string has one character -- not necessary though
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
    t->type = errsym;
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
    t->id = 3;
    // If the number exceeds five digits in length, throw an error
    if ( getLength(t->name) > MAX_NUM_LENGTH )
    {
        error = 1;
        printf(ANSI_COLOR_DARKRED"%s: "ANSI_COLOR_RESET, getErrorMessage(error));
        t->type = errsym;
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
        printf(ANSI_COLOR_DARKRED"%s: "ANSI_COLOR_RESET, getErrorMessage(error));
        // Free up the memory for the temp string
        free(temp);
        return;
      }

  if ( getLength(t->name) > 11 )
  {
      error = 2;
      printf(ANSI_COLOR_DARKRED"%s: "ANSI_COLOR_RESET, getErrorMessage(error));
  }

  // Free up the memory for the temp string
  free(temp);

  // Otherwise return the address of the struct that contains all the data about
  // the valid token
  return;
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
    while ( string[i] != '\0')
    {
        i++;
    }
    return i;
}

void writeLexemeTable() {
    int i;
    lexemeTable = fopen("lexemetable.txt", "w");
    fprintf(lexemeTable, "%-15s%s", "Lexeme\t", "Token Type\n");
    printf("%-15s%s", "Lexeme\t", "Token Type\n");
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
