namespace GameBoyAdvance {

struct ID {
  enum : uint {
    System,
    GameBoyAdvance,
  };

  struct Port { enum : uint {
    Hardware,
  };};

  struct Device { enum : uint {
    HorizontalControls,
    VerticalControls,
  };};
};

struct Interface : Emulator::Interface {
  using Emulator::Interface::load;

  Interface();

  auto manifest() -> string override;
  auto title() -> string override;
  auto videoFrequency() -> double override;
  auto videoColors() -> uint32 override;
  auto videoColor(uint32 color) -> uint64 override;
  auto audioFrequency() -> double override;

  auto loaded() -> bool override;
  auto load(uint id) -> bool override;
  auto save() -> void override;
  auto unload() -> void override;

  auto connect(uint port, uint device) -> void override;
  auto power() -> void override;
  auto reset() -> void override;
  auto run() -> void override;
  auto rotate() -> void override;

  auto serialize() -> serializer override;
  auto unserialize(serializer&) -> bool override;

  auto cheatSet(const lstring&) -> void override;

  auto cap(const string& name) -> bool override;
  auto get(const string& name) -> any override;
  auto set(const string& name, const any& value) -> bool override;

  //debugger functions
  auto exportMemory() -> void override;
};

struct Settings {
  bool blurEmulation = true;
  bool colorEmulation = true;
};

extern Interface* interface;
extern Settings settings;

}
