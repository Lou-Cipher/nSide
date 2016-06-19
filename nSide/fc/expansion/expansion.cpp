#include <fc/fc.hpp>

namespace Famicom {

#include "gamepad/gamepad.cpp"
#include "fourplayers/fourplayers.cpp"
#include "beamgun/beamgun.cpp"
#include "familytrainer/familytrainer.cpp"
#include "vaus/vaus.cpp"
#include "sfcgamepad/sfcgamepad.cpp"
#include "mouse/mouse.cpp"

Expansion::Expansion(uint index) : index(index) {
  if(!thread) create(Expansion::Enter, 1);
}

Expansion::~Expansion() {
}

auto Expansion::Enter() -> void {
  while(true) scheduler.synchronize(), peripherals.expansionPort->main();
}

auto Expansion::main() -> void {
  step(1);
}

}