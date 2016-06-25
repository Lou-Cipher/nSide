#include <fc/fc.hpp>

namespace Famicom {

System system;

#include "video.cpp"
#include "peripherals.cpp"
#include "serialization.cpp"

auto System::run() -> void {
  scheduler.enter();
}

auto System::runToSave() -> void {
  scheduler.synchronize(ppu.thread);
  scheduler.synchronize(cpu.thread);
  scheduler.synchronize(apu.thread);
  scheduler.synchronize(cartridge.thread);
  for(auto coprocessor : cpu.coprocessors) {
    scheduler.synchronize(coprocessor->thread);
  }
  for(auto peripheral : cpu.peripherals) {
    scheduler.synchronize(peripheral->thread);
  }
}

auto System::init() -> void {
  assert(interface != nullptr);

  vssystem.init();
  playchoice10.init();
  famicombox.init();
}

auto System::term() -> void {
}

auto System::load(Revision revision) -> bool {
  bus.reset();

  _revision = revision;

  if(auto fp = interface->open(ID::System, "manifest.bml", File::Read, File::Required)) {
    information.manifest = fp->reads();
  } else return false;

  auto document = BML::unserialize(information.manifest);

  if(pc10()) {
    if(auto firmware = document["system/pc10/cpu/rom/name"].text()) {
      if(auto fp = interface->open(ID::System, firmware, File::Read, File::Required)) {
        fp->read(playchoice10.bios, 16384);
      } else return false;
    }

    if(auto character = document["system/pc10/video-circuit/vrom/name"].text()) {
      if(auto fp = interface->open(ID::System, character, File::Read, File::Required)) {
        fp->read(playchoice10.videoCircuit.chrrom, 24576);
      } else return false;
    }

    if(auto palette = document["system/pc10/video-circuit/cgrom/name"].text()) {
      if(auto fp = interface->open(ID::System, palette, File::Read, File::Required)) {
        fp->read(playchoice10.videoCircuit.cgrom, 768);
      } else return false;
    }
  }

  if(fcb()) {
    if(auto bios_prg = document["system/prg/rom/name"].text()) {
      if(auto fp = interface->open(ID::System, bios_prg, File::Read, File::Required)) {
        fp->read(famicombox.bios_prg, 32768);
      } else return false;
    }

    if(auto bios_chr = document["system/prg/rom/name"].text()) {
      if(auto fp = interface->open(ID::System, bios_chr, File::Read, File::Required)) {
        fp->read(famicombox.bios_chr, 8192);
      } else return false;
    }
  }

  if(!cartridge.load()) return false;
  switch(cartridge.region()) {
  case Cartridge::Region::NTSC:  _region = Region::NTSC;  break;
  case Cartridge::Region::PAL:   _region = Region::PAL;   break;
  case Cartridge::Region::Dendy: _region = Region::Dendy; break;
  }
  _cpuFrequency = region() == Region::NTSC ? 21'477'272 : 26'601'712;

  interface->information.canvasWidth  = 256;
  interface->information.canvasHeight = 240;
  interface->information.aspectRatio = 8.0 / 7.0;

  switch(revision) {

  case Revision::Famicom: {
    apu.revision = APU::Revision::RP2A03G;
    // most Famicoms use a PPU with open bus OAMDATA (read).
    // For now, we use an NES PPU where OAMDATA (read) is defined.
    switch(region()) {
    case Region::NTSC:  ppu.revision = PPU::Revision::RP2C02G; break;
    case Region::PAL:   ppu.revision = PPU::Revision::RP2C07;  break;
    case Region::Dendy: ppu.revision = PPU::Revision::UA6538;  break;
    }
    if(region() != Region::NTSC) interface->information.aspectRatio = 2'950'000.0 / 2'128'137.0;
    peripherals.connect(ID::Port::Arcade, ID::Device::None);
    break;
  }

  case Revision::VSSystem: {
    apu.revision = APU::Revision::RP2A03;
    // VS. System PPU is set within cartridge.load().
    vssystem.load();
    peripherals.connect(ID::Port::Arcade, ID::Device::VSPanel);
    break;
  }

  case Revision::PlayChoice10: {
    apu.revision = APU::Revision::RP2A03G;
    ppu.revision = PPU::Revision::RP2C03B;
    playchoice10.screenConfig = min(max(document["system/pc10/screen/mode"].integer(), 1), 2);
    playchoice10.load();
    interface->information.canvasHeight = playchoice10.screenConfig * 240;
    peripherals.connect(ID::Port::Arcade, ID::Device::None);
    break;
  }

  case Revision::FamicomBox: {
    apu.revision = APU::Revision::RP2A03E;
    ppu.revision = PPU::Revision::RP2C02C;
    famicombox.load();
    peripherals.connect(ID::Port::Arcade, ID::Device::None);
    break;
  }

  }

  if(fc()) {
    //Force use of the RGB PPU if a "pc10" node is found in a Famicom manifest
    auto node = BML::unserialize(cartridge.information.manifest.cartridge);
    if(node["board/pc10"]) ppu.revision = PPU::Revision::RP2C03B;
  }

  serializeInit();
  return _loaded = true;
}

auto System::save() -> void {
  if(!loaded()) return;
  cartridge.save();
}

auto System::unload() -> void {
  if(!loaded()) return;
  peripherals.unload();

  switch(revision()) {
  case Revision::VSSystem:     vssystem.unload(); break;
  case Revision::PlayChoice10: playchoice10.unload(); break;
  case Revision::FamicomBox:   famicombox.unload(); break;
  }

  cartridge.unload();
  _loaded = false;
}

auto System::power() -> void {
  cartridge.power();
  cpu.power();
  apu.power();
  ppu.power();

  switch(revision()) {
  case Revision::VSSystem:     vssystem.power(); break;
  case Revision::PlayChoice10: playchoice10.power(); break;
  case Revision::FamicomBox:   famicombox.power(); break;
  }

  ppu.reset();
  reset();
}

auto System::reset() -> void {
  Emulator::video.reset();
  Emulator::video.setInterface(interface);
  //Emulator::video.resize() is called in configureVideoEffects()
  configureVideoPalette();
  configureVideoEffects();

  Emulator::audio.reset();
  Emulator::audio.setInterface(interface);

  cartridge.reset();
  cpu.reset();
  apu.reset();
  // Only the NES front-loader's PPU will reset. The Famicom's and NES
  // top-loader's PPU will not.
  //ppu.reset();

  switch(revision()) {
  case Revision::VSSystem:     vssystem.reset(); break;
  case Revision::PlayChoice10: playchoice10.reset(); break;
  case Revision::FamicomBox:   famicombox.reset(); break;
  }

  switch(revision()) {
  case Revision::VSSystem:     cpu.coprocessors.append(&vssystem); break;
  case Revision::PlayChoice10: cpu.coprocessors.append(&playchoice10.pc10cpu); break;
  case Revision::FamicomBox:   cpu.coprocessors.append(&famicombox); break;
  }

  scheduler.reset();
  peripherals.reset();
}

}
