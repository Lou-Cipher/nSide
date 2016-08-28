//Zilog Z80

struct APU : Processor::Z80, Thread {
  static auto Enter() -> void;
  auto main() -> void;
  auto step(uint clocks) -> void;

  auto wait() -> void override;
  auto read(uint16 addr) -> uint8 override;
  auto write(uint16 addr, uint8 data) -> void override;
  auto in(uint8 addr) -> uint8 override;
  auto out(uint8 addr, uint8 data) -> void override;
  auto stop() -> bool override;

  auto power() -> void;
  auto reset() -> void;
};

extern APU apu;
