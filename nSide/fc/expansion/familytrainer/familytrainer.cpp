FamilyTrainer::FamilyTrainer() {
  latched = 0;
  counter = 0;

  b1 = b2  = b3  = b4  = 0;
  b5 = b6  = b7  = b8  = 0;
  b9 = b10 = b11 = b12 = 0;
}

auto FamilyTrainer::data1() -> bool {
  return 0;
}

auto FamilyTrainer::data2() -> uint5 {
  if(counter >= 8) return 0x18;
  if(latched == 1) return (interface->inputPoll(ID::Port::Expansion, ID::Device::FamilyTrainer, 1) << 1) | (interface->inputPoll(ID::Port::Expansion, ID::Device::FamilyTrainer, 3) << 2);

  switch(counter++) {
  case  0: return (b2  << 3) | (b4  << 4);
  case  1: return (b1  << 3) | (b3  << 4);
  case  2: return (b5  << 3) | (b12 << 4);
  case  3: return (b9  << 3) | (b8  << 4);
  case  4: return (b6  << 3) | (1   << 4);
  case  5: return (b10 << 3) | (1   << 4);
  case  6: return (b11 << 3) | (1   << 4);
  case  7: return (b7  << 3) | (1   << 4);
  }
}

auto FamilyTrainer::latch(bool data) -> void {
  if(latched == data) return;
  latched = data;
  counter = 0;

  if(latched == 0) {
    auto id = ID::Device::FamilyTrainer;
    b1  = interface->inputPoll(ID::Port::Expansion, id,  0);
    b2  = interface->inputPoll(ID::Port::Expansion, id,  1);
    b3  = interface->inputPoll(ID::Port::Expansion, id,  2);
    b4  = interface->inputPoll(ID::Port::Expansion, id,  3);
    b5  = interface->inputPoll(ID::Port::Expansion, id,  4);
    b6  = interface->inputPoll(ID::Port::Expansion, id,  5);
    b7  = interface->inputPoll(ID::Port::Expansion, id,  6);
    b8  = interface->inputPoll(ID::Port::Expansion, id,  7);
    b9  = interface->inputPoll(ID::Port::Expansion, id,  8);
    b10 = interface->inputPoll(ID::Port::Expansion, id,  9);
    b11 = interface->inputPoll(ID::Port::Expansion, id, 10);
    b12 = interface->inputPoll(ID::Port::Expansion, id, 11);
  }
}
