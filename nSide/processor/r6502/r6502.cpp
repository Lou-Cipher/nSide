#include <processor/processor.hpp>
#include "r6502.hpp"

namespace Processor {

#define I
#define L lastCycle();
#define call(op) (this->*op)()

#include "instructions.cpp"
#include "disassembler.cpp"
#include "serialization.cpp"

auto R6502::mdr() const -> uint8 {
  return r.mdr;
}

auto R6502::power() -> void {
  r.a = 0x00;
  r.x = 0x00;
  r.y = 0x00;
  r.s = 0x00;
  r.p = 0x04;
}

auto R6502::reset() -> void {
  r.mdr = 0x00;
  r.s -= 3;
  r.p.i = 1;
}

auto R6502::interrupt() -> void {
  readPC();
  readPC();
  writeSP(r.pc >> 8);
  writeSP(r.pc >> 0);
  uint16 vector = 0xfffe;  //IRQ
  nmi(vector);
  writeSP(r.p | 0x20);
  abs.l = read(vector++);
  r.p.i = 1;
L abs.h = read(vector++);
  r.pc = abs.w;
}

auto R6502::instruction() -> void {
  uint8 opcode = readPCi();
  switch(opcode) {
  case 0x00: return op_brk();
  case 0x01: return opi_read_indirect_zero_page_x<&R6502::opf_ora>();
I case 0x02: return opill_stp();
I case 0x03: return opilli_rmwr_indirect_zero_page_x<&R6502::opf_asl,&R6502::opf_ora>();
I case 0x04: return opill_nop_zero_page();
  case 0x05: return opi_read_zero_page<&R6502::opf_ora>();
  case 0x06: return opi_rmw_zero_page<&R6502::opf_asl>();
I case 0x07: return opilli_rmwr_zero_page<&R6502::opf_asl,&R6502::opf_ora>();
  case 0x08: return op_php();
  case 0x09: return opi_read_immediate<&R6502::opf_ora>();
  case 0x0a: return opi_shift<&R6502::opf_sla>();
I case 0x0b: return opill_anc_immediate();
I case 0x0c: return opill_nop_absolute();
  case 0x0d: return opi_read_absolute<&R6502::opf_ora>();
  case 0x0e: return opi_rmw_absolute<&R6502::opf_asl>();
I case 0x0f: return opilli_rmwr_absolute<&R6502::opf_asl,&R6502::opf_ora>();
  case 0x10: return opi_branch(r.p.n == 0);
  case 0x11: return opi_read_indirect_zero_page_y<&R6502::opf_ora>();
I case 0x12: return opill_stp();
I case 0x13: return opilli_rmwr_indirect_zero_page_y<&R6502::opf_asl,&R6502::opf_ora>();
I case 0x14: return opill_nop_zero_page_x();
  case 0x15: return opi_read_zero_page_x<&R6502::opf_ora>();
  case 0x16: return opi_rmw_zero_page_x<&R6502::opf_asl>();
I case 0x17: return opilli_rmwr_zero_page_x<&R6502::opf_asl,&R6502::opf_ora>();
  case 0x18: return opi_clear_flag(r.p.c);
  case 0x19: return opi_read_absolute_y<&R6502::opf_ora>();
I case 0x1a: return opill_nop_implied();
I case 0x1b: return opilli_rmwr_absolute_y<&R6502::opf_asl,&R6502::opf_ora>();
I case 0x1c: return opill_nop_absolute_x();
  case 0x1d: return opi_read_absolute_x<&R6502::opf_ora>();
  case 0x1e: return opi_rmw_absolute_x<&R6502::opf_asl>();
I case 0x1f: return opilli_rmwr_absolute_x<&R6502::opf_asl,&R6502::opf_ora>();
  case 0x20: return op_jsr_absolute();
  case 0x21: return opi_read_indirect_zero_page_x<&R6502::opf_and>();
I case 0x22: return opill_stp();
I case 0x23: return opilli_rmwr_indirect_zero_page_x<&R6502::opf_rol,&R6502::opf_and>();
  case 0x24: return opi_read_zero_page<&R6502::opf_bit>();
  case 0x25: return opi_read_zero_page<&R6502::opf_and>();
  case 0x26: return opi_rmw_zero_page<&R6502::opf_rol>();
I case 0x27: return opilli_rmwr_zero_page<&R6502::opf_rol,&R6502::opf_and>();
  case 0x28: return op_plp();
  case 0x29: return opi_read_immediate<&R6502::opf_and>();
  case 0x2a: return opi_shift<&R6502::opf_rla>();
I case 0x2b: return opill_anc_immediate();
  case 0x2c: return opi_read_absolute<&R6502::opf_bit>();
  case 0x2d: return opi_read_absolute<&R6502::opf_and>();
  case 0x2e: return opi_rmw_absolute<&R6502::opf_rol>();
I case 0x2f: return opilli_rmwr_absolute<&R6502::opf_rol,&R6502::opf_and>();
  case 0x30: return opi_branch(r.p.n == 1);
  case 0x31: return opi_read_indirect_zero_page_y<&R6502::opf_and>();
I case 0x32: return opill_stp();
I case 0x33: return opilli_rmwr_indirect_zero_page_y<&R6502::opf_rol,&R6502::opf_and>();
I case 0x34: return opill_nop_zero_page_x();
  case 0x35: return opi_read_zero_page_x<&R6502::opf_and>();
  case 0x36: return opi_rmw_zero_page_x<&R6502::opf_rol>();
I case 0x37: return opilli_rmwr_zero_page_x<&R6502::opf_rol,&R6502::opf_and>();
  case 0x38: return opi_set_flag(r.p.c);
  case 0x39: return opi_read_absolute_y<&R6502::opf_and>();
I case 0x3a: return opill_nop_implied();
I case 0x3b: return opilli_rmwr_absolute_y<&R6502::opf_rol,&R6502::opf_and>();
I case 0x3c: return opill_nop_absolute_x();
  case 0x3d: return opi_read_absolute_x<&R6502::opf_and>();
  case 0x3e: return opi_rmw_absolute_x<&R6502::opf_rol>();
I case 0x3f: return opilli_rmwr_absolute_x<&R6502::opf_rol,&R6502::opf_and>();
  case 0x40: return op_rti();
  case 0x41: return opi_read_indirect_zero_page_x<&R6502::opf_eor>();
I case 0x42: return opill_stp();
I case 0x43: return opilli_rmwr_indirect_zero_page_x<&R6502::opf_lsr,&R6502::opf_eor>();
I case 0x44: return opill_nop_zero_page();
  case 0x45: return opi_read_zero_page<&R6502::opf_eor>();
  case 0x46: return opi_rmw_zero_page<&R6502::opf_lsr>();
I case 0x47: return opilli_rmwr_zero_page<&R6502::opf_lsr,&R6502::opf_eor>();
  case 0x48: return opi_push(r.a);
  case 0x49: return opi_read_immediate<&R6502::opf_eor>();
  case 0x4a: return opi_shift<&R6502::opf_sra>();
I case 0x4b: return opill_alr_immediate();
  case 0x4c: return op_jmp_absolute();
  case 0x4d: return opi_read_absolute<&R6502::opf_eor>();
  case 0x4e: return opi_rmw_absolute<&R6502::opf_lsr>();
I case 0x4f: return opilli_rmwr_absolute<&R6502::opf_lsr,&R6502::opf_eor>();
  case 0x50: return opi_branch(r.p.v == 0);
  case 0x51: return opi_read_indirect_zero_page_y<&R6502::opf_eor>();
I case 0x52: return opill_stp();
I case 0x53: return opilli_rmwr_indirect_zero_page_y<&R6502::opf_lsr,&R6502::opf_eor>();
I case 0x54: return opill_nop_zero_page_x();
  case 0x55: return opi_read_zero_page_x<&R6502::opf_eor>();
  case 0x56: return opi_rmw_zero_page_x<&R6502::opf_lsr>();
I case 0x57: return opilli_rmwr_zero_page_x<&R6502::opf_lsr,&R6502::opf_eor>();
  case 0x58: return opi_clear_flag(r.p.i);
  case 0x59: return opi_read_absolute_y<&R6502::opf_eor>();
I case 0x5a: return opill_nop_implied();
I case 0x5b: return opilli_rmwr_absolute_y<&R6502::opf_lsr,&R6502::opf_eor>();
I case 0x5c: return opill_nop_absolute_x();
  case 0x5d: return opi_read_absolute_x<&R6502::opf_eor>();
  case 0x5e: return opi_rmw_absolute_x<&R6502::opf_lsr>();
I case 0x5f: return opilli_rmwr_absolute_x<&R6502::opf_lsr,&R6502::opf_eor>();
  case 0x60: return op_rts();
  case 0x61: return opi_read_indirect_zero_page_x<&R6502::opf_adc>();
I case 0x62: return opill_stp();
I case 0x63: return opilli_rmwr_indirect_zero_page_x<&R6502::opf_ror,&R6502::opf_adc>();
I case 0x64: return opill_nop_zero_page();
  case 0x65: return opi_read_zero_page<&R6502::opf_adc>();
  case 0x66: return opi_rmw_zero_page<&R6502::opf_ror>();
I case 0x67: return opilli_rmwr_zero_page<&R6502::opf_ror,&R6502::opf_adc>();
  case 0x68: return opi_pull(r.a);
  case 0x69: return opi_read_immediate<&R6502::opf_adc>();
  case 0x6a: return opi_shift<&R6502::opf_rra>();
I case 0x6b: return opill_arr_immediate();
  case 0x6c: return op_jmp_indirect_absolute();
  case 0x6d: return opi_read_absolute<&R6502::opf_adc>();
  case 0x6e: return opi_rmw_absolute<&R6502::opf_ror>();
I case 0x6f: return opilli_rmwr_absolute<&R6502::opf_ror,&R6502::opf_adc>();
  case 0x70: return opi_branch(r.p.v == 1);
  case 0x71: return opi_read_indirect_zero_page_y<&R6502::opf_adc>();
I case 0x72: return opill_stp();
I case 0x73: return opilli_rmwr_indirect_zero_page_y<&R6502::opf_ror,&R6502::opf_adc>();
I case 0x74: return opill_nop_zero_page_x();
  case 0x75: return opi_read_zero_page_x<&R6502::opf_adc>();
  case 0x76: return opi_rmw_zero_page_x<&R6502::opf_ror>();
I case 0x77: return opilli_rmwr_zero_page_x<&R6502::opf_ror,&R6502::opf_adc>();
  case 0x78: return opi_set_flag(r.p.i);
  case 0x79: return opi_read_absolute_y<&R6502::opf_adc>();
I case 0x7a: return opill_nop_implied();
I case 0x7b: return opilli_rmwr_absolute_y<&R6502::opf_ror,&R6502::opf_adc>();
I case 0x7c: return opill_nop_absolute_x();
  case 0x7d: return opi_read_absolute_x<&R6502::opf_adc>();
  case 0x7e: return opi_rmw_absolute_x<&R6502::opf_ror>();
I case 0x7f: return opilli_rmwr_absolute_x<&R6502::opf_ror,&R6502::opf_adc>();
I case 0x80: return opill_nop_immediate(); // r.y
  case 0x81: return opi_store_indirect_zero_page_x(r.a);
I case 0x82: return opill_nop_immediate(); // r.x
I case 0x83: return opi_store_indirect_zero_page_x(r.a & r.x);
  case 0x84: return opi_store_zero_page(r.y);
  case 0x85: return opi_store_zero_page(r.a);
  case 0x86: return opi_store_zero_page(r.x);
I case 0x87: return opi_store_zero_page(r.a & r.x);
  case 0x88: return opi_decrement(r.y);
I case 0x89: return opill_nop_immediate(); // r.a
  case 0x8a: return opi_transfer(r.x, r.a, 1);
I case 0x8b: return opill_xaa_immediate();
  case 0x8c: return opi_store_absolute(r.y);
  case 0x8d: return opi_store_absolute(r.a);
  case 0x8e: return opi_store_absolute(r.x);
I case 0x8f: return opi_store_absolute(r.a & r.x);
  case 0x90: return opi_branch(r.p.c == 0);
  case 0x91: return opi_store_indirect_zero_page_y(r.a);
I case 0x92: return opill_stp();
I case 0x93: return opill_sha_indirect_zero_page_y();
  case 0x94: return opi_store_zero_page_x(r.y);
  case 0x95: return opi_store_zero_page_x(r.a);
  case 0x96: return opi_store_zero_page_y(r.x);
I case 0x97: return opi_store_zero_page_y(r.a & r.x);
  case 0x98: return opi_transfer(r.y, r.a, 1);
  case 0x99: return opi_store_absolute_y(r.a);
  case 0x9a: return opi_transfer(r.x, r.s, 0);
I case 0x9b: return opill_tas_absolute_y();
I case 0x9c: return opill_shy_absolute_x();
  case 0x9d: return opi_store_absolute_x(r.a);
I case 0x9e: return opill_shx_absolute_y();
I case 0x9f: return opill_sha_absolute_y();
  case 0xa0: return opi_read_immediate<&R6502::opf_ldy>();
  case 0xa1: return opi_read_indirect_zero_page_x<&R6502::opf_lda>();
  case 0xa2: return opi_read_immediate<&R6502::opf_ldx>();
I case 0xa3: return opi_read_indirect_zero_page_x<&R6502::opillf_lax>();
  case 0xa4: return opi_read_zero_page<&R6502::opf_ldy>();
  case 0xa5: return opi_read_zero_page<&R6502::opf_lda>();
  case 0xa6: return opi_read_zero_page<&R6502::opf_ldx>();
I case 0xa7: return opi_read_zero_page<&R6502::opillf_lax>();
  case 0xa8: return opi_transfer(r.a, r.y, 1);
  case 0xa9: return opi_read_immediate<&R6502::opf_lda>();
  case 0xaa: return opi_transfer(r.a, r.x, 1);
I case 0xab: return opill_lxa_immediate();
  case 0xac: return opi_read_absolute<&R6502::opf_ldy>();
  case 0xad: return opi_read_absolute<&R6502::opf_lda>();
  case 0xae: return opi_read_absolute<&R6502::opf_ldx>();
I case 0xaf: return opi_read_absolute<&R6502::opillf_lax>();
  case 0xb0: return opi_branch(r.p.c == 1);
  case 0xb1: return opi_read_indirect_zero_page_y<&R6502::opf_lda>();
I case 0xb2: return opill_stp();
I case 0xb3: return opi_read_indirect_zero_page_y<&R6502::opillf_lax>();
  case 0xb4: return opi_read_zero_page_x<&R6502::opf_ldy>();
  case 0xb5: return opi_read_zero_page_x<&R6502::opf_lda>();
  case 0xb6: return opi_read_zero_page_y<&R6502::opf_ldx>();
I case 0xb7: return opi_read_zero_page_y<&R6502::opillf_lax>();
  case 0xb8: return opi_clear_flag(r.p.v);
  case 0xb9: return opi_read_absolute_y<&R6502::opf_lda>();
  case 0xba: return opi_transfer(r.s, r.x, 1);
I case 0xbb: return opill_las_absolute_y();
  case 0xbc: return opi_read_absolute_x<&R6502::opf_ldy>();
  case 0xbd: return opi_read_absolute_x<&R6502::opf_lda>();
  case 0xbe: return opi_read_absolute_y<&R6502::opf_ldx>();
I case 0xbf: return opi_read_absolute_y<&R6502::opillf_lax>();
  case 0xc0: return opi_read_immediate<&R6502::opf_cpy>();
  case 0xc1: return opi_read_indirect_zero_page_x<&R6502::opf_cmp>();
I case 0xc2: return opill_nop_immediate();
I case 0xc3: return opill_dcp_indirect_zero_page_x();
  case 0xc4: return opi_read_zero_page<&R6502::opf_cpy>();
  case 0xc5: return opi_read_zero_page<&R6502::opf_cmp>();
  case 0xc6: return opi_rmw_zero_page<&R6502::opf_dec>();
I case 0xc7: return opi_rmw_zero_page<&R6502::opillf_dcp>();
  case 0xc8: return opi_increment(r.y);
  case 0xc9: return opi_read_immediate<&R6502::opf_cmp>();
  case 0xca: return opi_decrement(r.x);
I case 0xcb: return opill_axs_immediate();
  case 0xcc: return opi_read_absolute<&R6502::opf_cpy>();
  case 0xcd: return opi_read_absolute<&R6502::opf_cmp>();
  case 0xce: return opi_rmw_absolute<&R6502::opf_dec>();
I case 0xcf: return opi_rmw_absolute<&R6502::opillf_dcp>();
  case 0xd0: return opi_branch(r.p.z == 0);
  case 0xd1: return opi_read_indirect_zero_page_y<&R6502::opf_cmp>();
I case 0xd2: return opill_stp();
I case 0xd3: return opill_dcp_indirect_zero_page_y();
I case 0xd4: return opill_nop_zero_page_x();
  case 0xd5: return opi_read_zero_page_x<&R6502::opf_cmp>();
  case 0xd6: return opi_rmw_zero_page_x<&R6502::opf_dec>();
I case 0xd7: return opi_rmw_zero_page_x<&R6502::opillf_dcp>();
  case 0xd8: return opi_clear_flag(r.p.d);
  case 0xd9: return opi_read_absolute_y<&R6502::opf_cmp>();
I case 0xda: return opill_nop_implied();
I case 0xdb: return opill_dcp_absolute_y();
I case 0xdc: return opill_nop_absolute_x();
  case 0xdd: return opi_read_absolute_x<&R6502::opf_cmp>();
  case 0xde: return opi_rmw_absolute_x<&R6502::opf_dec>();
I case 0xdf: return opi_rmw_absolute_x<&R6502::opillf_dcp>();
  case 0xe0: return opi_read_immediate<&R6502::opf_cpx>();
  case 0xe1: return opi_read_indirect_zero_page_x<&R6502::opf_sbc>();
I case 0xe2: return opill_nop_immediate();
I case 0xe3: return opilli_rmwr_indirect_zero_page_x<&R6502::opf_inc,&R6502::opf_sbc>();
  case 0xe4: return opi_read_zero_page<&R6502::opf_cpx>();
  case 0xe5: return opi_read_zero_page<&R6502::opf_sbc>();
  case 0xe6: return opi_rmw_zero_page<&R6502::opf_inc>();
I case 0xe7: return opilli_rmwr_zero_page<&R6502::opf_inc,&R6502::opf_sbc>();
  case 0xe8: return opi_increment(r.x);
  case 0xe9: return opi_read_immediate<&R6502::opf_sbc>();
  case 0xea: return op_nop();
I case 0xeb: return opi_read_immediate<&R6502::opf_sbc>();
  case 0xec: return opi_read_absolute<&R6502::opf_cpx>();
  case 0xed: return opi_read_absolute<&R6502::opf_sbc>();
  case 0xee: return opi_rmw_absolute<&R6502::opf_inc>();
I case 0xef: return opilli_rmwr_absolute<&R6502::opf_inc,&R6502::opf_sbc>();
  case 0xf0: return opi_branch(r.p.z == 1);
  case 0xf1: return opi_read_indirect_zero_page_y<&R6502::opf_sbc>();
I case 0xf2: return opill_stp();
I case 0xf3: return opilli_rmwr_indirect_zero_page_y<&R6502::opf_inc,&R6502::opf_sbc>();
I case 0xf4: return opill_nop_zero_page_x();
  case 0xf5: return opi_read_zero_page_x<&R6502::opf_sbc>();
  case 0xf6: return opi_rmw_zero_page_x<&R6502::opf_inc>();
I case 0xf7: return opilli_rmwr_zero_page_x<&R6502::opf_inc,&R6502::opf_sbc>();
  case 0xf8: return opi_set_flag(r.p.d);
  case 0xf9: return opi_read_absolute_y<&R6502::opf_sbc>();
I case 0xfa: return opill_nop_implied();
I case 0xfb: return opilli_rmwr_absolute_y<&R6502::opf_inc,&R6502::opf_sbc>();
I case 0xfc: return opill_nop_absolute_x();
  case 0xfd: return opi_read_absolute_x<&R6502::opf_sbc>();
  case 0xfe: return opi_rmw_absolute_x<&R6502::opf_inc>();
I case 0xff: return opilli_rmwr_absolute_x<&R6502::opf_inc,&R6502::opf_sbc>();
  }

  //unimplemented opcode
  return op_nop();
}

#undef I
#undef L
#undef call

}
