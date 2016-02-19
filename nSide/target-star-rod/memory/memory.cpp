#include "../laevateinn.hpp"
MemoryEditor* memoryEditor = nullptr;

MemoryEditor::MemoryEditor() {
  memoryEditor = this;
  setTitle("Memory Editor");
  setGeometry({128, 128, 485, 255});

  gotoLabel.setText("Goto:");
  gotoAddress.setFont(Font().setFamily(Font::Mono));
  source.append(ComboButtonItem().setText("CPU-Bus"));
  source.append(ComboButtonItem().setText("APU-Bus"));
  source.append(ComboButtonItem().setText("VRAM"));
  source.append(ComboButtonItem().setText("OAM"));
  source.append(ComboButtonItem().setText("CGRAM"));
  exportMemory.setText("Export");
  autoUpdate.setText("Auto");
  update.setText("Update");
  editor.setFont(Font().setFamily(Font::Mono));
  editor.setColumns(16);
  editor.setRows(16);

  layout.setMargin(5);

  gotoAddress.onChange([&] {
    editor.setAddress(hex(gotoAddress.text()));
    editor.update();
  });

  gotoAddress.onActivate([&] {
    gotoAddress.doChange();
  });

  update.onActivate({ &MemoryEditor::updateView, this });

  source.onChange({ &MemoryEditor::selectSource, this });
  exportMemory.onActivate({ &MemoryEditor::exportMemoryToDisk, this });
  editor.onRead({ &MemoryEditor::read, this });
  editor.onWrite({ &MemoryEditor::write, this });

  windowManager->append(this, "MemoryEditor");
}

uint8 MemoryEditor::read(uint addr) {
  if(SuperFamicom::cartridge.loaded() == false) return 0x00;
  switch(source.selected().offset()) {
  case 0: return cpuDebugger->read(addr);
  case 1: return smpDebugger->read(addr);
  case 2: return SuperFamicom::ppu.vram[addr & 0xffff];
  case 3: return SuperFamicom::ppu.oam[addr % 544];
  case 4: return SuperFamicom::ppu.cgram[addr & 0x01ff];
  }
  return ~0;
}

void MemoryEditor::write(uint addr, uint8 data) {
  if(SuperFamicom::cartridge.loaded() == false) return;
  switch(source.selected().offset()) {
  case 0:
    SuperFamicom::cartridge.rom.write_protect(false);
    cpuDebugger->write(addr, data);
    SuperFamicom::cartridge.rom.write_protect(true);
    break;
  case 1:
    smpDebugger->write(addr, data);
    break;
  case 2:
    SuperFamicom::ppu.vram[addr & 0xffff] = data;
    break;
  case 3:
    SuperFamicom::ppu.oam[addr % 544] = data;
    SuperFamicom::ppu.sprite.synchronize();  //cache OAM changes internally
    break;
  case 4:
    SuperFamicom::ppu.cgram[addr & 0x01ff] = data;
    break;
  }
}

void MemoryEditor::selectSource() {
  editor.setAddress(0);
  switch(source.selected().offset()) {
  case 0: editor.setLength(16 * 1024 * 1024); break;
  case 1: editor.setLength(64 * 1024); break;
  case 2: editor.setLength(64 * 1024); break;
  case 3: editor.setLength(544); break;
  case 4: editor.setLength(512); break;
  }
  updateView();
}

void MemoryEditor::exportMemoryToDisk() {
  string filename = { interface->pathName, "debug/memory-" };
  switch(source.selected().offset()) {
  case 0: filename.append("cpu.bin"); break;
  case 1: filename.append("apu.bin"); break;
  case 2: filename.append("vram.bin"); break;
  case 3: filename.append("oam.bin"); break;
  case 4: filename.append("cgram.bin"); break;
  }
  file fp;
  if(fp.open(filename, file::mode::write) == false) return;
  switch(source.selected().offset()) {
  case 0: for(uint addr = 0; addr <= 0xffffff; addr++) fp.write(cpuDebugger->read(addr)); break;
  case 1: for(uint addr = 0; addr <= 0xffff; addr++) fp.write(smpDebugger->read(addr)); break;
  case 2: for(uint addr = 0; addr <= 0xffff; addr++) fp.write(SuperFamicom::ppu.vram[addr]); break;
  case 3: for(uint addr = 0; addr <= 0x021f; addr++) fp.write(SuperFamicom::ppu.oam[addr]); break;
  case 4: for(uint addr = 0; addr <= 0x01ff; addr++) fp.write(SuperFamicom::ppu.cgram[addr]); break;
  }
  debugger->print("Exported memory to ", filename, "\n");
}

void MemoryEditor::updateView() {
  editor.update();
}