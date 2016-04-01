#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVELS 3
#define MAX_AR_HEIGHT 6

typedef struct {
  int line; // Line of code
  int op;   // operation code
  int l;    // L
  int m;    // M
} instruction;

// Initial state of counters, instruction register, and activation records
int SP = 0;
int BP = 1;
int PC = 0;
instruction ir;
// Track the previous BP, and assume it's the same as the initial BP
int oldBP = 1;

// Assume that the code should not halt
int halt = 0;
// Allocate memory for activation records
int AR[MAX_AR_HEIGHT];
// Number of activation records
int nARs;
// Code Memory
instruction code[MAX_CODE_LENGTH];
// Stack
int stack[MAX_STACK_HEIGHT];

// Input File pointer
FILE* ifp;
// Output File Pointer
FILE* ofp;

void pmachine();

// Utility Functions
// Fetchs instruction: gets an instruction out of code memory, given the a
// program counter location (i.e., it's memory address in the code memory)
instruction fetch(int counter);
// Executes an instruction
void execute(instruction ir);
// Displays raw, numeric code with its appropriate name of each instruction, given
// the number of lines in the code
void displayCode(int nLines);
// Converts numeric instruction to its human readable name, given a line from the raw code
char const* interpret(int line);
// Find base L levels down (provided in assignment spec)
int base(int l, int m);
// Display the current stack
void printStack();

void pmachine() {
  // Open file containing numeric code
  ifp = fopen("mcode.txt", "r");
  ofp = fopen("stacktrace.txt", "w");

  // Initalize code to be empty
  int i;
  for (i = 0; i < MAX_CODE_LENGTH; i++) {
    code[i].op = 0;
    code[i].l = 0;
    code[i].m = 0;
  }

  // Read in raw data
  // i counts the number lines already read in from the file
  i = 0;
  while (!feof(ifp)) {
    fscanf(ifp, "%d %d %d", &code[i].op, &code[i].l, &code[i].m);
    code[i].line = i;
    // Increment the number of lines found
    i++;
  }

  // Close file, as it is no longer needed
  fclose(ifp);

  // Display the numeric code with their line
  displayCode(i);

  // Initialize stack
  for ( i = 0; i < MAX_STACK_HEIGHT; i++ ) {
      stack[i] = 0;
  }
  // Initialize activation record
  for (i = 0; i < MAX_AR_HEIGHT; i++) {
    AR[i] = 0;
  }

  // Display the headers and initial values
  fprintf(ofp, "\t\t\t\tPC\tBP\tSP\tstack \n");
  fprintf(ofp, "Initial values \t\t\t%d\t", PC);
  fprintf(ofp, "%d\t", BP);
  fprintf(ofp, "%d\t\n", SP);

  // Execute code

  // So long as the halt condition is not met, fetch an instruction from code
  while (!halt) {
    ir = fetch(PC);
    PC++;
    execute(ir);

    // If the SP, BP, and PC are all zero, that defines a halt condition
    // so set the halt state to TRUE
    if (BP == 0 && PC == 0)
    {
      halt = 1;
    }
  }
}

void displayCode(int nLines) {

    fprintf(ofp, "Line\tOP\tL\tM\n");

    int i;
    for (i = 0; i < nLines; i++) {
      fprintf(ofp, "%d\t%s\t%d\t%d\n", i, interpret(code[i].op), code[i].l, code[i].m);
    }

}
instruction fetch(int counter) {
  return code[counter];
}

/*
  Name: execute
  Parameters: instruction register pulled from code memory
  Description: processes instruction from memory and then prints the result
*/
void execute(instruction ir) {
  switch (ir.op) {
    // LIT: Push constant value (literal) M onto the stack
    case 1:
      SP++;
      stack[SP] = ir.m;
      break;
    // OPR: Operation to be performed on the data at the top of the stack
    case 2:
      switch (ir.m) {
        // Return
        case 0:
          SP = BP - 1;
          PC = stack[SP + 4];
          BP = stack[SP + 3];
          // Clear the previous AR from memory
          AR[nARs] = 0;
          // Since a return removes the last AR, decrement its count
          nARs--;
          break;
        // Negation
        case 1:
          SP--;
          stack[SP] = -stack[SP];
          break;
        // Addition
        case 2:
          SP--;
          stack[SP] += stack[SP + 1];
          break;
        // Subtraction
        case 3:
          SP--;
          stack[SP] -= stack[SP + 1];
          break;
        // Multiplication
        case 4:
          SP--;
          stack[SP] *= stack[SP + 1];
          break;
        // Divide
        case 5:
          SP--;
          stack[SP] /= stack[SP + 1];
          break;
        // Odd parity
        case 6:
          stack[SP] %= 2;
          break;
        // Mod
        case 7:
          SP--;
          stack[SP] %= stack[SP + 1];
          break;
        // Equality
        case 8:
          SP--;
          stack[SP] = (stack[SP] == stack[SP + 1]);
          break;
        // Not equal
        case 9:
          SP--;
          stack[SP] = (stack[SP] != stack[SP + 1]);
          break;
        // LSS
        case 10:
          SP--;
          stack[SP] = (stack[SP] < stack[SP + 1]);
          break;
        // LEQ
        case 11:
          SP--;
          stack[SP] = (stack[SP] <= stack[SP + 1]);
          break;
        // GTR
        case 12:
          SP--;
          stack[SP] = stack[SP] > stack[SP + 1];
          break;
        // GEQ
        case 13:
          SP--;
          stack[SP] = stack[SP] >= stack[SP + 1];
          break;
      }
      break;
    // LOD: Load value into top of stack from the stack location at offset
    // M from L lexicographical levels down
    case 3:
      SP++;
      stack[SP] = stack[base(ir.l, BP) + ir.m];
      break;
    // STO: Store value at the top of stack in the stack location at offset
    // from L lexicographical levels down
    case 4:
      stack[base(ir.l, BP) + ir.m] =  stack[SP];
      SP--;
      break;
    // CAL: Call procedure at code index M (generates new AR and PC <- M)
    case 5:
      stack[SP + 1] = 0;                // space to return value (FV)
      stack[SP + 2] = base(ir.l, BP);   // static link (SL)
      stack[SP + 3] = BP;               // dynamic link (DL)
      stack[SP + 4] = PC;               // return address (RA)
      oldBP = BP;
      BP = SP + 1;
      PC = ir.m;

      // Save the last AR as the BP, for piping
      AR[nARs] = BP;
      // Increment the number of ARs created
      nARs++;

      break;
    // INC: Allocate M locals (increment SP by M). First four are
    // Functional Value (FV) or return value (RV), Static Link (SL),
    // Dynamic Link (DL), and Return Address (RA).
    case 6:
      SP += ir.m;
      break;
    // JMP: Jump to instruction M
    case 7:
      PC = ir.m;
      break;
    // JPC: JUmp to instruction M if top of stack is ZERO
    case 8:
      if (stack[SP] == 0) {
        PC = ir.m;
      }
      SP--;
      break;
    // SIO
    // Case 1. Write the top stack element to the screen
    // Case 2. Read in input from the user and store it at the top of the stack
    // Case 3. Halt the machine
    case 9:
    case 10:
    case 11:
      switch(ir.m) {
        case 1:
          printf("%d ", stack[SP]);
          SP--;
          break;
        case 2:
          SP++;
          // read(stack[SP]); Not sure what to do with this, but it was given in the spec
          scanf("%d", &stack[SP]);
          break;
        case 3:
          halt = 1;
      }
      break;
  }

  // Display the details about the instruction, post-execution
  fprintf(ofp, "%d\t", ir.line );
  fprintf(ofp, "%s\t", interpret(ir.op) );
  fprintf(ofp, "%d\t", ir.l);
  fprintf(ofp, "%d\t", ir.m);
  fprintf(ofp, "%d\t", PC);
  fprintf(ofp, "%d\t", BP);
  fprintf(ofp, "%d\t", SP);
  // Display the stack
  printStack();
  fprintf(ofp, "\n");
}

/*
  Name: interpret
  Parameters: the operation code for an instruction
  Description: Returns a string constant to the caller based on the opcode provived.
   It is simply a utility function to clean up the execution function.
*/
char const* interpret( int opcode ) {
  switch ( opcode )
  {
    case 1:
        return "lit";
    case 2:
        return "opr";
    case 3:
        return "lod";
    case 4:
        return "sto";
    case 5:
        return "cal";
    case 6:
        return "inc";
    case 7:
        return "jmp";
    case 8:
        return "jpc";
    case 9:
        return "sio";
    case 10:
        return "sio";
    case 11:
        return "sio";
    case 12:
        return "neg";
    case 13:
        return "add";
    case 14:
        return "sub";
    case 15:
        return "mul";
    case 16:
        return "div";
    case 17:
        return "odd";
    case 18:
        return "mod";
    case 19:
        return "eql";
    case 20:
        return "neq";
    case 21:
        return "lss";
    case 22:
        return "leq";
    case 23:
        return "gtr";
    case 24:
        return "geq";
    default:
       return "error";
  }
}

/*
  Name: base
  Parameters: instruction L, base
  Description: provied by assignment spec
*/
int base( int l, int base ) {
  int b1; //find base L levels down
  b1 = base;
  while (l > 0)
    {
      b1 = stack[b1 + 1];
      l--;
    }
  return b1;
  }

/*
  Name: printStack
  Parameters: n/a
  Description: Prints the current contents of the stack and separates
    each activation record from one another with a pipe
*/
void printStack() {
  // Base case: Halt condition
  if (BP == 0) return;

  // Loop through the values within the stack before the SP's position
  int i, j;
  // Start at 1 to avoid printing out a pipe in front
  for(i = 1; i <= SP; i++)  {
    // For each one of the ARs, see if a pipe needs to be placed
    for (j = 0 ; j < nARs; j++) {
      // If the BP associated with the AR is the same as current BP, print a pipe
      if (AR[j] == i) {
        fprintf(ofp, "| ");
      }
    }
    // Print the item in the stack
    fprintf(ofp, "%d ", stack[i]);
  }
}
