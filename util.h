// _________________________________________
//
//            Function Prototypes
// _________________________________________

// Stringify
const char* instructionToString(int type);
const char* operationToString(int n);
const char* translate(int n);
const char* kindToString(int n);

// _________________________________________
//
//            Function Defintions
// _________________________________________

// Stringify
const char* instructionToString(int type) {
  switch(type) {
    case 1:
      return "LIT";
    case 2:
      return "OPR";
    case 3:
      return "LOD";
    case 4:
      return "STO";
    case 5:
      return "CAL";
    case 6:
      return "INC";
    case 7:
      return "JMP";
    case 8:
      return "JPC";
    case 9:
    case 10:
    case 11:
      return "SIO";
    default:
      return "???";
  }
}
const char* operationToString(int type) {
  switch(type) {
    case 0:
      return "RET";
    case 1:
      return "NEG";
    case 2:
      return "ADD";
    case 3:
      return "SUB";
    case 4:
      return "MUL";
    case 5:
      return "DIV";
    case 6:
      return "ODD";
    case 7:
      return "MOD";
    case 8:
      return "EQL";
    case 9:
      return "NEQ";
    case 10:
      return "LSS";
    case 11:
      return "LEQ";
    case 12:
      return "GTR";
    case 13:
      return "GEQ";
    default:
      return "Unsupported OPR";
  }
}
const char* kindToString(int n) {
  switch(n)
  {
    case 1:
      return "const";
    case 2:
      return "var";
    case 3:
      return "proc";
    default:
      return "";
  }
}
const char* symbolToString(int n) {
  switch (n)
  {
    case 0:
      return "error";
    case 1:
      return "nulsym";
    case 2:
      return "identsym";
    case 3:
      return "numbersym";
    case 4:
      return "plussym";
    case 5:
      return "minussym";
    case 6:
      return "mulsym";
    case 7:
      return "slashsym";
    case 8:
      return "oddsym";
    case 9:
      return "eqlsym";
    case 10:
      return "neqsym";
    case 11:
      return "lessym";
    case 12:
      return "leqsym";
    case 13:
      return "gtrsym";
    case 14:
      return "geqsym";
    case 15:
      return "lparentsym";
    case 16:
      return "rparentsym";
    case 17:
      return "commasym";
    case 18:
      return "semicolonsym";
    case 19:
      return "periodsym";
    case 20:
      return "becomessym";
    case 21:
      return "beginsym";
    case 22:
      return "endsym";
    case 23:
      return "ifsym";
    case 24:
      return "thensym";
    case 25:
      return "whilesym";
    case 26:
      return "dosym";
    case 27:
      return "callsym";
    case 28:
      return "constsym";
    case 29:
      return "varsym";
    case 30:
      return "procsym";
    case 31:
      return "writesym";
    case 32:
      return "readsym";
    case 33:
      return "elsesym";
    case 34:
      return "errsym";
    default:
      return "unknownsym";
  }
}
