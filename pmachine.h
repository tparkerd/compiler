//function prototypes
void codeTrace();
void executeCycle(int operation);
void fetchCycle();
int getBase(int basePlace, int base);
void loadFile();
void opCodes();
void pipeLaying();
void printOut(int operation);
void runCode(int operation, int operation2);
void virtualMachine(int operation);
void pmachine();


//Takes the code file you read into it and prints into the filetrace the steps it's taking
void codeTrace(int operation) {
    int i;
    for(i = 0; i < codeSize; i++){
        fprintf(fileTrace,"%3d    %s %2d   %3d\n", i, INSTRUCTSTRINGS[code[i].OP], code[i].L, code[i].M);
    }
}

void executeCycle(int operation){
    int value = 0, i = 0;
    switch (IR.OP) {
        case 1: //LIT - Pushes constant value M on top of the stack
            stackPointer++;
            stack[stackPointer] = IR.M;
            break;
        case 2: //OPR - Gets op code to be performed on the data at the top of the stack
            opCodes();
            break;
        case 3: //LOD - Load value from into top of stack from location offset at M from L levels down
            (stackPointer)++;
            stack[stackPointer] = stack[getBase(IR.L, basePointer) + IR.M];
            break;
        case 4: //STO - Store value at top of stack in stack location offset by M
            if(IR.L != 0)
                stack[getBase(IR.L, basePointer) + IR.M] = stack[stackPointer];
            else
                stack[getBase(IR.L+0, basePointer) + IR.M] = stack[stackPointer];
            stackPointer--;
            break;
        case 5://INC
            stack[stackPointer + 1] = 0;//Return Value
            stack[stackPointer + 2] = getBase(IR.L, basePointer);//Static Link
            stack[stackPointer + 3] = basePointer;//Dynamic Link
            stack[stackPointer + 4] = programCounter;//Return Address
            basePointer = stackPointer + 1;
            programCounter = IR.M;
            break;
        case 6://CAL - Generates new activation record
            stackPointer += IR.M;
            break;
        case 7://JMP - Jump to M
            programCounter = IR.M;
            break;
        case 8://JPC - Jump to M if top element on stack is 0
            if(stack[stackPointer] == 0){
                programCounter = IR.M;
            }
            stackPointer--;
            break;
        case 9://SIO - Write top stack element to screen
            printf("\nStacktrace has been written.\n");
            stackPointer--;
            break;
        case 10://SIO - Read in input from user and store at top of stack
            printf("\nInput: ");
            stackPointer++;
            scanf("%d", &stack[stackPointer]);
            break;
        case 11://SIO - Halt machine
                fclose(fileTrace);
                printOut(operation);
            break;
    }
}

void fetchCycle(){
    IR = code[programCounter]; //Fetches instructions from code
    programCounter++; //Increments PC by 1
}

//Find the BP location and returns it
int getBase(int basePlace, int base){
    while(basePlace > 0){
        base = stack[base + 1];
        basePlace--;
    }
    return base;
}

//Opens up the MCode file
void loadFile(){
    int OP = 0, L, M, i = 0;
    fileCode = fopen(VM_INPUT, "r");
    while(fscanf(fileCode, "%i", &OP) == 1){
        fscanf(fileCode, "%i", &L);
        fscanf(fileCode, "%i", &M);
        code[i].OP = OP;
        code[i].L = L;
        code[i].M = M;
        i++;
    }
    codeSize = i;
}

//Performs actions on the stack depending upon which op code is called
void opCodes(){
    switch (IR.M){
        case 0://RET
            stackPointer = basePointer - 1;
            programCounter = stack[stackPointer + 4];
            basePointer = stack[stackPointer + 3];
            break;
        case 1://NEG
            stack[stackPointer] *= -1;
            break;
        case 2://ADD
            stackPointer--;
            stack[stackPointer] = stack[stackPointer] + stack[stackPointer + 1];
            break;
        case 3://SUB
            stackPointer--;
            stack[stackPointer] = stack[stackPointer] - stack[stackPointer + 1];
            break;
        case 4://MUL
            stackPointer--;
            stack[stackPointer] = stack[stackPointer] * stack[stackPointer + 1];
            break;
        case 5://DIV
            stackPointer--;
            stack[stackPointer] = stack[stackPointer] / stack[stackPointer + 1];
            break;
        case 6://ODD
            stack[stackPointer] %= 2;
            break;
        case 7://MOD
            stackPointer--;
            stack[stackPointer] %= stack[stackPointer + 1];
            break;
        case 8://EQL
            stackPointer--;
            stack[stackPointer] = stack[stackPointer] == stack[stackPointer + 1];
            break;
        case 9://NEQ
            stackPointer--;
            stack[stackPointer] = stack[stackPointer] != stack[stackPointer + 1];
            break;
        case 10://LSS
            stackPointer--;
            stack[stackPointer] = stack[stackPointer] < stack[stackPointer + 1];
            break;
        case 11://LEQ
            stackPointer--;
            stack[stackPointer] = stack[stackPointer] <= stack[stackPointer + 1];
            break;
        case 12://LEQ
            stackPointer--;
            stack[stackPointer] = stack[stackPointer] > stack[stackPointer + 1];
            break;
        case 13://GEQ
            stackPointer--;
            stack[stackPointer] = stack[stackPointer] >= stack[stackPointer + 1];
            break;
    }
}

//This function prints our stack and also places our pipes based upon our activation records
void pipeLaying(int operation){
    int i = 1;
    int tempBasePointer = basePointer;
    int countBasePointers = 1;
    int basePointers[MAX_LEXI_LEVELS];
    basePointers[0] = 1;

    while (tempBasePointer > 1){
        basePointers[i++] = tempBasePointer;
        tempBasePointer = stack[tempBasePointer + 2];
    }
    countBasePointers = i - 1;

    for(i = 1; i <= stackPointer; i++) {
        if (i == basePointers[countBasePointers] && i != 1) {
            if(operation)
                printf("| ");
            fprintf(fileTrace,"| ");
            countBasePointers--;
        }
        if(operation)
            printf("%d ", stack[i]);
            fprintf(fileTrace,"%d ", stack[i]);
    }
}

//Opens the trace file and prints to it
void printOut(int operation){
    char c, readIn[MAX_CODE_LENGTH];
    fileTrace = fopen(VM_OUTPUT, "r");
    if(operation){
        fscanf(fileTrace,"%s",readIn);
        fscanf(fileTrace,"%s",readIn);
        fscanf(fileTrace,"%s",readIn);
        c = fgetc(fileTrace);
        while(c != EOF){
            c = fgetc(fileTrace);
        }
    }
    fclose(fileTrace);
}

//Goes through the MCode file you opened and prints the output
//of all of its cycles along with the present values of the PC,
//BP, and SP
void runCode(int operation, int operation2){
    int a,c,d;
    char b[3];
    while (basePointer > 0){
        if (programCounter < codeSize){
            if(operation)
                printf(" %5d %s %2d %3d ", programCounter, INSTRUCTSTRINGS[code[programCounter].OP], code[programCounter].L, code[programCounter].M);
                fprintf(fileTrace," %3d %s %2d %3d ", programCounter, INSTRUCTSTRINGS[code[programCounter].OP], code[programCounter].L, code[programCounter].M);
                fetchCycle();
                executeCycle(operation2);

            if(operation)
                printf("%3d %3d %3d  ", programCounter, basePointer, stackPointer);
                fprintf(fileTrace,"%3d %3d %3d  ", programCounter, basePointer, stackPointer);
                pipeLaying(operation);
            if(operation)
                printf("\n");
                fprintf(fileTrace,"\n");
        }
        else{
            printOut(operation2);
            exit(0);
        }
    }
}

//Holds stack store values, opens MCode, opens Trace, and calls functions
//to write their output to stack trace and close the files
void virtualMachine(int operation){
    stack[1] = 0;
    stack[2] = 0;
    stack[3] = 0;

    fileCode = fopen(VM_INPUT,"r");
    loadFile();
    fileTrace = fopen(VM_OUTPUT,"w");

    codeTrace(0);

    runCode(0, operation);

    fclose(fileTrace);
    fclose(fileCode);
}

//Find out the MCode file name and calls virtualMachine function to open, go through, and write output to stacktrace
void pmachine(){
    int v;
    char inputFileName[500];

    strcpy(inputFileName,"mcode.txt");
    virtualMachine(v);
}
