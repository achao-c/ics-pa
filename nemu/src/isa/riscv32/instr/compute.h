def_EHelper(lui) {
  //printf("0x%x", id_src1->imm);
  rtl_li(s, ddest, id_src1->imm);
}

def_EHelper(addi) {
  rtl_addi(s, ddest, dsrc1, id_src2->imm);
}


def_EHelper(auipc) {
  rtl_addi(s, ddest, &cpu.pc, id_src1->imm);
}

def_EHelper(jal) {
  //word_t* next_isr = &(cpu.pc+4);
  rtl_addi(s, ddest, rz, cpu.pc+4);
  rtl_addi(s, &cpu.pc, &cpu.pc, id_src1->imm);
}