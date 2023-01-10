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

def_EHelper(add) {
  rtl_add(s, ddest, dsrc1, dsrc2);
}

def_EHelper(sub) {
  rtl_sub(s, ddest, dsrc1, dsrc2);
}

def_EHelper(sltiu) {
  if (id_src2->imm > (*dsrc1)) {
    rtl_addi(s, ddest, rz, 1);
  }
  else {
    rtl_addi(s, ddest, rz, 0);
  }
}

def_EHelper(beq) {
  if ((*dsrc1) == (*dsrc2)) {
    rtl_addi(s, &(s->dnpc), &(s->pc), id_dest->simm);
  }

}

def_EHelper(bne) {
  if ((*dsrc1) != (*dsrc2)) {
    rtl_addi(s, &(s->dnpc), &(s->pc), id_dest->simm);
  }
}

