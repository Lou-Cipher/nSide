struct MemoryEditor : Window {
  VerticalLayout layout{this};
    HorizontalLayout controlLayout{&layout, Size{~0, 0}, 5};
      Label gotoLabel{&controlLayout, Size{0, 0}, 5};
      LineEdit gotoAddress{&controlLayout, Size{50, 0}, 5};
      ComboButton source{&controlLayout, Size{0, 0}, 5};
      Button exportMemory{&controlLayout, Size{80, 0}, 5};
      Widget spacer{&controlLayout, Size{~0, 0}};
      CheckLabel autoUpdate{&controlLayout, Size{0, 0}, 5};
      Button update{&controlLayout, Size{80, 0}};
    HexEdit editor{&layout, Size{~0, ~0}};

  uint8 read(uint addr);
  void write(uint addr, uint8 data);
  void selectSource();
  void exportMemoryToDisk();
  void updateView();
  MemoryEditor();
};

extern MemoryEditor* memoryEditor;