#include <cpu/decode.h>
#include "../local-include/rtl.h"

#define INSTR_LIST(f) f(lui) f(addi)  f(lw) f(lbu) f(sw) f(sh) f(inv) f(auipc) f(jal) f(jalr) \
                      f(add) f(sltiu) f(srai) f(sub) f(beq) f(bne) f(nemu_trap) \
                      f(andi) f(sll) f(and) f(sltu) f(xori) f(or) f(sb)

def_all_EXEC_ID();
