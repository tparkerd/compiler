#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Debug flag
#define DEBUG 1

#define MAX_FILE_LENGTH 500

typedef enum{
    nulsym = 1, identsym, numbersym, plussym, minussym, multsym, slashsym,
    oddsym, eqlsym, neqsym, lessym, leqsym, gtrsym, geqsym, lparentsym,
    rparentsym, commasym, semicolonsym, periodsym, becomessym, beginsym, endsym,
    ifsym, thensym, whilesym, dosym, callsym, constsym, varsym, procsym, writesym,
    readsym, elsesym, errsym, newlinesym
} tokenType;

typedef struct token {
    int id;
    char name[20];
} token ;

struct token* reservedWords;

char rawInput[MAX_FILE_LENGTH];
char cleanInput[MAX_FILE_LENGTH];
char textInput[MAX_FILE_LENGTH];

// Debug functions
void initReservedWords();
void readRawFile();
void deleteComments();
void removeWhiteSpace();
void process();
int lookUpWord(char* word);

int main() {

  initReservedWords();
  readRawFile();
  deleteComments();
  removeWhiteSpace();
  process();


  return 0;
}

void initReservedWords() {
    // Allocate space for a list of reserved words
    struct token* rWordList = malloc(20 * sizeof(struct token));

    // Const
    rWordList[0].id = 28;
    strcpy(rWordList[0].name, "const");

    // Var
    rWordList[1].id = 29;
    strcpy(rWordList[1].name, "var");

    // Procedure
    rWordList[2].id = 30;
    strcpy(rWordList[2].name, "procedure");

    // Call
    rWordList[3].id = 27;
    strcpy(rWordList[3].name, "call");

    // Begin
    rWordList[4].id = 21;
    strcpy(rWordList[4].name, "begin");

    // End
    rWordList[5].id = 22;
    strcpy(rWordList[5].name, "end");

    // If
    rWordList[6].id = 23;
    strcpy(rWordList[6].name, "if");

    // Then
    rWordList[7].id = 24;
    strcpy(rWordList[7].name, "then");

    // Else
    rWordList[8].id = 33;
    strcpy(rWordList[8].name, "else");

    // While
    rWordList[9].id = 25;
    strcpy(rWordList[9].name, "while");

    // Do
    rWordList[10].id = 26;
    strcpy(rWordList[10].name, "do");

    // Read
    rWordList[11].id = 32;
    strcpy(rWordList[11].name, "read");

    // Write
    rWordList[12].id = 31;
    strcpy(rWordList[12].name, "write");

    // Odd
    rWordList[13].id = 8;
    strcpy(rWordList[13].name, "odd");

    reservedWords = rWordList;
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

    for (i = 0; i < MAX_FILE_LENGTH - 1; i++)
    {
        printf("%c", rawInput[i]);
    }
}

void deleteComments(){
  int i, j;
  FILE* cleanInputOutPut;
  cleanInputOutPut = fopen("cleaninput.txt", "w");
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
  for ( i = 0; i < MAX_FILE_LENGTH; i++ ) {
    printf("%c", cleanInput[i]);
    fprintf(cleanInputOutPut, "%c", cleanInput[i]);
  }
}

void removeWhiteSpace() {

  int i;
  for (i = 0; i < MAX_FILE_LENGTH - 1; i++)
  {
    // Ignore spaces
    if (cleanInput[i] == ' ')
      continue;
    // Ignore  newline feeds
    if (cleanInput[i] == '\n')
      continue;
    // Ignore horizontal tabs
    if (cleanInput[i] == '\t')
      continue;
    // Ignore carriage return
    if (cleanInput[i] == '\r')
      continue;

    // Otherwise, send the current character to the character only array
    textInput[i] = cleanInput[i];
  }

  // Debug: show the text without any white space
  for (i = 0; i < MAX_FILE_LENGTH - 1; i++)
  {
    printf("%c", textInput[i]);
  }
  printf("\n");
}

void process()
{
    // Initialize the first token
    int token = (int)cleanInput[0];

    // Test each individual character to see what it does!
    // *P*osition
    int i, p;
    // 11 is the maximum size of a variable name
    char* tmp = calloc(12, sizeof(char));
    // Run down the entire array for clean input
    for ( i = 0; i < MAX_FILE_LENGTH; i++ )
    {
        // Found a letter
        if ( 65 >= cleanInput[i] && 122 <= cleanInput[i] )
        {
            
        }
    }

    free(tmp);
}


int lookUpWord(char* word)
{

    return -1;
}
