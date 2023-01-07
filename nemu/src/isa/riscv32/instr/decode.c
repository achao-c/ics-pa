#include "../local-include/reg.h"
#include <cpu/ifetch.h>
#include <isa-all-instr.h>

def_all_THelper();

static uint32_t get_instr(Decode *s) {
  return s->isa.instr.val;
}

// decode operand helper
#define def_DopHelper(name) \
  void concat(decode_op_, name) (Decode *s, Operand *op, word_t val, bool flag)

static def_DopHelper(i) {
  op->imm = val;
}

static def_DopHelper(r) {
  bool is_write = flag;
  static word_t zero_null = 0;
  op->preg = (is_write && val == 0) ? &zero_null : &gpr(val); // 获取指向该寄存器值的地址
  // 检查指令是否尝试往0号寄存器进行写入, 若是, 则把写入的目的修改成一个"黑洞变量", 这个变量不会被其它代码读取, 从而实现"往0号寄存器的写入操作都会被硬件忽略"的效果.
}

static def_DHelper(I) {
  decode_op_r(s, id_src1, s->isa.instr.i.rs1, false);
  decode_op_i(s, id_src2, s->isa.instr.i.simm11_0, false);
  decode_op_r(s, id_dest, s->isa.instr.i.rd, true);
}

static def_DHelper(U) {
  decode_op_i(s, id_src1, s->isa.instr.u.imm31_12 << 12, true);
  decode_op_r(s, id_dest, s->isa.instr.u.rd, true);
}

static def_DHelper(J) {
  sword_t simm = (s->isa.instr.j.simm20 << 20) | (s->isa.instr.j.imm19_12 << 12)\
  | (s->isa.instr.j.imm11 << 11) | (s->isa.instr.j.imm10_1 << 1); 
  decode_op_i(s, id_src1, simm, false);
  decode_op_r(s, id_dest, s->isa.instr.j.rd, true);
}

static def_DHelper(S) {
  decode_op_r(s, id_src1, s->isa.instr.s.rs1, false);
  sword_t simm = (s->isa.instr.s.simm11_5 << 5) | s->isa.instr.s.imm4_0;
  decode_op_i(s, id_src2, simm, false);
  decode_op_r(s, id_dest, s->isa.instr.s.rs2, false);
}

def_THelper(load) {
  def_INSTR_TAB("??????? ????? ????? 010 ????? ????? ??", lw);
  return EXEC_ID_inv;
}

def_THelper(store) {
  def_INSTR_TAB("??????? ????? ????? 010 ????? ????? ??", sw);
  return EXEC_ID_inv;
}

def_THelper(normal) {
  def_INSTR_TAB("??????? ????? ????? 000 ????? ????? ??", addi);
  return EXEC_ID_inv;
}


// table_main(s)
//------------------ 3 -----------------
/*
do {
  uint32_t key, mask, shift;
  pattern_decode("??????? ????? ????? ??? ????? 01101 11", 37, &key, &mask, &shift);
  if (((s->isa.instr.val >> shift) & mask) == key) {
    decode_U(s, 0);  // 译码辅助函数 包括译码操作数辅助函数
    return table_lui(s); //  它直接返回一个标识lui指令的唯一ID. 这个ID会作为译码结果的返回值, 
                          //  在fetch_decode()中索引g_exec_table数组.
  }
} while (0);
*/
def_THelper(main) {
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 00000 11", I  , load);
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 00100 11", I, normal);

  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 01000 11", S, store);
  
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 01101 11", U  , lui);
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 00101 11", U, auipc);

  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 11011 11", J  , jal);

  def_INSTR_TAB  ("??????? ????? ????? ??? ????? 11010 11",         nemu_trap);
  return table_inv(s);
};

//------------------ 2 -----------------
int isa_fetch_decode(Decode *s) {
  s->isa.instr.val = instr_fetch(&s->snpc, 4); // 所有指令长度为32位,将取出的32位指令放入s->isa
  int idx = table_main(s);
  return idx;
}
