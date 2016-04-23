void emit(int instruction, int l, int m);
int gen(int instruction, int l, int m);
const char* opTrans(int type);

void codegen() {
  // codeGenOutput = fopen(PARSER_OUTPUT_ASMCODE, "w");
}

int gen(int instruction, int l, int m) {
  asm_code[asm_line].addr = asm_line;
  asm_code[asm_line].instruction = instruction;
  asm_code[asm_line].l = l;
  asm_code[asm_line].m = m;
  asm_line++;
  return asm_line;
}

void emit(int instruction, int l, int m) {
  FILE* ofp = fopen("asm.s", "a");
  switch(instruction) {
    case 1:
      printf("00%d LIT\t%d\t%d\n", asm_line, l, m);
      fprintf(ofp, "00%d LIT\t%d\t%d\n", asm_line, l, m);
      break;
    case 2:
      printf("00%d\tOPR\t%d\t%d\n", asm_line, l, m);
      fprintf(ofp, "00%d\tOPR\t%d\t%d\n", asm_line, l, m);
      break;
    case 3:
      printf("00%d\tLOD\t%d\t%d\n", asm_line, l, m);
      fprintf(ofp, "00%d\tLOD\t%d\t%d\n", asm_line, l, m);
      break;
    case 4:
      printf("00%d\tSTO\t%d\t%d\n", asm_line, l, m);
      fprintf(ofp, "00%d\tSTO\t%d\t%d\n", asm_line, l, m);
      break;
    case 5:
      printf("00%d\tCAL\t%d\t%d\n", asm_line, l, m);
      fprintf(ofp, "00%d\tCAL\t%d\t%d\n", asm_line, l, m);
      break;
    case 6:
      printf("00%d\tINC\t%d\t%d\n", asm_line, l, m);
      fprintf(ofp, "00%d\tINC\t%d\t%d\n", asm_line, l, m);
      break;
    case 7:
      printf("00%d\tJMP\t%d\t%d\n", asm_line, l, m);
      fprintf(ofp, "00%d\tJMP\t%d\t%d\n", asm_line, l, m);
      break;
    case 8:
      printf("00%d\tJPC\t%d\t%d\n", asm_line, l, m);
      fprintf(ofp, "00%d\tJPC\t%d\t%d\n", asm_line, l, m);
      break;
    case 9:
    case 10:
    case 11:
      printf("00%d\tSIO\t%d\t%d\n", asm_line, l, m);
      fprintf(ofp, "00%d\tSIO\t%d\t%d\n", asm_line, l, m);
      break;
    default:
      printf("00%d\tUNKNOWN OP\t%d\t%d\n", asm_line, l, m);
      fprintf(ofp, "00%d\tUNKNOWN OP\t%d\t%d\n", asm_line, l, m);
      break;
  }
  fclose(ofp);
}

void displayCodeGen() {
  int i;
  FILE* ofp = fopen(PARSER_OUTPUT_ASMCODE, "w");
  printf("Number of asm_lines: %d\n", asm_line);
  for (i = 0; i < asm_line; i++)
  {
    printf("%*d %s %*d %*d\n", 2, asm_code[i].addr, opTrans(asm_code[i].instruction), 3, asm_code[i].l, 3, asm_code[i].m);
    // Why does the gen function seem to insert anything into the asm_code, but should be the symbol table?
    fprintf(ofp, "%d %d %d\n", asm_code[i].instruction, asm_code[i].l, asm_code[i].m);
  }
  fclose(ofp);
}

const char* opTrans(int type) {
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
