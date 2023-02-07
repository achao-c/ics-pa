def_EHelper(lui) {
  //printf("0x%x", id_src1->imm);
  rtl_li(s, ddest, id_src1->imm);
}

def_EHelper(addi) {
  rtl_addi(s, ddest, dsrc1, id_src2->simm);
}

def_EHelper(andi) {
  uint32_t value = ((id_src2->simm) & (*dsrc1));
  rtl_addi(s, ddest, rz, value);
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

def_EHelper(mul) {
  rtl_mulu_lo(s, ddest, dsrc1, dsrc2);
}

def_EHelper(div) {
  rtl_divs_q(s, ddest, dsrc1, dsrc2);
}

def_EHelper(rem) {
  rtl_divs_r(s, ddest, dsrc1, dsrc2);
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

def_EHelper(bge) {
  sword_t sreg1 = *dsrc1;
  sword_t sreg2 = *dsrc2;
  if (sreg1 >= sreg2) {
    rtl_addi(s, &(s->dnpc), &(s->pc), id_dest->simm);
  }
}

def_EHelper(blt) {
  sword_t sreg1 = *dsrc1;
  sword_t sreg2 = *dsrc2;
  if (sreg1 < sreg2) {
    rtl_addi(s, &(s->dnpc), &(s->pc), id_dest->simm);
  }
}

def_EHelper(bltu) {
  word_t sreg1 = *dsrc1;
  word_t sreg2 = *dsrc2;
  if (sreg1 < sreg2) {
    rtl_addi(s, &(s->dnpc), &(s->pc), id_dest->simm);
  }
}

def_EHelper(bgeu) {
  word_t sreg1 = *dsrc1;
  word_t sreg2 = *dsrc2;
  if (sreg1 >= sreg2) {
    rtl_addi(s, &(s->dnpc), &(s->pc), id_dest->simm);
  }
}

def_EHelper(srl) {
  uint32_t right_num = ((*dsrc2) & (0x1F));
  uint32_t value = (uint32_t)(*dsrc1) >> right_num;
  rtl_addi(s, ddest, rz, value);
}

def_EHelper(sra) {
  uint32_t right_num = ((*dsrc2) & (0x1F));
  uint32_t value = (int32_t)(*dsrc1) >> right_num;
  rtl_addi(s, ddest, rz, value);
}

def_EHelper(srai) {
  uint32_t right_num = (id_src2->imm & 0x3F);
  if ((right_num & 0x20)) panic("invalid instr!");
  int32_t val = (*dsrc1);
  val = val >> right_num;
  // 处理同样字长的有符号数和无符号数之间的相互转换的一般规则是：数值可能会改变，但是位模式不变。
  // 也就是说，将unsigned int强制类型转换成int，或将int转换成unsigned int底层的位表示保持不变。
  rtl_addi(s, ddest, rz, (uint32_t)val);
}

def_EHelper(sll) {
  uint32_t left_num = ((*dsrc2) & (0x1F));
  uint32_t value = (*dsrc1) << left_num;
  rtl_addi(s, ddest, rz, value);
}

def_EHelper(slli) {
  uint32_t left_num = id_src2->imm;
  if ((left_num >> 5)) panic("invalid instr!");
  uint32_t value = (*dsrc1) << left_num;
  rtl_addi(s, ddest, rz, value);
}

def_EHelper(srli) {
  uint32_t right_num = id_src2->imm;
  if ((right_num >> 5)) panic("invalid instr!");
  uint32_t value = (uint32_t)(*dsrc1) >> right_num;
  rtl_addi(s, ddest, rz, value);
}

def_EHelper(and) {
  uint32_t value = ((*dsrc1) & (*dsrc2));
  rtl_addi(s, ddest, rz, value);
}

def_EHelper(xori) {
  uint32_t value = ((*dsrc1) ^ (id_src2->simm));
  rtl_addi(s, ddest, rz, value);
}

def_EHelper(or) {
  uint32_t value = ((*dsrc1) | (*dsrc2));
  rtl_addi(s, ddest, rz, value);
}

def_EHelper(slt) {
  if ((sword_t)(*dsrc1) < (sword_t)(*dsrc2)) rtl_addi(s, ddest, rz, 1);
  else rtl_addi(s, ddest, rz, 0);
}

def_EHelper(sltu) {
  if ((uint32_t)(*dsrc1) < (uint32_t)(*dsrc2)) rtl_addi(s, ddest, rz, 1);
  else rtl_addi(s, ddest, rz, 0);
}
