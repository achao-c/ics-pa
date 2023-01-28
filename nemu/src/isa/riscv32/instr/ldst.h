def_EHelper(lw) {
  rtl_lm(s, ddest, dsrc1, id_src2->imm, 4);
}

def_EHelper(lbu) {
  rtl_lm(s, ddest, dsrc1, id_src2->imm, 1);
}

def_EHelper(sw) {
  rtl_sm(s, ddest, dsrc1, id_src2->imm, 4);
}

def_EHelper(sh) {
  rtlreg_t value = (rtlreg_t)(uint16_t)(rtlreg_t)(*ddest);  // 在栈上有没有可能造成悬浮指针？
  rtl_sm(s, &value, dsrc1, id_src2->imm, 2);
}

def_EHelper(sb) {
  rtlreg_t value = (rtlreg_t)(uint8_t)(rtlreg_t)(*ddest);  // 在栈上有没有可能造成悬浮指针？
  rtl_sm(s, &value, dsrc1, id_src2->imm, 1);
}