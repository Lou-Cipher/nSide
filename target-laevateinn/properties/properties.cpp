#include "../base.hpp"
PropertiesViewer *propertiesViewer = nullptr;

PropertiesViewer::PropertiesViewer() {
  setTitle("Properties Viewer");
  setGeometry({128, 128, 300, 400});

  layout.setMargin(5);
  sourceLabel.setText("Source:");
  sourceSelection.append("PPU");
  sourceSelection.setEnabled(false);
  autoUpdate.setText("Auto");
  update.setText("Update");
  properties.setFont(program->monospaceFont);

  layout.append(controlLayout, {~0, 0}, 5);
    controlLayout.append(sourceLabel, {0, 0}, 5);
    controlLayout.append(sourceSelection, {0, 0}, 5);
    controlLayout.append(spacer, {~0, 0});
    controlLayout.append(autoUpdate, {0, 0}, 5);
    controlLayout.append(update, {0, 0});
  layout.append(properties, {~0, ~0});
  append(layout);

  update.onActivate = { &PropertiesViewer::updateProperties, this };

  windowManager->append(this, "PropertiesViewer");
}

void PropertiesViewer::updateProperties() {
  string output;

  output.append("$2100  INIDISP\n"
    "  Display Disable = ", SuperFamicom::ppu.regs.display_disable, "\n",
    "  Display Brightness = ", SuperFamicom::ppu.regs.display_brightness, "\n",
    "\n"
  );

  output.append("$2101  OBSEL\n"
    "  Base Size = ", SuperFamicom::ppu.sprite.regs.base_size, "\n",
    "  Name Select = $", hex<4>(SuperFamicom::ppu.sprite.regs.nameselect << 13), "\n",
    "  Tiledata Address = $", hex<4>(SuperFamicom::ppu.sprite.regs.tiledata_addr), "\n",
    "\n"
  );

  output.append("$2102  OAMADDL\n"
                "$2103  OAMADDH\n"
    "  Base Address = $", hex<4>(SuperFamicom::ppu.regs.oam_baseaddr), "\n",
    "  Priority = ", SuperFamicom::ppu.regs.oam_priority, "\n",
    "\n"
  );

  output.append("$2105  BGMODE\n"
    "  Mode = ", SuperFamicom::ppu.regs.bgmode, "\n",
    "  BG3 Priority = ", SuperFamicom::ppu.regs.bg3_priority, "\n",
    "  BG1 Tile Size = ", SuperFamicom::ppu.bg1.regs.tile_size ? "16x16" : "8x8", "\n",
    "  BG2 Tile Size = ", SuperFamicom::ppu.bg2.regs.tile_size ? "16x16" : "8x8", "\n",
    "  BG3 Tile Size = ", SuperFamicom::ppu.bg3.regs.tile_size ? "16x16" : "8x8", "\n",
    "  BG4 Tile Size = ", SuperFamicom::ppu.bg4.regs.tile_size ? "16x16" : "8x8", "\n",
    "\n"
  );

  output.append("$2106  MOSAIC\n"
    "  BG1 Mosaic = ", 1u + SuperFamicom::ppu.bg1.regs.mosaic, "x", 1u + SuperFamicom::ppu.bg1.regs.mosaic, "\n",
    "  BG2 Mosaic = ", 1u + SuperFamicom::ppu.bg2.regs.mosaic, "x", 1u + SuperFamicom::ppu.bg2.regs.mosaic, "\n",
    "  BG3 Mosaic = ", 1u + SuperFamicom::ppu.bg3.regs.mosaic, "x", 1u + SuperFamicom::ppu.bg3.regs.mosaic, "\n",
    "  BG4 Mosaic = ", 1u + SuperFamicom::ppu.bg4.regs.mosaic, "x", 1u + SuperFamicom::ppu.bg4.regs.mosaic, "\n",
    "\n"
  );

  lstring screenSizes = { "32x32", "32x64", "64x32", "64x64" };

  output.append("$2107  BG1SC\n"
    "  BG1 Screen Address = $", hex<4>(SuperFamicom::ppu.bg1.regs.screen_addr), "\n",
    "  BG1 Screen Size = ", screenSizes[SuperFamicom::ppu.bg1.regs.screen_size], "\n",
    "\n"
  );

  output.append("$2108  BG2SC\n"
    "  BG2 Screen Address = $", hex<4>(SuperFamicom::ppu.bg2.regs.screen_addr), "\n",
    "  BG2 Screen Size = ", screenSizes[SuperFamicom::ppu.bg2.regs.screen_size], "\n",
    "\n"
  );

  output.append("$2109  BG3SC\n"
    "  BG3 Screen Address = $", hex<4>(SuperFamicom::ppu.bg3.regs.screen_addr), "\n",
    "  BG3 Screen Size = ", screenSizes[SuperFamicom::ppu.bg3.regs.screen_size], "\n",
    "\n"
  );

  output.append("$210a  BG4SC\n"
    "  BG4 Screen Address = $", hex<4>(SuperFamicom::ppu.bg4.regs.screen_addr), "\n",
    "  BG4 Screen Size = ", screenSizes[SuperFamicom::ppu.bg4.regs.screen_size], "\n",
    "\n"
  );

  output.append("$210b  BG12NBA\n"
    "  BG1 Tiledata Address = $", hex<4>(SuperFamicom::ppu.bg1.regs.tiledata_addr), "\n",
    "  BG2 Tiledata Address = $", hex<4>(SuperFamicom::ppu.bg2.regs.tiledata_addr), "\n",
    "\n"
  );

  output.append("$210c  BG34NBA\n"
    "  BG3 Tiledata Address = $", hex<4>(SuperFamicom::ppu.bg3.regs.tiledata_addr), "\n",
    "  BG4 Tiledata Address = $", hex<4>(SuperFamicom::ppu.bg4.regs.tiledata_addr), "\n",
    "\n"
  );

  properties.setText(output);
}
