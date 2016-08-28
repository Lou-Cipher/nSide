auto Z80::legacyInstruction(uint8 opcode) -> void {
  switch(opcode) {
  //0x00-0x00 covered by new instructions
  case 0x01: return op_ld_rr_nn(r.bc);
  case 0x02: return op_ld_rr_a(r.bc);
  case 0x03: return op_inc_rr(r.bc);
  case 0x04: return op_inc_r(r.b);
  case 0x05: return op_dec_r(r.b);
  case 0x06: return op_ld_r_n(r.b);
  case 0x07: return op_rlca();
  case 0x09: return op_add_hl_rr(r.bc);
  case 0x0a: return op_ld_a_rr(r.bc);
  case 0x0b: return op_dec_rr(r.bc);
  case 0x0c: return op_inc_r(r.c);
  case 0x0d: return op_dec_r(r.c);
  case 0x0e: return op_ld_r_n(r.c);
  case 0x0f: return op_rrca();
  case 0x10: return op_stop();
  case 0x11: return op_ld_rr_nn(r.de);
  case 0x12: return op_ld_rr_a(r.de);
  case 0x13: return op_inc_rr(r.de);
  case 0x14: return op_inc_r(r.d);
  case 0x15: return op_dec_r(r.d);
  case 0x16: return op_ld_r_n(r.d);
  case 0x17: return op_rla();
  //0x18-0x18 covered by new instructions
  case 0x19: return op_add_hl_rr(r.de);
  case 0x1a: return op_ld_a_rr(r.de);
  case 0x1b: return op_dec_rr(r.de);
  case 0x1c: return op_inc_r(r.e);
  case 0x1d: return op_dec_r(r.e);
  case 0x1e: return op_ld_r_n(r.e);
  case 0x1f: return op_rra();
  //0x20-0x20 covered by new instructions
  case 0x21: return op_ld_rr_nn(r.hl);
  case 0x22: return op_ldi_hl_a();
  case 0x23: return op_inc_rr(r.hl);
  case 0x24: return op_inc_r(r.h);
  case 0x25: return op_dec_r(r.h);
  case 0x26: return op_ld_r_n(r.h);
  case 0x27: return op_daa();
  //0x28-0x28 covered by new instructions
  case 0x29: return op_add_hl_rr(r.hl);
  case 0x2a: return op_ldi_a_hl();
  case 0x2b: return op_dec_rr(r.hl);
  case 0x2c: return op_inc_r(r.l);
  case 0x2d: return op_dec_r(r.l);
  case 0x2e: return op_ld_r_n(r.l);
  case 0x2f: return op_cpl();
  //0x30-0x30 covered by new instructions
  case 0x31: return op_ld_rr_nn(r.sp);
  case 0x32: return op_ldd_hl_a();
  case 0x33: return op_inc_rr(r.sp);
  case 0x34: return op_inc_hl();
  case 0x35: return op_dec_hl();
  case 0x36: return op_ld_hl_n();
  case 0x37: return op_scf();
  //0x38-0x38 covered by new instructions
  case 0x39: return op_add_hl_rr(r.sp);
  case 0x3a: return op_ldd_a_hl();
  case 0x3b: return op_dec_rr(r.sp);
  case 0x3c: return op_inc_r(r.a);
  case 0x3d: return op_dec_r(r.a);
  case 0x3e: return op_ld_r_n(r.a);
  case 0x3f: return op_ccf();
  case 0x40: return op_ld_r_r(r.b, r.b);
  case 0x41: return op_ld_r_r(r.b, r.c);
  case 0x42: return op_ld_r_r(r.b, r.d);
  case 0x43: return op_ld_r_r(r.b, r.e);
  case 0x44: return op_ld_r_r(r.b, r.h);
  case 0x45: return op_ld_r_r(r.b, r.l);
  case 0x46: return op_ld_r_hl(r.b);
  case 0x47: return op_ld_r_r(r.b, r.a);
  case 0x48: return op_ld_r_r(r.c, r.b);
  case 0x49: return op_ld_r_r(r.c, r.c);
  case 0x4a: return op_ld_r_r(r.c, r.d);
  case 0x4b: return op_ld_r_r(r.c, r.e);
  case 0x4c: return op_ld_r_r(r.c, r.h);
  case 0x4d: return op_ld_r_r(r.c, r.l);
  case 0x4e: return op_ld_r_hl(r.c);
  case 0x4f: return op_ld_r_r(r.c, r.a);
  case 0x50: return op_ld_r_r(r.d, r.b);
  case 0x51: return op_ld_r_r(r.d, r.c);
  case 0x52: return op_ld_r_r(r.d, r.d);
  case 0x53: return op_ld_r_r(r.d, r.e);
  case 0x54: return op_ld_r_r(r.d, r.h);
  case 0x55: return op_ld_r_r(r.d, r.l);
  case 0x56: return op_ld_r_hl(r.d);
  case 0x57: return op_ld_r_r(r.d, r.a);
  case 0x58: return op_ld_r_r(r.e, r.b);
  case 0x59: return op_ld_r_r(r.e, r.c);
  case 0x5a: return op_ld_r_r(r.e, r.d);
  case 0x5b: return op_ld_r_r(r.e, r.e);
  case 0x5c: return op_ld_r_r(r.e, r.h);
  case 0x5d: return op_ld_r_r(r.e, r.l);
  case 0x5e: return op_ld_r_hl(r.e);
  case 0x5f: return op_ld_r_r(r.e, r.a);
  case 0x60: return op_ld_r_r(r.h, r.b);
  case 0x61: return op_ld_r_r(r.h, r.c);
  case 0x62: return op_ld_r_r(r.h, r.d);
  case 0x63: return op_ld_r_r(r.h, r.e);
  case 0x64: return op_ld_r_r(r.h, r.h);
  case 0x65: return op_ld_r_r(r.h, r.l);
  case 0x66: return op_ld_r_hl(r.h);
  case 0x67: return op_ld_r_r(r.h, r.a);
  case 0x68: return op_ld_r_r(r.l, r.b);
  case 0x69: return op_ld_r_r(r.l, r.c);
  case 0x6a: return op_ld_r_r(r.l, r.d);
  case 0x6b: return op_ld_r_r(r.l, r.e);
  case 0x6c: return op_ld_r_r(r.l, r.h);
  case 0x6d: return op_ld_r_r(r.l, r.l);
  case 0x6e: return op_ld_r_hl(r.l);
  case 0x6f: return op_ld_r_r(r.l, r.a);
  case 0x70: return op_ld_hl_r(r.b);
  case 0x71: return op_ld_hl_r(r.c);
  case 0x72: return op_ld_hl_r(r.d);
  case 0x73: return op_ld_hl_r(r.e);
  case 0x74: return op_ld_hl_r(r.h);
  case 0x75: return op_ld_hl_r(r.l);
  case 0x76: return op_halt();
  case 0x77: return op_ld_hl_r(r.a);
  case 0x78: return op_ld_r_r(r.a, r.b);
  case 0x79: return op_ld_r_r(r.a, r.c);
  case 0x7a: return op_ld_r_r(r.a, r.d);
  case 0x7b: return op_ld_r_r(r.a, r.e);
  case 0x7c: return op_ld_r_r(r.a, r.h);
  case 0x7d: return op_ld_r_r(r.a, r.l);
  case 0x7e: return op_ld_r_hl(r.a);
  case 0x7f: return op_ld_r_r(r.a, r.a);
  case 0x80: return op_add_a_r(r.b);
  case 0x81: return op_add_a_r(r.c);
  case 0x82: return op_add_a_r(r.d);
  case 0x83: return op_add_a_r(r.e);
  case 0x84: return op_add_a_r(r.h);
  case 0x85: return op_add_a_r(r.l);
  case 0x86: return op_add_a_hl();
  case 0x87: return op_add_a_r(r.a);
  case 0x88: return op_adc_a_r(r.b);
  case 0x89: return op_adc_a_r(r.c);
  case 0x8a: return op_adc_a_r(r.d);
  case 0x8b: return op_adc_a_r(r.e);
  case 0x8c: return op_adc_a_r(r.h);
  case 0x8d: return op_adc_a_r(r.l);
  case 0x8e: return op_adc_a_hl();
  case 0x8f: return op_adc_a_r(r.a);
  case 0x90: return op_sub_a_r(r.b);
  case 0x91: return op_sub_a_r(r.c);
  case 0x92: return op_sub_a_r(r.d);
  case 0x93: return op_sub_a_r(r.e);
  case 0x94: return op_sub_a_r(r.h);
  case 0x95: return op_sub_a_r(r.l);
  case 0x96: return op_sub_a_hl();
  case 0x97: return op_sub_a_r(r.a);
  case 0x98: return op_sbc_a_r(r.b);
  case 0x99: return op_sbc_a_r(r.c);
  case 0x9a: return op_sbc_a_r(r.d);
  case 0x9b: return op_sbc_a_r(r.e);
  case 0x9c: return op_sbc_a_r(r.h);
  case 0x9d: return op_sbc_a_r(r.l);
  case 0x9e: return op_sbc_a_hl();
  case 0x9f: return op_sbc_a_r(r.a);
  case 0xa0: return op_and_a_r(r.b);
  case 0xa1: return op_and_a_r(r.c);
  case 0xa2: return op_and_a_r(r.d);
  case 0xa3: return op_and_a_r(r.e);
  case 0xa4: return op_and_a_r(r.h);
  case 0xa5: return op_and_a_r(r.l);
  case 0xa6: return op_and_a_hl();
  case 0xa7: return op_and_a_r(r.a);
  case 0xa8: return op_xor_a_r(r.b);
  case 0xa9: return op_xor_a_r(r.c);
  case 0xaa: return op_xor_a_r(r.d);
  case 0xab: return op_xor_a_r(r.e);
  case 0xac: return op_xor_a_r(r.h);
  case 0xad: return op_xor_a_r(r.l);
  case 0xae: return op_xor_a_hl();
  case 0xaf: return op_xor_a_r(r.a);
  case 0xb0: return op_or_a_r(r.b);
  case 0xb1: return op_or_a_r(r.c);
  case 0xb2: return op_or_a_r(r.d);
  case 0xb3: return op_or_a_r(r.e);
  case 0xb4: return op_or_a_r(r.h);
  case 0xb5: return op_or_a_r(r.l);
  case 0xb6: return op_or_a_hl();
  case 0xb7: return op_or_a_r(r.a);
  //0xb8-0xbf covered by new instructions
  case 0xc0: return op_ret_f(r.p.z, 0);
  case 0xc1: return op_pop_rr(r.bc);
  //0xc2-0xc3 covered by new instructions
  case 0xc4: return op_call_f_nn(r.p.z, 0);
  case 0xc5: return op_push_rr(r.bc);
  case 0xc6: return op_add_a_n();
  case 0xc7: return op_rst_n(0x00);
  case 0xc8: return op_ret_f(r.p.z, 1);
  case 0xc9: return op_ret();
  //0xca-0xca covered by new instructions
  case 0xcb: return legacyInstructionCB(read(r.pc++));
  case 0xcc: return op_call_f_nn(r.p.z, 1);
  case 0xcd: return op_call_nn();
  case 0xce: return op_adc_a_n();
  case 0xcf: return op_rst_n(0x08);
  case 0xd0: return op_ret_f(r.p.c, 0);
  case 0xd1: return op_pop_rr(r.de);
  //0xd2-0xd2 covered by new instructions
  case 0xd3: return op_out_n_a(r.a);
  case 0xd4: return op_call_f_nn(r.p.c, 0);
  case 0xd5: return op_push_rr(r.de);
  case 0xd6: return op_sub_a_n();
  case 0xd7: return op_rst_n(0x10);
  case 0xd8: return op_ret_f(r.p.c, 1);
  //0xda-0xdb covered by new instructions
  case 0xdc: return op_call_f_nn(r.p.c, 1);
  //0xdd-0xdd covered by new instructions
  case 0xde: return op_sbc_a_n();
  case 0xdf: return op_rst_n(0x18);
  case 0xe0: return op_ret_f(r.p.v, 0);
  case 0xe1: return op_pop_rr(r.hl);
  //0xe2-0xe2 covered by new instructions
  case 0xe4: return op_call_f_nn(r.p.v, 0);
  case 0xe5: return op_push_rr(r.hl);
  case 0xe6: return op_and_a_n();
  case 0xe7: return op_rst_n(0x20);
  case 0xe8: return op_ret_f(r.p.v, 1);
  //0xe9-0xea covered by new instructions
  case 0xec: return op_call_f_nn(r.p.v, 1);
  //0xed-0xed covered by new instructions
  case 0xee: return op_xor_a_n();
  case 0xef: return op_rst_n(0x28);
  case 0xf0: return op_ret_f(r.p.n, 0);
  case 0xf1: return op_pop_rr(r.af);
  //0xf2-0xf3 covered by new instructions
  case 0xf4: return op_call_f_nn(r.p.n, 0);
  case 0xf5: return op_push_rr(r.af);
  case 0xf6: return op_or_a_n();
  case 0xf7: return op_rst_n(0x30);
  case 0xf8: return op_ret_f(r.p.n, 1);
  case 0xf9: return op_ld_sp_rr(r.hl);
  //0xfa-0xfa covered by new instructions
  case 0xfb: return op_ei();
  case 0xfc: return op_call_f_nn(r.p.n, 1);
  //0xfd-0xfe covered by new instructions
  case 0xff: return op_rst_n(0x38);
  }
  print("[Z80] unimplemented legacy instruction: ", hex(opcode, 2L), "\n");
  while(true) wait();
}

auto Z80::legacyInstructionCB(uint8 opcode) -> void {
  switch(opcode) {
  case 0x00: return op_rlc_r(r.b);
  case 0x01: return op_rlc_r(r.c);
  case 0x02: return op_rlc_r(r.d);
  case 0x03: return op_rlc_r(r.e);
  case 0x04: return op_rlc_r(r.h);
  case 0x05: return op_rlc_r(r.l);
  case 0x06: return op_rlc_hl();
  case 0x07: return op_rlc_r(r.a);
  case 0x08: return op_rrc_r(r.b);
  case 0x09: return op_rrc_r(r.c);
  case 0x0a: return op_rrc_r(r.d);
  case 0x0b: return op_rrc_r(r.e);
  case 0x0c: return op_rrc_r(r.h);
  case 0x0d: return op_rrc_r(r.l);
  case 0x0e: return op_rrc_hl();
  case 0x0f: return op_rrc_r(r.a);
  case 0x10: return op_rl_r(r.b);
  case 0x11: return op_rl_r(r.c);
  case 0x12: return op_rl_r(r.d);
  case 0x13: return op_rl_r(r.e);
  case 0x14: return op_rl_r(r.h);
  case 0x15: return op_rl_r(r.l);
  case 0x16: return op_rl_hl();
  case 0x17: return op_rl_r(r.a);
  case 0x18: return op_rr_r(r.b);
  case 0x19: return op_rr_r(r.c);
  case 0x1a: return op_rr_r(r.d);
  case 0x1b: return op_rr_r(r.e);
  case 0x1c: return op_rr_r(r.h);
  case 0x1d: return op_rr_r(r.l);
  case 0x1e: return op_rr_hl();
  case 0x1f: return op_rr_r(r.a);
  case 0x20: return op_sla_r(r.b);
  case 0x21: return op_sla_r(r.c);
  case 0x22: return op_sla_r(r.d);
  case 0x23: return op_sla_r(r.e);
  case 0x24: return op_sla_r(r.h);
  case 0x25: return op_sla_r(r.l);
  case 0x26: return op_sla_hl();
  case 0x27: return op_sla_r(r.a);
  case 0x28: return op_sra_r(r.b);
  case 0x29: return op_sra_r(r.c);
  case 0x2a: return op_sra_r(r.d);
  case 0x2b: return op_sra_r(r.e);
  case 0x2c: return op_sra_r(r.h);
  case 0x2d: return op_sra_r(r.l);
  case 0x2e: return op_sra_hl();
  case 0x2f: return op_sra_r(r.a);
  case 0x30: return op_swap_r(r.b);
  case 0x31: return op_swap_r(r.c);
  case 0x32: return op_swap_r(r.d);
  case 0x33: return op_swap_r(r.e);
  case 0x34: return op_swap_r(r.h);
  case 0x35: return op_swap_r(r.l);
  case 0x36: return op_swap_hl();
  case 0x37: return op_swap_r(r.a);
  case 0x38: return op_srl_r(r.b);
  case 0x39: return op_srl_r(r.c);
  case 0x3a: return op_srl_r(r.d);
  case 0x3b: return op_srl_r(r.e);
  case 0x3c: return op_srl_r(r.h);
  case 0x3d: return op_srl_r(r.l);
  case 0x3e: return op_srl_hl();
  case 0x3f: return op_srl_r(r.a);
  case 0x40: return op_bit_n_r(0, r.b);
  case 0x41: return op_bit_n_r(0, r.c);
  case 0x42: return op_bit_n_r(0, r.d);
  case 0x43: return op_bit_n_r(0, r.e);
  case 0x44: return op_bit_n_r(0, r.h);
  case 0x45: return op_bit_n_r(0, r.l);
  case 0x46: return op_bit_n_hl(0);
  case 0x47: return op_bit_n_r(0, r.a);
  case 0x48: return op_bit_n_r(1, r.b);
  case 0x49: return op_bit_n_r(1, r.c);
  case 0x4a: return op_bit_n_r(1, r.d);
  case 0x4b: return op_bit_n_r(1, r.e);
  case 0x4c: return op_bit_n_r(1, r.h);
  case 0x4d: return op_bit_n_r(1, r.l);
  case 0x4e: return op_bit_n_hl(1);
  case 0x4f: return op_bit_n_r(1, r.a);
  case 0x50: return op_bit_n_r(2, r.b);
  case 0x51: return op_bit_n_r(2, r.c);
  case 0x52: return op_bit_n_r(2, r.d);
  case 0x53: return op_bit_n_r(2, r.e);
  case 0x54: return op_bit_n_r(2, r.h);
  case 0x55: return op_bit_n_r(2, r.l);
  case 0x56: return op_bit_n_hl(2);
  case 0x57: return op_bit_n_r(2, r.a);
  case 0x58: return op_bit_n_r(3, r.b);
  case 0x59: return op_bit_n_r(3, r.c);
  case 0x5a: return op_bit_n_r(3, r.d);
  case 0x5b: return op_bit_n_r(3, r.e);
  case 0x5c: return op_bit_n_r(3, r.h);
  case 0x5d: return op_bit_n_r(3, r.l);
  case 0x5e: return op_bit_n_hl(3);
  case 0x5f: return op_bit_n_r(3, r.a);
  case 0x60: return op_bit_n_r(4, r.b);
  case 0x61: return op_bit_n_r(4, r.c);
  case 0x62: return op_bit_n_r(4, r.d);
  case 0x63: return op_bit_n_r(4, r.e);
  case 0x64: return op_bit_n_r(4, r.h);
  case 0x65: return op_bit_n_r(4, r.l);
  case 0x66: return op_bit_n_hl(4);
  case 0x67: return op_bit_n_r(4, r.a);
  case 0x68: return op_bit_n_r(5, r.b);
  case 0x69: return op_bit_n_r(5, r.c);
  case 0x6a: return op_bit_n_r(5, r.d);
  case 0x6b: return op_bit_n_r(5, r.e);
  case 0x6c: return op_bit_n_r(5, r.h);
  case 0x6d: return op_bit_n_r(5, r.l);
  case 0x6e: return op_bit_n_hl(5);
  case 0x6f: return op_bit_n_r(5, r.a);
  case 0x70: return op_bit_n_r(6, r.b);
  case 0x71: return op_bit_n_r(6, r.c);
  case 0x72: return op_bit_n_r(6, r.d);
  case 0x73: return op_bit_n_r(6, r.e);
  case 0x74: return op_bit_n_r(6, r.h);
  case 0x75: return op_bit_n_r(6, r.l);
  case 0x76: return op_bit_n_hl(6);
  case 0x77: return op_bit_n_r(6, r.a);
  case 0x78: return op_bit_n_r(7, r.b);
  case 0x79: return op_bit_n_r(7, r.c);
  case 0x7a: return op_bit_n_r(7, r.d);
  case 0x7b: return op_bit_n_r(7, r.e);
  case 0x7c: return op_bit_n_r(7, r.h);
  case 0x7d: return op_bit_n_r(7, r.l);
  case 0x7e: return op_bit_n_hl(7);
  case 0x7f: return op_bit_n_r(7, r.a);
  case 0x80: return op_res_n_r(0, r.b);
  case 0x81: return op_res_n_r(0, r.c);
  case 0x82: return op_res_n_r(0, r.d);
  case 0x83: return op_res_n_r(0, r.e);
  case 0x84: return op_res_n_r(0, r.h);
  case 0x85: return op_res_n_r(0, r.l);
  case 0x86: return op_res_n_hl(0);
  case 0x87: return op_res_n_r(0, r.a);
  case 0x88: return op_res_n_r(1, r.b);
  case 0x89: return op_res_n_r(1, r.c);
  case 0x8a: return op_res_n_r(1, r.d);
  case 0x8b: return op_res_n_r(1, r.e);
  case 0x8c: return op_res_n_r(1, r.h);
  case 0x8d: return op_res_n_r(1, r.l);
  case 0x8e: return op_res_n_hl(1);
  case 0x8f: return op_res_n_r(1, r.a);
  case 0x90: return op_res_n_r(2, r.b);
  case 0x91: return op_res_n_r(2, r.c);
  case 0x92: return op_res_n_r(2, r.d);
  case 0x93: return op_res_n_r(2, r.e);
  case 0x94: return op_res_n_r(2, r.h);
  case 0x95: return op_res_n_r(2, r.l);
  case 0x96: return op_res_n_hl(2);
  case 0x97: return op_res_n_r(2, r.a);
  case 0x98: return op_res_n_r(3, r.b);
  case 0x99: return op_res_n_r(3, r.c);
  case 0x9a: return op_res_n_r(3, r.d);
  case 0x9b: return op_res_n_r(3, r.e);
  case 0x9c: return op_res_n_r(3, r.h);
  case 0x9d: return op_res_n_r(3, r.l);
  case 0x9e: return op_res_n_hl(3);
  case 0x9f: return op_res_n_r(3, r.a);
  case 0xa0: return op_res_n_r(4, r.b);
  case 0xa1: return op_res_n_r(4, r.c);
  case 0xa2: return op_res_n_r(4, r.d);
  case 0xa3: return op_res_n_r(4, r.e);
  case 0xa4: return op_res_n_r(4, r.h);
  case 0xa5: return op_res_n_r(4, r.l);
  case 0xa6: return op_res_n_hl(4);
  case 0xa7: return op_res_n_r(4, r.a);
  case 0xa8: return op_res_n_r(5, r.b);
  case 0xa9: return op_res_n_r(5, r.c);
  case 0xaa: return op_res_n_r(5, r.d);
  case 0xab: return op_res_n_r(5, r.e);
  case 0xac: return op_res_n_r(5, r.h);
  case 0xad: return op_res_n_r(5, r.l);
  case 0xae: return op_res_n_hl(5);
  case 0xaf: return op_res_n_r(5, r.a);
  case 0xb0: return op_res_n_r(6, r.b);
  case 0xb1: return op_res_n_r(6, r.c);
  case 0xb2: return op_res_n_r(6, r.d);
  case 0xb3: return op_res_n_r(6, r.e);
  case 0xb4: return op_res_n_r(6, r.h);
  case 0xb5: return op_res_n_r(6, r.l);
  case 0xb6: return op_res_n_hl(6);
  case 0xb7: return op_res_n_r(6, r.a);
  case 0xb8: return op_res_n_r(7, r.b);
  case 0xb9: return op_res_n_r(7, r.c);
  case 0xba: return op_res_n_r(7, r.d);
  case 0xbb: return op_res_n_r(7, r.e);
  case 0xbc: return op_res_n_r(7, r.h);
  case 0xbd: return op_res_n_r(7, r.l);
  case 0xbe: return op_res_n_hl(7);
  case 0xbf: return op_res_n_r(7, r.a);
  case 0xc0: return op_set_n_r(0, r.b);
  case 0xc1: return op_set_n_r(0, r.c);
  case 0xc2: return op_set_n_r(0, r.d);
  case 0xc3: return op_set_n_r(0, r.e);
  case 0xc4: return op_set_n_r(0, r.h);
  case 0xc5: return op_set_n_r(0, r.l);
  case 0xc6: return op_set_n_hl(0);
  case 0xc7: return op_set_n_r(0, r.a);
  case 0xc8: return op_set_n_r(1, r.b);
  case 0xc9: return op_set_n_r(1, r.c);
  case 0xca: return op_set_n_r(1, r.d);
  case 0xcb: return op_set_n_r(1, r.e);
  case 0xcc: return op_set_n_r(1, r.h);
  case 0xcd: return op_set_n_r(1, r.l);
  case 0xce: return op_set_n_hl(1);
  case 0xcf: return op_set_n_r(1, r.a);
  case 0xd0: return op_set_n_r(2, r.b);
  case 0xd1: return op_set_n_r(2, r.c);
  case 0xd2: return op_set_n_r(2, r.d);
  case 0xd3: return op_set_n_r(2, r.e);
  case 0xd4: return op_set_n_r(2, r.h);
  case 0xd5: return op_set_n_r(2, r.l);
  case 0xd6: return op_set_n_hl(2);
  case 0xd7: return op_set_n_r(2, r.a);
  case 0xd8: return op_set_n_r(3, r.b);
  case 0xd9: return op_set_n_r(3, r.c);
  case 0xda: return op_set_n_r(3, r.d);
  case 0xdb: return op_set_n_r(3, r.e);
  case 0xdc: return op_set_n_r(3, r.h);
  case 0xdd: return op_set_n_r(3, r.l);
  case 0xde: return op_set_n_hl(3);
  case 0xdf: return op_set_n_r(3, r.a);
  case 0xe0: return op_set_n_r(4, r.b);
  case 0xe1: return op_set_n_r(4, r.c);
  case 0xe2: return op_set_n_r(4, r.d);
  case 0xe3: return op_set_n_r(4, r.e);
  case 0xe4: return op_set_n_r(4, r.h);
  case 0xe5: return op_set_n_r(4, r.l);
  case 0xe6: return op_set_n_hl(4);
  case 0xe7: return op_set_n_r(4, r.a);
  case 0xe8: return op_set_n_r(5, r.b);
  case 0xe9: return op_set_n_r(5, r.c);
  case 0xea: return op_set_n_r(5, r.d);
  case 0xeb: return op_set_n_r(5, r.e);
  case 0xec: return op_set_n_r(5, r.h);
  case 0xed: return op_set_n_r(5, r.l);
  case 0xee: return op_set_n_hl(5);
  case 0xef: return op_set_n_r(5, r.a);
  case 0xf0: return op_set_n_r(6, r.b);
  case 0xf1: return op_set_n_r(6, r.c);
  case 0xf2: return op_set_n_r(6, r.d);
  case 0xf3: return op_set_n_r(6, r.e);
  case 0xf4: return op_set_n_r(6, r.h);
  case 0xf5: return op_set_n_r(6, r.l);
  case 0xf6: return op_set_n_hl(6);
  case 0xf7: return op_set_n_r(6, r.a);
  case 0xf8: return op_set_n_r(7, r.b);
  case 0xf9: return op_set_n_r(7, r.c);
  case 0xfa: return op_set_n_r(7, r.d);
  case 0xfb: return op_set_n_r(7, r.e);
  case 0xfc: return op_set_n_r(7, r.h);
  case 0xfd: return op_set_n_r(7, r.l);
  case 0xfe: return op_set_n_hl(7);
  case 0xff: return op_set_n_r(7, r.a);
  }
  print("[Z80] unimplemented legacy CB instruction: ", hex(opcode, 2L), "\n");
  while(true) wait();
}

auto Z80::legacyInstructionED(uint8 opcode) -> void {
  switch(opcode) {
  //0x40-0x40 covered by new instructions
  case 0x41: return op_out_c_r(r.b);
  //0x46-0x46 covered by new instructions
  case 0x47: return wait(), op_ld_r_r(r.i, r.a);
  //0x48-0x48 covered by new instructions
  case 0x49: return op_out_c_r(r.c);
  case 0x4d: return op_reti();
  case 0x4f: return wait(), op_ld_r_r(r.r, r.a);
  //0x50-0x50 covered by new instructions
  case 0x51: return op_out_c_r(r.d);
  //0x56-0x56 covered by new instructions
  case 0x57: return wait(), op_ld_r_r(r.a, r.i);
  //0x58-0x58 covered by new instructions
  case 0x59: return op_out_c_r(r.e);
  //0x5e-0x5e covered by new instructions
  case 0x5f: return wait(), op_ld_r_r(r.a, r.r);
  //0x60-0x60 covered by new instructions
  case 0x61: return op_out_c_r(r.h);
  //0x66-0x66 covered by new instructions
  //0x68-0x68 covered by new instructions
  case 0x69: return op_out_c_r(r.l);
  //0x70-0x70 covered by new instructions
  case 0x71: return op_out_c_r(0);
  case 0x73: return op_ld_nn_sp();
  //0x76-0x76 covered by new instructions
  //0x78-0x78 covered by new instructions
  case 0x79: return op_out_c_r(r.a);
  //0x7e-0x7e covered by new instructions
  }
  print("[Z80] unimplemented legacy ED instruction: ", hex(opcode, 2L), "\n");
  while(true) wait();
}

auto Z80::legacyInstructionIndex(uint8 opcode, uint16_t& ir) -> void {
  switch(opcode) {
  case 0x21: return op_ld_rr_nn(ir);
  case 0xcb: return legacyInstructionIndexCB(read(r.pc++), ir);
  case 0xe1: return op_pop_rr(ir);
  case 0xe5: return op_push_rr(ir);
  //0xe9-0xe9 covered by new instructions
  case 0xf9: return op_ld_sp_rr(ir);
  }
  print("[Z80] unimplemented legacy index instruction: ", hex(opcode, 2L), "\n");
  while(true) wait();
}

auto Z80::legacyInstructionIndexCB(uint8 opcode, uint16_t& ir) -> void {
  print("[Z80] unimplemented legacy index CB instruction: ", hex(opcode, 2L), "\n");
  while(true) wait();
}
