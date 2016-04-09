struct PAL_ZZ : Board {
  PAL_ZZ(Markup::Node& board_node) : Board(board_node), mmc3(*this, board_node) {
  }

  auto main() -> void {
    mmc3.main();
  }

  auto prg_read(uint addr) -> uint8 {
    if(addr & 0x8000) {
      addr = (mmc3.prg_addr(addr) & (0xffff | (bank << 16))) | (bank << 17);
      addr |= (prg_a16_force == 0x03) << 16;
      return read(prgrom, addr);
    }
    return cpu.mdr();
  }

  auto prg_write(uint addr, uint8 data) -> void {
    if((addr & 0xe000) == 0x6000 && mmc3.ram_enable && !mmc3.ram_write_protect) {
      prg_a16_force = data & 0x03;
      bank = data & 0x04;
    }
    if(addr & 0x8000) return mmc3.reg_write(addr, data);
  }

  auto chr_read(uint addr) -> uint8 {
    if(addr & 0x2000) return ppu.ciramRead(mmc3.ciram_addr(addr));
    return Board::chr_read((mmc3.chr_addr(addr) & 0x1ffff) | (bank << 17));
  }

  auto chr_write(uint addr, uint8 data) -> void {
    if(addr & 0x2000) return ppu.ciramWrite(mmc3.ciram_addr(addr), data);
    return Board::chr_write((mmc3.chr_addr(addr) & 0x1ffff) | (bank << 17), data);
  }

  auto ciram_addr(uint addr) -> uint {
    return mmc3.ciram_addr(addr);
  }

  auto power() -> void {
    mmc3.power();
  }

  auto reset() -> void {
    mmc3.reset();
    cic_reset();
  }

  auto cic_reset() -> void {
    // this register is cleared by the CIC reset line.
    // On a Famicom or toploader, only a power cycle can clear it.
    //TODO: Check if Europe got the toploader.
    prg_a16_force = 0;
    bank = 0;
  }

  auto serialize(serializer& s) -> void {
    Board::serialize(s);
    mmc3.serialize(s);
    s.integer(prg_a16_force);
    s.integer(bank);
  }

  MMC3 mmc3;
  uint2 prg_a16_force;
  bool bank;
};
