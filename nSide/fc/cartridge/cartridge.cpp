#include <fc/fc.hpp>

namespace Famicom {

#include "load.cpp"
#include "save.cpp"

#include "chip/chip.cpp"

#define cpu (Model::VSSystem() && slot ? cpuS : cpuM)
#define apu (Model::VSSystem() && slot ? apuS : apuM)
#define ppu (Model::VSSystem() && slot ? ppuS : ppuM)

#include "board/board.cpp"
#include "serialization.cpp"
vector<Cartridge> cartridgeSlot;

Cartridge::Cartridge(uint slot) : slot(slot) {
}

auto Cartridge::manifest() const -> string {
  string manifest = information.manifest.cartridge;
  if(information.manifest.famicomDiskSystem) manifest.append("\n[[Famicom Disk System]]\n\n", information.manifest.famicomDiskSystem);
  return manifest;
}

auto Cartridge::title() const -> string {
  string title = information.title.cartridge;
  if(information.title.famicomDiskSystem) title.append(" + ", information.title.famicomDiskSystem);
  return title;
}

auto Cartridge::Enter() -> void {
  while(true) {
    scheduler.synchronize();
    if(cartridgeSlot[busM.slot].active()) cartridgeSlot[busM.slot].main();
    if(cartridgeSlot[busS.slot].active()) cartridgeSlot[busS.slot].main();
  }
}

auto Cartridge::main() -> void {
  board->main();
}

auto Cartridge::load() -> bool {
  information = Information();

  if(Model::Famicom()) {
    if(auto loaded = platform->load(ID::Famicom, "Famicom", "fc", {"Auto", "NTSC-J", "NTSC-U", "PAL", "Dendy"})) {
      information.pathID = loaded.pathID();
      information.region = loaded.option();
    } else return false;
  }

  if(Model::VSSystem()) {
    if(auto loaded = platform->load(ID::VSSystem, "VS. System", "vs")) {
      information.pathID = loaded.pathID();
      information.region = "NTSC-J";
    } else return false;
  }

  if(Model::PlayChoice10()) {
    if(auto loaded = platform->load(ID::PlayChoice10, "PlayChoice-10", "pc10")) {
      information.pathID = loaded.pathID();
      information.region = "NTSC-U";
    } else return false;
  }

  if(Model::FamicomBox()) {
    if(auto loaded = platform->load(ID::FamicomBox, "FamicomBox", "fcb")) {
      information.pathID = loaded.pathID();
      information.region = "NTSC-J";
    } else return false;
  }

  if(auto fp = platform->open(pathID(), "manifest.bml", File::Read, File::Required)) {
    information.manifest.cartridge = fp->reads();
  } else return false;

  auto document = BML::unserialize(information.manifest.cartridge);
  loadCartridge(document);
  if(!board) return Model::VSSystem() && cartridgeSlot[1].board;

  //Famicom Disk System
  if(false) {
  }

  //Famicom
  else {
    Hash::SHA256 sha;
    //hash each ROM image that exists; any with size() == 0 is ignored by sha256_chunk()
    sha.input(board->prgrom.data(), board->prgrom.size());
    sha.input(board->chrrom.data(), board->chrrom.size());
    sha.input(board->instrom.data(), board->instrom.size());
    sha.input(board->keyrom.data(), board->keyrom.size());
    //finalize hash
    information.sha256 = sha.digest();
  }

  board->prgrom.writeProtect(true);
  board->prgram.writeProtect(false);
  board->chrrom.writeProtect(true);
  board->chrram.writeProtect(false);
  if(board->chip) board->chip->ram.writeProtect(false);
  board->instrom.writeProtect(true);
  board->keyrom.writeProtect(true);
  return true;
}

auto Cartridge::save() -> void {
  saveCartridge(BML::unserialize(information.manifest.cartridge));
}

auto Cartridge::unload() -> void {
  board->prgrom.reset();
  board->chrrom.reset();
  board->instrom.reset();
  board->keyrom.reset();
}

auto Cartridge::power(bool reset) -> void {
  create(Cartridge::Enter, system.frequency());
  board->power(reset);
}

auto Cartridge::readPRG(uint addr, uint8 data) -> uint8 {
  return board->readPRG(addr, data);
}

auto Cartridge::writePRG(uint addr, uint8 data) -> void {
  return board->writePRG(addr, data);
}

auto Cartridge::readCHR(uint addr, uint8 data) -> uint8 {
  return board->readCHR(addr, data);
}

auto Cartridge::writeCHR(uint addr, uint8 data) -> void {
  return board->writeCHR(addr, data);
}

auto Cartridge::scanline(uint y) -> void {
  return board->scanline(y);
}

#undef cpu
#undef apu
#undef ppu

}
