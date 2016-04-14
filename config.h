#ifndef CONFIG_H
#define CONFIG_H

#define DEBUG 1

// This file will keep all the FILE pointers, global counters, and whatnot

// Constants
  // VM
    #define MAX_CODE_LENGTH 500
    #define MAX_LEXI_LEVELS 3
    #define MAX_STACK_HEIGHT 2000
    //Char arrays to hold instructions and op codes
    const char* INSTRUCTSTRINGS[] = {"", "LIT", "OPR", "LOD", "STO", "CAL", "INC", "JMP", "JPC", "SIO", "SIO", "SIO"};
    const char* OPSTRINGS[] = {"RET", "NEG", "ADD", "SUB", "MUL", "DIV", "ODD", "MOD", "EQL", "NEQ", "LSS", "LEQ", "GTR", "GEQ"};
  // Lexical Analyzer
    #define MAX_FILE_LENGTH 25000
    #define MAX_VARIABLE_LENGTH 11
    #define MAX_NUM_LENGTH 5
    #define MAX_TOKEN_SPACE 50
    #define NUM_RESERVED_WORDS 14
    #define NUM_SPECIAL_SYMBOLS 16
    #define NUM_ERROR_TYPES 4


// Files
  // VM
    #define VM_INPUT "mcode.txt"
    #define VM_OUTPUT "stacktrace.txt"
    FILE *fileCode;
    FILE *fileTrace;
  // Lexical Analyzer


  // Structs & Enums
  // VM
    //Instruction registers
    typedef struct instr{
        int OP;
        int L;
        int M;
    } instruction;

    // Counts, Counters, Trackers
    // VM
    int codeSize = 0;
    int basePointer = 1;
    int stackPointer = 0;
    int programCounter = 0;
    instruction IR;
    int stack[MAX_STACK_HEIGHT];
    instruction code[MAX_CODE_LENGTH];




//Global variables






////////////////////////////////////////////////////////////////////////////////
// Lexical Analyzer
////////////////////////////////////////////////////////////////////////////////


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

// Output file: cleaninput.txt
FILE* cleanInputOutput;
FILE* lexemeTable;
FILE* lexemeList;

// Global Variables
char rawInput[MAX_FILE_LENGTH];
char cleanInput[MAX_FILE_LENGTH];
char textInput[MAX_FILE_LENGTH];

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


////////////////////////////////////////////////////////////////////////////////
// Parser
////////////////////////////////////////////////////////////////////////////////

#define MAX_SYMBOL_TABLE_SIZE 100
#define CODE_GEN_OUTPUT "mcode.txt"

typedef struct symbol {
  int kind;       // const = 1, var = 2, proc = 3
  char name[12];  // name up to 11 characters
  int val;        // number (ASCII value)
  int level;      // L level
  int addr;       // M address
} symbol;

FILE* parserInput;
FILE* symbolTable;

// Global Variables
int tokenCounter = 0;
int level = 0;
struct token t;
struct token lexList[MAX_FILE_LENGTH];
struct symbol symbolList[MAX_SYMBOL_TABLE_SIZE];


////////////////////////////////////////////////////////////////////////////////
// Code Generator
////////////////////////////////////////////////////////////////////////////////




#endif
