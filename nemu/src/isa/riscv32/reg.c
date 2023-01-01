#include <isa.h>
#include "local-include/reg.h"

const char *regs[] = {
  "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
  "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
  "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
  "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
};

void isa_reg_display() {
  printf("common reg--------------------\n");
  for (int idx = 0; idx < 32; ++idx) {
    printf("%s: 0x%08x\n", reg_name(idx, 0), cpu.gpr[idx]._32);
  }
  
  printf("pc reg--------------------\n");
  printf("pc: 0x%08x\n", cpu.pc);
}

word_t isa_reg_str2val(const char *s, bool *success) {
  for (int idx = 0; idx < 32; ++idx) {
    if (!strcmp(reg_name(idx, 0), s)) return cpu.gpr[idx];
  }
  return 0;
}
