#ifndef CONFIG_H
#define CONFIG_H

// Show debug data
#define DEBUG 1

////////////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////////////
  // Lexical Analyzer
    #define MAX_FILE_LENGTH 25000 // file buffer for "code memory"
    #define MAX_VARIABLE_LENGTH 11  // longest an identifier can be
    #define MAX_NUM_LENGTH 5  // longest a number can be
    #define MAX_TOKEN_SPACE 50  // ???
    #define NUM_RESERVED_WORDS 14 // number of known reserved words
    #define NUM_SPECIAL_SYMBOLS 16  // number of known special symbols
    #define NUM_ERROR_TYPES 4 // number of known lexical errors
  // Parser
    #define MAX_SYMBOL_TABLE_SIZE 100
  // VM
    #define MAX_CODE_LENGTH 500
    #define MAX_LEXI_LEVELS 4
    #define MAX_STACK_HEIGHT 2000
    const char* INSTRUCTSTRINGS[] = {"", "LIT", "OPR", "LOD", "STO", "CAL", "INC", "JMP", "JPC", "SIO", "SIO", "SIO"};  //Char arrays to hold instructions and op codes
    const char* OPSTRINGS[] = {"RET", "NEG", "ADD", "SUB", "MUL", "DIV", "ODD", "MOD", "EQL", "NEQ", "LSS", "LEQ", "GTR", "GEQ"};

////////////////////////////////////////////////////////////////////////////////
// Files
////////////////////////////////////////////////////////////////////////////////
  // Lexical Analyzer
    #define LA_INPUT "input.txt"
    #define LA_OUTPUT_CLEANINPUT "cleaninput.txt"
    #define LA_OUTPUT_TABLE "lexemetable.txt"
    #define LA_OUTPUT_LIST "lexemelist.txt"
    FILE* cleanInputOutput;
    FILE* lexemeTable;
    FILE* lexemeList;
  // Parser
    #define PARSER_INPUT "lexemelist.txt"
    #define PARSER_OUTPUT_SYMLIST "symlist.txt"
    #define PARSER_OUTPUT_ASMCODE "mcode.txt"
    #define PARSER_LOG "parseoutput.txt"
    FILE* parserInput;
    FILE* symbolTable;
    FILE* codeGenOutput;
  // VM
    #define VM_INPUT "mcode.txt"
    #define VM_OUTPUT "stacktrace.txt"
    FILE *fileCode;
    FILE *fileTrace;

////////////////////////////////////////////////////////////////////////////////
// Structs & Enums
////////////////////////////////////////////////////////////////////////////////
  // Lexical Analyzer
    typedef enum {
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
  // Parser
    typedef struct symbol {
      int kind;       // const = 1, var = 2, proc = 3
      char name[12];  // name up to 11 characters
      int val;        // number (ASCII value)
      int level;      // L level
      int addr;       // M address
    } symbol;
    typedef struct asmCode {
      int addr;
      int instruction;
      int l;
      int m;
    } asmCode;
  // VM
    typedef struct instr {
        int OP;
        int L;
        int M;
    } instruction;

////////////////////////////////////////////////////////////////////////////////
// Counts, Counters, Trackers
////////////////////////////////////////////////////////////////////////////////

  // Lexical Analyzer
    char rawInput[MAX_FILE_LENGTH];
    char cleanInput[MAX_FILE_LENGTH];
    char textInput[MAX_FILE_LENGTH];
    struct token digit[MAX_FILE_LENGTH];  // Known digits, reserved words, and special symbols
    struct token reservedWords[MAX_FILE_LENGTH];
    struct token specialSymbols[MAX_FILE_LENGTH];
    struct token isError[MAX_FILE_LENGTH];
    token tokenStorage[MAX_FILE_LENGTH];    // Stores all known tokens
    int counter;  // Counter to move along the input file's contents, character by character
    int tokenCount;  // Number of known tokens
  // Parser
    struct token lexList[MAX_FILE_LENGTH];
    struct symbol symbolList[MAX_SYMBOL_TABLE_SIZE];
    struct asmCode asm_code[MAX_FILE_LENGTH];
    struct token t;
    int tokenCounter = 0;
    int level = -1;
    int jmpCodeAddr = 0;
    int asm_line = 0;
    int NEXT_CODE_ADDR;
    int symbolCounter = 0;
    int tmpM = 0;
  // VM
    int codeSize = 0;
    int basePointer = 1;
    int stackPointer = 0;
    int programCounter = 0;
    instruction IR;
    int stack[MAX_STACK_HEIGHT];
    instruction code[MAX_CODE_LENGTH];
#endif
