struct PCEngineCartridge {
  PCEngineCartridge(string location, uint8_t* data, uint size);

  string manifest;

//private:
  struct Information {
  } information;
};

PCEngineCartridge::PCEngineCartridge(string location, uint8_t* data, uint size) {
  manifest.append("board\n");
  manifest.append("  rom name=program.rom size=0x", hex(size), "\n");
  manifest.append("  ram name=save.ram size=0x8000\n");
  manifest.append("\n");
  manifest.append("information\n");
  manifest.append("  title:  ", Location::prefix(location), "\n");
  manifest.append("  sha256: ", Hash::SHA256(data, size).digest(), "\n");
  manifest.append("\n");
  manifest.append("note: heuristically generated by cart-pal\n");
}
