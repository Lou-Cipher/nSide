auto PPU::enable() const -> bool {
  return r.bgEnable || r.objEnable;
}

auto PPU::loadCHR(uint14 addr) -> uint8 {
  return enable() ? cartridge.readCHR(r.chrAddressBus = addr) : (uint8)0x00;
}

auto PPU::renderPixel() -> void {
  if(vcounter() >= 240) return;
  uint x = hcounter() - 1;
  uint mask = 0x8000 >> (r.v.fineX + (x & 7));
  uint palette = 0;
  uint objectPalette = 0;
  bool objectPriority = 0;

  palette |= l.tiledataLo & mask ? 1 : 0;
  palette |= l.tiledataHi & mask ? 2 : 0;
  if(palette) {
    uint attr = l.attribute;
    if(mask >= 256) attr >>= 2;
    palette |= (attr & 3) << 2;
  }

  if(!r.bgEnable) palette = 0;
  if(!r.bgEdgeEnable && x < 8) palette = 0;

  if(r.objEnable)
  for(int sprite = 7; sprite >= 0; sprite--) {
    if(!r.objEdgeEnable && x < 8) continue;
    if(l.oam[sprite].id == 64) continue;

    uint spriteX = x - l.oam[sprite].x;
    if(spriteX >= 8) continue;

    if(l.oam[sprite].attr & 0x40) spriteX ^= 7;
    uint mask = 0x80 >> spriteX;
    uint spritePalette = 0;
    spritePalette |= l.oam[sprite].tiledataLo & mask ? 1 : 0;
    spritePalette |= l.oam[sprite].tiledataHi & mask ? 2 : 0;
    if(spritePalette == 0) continue;

    if(l.oam[sprite].id == 0 && palette && x != 255) r.spriteZeroHit = 1;
    spritePalette |= (l.oam[sprite].attr & 3) << 2;

    objectPriority = l.oam[sprite].attr & 0x20;
    objectPalette = 16 + spritePalette;
  }

  if(objectPalette) {
    if(palette == 0 || objectPriority == 0) palette = objectPalette;
  }

  if(version >= Version::RP2C07 && (x < 2 || x >= 254 || vcounter() <= 0)) {
    output[vcounter() * 256 + x] = (r.emphasis << 6) | 0x1d;
    return;
  } else if(!enable()) {
    if((r.v.addressHi & 0x3f) != 0x3f) palette = ext();
    else palette = r.v.addressLo;
  } else {
    if(!palette) palette = ext();
  }
  output[vcounter() * 256 + x] = r.emphasis << 6 | readCGRAM(palette);
}

auto PPU::renderSprite() -> void {
  if(!enable()) return;

  uint n = l.oamIterator++;
  uint lastScanline = system.region() == System::Region::NTSC ? 261 : 311;
  int ly = vcounter() == lastScanline ? -1 : (int)vcounter();
  uint y = ly - oam[n * 4 + 0];

  if(y >= r.spriteHeight) return;
  if(l.oamCounter == 8) {
    r.spriteOverflow = 1;
    return;
  }

  auto& o = l.soam[l.oamCounter++];
  o.id   = n;
  o.y    = oam[n * 4 + 0];
  o.tile = oam[n * 4 + 1];
  o.attr = oam[n * 4 + 2];
  o.x    = oam[n * 4 + 3];
}

auto PPU::renderScanline() -> void {
  uint lastScanline = system.region() == System::Region::NTSC ? 261 : 311;
  //Vblank
  if((vcounter() >= 240 && vcounter() < lastScanline)) return addClocks(341), scanline();

  l.oamIterator = 0;
  l.oamCounter = 0;

  for(auto n : range(8)) l.soam[n] = {};

  //  0
  addClocks(1);

  //  1-256
  for(uint tile : range(32)) {
    uint nametable = loadCHR(0x2000 | (uint12)r.v.address);
    uint tileaddr = r.bgAddress | nametable << 4 | r.v.fineY;
    renderPixel();
    addClocks(1);

    renderPixel();
    addClocks(1);

    uint attribute = loadCHR(0x23c0 | r.v.nametable << 10 | (r.v.tileY >> 2) << 3 | r.v.tileX >> 2);
    if(r.v.tileY & 2) attribute >>= 4;
    if(r.v.tileX & 2) attribute >>= 2;
    renderPixel();
    addClocks(1);

    if(enable() && ++r.v.tileX == 0) r.v.nametableX ^= 1;
    if(enable() && tile == 31 && ++r.v.fineY == 0 && ++r.v.tileY == 30) r.v.nametableY ^= 1, r.v.tileY = 0;
    renderPixel();
    renderSprite();
    addClocks(1);

    uint tiledataLo = loadCHR(tileaddr + 0);
    renderPixel();
    addClocks(1);

    renderPixel();
    addClocks(1);

    uint tiledataHi = loadCHR(tileaddr + 8);
    renderPixel();
    addClocks(1);

    renderPixel();
    renderSprite();
    addClocks(1);

    l.nametable = l.nametable << 8 | nametable;
    l.attribute = l.attribute << 2 | (attribute & 3);
    l.tiledataLo = l.tiledataLo << 8 | tiledataLo;
    l.tiledataHi = l.tiledataHi << 8 | tiledataHi;
  }

  for(auto n : range(8)) l.oam[n] = l.soam[n];

  //257-320
  for(uint sprite : range(8)) {
    uint nametable = loadCHR(0x2000 | (uint12)r.v.address);
    addClocks(1);

    if(enable() && sprite == 0) {
      //258
      r.v.nametableX = r.t.nametableX;
      r.v.tileX = r.t.tileX;
    }
    addClocks(1);

    uint attribute = loadCHR(0x23c0 | r.v.nametable << 10 | (r.v.tileY >> 2) << 3 | r.v.tileX >> 2);
    uint tileaddr = r.spriteHeight == 8
    ? r.objAddress + l.oam[sprite].tile * 16
    : (l.oam[sprite].tile & ~1) * 16 + (l.oam[sprite].tile & 1) * 0x1000;
    addClocks(2);

    uint spriteY = (vcounter() - l.oam[sprite].y) & (r.spriteHeight - 1);
    if(l.oam[sprite].attr & 0x80) spriteY ^= r.spriteHeight - 1;
    tileaddr += spriteY + (spriteY & 8);

    l.oam[sprite].tiledataLo = loadCHR(tileaddr + 0);
    addClocks(2);

    l.oam[sprite].tiledataHi = loadCHR(tileaddr + 8);
    addClocks(2);

    if(enable() && sprite == 6 && vcounter() == lastScanline) {
      //305
      r.v.address = r.t.address;
    }
  }

  //321-336
  for(uint tile : range(2)) {
    uint nametable = loadCHR(0x2000 | (uint12)r.v.address);
    uint tileaddr = r.bgAddress | nametable << 4 | r.v.fineY;
    addClocks(2);

    uint attribute = loadCHR(0x23c0 | r.v.nametable << 10 | (r.v.tileY >> 2) << 3 | r.v.tileX >> 2);
    if(r.v.tileY & 2) attribute >>= 4;
    if(r.v.tileX & 2) attribute >>= 2;
    addClocks(1);

    if(enable() && ++r.v.tileX == 0) r.v.nametableX ^= 1;
    addClocks(1);

    uint tiledataLo = loadCHR(tileaddr + 0);
    addClocks(2);

    uint tiledataHi = loadCHR(tileaddr + 8);
    addClocks(2);

    l.nametable = l.nametable << 8 | nametable;
    l.attribute = l.attribute << 2 | (attribute & 3);
    l.tiledataLo = l.tiledataLo << 8 | tiledataLo;
    l.tiledataHi = l.tiledataHi << 8 | tiledataHi;
  }

  //337-338
  loadCHR(0x2000 | (uint12)r.v.address);
  addClocks(2);

  //339
  loadCHR(0x2000 | (uint12)r.v.address);
  addClocks(1);

  //340
  if(hcounter() > 0) addClocks(1);

  return scanline();
}