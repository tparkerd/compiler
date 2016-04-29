#ifndef CONFIG_H
#define CONFIG_H

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
      nulsym = 1, identsym = 2, numbersym = 3, plussym = 4, minussym = 5,
      multsym = 6, slashsym = 7, oddsym = 8, eqlsym = 9, neqsym = 10,
      lessym = 11, leqsym = 12, gtrsym = 13, geqsym = 14, lparentsym = 15,
      rparentsym = 16, commasym = 17, semicolonsym = 18, periodsym = 19,
      becomessym = 20, beginsym = 21, endsym = 22, ifsym = 23, thensym = 24,
      whilesym = 25, dosym = 26, callsym = 27, constsym = 28, varsym = 29,
      procsym = 30, writesym = 31, readsym = 32, elsesym = 33, errsym = 34
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
    typedef enum {
      LIT = 1, OPR = 2, LOD = 3, STO = 4, CAL = 5, INC = 6, JMP = 7, JPC = 8,
      WRITE = 9, READ = 10, HALT = 11 // SIO 0 1, SIO 0 2, and SIO 0 3
    } mcodeOperation;
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
