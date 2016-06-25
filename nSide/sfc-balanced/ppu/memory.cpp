auto PPU::getVramAddress() -> uint16 {
  uint16 address = r.vramAddress;
  switch(r.vramMapping) {
  case 0: return (address);
  case 1: return (address & 0xff00) | ((address & 0x001f) << 3) | ((address >> 5) & 7);
  case 2: return (address & 0xfe00) | ((address & 0x003f) << 3) | ((address >> 6) & 7);
  case 3: return (address & 0xfc00) | ((address & 0x007f) << 3) | ((address >> 7) & 7);
  }
  unreachable;
}

//NOTE: all VRAM writes during active display are invalid. Unlike OAM and CGRAM, they will
//not be written anywhere at all. The below address ranges for where writes are invalid have
//been validated on hardware, as has the edge case where the S-CPU MDR can be written if the
//write occurs during the very last clock cycle of vblank.

auto PPU::vramRead(bool chip, uint15 addr) -> uint8 {
  uint8 data;

  if(r.displayDisable) {
    data = vram[addr].byte(chip);
  } else {
    uint16 v = cpu.vcounter();
    uint16 h = cpu.hcounter();
    uint16 ls = ((system.region() == System::Region::NTSC ? 525 : 625) >> 1) - 1;
    if(interlace() && !cpu.field()) ls++;

    if(v == ls && h == 1362) {
      data = 0x00;
    } else if(v < vdisp() - 1) {
      data = 0x00;
    } else if(v == vdisp() - 1) {
      if(h == 1362) {
        data = vram[addr].byte(chip);
      } else {
        data = 0x00;
      }
    } else {
      data = vram[addr].byte(chip);
    }
  }

  return data;
}

auto PPU::vramWrite(bool chip, uint15 addr, uint8 data) -> void {
  if(r.displayDisable) {
    vram[addr].byte(chip) = data;
  } else {
    uint16 v = cpu.vcounter();
    uint16 h = cpu.hcounter();
    if(v == 0) {
      if(h <= 4) {
        vram[addr].byte(chip) = data;
      } else if(h == 6) {
        vram[addr].byte(chip) = cpu.r.mdr;
      } else {
        //no write
      }
    } else if(v < vdisp()) {
      //no write
    } else if(v == vdisp()) {
      if(h <= 4) {
        //no write
      } else {
        vram[addr].byte(chip) = data;
      }
    } else {
      vram[addr].byte(chip) = data;
    }
  }
}

auto PPU::oamRead(uint addr) -> uint8 {
  addr &= 0x03ff;
  if(addr & 0x0200) addr &= 0x021f;
  uint8 data;

  if(r.displayDisable) {
    data = oam[addr];
  } else {
    if(cpu.vcounter() < vdisp()) {
      data = oam[latch.oamAddress];
    } else {
      data = oam[addr];
    }
  }

  return data;
}

auto PPU::oamWrite(uint addr, uint8 data) -> void {
  addr &= 0x03ff;
  if(addr & 0x0200) addr &= 0x021f;

  spriteListValid = false;

  if(r.displayDisable) {
    oam[addr] = data;
    updateSpriteList(addr, data);
  } else {
    if(cpu.vcounter() < vdisp()) {
      oam[latch.oamAddress] = data;
      updateSpriteList(latch.oamAddress, data);
    } else {
      oam[addr] = data;
      updateSpriteList(addr, data);
    }
  }
}

auto PPU::cgramRead(uint addr) -> uint8 {
  addr &= 0x01ff;
  uint8 data;

  if(1 || r.displayDisable) {
    data = cgram[addr];
  } else {
    uint16 v = cpu.vcounter();
    uint16 h = cpu.hcounter();
    if(v < vdisp() && h >= 128 && h < 1096) {
      data = cgram[latch.cgramAddress] & 0x7f;
    } else {
      data = cgram[addr];
    }
  }

  if(addr & 1) data &= 0x7f;
  return data;
}

auto PPU::cgramWrite(uint addr, uint8 data) -> void {
  addr &= 0x01ff;
  if(addr & 1) data &= 0x7f;

  if(1 || r.displayDisable) {
    cgram[addr] = data;
  } else {
    uint16 v = cpu.vcounter();
    uint16 h = cpu.hcounter();
    if(v < vdisp() && h >= 128 && h < 1096) {
      cgram[latch.cgramAddress] = data & 0x7f;
    } else {
      cgram[addr] = data;
    }
  }
}
