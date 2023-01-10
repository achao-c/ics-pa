#include <cpu/decode.h>
#include "../local-include/rtl.h"

#define INSTR_LIST(f) f(lui) f(addi)  f(lw) f(sw) f(inv) f(auipc) f(jal) f(jalr) \
                      f(add) f(sltiu) f(sub) f(beq) f(bne) f(nemu_trap)

def_all_EXEC_ID();
