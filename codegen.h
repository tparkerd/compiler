int gen(int instruction, int l, int m);
const char* opTrans(int type);

void codegen() {
  // codeGenOutput = fopen(PARSER_OUTPUT_ASMCODE, "w");
}

int gen(int instruction, int l, int m) {
  ((DEBUG) ? printf(ANSI_COLOR_YELLOW"gen(%s, %d, %d)\n"ANSI_COLOR_RESET, opTrans(instruction), l, m) : printf(" "));
  asm_code[asm_line].addr = asm_line;
  asm_code[asm_line].instruction = instruction;
  asm_code[asm_line].l = l;
  asm_code[asm_line].m = m;
  asm_line++;
  return asm_line;
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
