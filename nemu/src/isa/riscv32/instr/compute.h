def_EHelper(lui) {
  //printf("0x%x", id_src1->imm);
  rtl_li(s, ddest, id_src1->imm);
}

def_EHelper(addi) {
  rtl_addi(s, ddest, dsrc1, id_src2->simm);
}


def_EHelper(auipc) {
  rtl_addi(s, ddest, &cpu.pc, id_src1->simm);
}

def_EHelper(jal) {
  //word_t* next_isr = &(cpu.pc+4);
  rtl_addi(s, ddest, rz, s->pc+4);
  int32_t a = s->pc + id_src1->imm;
  rtl_addi(s, &(s->dnpc), rz, a);
  printf("ha0x%x\n", s->dnpc);


}

def_EHelper(jalr) {
  word_t t = cpu.pc+4;
  rtl_addi(s, &cpu.pc, dsrc1, id_src2->simm);
  cpu.pc = cpu.pc & (~1);
  rtl_addi(s, ddest, rz, t);
}