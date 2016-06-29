FourPlayers::FourPlayers() {
  latched = 0;
  counter1 = 0;
  counter2 = 0;
}

auto FourPlayers::data1() -> bool {
  if(counter1 >= 8) return 1;
  if(latched) return interface->inputPoll(ID::Port::Expansion, ID::Device::FourPlayers, 0 + A);

  auto& A = gamepads[0];

  switch(counter1++) {
  case 0: return A.a;
  case 1: return A.b;
  case 2: return A.select;
  case 3: return A.start;
  case 4: return A.up & !A.down;
  case 5: return A.down & !A.up;
  case 6: return A.left & !A.right;
  case 7: return A.right & !A.left;
  }
  unreachable;
}

auto FourPlayers::data2() -> uint5 {
  if(counter2 >= 8) return 2;
  if(latched) return interface->inputPoll(ID::Port::Expansion, ID::Device::FourPlayers, 8 + A) << 1;

  auto& B = gamepads[1];

  switch(counter2++) {
  case 0: return B.a << 1;
  case 1: return B.b << 1;
  case 2: return B.select << 1;
  case 3: return B.start << 1;
  case 4: return (B.up & !B.down) << 1;
  case 5: return (B.down & !B.up) << 1;
  case 6: return (B.left & !B.right) << 1;
  case 7: return (B.right & !B.left) << 1;
  }
  unreachable;
}

auto FourPlayers::latch(bool data) -> void {
  if(latched == data) return;
  latched = data;
  counter1 = 0;
  counter2 = 0;

  if(latched == 0) {
    for(uint id : range(2)) {
      auto& gamepad = gamepads[id];
      gamepad.a      = interface->inputPoll(ID::Port::Expansion, ID::Device::FourPlayers, id * 8 + A);
      gamepad.b      = interface->inputPoll(ID::Port::Expansion, ID::Device::FourPlayers, id * 8 + B);
      gamepad.select = interface->inputPoll(ID::Port::Expansion, ID::Device::FourPlayers, id * 8 + Select);
      gamepad.start  = interface->inputPoll(ID::Port::Expansion, ID::Device::FourPlayers, id * 8 + Start);
      gamepad.up     = interface->inputPoll(ID::Port::Expansion, ID::Device::FourPlayers, id * 8 + Up);
      gamepad.down   = interface->inputPoll(ID::Port::Expansion, ID::Device::FourPlayers, id * 8 + Down);
      gamepad.left   = interface->inputPoll(ID::Port::Expansion, ID::Device::FourPlayers, id * 8 + Left);
      gamepad.right  = interface->inputPoll(ID::Port::Expansion, ID::Device::FourPlayers, id * 8 + Right);
    }
  }
}
