#include <sfc-balanced/sfc.hpp>

namespace SuperFamicom {

#include "serialization.cpp"
SufamiTurboCartridge sufamiturboA;
SufamiTurboCartridge sufamiturboB;

auto SufamiTurboCartridge::unload() -> void {
  rom.reset();
  ram.reset();
}

}
