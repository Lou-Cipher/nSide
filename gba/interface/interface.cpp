#include <gba/gba.hpp>

namespace GameBoyAdvance {

Interface* interface = nullptr;

string Interface::title() {
  return cartridge.title();
}

double Interface::videoFrequency() {
  return 16777216.0 / (228.0 * 1232.0);
}

double Interface::audioFrequency() {
  return 16777216.0 / 512.0;
}

bool Interface::loaded() {
  return cartridge.loaded();
}

unsigned Interface::group(unsigned id) {
  switch(id) {
  case ID::SystemManifest:
  case ID::BIOS:
    return ID::System;
  case ID::Manifest:
  case ID::ROM:
  case ID::RAM:
  case ID::EEPROM:
  case ID::FlashROM:
    return ID::GameBoyAdvance;
  }

  throw;
}

void Interface::load(unsigned id) {
  cartridge.load();
}

void Interface::save() {
  for(auto& memory : cartridge.memory) {
    interface->saveRequest(memory.id, memory.name);
  }
}

void Interface::load(unsigned id, const stream& stream) {
  if(id == ID::SystemManifest) {
    system.information.manifest = stream.text();
  }

  if(id == ID::BIOS) {
    stream.read(bios.data, min(bios.size, stream.size()));
  }

  if(id == ID::Manifest) {
    cartridge.information.markup = stream.text();
  }

  if(id == ID::ROM) {
    stream.read(cartridge.rom.data, min(cartridge.rom.size, stream.size()));
  }

  if(id == ID::RAM) {
    stream.read(cartridge.ram.data, min(cartridge.ram.size, stream.size()));
  }

  if(id == ID::EEPROM) {
    stream.read(cartridge.eeprom.data, min(cartridge.eeprom.size, stream.size()));
  }

  if(id == ID::FlashROM) {
    stream.read(cartridge.flashrom.data, min(cartridge.flashrom.size, stream.size()));
  }
}

void Interface::save(unsigned id, const stream& stream) {
  if(id == ID::RAM) {
    stream.write(cartridge.ram.data, cartridge.ram.size);
  }

  if(id == ID::EEPROM) {
    stream.write(cartridge.eeprom.data, cartridge.eeprom.size);
  }

  if(id == ID::FlashROM) {
    stream.write(cartridge.flashrom.data, cartridge.flashrom.size);
  }
}

void Interface::unload() {
  save();
  cartridge.unload();
}

void Interface::power() {
  system.power();
}

void Interface::reset() {
  system.power();
}

void Interface::run() {
  system.run();
}

serializer Interface::serialize() {
  system.runtosave();
  return system.serialize();
}

bool Interface::unserialize(serializer& s) {
  return system.unserialize(s);
}

void Interface::paletteUpdate(PaletteMode mode) {
  video.generate_palette(mode);
}

void Interface::exportMemory() {
  string pathname = {path(group(ID::ROM)), "debug/"};
  directory::create(pathname);

  file::write({pathname, "i-work.ram"}, cpu.iwram, 32 * 1024);
  file::write({pathname, "e-work.ram"}, cpu.ewram, 256 * 1024);
  file::write({pathname, "video.ram"}, ppu.vram, 96 * 1024);
  uint8 obj_data[128 * 8];
  for(unsigned addr = 0; addr < 128 * 8; addr++) {
    obj_data[addr] = ppu.oam_read(Byte, addr | 0x0700'0000);
  }
  file::write({pathname, "sprite.ram"}, obj_data, 128 * 8);
  uint8 pal_data[256 * 2 * 2];
  for(unsigned addr = 0; addr < 256 * 2 * 2; addr++) {
    pal_data[addr] = ppu.pram_read(Byte, addr | 0x0500'0000);
  }
  file::write({pathname, "palette.ram"}, pal_data, 256 * 2 * 2);
  if(cartridge.has_sram()) saveRequest(ID::RAM, "debug/save-static.ram");
  if(cartridge.has_eeprom()) saveRequest(ID::EEPROM, "debug/save-eeprom.ram");
  if(cartridge.has_flashrom()) saveRequest(ID::FlashROM, "debug/save-flashrom.ram");
}

Interface::Interface() {
  interface = this;

  information.name        = "Game Boy Advance";
  information.width       = 240;
  information.height      = 160;
  information.overscan    = false;
  information.aspectRatio = 1.0;
  information.resettable  = false;
  information.capability.states = true;
  information.capability.cheats = false;

  media.append({ID::GameBoyAdvance, "Game Boy Advance", "gba", true});

  {
    Device device{0, ID::Device, "Controller"};
    device.input.append({ 0, 0, "A"     });
    device.input.append({ 1, 0, "B"     });
    device.input.append({ 2, 0, "Select"});
    device.input.append({ 3, 0, "Start" });
    device.input.append({ 4, 0, "Right" });
    device.input.append({ 5, 0, "Left"  });
    device.input.append({ 6, 0, "Up"    });
    device.input.append({ 7, 0, "Down"  });
    device.input.append({ 8, 0, "R"     });
    device.input.append({ 9, 0, "L"     });
    device.input.append({10, 2, "Rumble"});
    device.order = {6, 7, 5, 4, 1, 0, 9, 8, 2, 3, 10};
    this->device.append(device);
  }

  port.append({0, "Device", {device[0]}});
}

}
