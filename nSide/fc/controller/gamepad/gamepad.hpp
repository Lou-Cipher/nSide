struct Gamepad : Controller {
  enum : uint {
    Up, Down, Left, Right, B, A, Select, Start,
  };

  Gamepad(bool port);

  auto data() -> uint3;
  auto latch(bool data) -> void;

private:
  bool latched;
  uint counter;

  boolean a, b, select, start;
  boolean up, down, left, right;
};
