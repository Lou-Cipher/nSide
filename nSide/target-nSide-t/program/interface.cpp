auto Program::path(uint id) -> string {
  return mediumPaths(id);
}

auto Program::open(uint id, string name, vfs::file::mode mode, bool required) -> vfs::shared::file {
  if(auto result = vfs::fs::file::open({path(id), name}, mode)) return result;
  if(name == "manifest.bml") {
    if(auto manifest = execute("cart-pal", "--manifest", path(id))) {
      return vfs::memory::file::open(manifest.output.data<uint8_t>(), manifest.output.size());
    }
  }
  if(required) {
    MessageDialog()
    .setTitle({"Error"})
    .setText({"Error: missing required file:\n\n", path(id), name})
    .error();
  }
  return {};
}

auto Program::load(uint id, string name, string type) -> maybe<uint> {
  string location = BrowserDialog()
  .setTitle({"Load ", name})
  .setPath({settings["Library/Location"].text(), name})
  .setFilters({string{name, "|*.", type}, "All|*.*"})
  .openFolder();
  if(!directory::exists(location)) return nothing;

  uint pathID = mediumPaths.size();
  mediumPaths.append(location);
  return pathID;
}

auto Program::videoRefresh(const uint32* data, uint pitch, uint width, uint height) -> void {
  uint32_t* output;
  uint length;

  if(video->lock(output, length, width, height)) {
    pitch >>= 2, length >>= 2;

    for(auto y : range(height)) {
      memory::copy(output + y * length, data + y * pitch, width * sizeof(uint32));
    }

    if(emulator->information.overscan && settings["Video/Overscan/Mask"].boolean()) {
      auto h = settings["Video/Overscan/Horizontal"].natural();
      auto v = settings["Video/Overscan/Vertical"].natural();

      if(h) for(auto y : range(height)) {
        memory::fill(output + y * length, 4 * h);
        memory::fill(output + y * length + (width - h), 4 * h);
      }

      if(v) for(auto y : range(v)) {
        memory::fill(output + y * length, 4 * width);
        memory::fill(output + (height - 1 - y) * length, 4 * width);
      }
    }

    video->unlock();
    video->refresh();
  }

  static uint frameCounter = 0;
  static time_t previous, current;
  frameCounter++;

  time(&current);
  if(current != previous) {
    previous = current;
    statusText = {"FPS: ", frameCounter};
    frameCounter = 0;
  }
}

auto Program::audioSample(const double* samples, uint channels) -> void {
  int16 left  = sclamp<16>(samples[0] * 32768.0);
  int16 right = sclamp<16>(samples[1] * 32768.0);
  audio->sample(left, right);
}

auto Program::inputPoll(uint port, uint device, uint input) -> int16 {
  if(presentation->focused() || settings["Input/FocusLoss/AllowInput"].boolean()) {
    auto userData = emulator->ports[port].devices[device].inputs[input].userData;
    auto mapping = (InputMapping*)userData;
    if(mapping) return mapping->poll();
  }
  return 0;
}

auto Program::inputRumble(uint port, uint device, uint input, bool enable) -> void {
  if(presentation->focused() || settings["Input/FocusLoss/AllowInput"].boolean() || !enable) {
    auto userData = emulator->ports[port].devices[device].inputs[input].userData;
    auto mapping = (InputMapping*)userData;
    if(mapping) return mapping->rumble(enable);
  }
}

auto Program::deviceChanged(uint port, uint device) -> void {
  Menu& portMenu = (
    port == 0 ? presentation->inputPort1 :
    port == 1 ? presentation->inputPort2 :
    port == 2 ? presentation->inputPort3 :
                presentation->inputPort4
  );
  ((MenuRadioItem)portMenu.action(device)).setChecked();
}

auto Program::dipSettings(const Markup::Node& node) -> uint {
  return dipSwitches->run(node);
}

auto Program::notify(string text) -> void {
  MessageWindow().setTitle("nSide").setText(text).information();
}
