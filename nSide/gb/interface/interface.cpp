#include <gb/gb.hpp>

namespace GameBoy {

SuperGameBoyInterface* superGameBoy = nullptr;
Settings settings;
#include "game-boy.cpp"
#include "game-boy-color.cpp"

Interface::Interface() {
  Port hardwarePort{ID::Port::Hardware, "Hardware"};

  { Device device{ID::Device::Controls, "Controls"};
    device.inputs.append({0, "Up"    });
    device.inputs.append({0, "Down"  });
    device.inputs.append({0, "Left"  });
    device.inputs.append({0, "Right" });
    device.inputs.append({0, "B"     });
    device.inputs.append({0, "A"     });
    device.inputs.append({0, "Select"});
    device.inputs.append({0, "Start" });
    hardwarePort.devices.append(device);
  }

  ports.append(move(hardwarePort));
}

auto Interface::manifest() -> string {
  return cartridge.manifest();
}

auto Interface::title() -> string {
  return cartridge.title();
}

auto Interface::videoResolution() -> VideoSize {
  return {160, 144};
}

auto Interface::videoSize(uint width, uint height, bool, bool integerScale, uint, uint) -> VideoSize {
  double widthDivider = 160;
  double heightDivider = 144;
  double multiplier = integerScale
  ? min(  uint(width / widthDivider),   uint(height / heightDivider))
  : min(double(width / widthDivider), double(height / heightDivider));
  return {uint(widthDivider * multiplier), uint(heightDivider * multiplier)};
}

auto Interface::loaded() -> bool {
  return system.loaded();
}

auto Interface::sha256() -> string {
  return cartridge.sha256();
}

auto Interface::save() -> void {
  system.save();
}

auto Interface::unload() -> void {
  save();
  system.unload();
}

auto Interface::power() -> void {
  system.power();
}

auto Interface::run() -> void {
  system.run();
}

auto Interface::serialize() -> serializer {
  system.runToSave();
  return system.serialize();
}

auto Interface::unserialize(serializer& s) -> bool {
  return system.unserialize(s);
}

auto Interface::cheatSet(const string_vector& list) -> void {
  cheat.assign(list);
}

auto Interface::cap(const string& name) -> bool {
  if(name == "Blur Emulation") return true;
  if(name == "Color Emulation") return true;
  return false;
}

auto Interface::get(const string& name) -> any {
  if(name == "Blur Emulation") return settings.blurEmulation;
  if(name == "Color Emulation") return settings.colorEmulation;
  return {};
}

auto Interface::set(const string& name, const any& value) -> bool {
  if(name == "Blur Emulation" && value.is<bool>()) {
    settings.blurEmulation = value.get<bool>();
    system.configureVideoEffects();
    return true;
  }

  if(name == "Color Emulation" && value.is<bool>()) {
    settings.colorEmulation = value.get<bool>();
    system.configureVideoPalette();
    return true;
  }

  return false;
}

auto Interface::exportMemory() -> void {
  string pathname = {platform->path(cartridge.pathID()), "debug/"};
  directory::create(pathname);

  if(auto fp = platform->open(cartridge.pathID(), "debug/work.ram", File::Write)) fp->write(cpu.wram, !Model::GameBoyColor() ? 8192 : 32768);
  if(auto fp = platform->open(cartridge.pathID(), "debug/internal.ram", File::Write)) fp->write(cpu.hram, 128);
  if(cartridge.ram.size) if(auto fp = platform->open(cartridge.pathID(), "debug/program-save.ram", File::Write)) {
    fp->write(cartridge.ram.data, cartridge.ram.size);
  }
}

}
