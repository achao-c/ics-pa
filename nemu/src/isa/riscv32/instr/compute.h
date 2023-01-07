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

}

def_EHelper(jalr) {
  word_t t = s->pc+4;
  rtl_addi(s, &(s->dnpc), dsrc1, id_src2->simm);
  s->dnpc = s->dnpc & (~1);
  rtl_addi(s, ddest, rz, t);
}