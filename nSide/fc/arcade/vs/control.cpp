auto VSArcadeBoard::resetButtons() -> void {
  mainControlLatched = 0;
  mainControlCounter1 = 0;
  mainControlCounter2 = 0;

  subControlLatched = 0;
  subControlCounter1 = 0;
  subControlCounter2 = 0;

  for(bool& button : mainButtons) button = false;
  for(bool& button : subButtons) button = false;
}

auto VSArcadeBoard::poll(bool side, uint input) -> int16 {
  return interface->inputPoll(3, interface->device_ref[(uint)Device::ID::VSPanel].port[3], input);
}

auto VSArcadeBoard::data1(bool side) -> bool {
  uint& counter = side == 0 ? mainControlCounter1 : subControlCounter1;
  bool* buttons = side == 0 ? mainButtons : subButtons;
  bool data;
  if(!swapControllers) data = device.controllerPort1->data() & 0x01;
  else                 data = device.controllerPort2->data() & 0x01;
  if(counter == 2) return buttons[Button1];
  if(counter == 3) return buttons[Button3];
  return data;
}

auto VSArcadeBoard::data2(bool side) -> bool {
  uint& counter = side == 0 ? mainControlCounter2 : subControlCounter2;
  bool* buttons = side == 0 ? mainButtons : subButtons;
  bool data;
  if(!swapControllers) data = device.controllerPort2->data() & 0x01;
  else                 data = device.controllerPort1->data() & 0x01;
  if(counter == 2) return buttons[Button2];
  if(counter == 3) return buttons[Button4];
  return data;
}

auto VSArcadeBoard::latch(bool side, bool data) -> void {
  if(side == 0) {
    if(mainControlLatched == data) return;
    mainControlLatched = data;
    mainControlCounter1 = 0;
    mainControlCounter2 = 0;

    if(mainControlLatched == 0) {
      for(uint i : {Button1, Button2, Button3, Button4}) mainButtons[i] = poll(0, i);
    }
  } else {
    if(subControlLatched == data) return;
    subControlLatched = data;
    subControlCounter1 = 0;
    subControlCounter2 = 0;

    if(subControlLatched == 0) {
      for(uint i : {Button1, Button2, Button3, Button4}) subButtons[i] = poll(1, i);
    }
  }
}