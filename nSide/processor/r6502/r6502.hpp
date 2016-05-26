//Ricoh 6502
//* Ricoh 2A03
//* Ricoh 2A07

#pragma once

namespace Processor {

struct R6502 {
  #include "registers.hpp"
  #include "memory.hpp"

  virtual auto read(uint16 addr) -> uint8 = 0;
  virtual auto write(uint16 addr, uint8 data) -> void = 0;
  virtual auto lastCycle() -> void = 0;
  virtual auto nmi(uint16& vector) -> void = 0;

  virtual auto disassemblerRead(uint16 addr) -> uint8 { return 0u; }

  auto mdr() const -> uint8;
  auto power() -> void;
  auto reset() -> void;
  auto interrupt() -> void;
  auto instruction() -> void;

  //instructions.cpp
  auto opf_asl();
  auto opf_adc();
  auto opf_and();
  auto opf_bit();
  auto opf_cmp();
  auto opf_cpx();
  auto opf_cpy();
  auto opf_dec();
  auto opf_eor();
  auto opf_inc();
  auto opf_lda();
  auto opf_ldx();
  auto opf_ldy();
  auto opf_lsr();
  auto opf_ora();
  auto opf_rla();
  auto opf_rol();
  auto opf_ror();
  auto opf_rra();
  auto opf_sbc();
  auto opf_sla();
  auto opf_sra();

  auto opi_branch(bool condition);
  auto opi_clear_flag(bool& flag);
  auto opi_decrement(uint8& r);
  auto opi_increment(uint8& r);
  auto opi_pull(uint8& r);
  auto opi_push(uint8& r);
  template<auto (R6502::*op)() -> void> auto opi_read_absolute();
  template<auto (R6502::*op)() -> void> auto opi_read_absolute_x();
  template<auto (R6502::*op)() -> void> auto opi_read_absolute_y();
  template<auto (R6502::*op)() -> void> auto opi_read_immediate();
  template<auto (R6502::*op)() -> void> auto opi_read_indirect_zero_page_x();
  template<auto (R6502::*op)() -> void> auto opi_read_indirect_zero_page_y();
  template<auto (R6502::*op)() -> void> auto opi_read_zero_page();
  template<auto (R6502::*op)() -> void> auto opi_read_zero_page_x();
  template<auto (R6502::*op)() -> void> auto opi_read_zero_page_y();
  template<auto (R6502::*op)() -> void> auto opi_rmw_absolute();
  template<auto (R6502::*op)() -> void> auto opi_rmw_absolute_x();
  template<auto (R6502::*op)() -> void> auto opi_rmw_zero_page();
  template<auto (R6502::*op)() -> void> auto opi_rmw_zero_page_x();
  auto opi_set_flag(bool& flag);
  template<auto (R6502::*op)() -> void> auto opi_shift();
  auto opi_store_absolute(uint8);
  auto opi_store_absolute_x(uint8&);
  auto opi_store_absolute_y(uint8&);
  auto opi_store_indirect_zero_page_x(uint8);
  auto opi_store_indirect_zero_page_y(uint8&);
  auto opi_store_zero_page(uint8);
  auto opi_store_zero_page_x(uint8);
  auto opi_store_zero_page_y(uint8);
  auto opi_transfer(uint8& s, uint8& d, bool flag);

  auto op_brk();
  auto op_jmp_absolute();
  auto op_jmp_indirect_absolute();
  auto op_jsr_absolute();
  auto op_nop();
  auto op_php();
  auto op_plp();
  auto op_rti();
  auto op_rts();

  auto opillf_dcp();
  auto opillf_lax();

  template<auto (R6502::*opw)() -> void,auto (R6502::*opr)() -> void>
  auto opilli_rmwr_absolute();
  template<auto (R6502::*opw)() -> void,auto (R6502::*opr)() -> void>
  auto opilli_rmwr_absolute_x();
  template<auto (R6502::*opw)() -> void,auto (R6502::*opr)() -> void>
  auto opilli_rmwr_absolute_y();
  template<auto (R6502::*opw)() -> void,auto (R6502::*opr)() -> void>
  auto opilli_rmwr_indirect_zero_page_x();
  template<auto (R6502::*opw)() -> void,auto (R6502::*opr)() -> void>
  auto opilli_rmwr_indirect_zero_page_y();
  template<auto (R6502::*opw)() -> void,auto (R6502::*opr)() -> void>
  auto opilli_rmwr_zero_page();
  template<auto (R6502::*opw)() -> void,auto (R6502::*opr)() -> void>
  auto opilli_rmwr_zero_page_x();

  auto opill_alr_immediate();
  auto opill_anc_immediate();
  auto opill_arr_immediate();
  auto opill_axs_immediate();
  auto opill_dcp_absolute_y();
  auto opill_dcp_indirect_zero_page_x();
  auto opill_dcp_indirect_zero_page_y();
  auto opill_las_absolute_y();
  auto opill_lxa_immediate();
  auto opill_nop_absolute();
  auto opill_nop_absolute_x();
  auto opill_nop_immediate();
  auto opill_nop_implied();
  auto opill_nop_zero_page();
  auto opill_nop_zero_page_x();
  auto opill_sha_absolute_y();
  auto opill_sha_indirect_zero_page_y();
  auto opill_shx_absolute_y();
  auto opill_shy_absolute_x();
  auto opill_stp();
  auto opill_tas_absolute_y();
  auto opill_xaa_immediate();

  //serialization.cpp
  auto serialize(serializer&) -> void;

  //disassembler.cpp
  auto disassemble() -> string;

  Registers r;
  reg16 abs, iabs;
  uint8 rd, zp;
  uint16 aa;
};

}
