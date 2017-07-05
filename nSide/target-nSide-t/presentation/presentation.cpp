#include "../nSide.hpp"
#include "about.cpp"
unique_pointer<AboutWindow> aboutWindow;
unique_pointer<Presentation> presentation;

Presentation::Presentation() {
  presentation = this;

  libraryMenu.setText("Library");
  refreshLibraryMenu();

  systemMenu.setText("System").setVisible(false);
  reloadSystem.setText("Power Cycle").onActivate([&] { program->powerCycle(); });
  unloadSystem.setText("Unload").onActivate([&] { program->unloadMedium(); });

  settingsMenu.setText("Settings");
  videoScaleMenu.setText("Video Scale");
  if(settings["Video/Scale"].text() == "Tiny") videoScaleTiny.setChecked();
  if(settings["Video/Scale"].text() == "Small") videoScaleSmall.setChecked();
  if(settings["Video/Scale"].text() == "Medium") videoScaleMedium.setChecked();
  if(settings["Video/Scale"].text() == "Large") videoScaleLarge.setChecked();
  videoScaleTiny.setText("Tiny").onActivate([&] {
    settings["Video/Scale"].setValue("Tiny");
    resizeViewport();
  });
  videoScaleSmall.setText("Small").onActivate([&] {
    settings["Video/Scale"].setValue("Small");
    resizeViewport();
  });
  videoScaleMedium.setText("Medium").onActivate([&] {
    settings["Video/Scale"].setValue("Medium");
    resizeViewport();
  });
  videoScaleLarge.setText("Large").onActivate([&] {
    settings["Video/Scale"].setValue("Large");
    resizeViewport();
  });
  aspectCorrection.setText("Aspect Correction").setChecked(settings["Video/AspectCorrection"].boolean()).onToggle([&] {
    settings["Video/AspectCorrection"].setValue(aspectCorrection.checked());
    resizeViewport();
  });
  videoEmulationMenu.setText("Video Emulation");
  blurEmulation.setText("Blurring").setChecked(settings["Video/BlurEmulation"].boolean()).onToggle([&] {
    settings["Video/BlurEmulation"].setValue(blurEmulation.checked());
    if(emulator) emulator->set("Blur Emulation", blurEmulation.checked());
  });
  colorEmulation.setText("Colors").setChecked(settings["Video/ColorEmulation"].boolean()).onToggle([&] {
    settings["Video/ColorEmulation"].setValue(colorEmulation.checked());
    if(emulator) emulator->set("Color Emulation", colorEmulation.checked());
  });
  scanlineEmulation.setText("Scanlines").setChecked(settings["Video/ScanlineEmulation"].boolean()).setVisible(false).onToggle([&] {
    settings["Video/ScanlineEmulation"].setValue(scanlineEmulation.checked());
    if(emulator) emulator->set("Scanline Emulation", scanlineEmulation.checked());
  });
  maskOverscan.setText("Mask Overscan").setChecked(settings["Video/Overscan/Mask"].boolean()).onToggle([&] {
    settings["Video/Overscan/Mask"].setValue(maskOverscan.checked());
    resizeViewport();
  });
  videoShaderMenu.setText("Video Shader");
  videoShaderNone.setText("None").onActivate([&] {
    settings["Video/Shader"].setValue("None");
    program->updateVideoShader();
  });
  videoShaderBlur.setText("Blur").onActivate([&] {
    settings["Video/Shader"].setValue("Blur");
    program->updateVideoShader();
  });
  loadShaders();
  synchronizeVideo.setText("Synchronize Video").setChecked(settings["Video/Synchronize"].boolean()).setVisible(false).onToggle([&] {
    settings["Video/Synchronize"].setValue(synchronizeVideo.checked());
    video->set(Video::Synchronize, synchronizeVideo.checked());
  });
  synchronizeAudio.setText("Synchronize Audio").setChecked(settings["Audio/Synchronize"].boolean()).onToggle([&] {
    settings["Audio/Synchronize"].setValue(synchronizeAudio.checked());
    audio->set(Audio::Synchronize, synchronizeAudio.checked());
  });
  muteAudio.setText("Mute Audio").setChecked(settings["Audio/Mute"].boolean()).onToggle([&] {
    settings["Audio/Mute"].setValue(muteAudio.checked());
    program->updateAudioEffects();
  });
  showStatusBar.setText("Show Status Bar").setChecked(settings["UserInterface/ShowStatusBar"].boolean()).onToggle([&] {
    settings["UserInterface/ShowStatusBar"].setValue(showStatusBar.checked());
    statusBar.setVisible(showStatusBar.checked());
    if(visible()) resizeViewport();
  });
  showConfiguration.setText("Configuration ...").onActivate([&] {
    //if no emulation core active; default to hotkeys panel
    if(!emulator) return settingsManager->show(3);

    //default to input panel with current core's input settings active
    for(auto item : settingsManager->input.emulatorList.items()) {
      if(systemMenu.text() == item.text()) {
        item.setSelected();
        settingsManager->input.emulatorList.doChange();
        break;
      }
    }
    settingsManager->show(2);
  });

  toolsMenu.setText("Tools").setVisible(false);
  saveStateMenu.setText("Save Quickstate");
  saveSlot0.setText("Slot 0").onActivate([&] { program->saveState(0); });
  saveSlot1.setText("Slot 1").onActivate([&] { program->saveState(1); });
  saveSlot2.setText("Slot 2").onActivate([&] { program->saveState(2); });
  saveSlot3.setText("Slot 3").onActivate([&] { program->saveState(3); });
  saveSlot4.setText("Slot 4").onActivate([&] { program->saveState(4); });
  saveSlot5.setText("Slot 5").onActivate([&] { program->saveState(5); });
  saveSlot6.setText("Slot 6").onActivate([&] { program->saveState(6); });
  saveSlot7.setText("Slot 7").onActivate([&] { program->saveState(7); });
  saveSlot8.setText("Slot 8").onActivate([&] { program->saveState(8); });
  saveSlot9.setText("Slot 9").onActivate([&] { program->saveState(9); });
  loadStateMenu.setText("Load Quickstate");
  loadSlot0.setText("Slot 0").onActivate([&] { program->loadState(0); });
  loadSlot1.setText("Slot 1").onActivate([&] { program->loadState(1); });
  loadSlot2.setText("Slot 2").onActivate([&] { program->loadState(2); });
  loadSlot3.setText("Slot 3").onActivate([&] { program->loadState(3); });
  loadSlot4.setText("Slot 4").onActivate([&] { program->loadState(4); });
  loadSlot5.setText("Slot 5").onActivate([&] { program->loadState(5); });
  loadSlot6.setText("Slot 6").onActivate([&] { program->loadState(6); });
  loadSlot7.setText("Slot 7").onActivate([&] { program->loadState(7); });
  loadSlot8.setText("Slot 8").onActivate([&] { program->loadState(8); });
  loadSlot9.setText("Slot 9").onActivate([&] { program->loadState(9); });
  cheatEditor.setText("Cheat Editor").onActivate([&] { toolsManager->show(0); });
  stateManager.setText("State Manager").onActivate([&] { toolsManager->show(1); });
  manifestViewer.setText("Manifest Viewer").onActivate([&] { toolsManager->show(2); });

  helpMenu.setText("Help");
  documentation.setText("Documentation for higan ...").onActivate([&] {
    invoke("http://doc.byuu.org/higan/");
  });
  about.setText("About ...").onActivate([&] {
    aboutWindow->setVisible().setFocused();
  });

  statusBar.setFont(Font().setBold());
  statusBar.setVisible(settings["UserInterface/ShowStatusBar"].boolean());

  viewport.setDroppable().onDrop([&](auto locations) {
    if(!directory::exists(locations(0))) return;
    program->mediumQueue.append(locations(0));
    program->loadMedium();
  });

  onClose([&] { program->quit(); });

  setTitle({"nSide v", Emulator::Version});
  setResizable(false);
  setBackgroundColor({0, 0, 0});
  resizeViewport();
  setCentered();

  #if defined(PLATFORM_WINDOWS)
  Application::Windows::onModalChange([](bool modal) { if(modal && audio) audio->clear(); });
  #endif

  #if defined(PLATFORM_MACOSX)
  showConfigurationSeparator.setVisible(false);
  showConfiguration.setVisible(false);
  about.setVisible(false);
  Application::Cocoa::onAbout([&] { about.doActivate(); });
  Application::Cocoa::onActivate([&] { setFocused(); });
  Application::Cocoa::onPreferences([&] { showConfiguration.doActivate(); });
  Application::Cocoa::onQuit([&] { doClose(); });
  #endif
}

auto Presentation::refreshLibraryMenu() -> void {
  libraryMenu.reset();
  string_vector manufacturers;
  for(auto& emulator : program->emulators) {
    if(emulator->information.devState > settings["Library/DevState"].natural()) continue;
    if(!manufacturers.find(emulator->information.manufacturer)) {
      manufacturers.append(emulator->information.manufacturer);
    }
  }
  for(auto& manufacturer : manufacturers) {
    Menu manufacturerMenu{&libraryMenu};
    manufacturerMenu.setText(manufacturer);
    for(auto& emulator : program->emulators) {
      if(emulator->information.manufacturer != manufacturer) continue;
      if(emulator->information.devState > settings["Library/DevState"].natural()) continue;
      for(auto& medium : emulator->media) {
        auto item = new MenuItem{&manufacturerMenu};
        item->setText({emulator->information.devState == 2 ? "(!) " : "", medium.name, " ..."}).onActivate([=] {
          program->loadMedium(*emulator, medium);
        });
      }
    }
  }
  //add cart-pal menu options -- but only if cart-pal binary is present
  if(execute("cart-pal", "--name").output.strip() == "cart-pal") {
    libraryMenu.append(MenuSeparator());
    libraryMenu.append(MenuItem().setText("Load ROM File ...").onActivate([&] {
      audio->clear();
      if(auto location = execute("cart-pal", "--import")) {
        program->mediumQueue.append(location.output.strip());
        program->loadMedium();
      }
    }));
    libraryMenu.append(MenuItem().setText("Import ROM Files ...").onActivate([&] {
      invoke("cart-pal");
    }));
  }
}

auto Presentation::updateEmulator() -> void {
  if(!emulator) return;
  inputPort1.setVisible(false).reset();
  inputPort2.setVisible(false).reset();
  inputPort3.setVisible(false).reset();
  inputPort4.setVisible(false).reset();

  for(auto n : range(emulator->ports)) {
    if(n >= 4) break;
    auto& port = emulator->ports[n];
    auto& menu = (
    n == 0 ? inputPort1 :
    n == 1 ? inputPort2 :
    n == 2 ? inputPort3 :
             inputPort4
    );
    menu.setText(port.name);

    Group devices;
    for(auto& device : port.devices) {
      MenuRadioItem item{&menu};
      item.setText(device.name).onActivate([=] {
        auto path = string{emulator->information.name, "/", port.name}.replace(" ", "");
        settings[path].setValue(device.name);
        emulator->connect(port.id, device.id);
      });
      devices.append(item);
    }
    if(devices.objectCount() > 1) {
      auto path = string{emulator->information.name, "/", port.name}.replace(" ", "");
      auto device = settings(path).text();
      for(auto item : devices.objects<MenuRadioItem>()) {
        if(item.text() == device) item.setChecked();
      }
      menu.setVisible();
    }
  }

  systemMenuSeparatorPorts.setVisible(
    inputPort1.visible() ||
    inputPort2.visible() ||
    inputPort3.visible() ||
    inputPort4.visible()
  );

  emulator->set("Blur Emulation", blurEmulation.checked());
  emulator->set("Color Emulation", colorEmulation.checked());
  emulator->set("Scanline Emulation", scanlineEmulation.checked());
}

auto Presentation::clearViewport() -> void {
  if(!video) return;

  uint32_t* output;
  uint length = 0;
  uint width = viewport.geometry().width();
  uint height = viewport.geometry().height();
  if(video->lock(output, length, width, height)) {
    for(uint y : range(height)) {
      auto dp = output + y * (length >> 2);
      for(uint x : range(width)) *dp++ = 0xff000000;
    }

    video->unlock();
    video->refresh();
  }
}

auto Presentation::resizeViewport() -> void {
  //clear video area before resizing to avoid seeing distorted video momentarily
  clearViewport();

  uint scale = 2;
  if(settings["Video/Scale"].text() == "Tiny"  ) scale = 1;
  if(settings["Video/Scale"].text() == "Small" ) scale = 2;
  if(settings["Video/Scale"].text() == "Medium") scale = 3;
  if(settings["Video/Scale"].text() == "Large" ) scale = 4;

  uint windowWidth = 0, windowHeight = 0;
  bool aspectCorrection = true;
  bool integerScale = true;
  if(!fullScreen()) {
    aspectCorrection = settings["Video/AspectCorrection"].boolean();
    if(emulator) {
      auto size = emulator->videoSize(aspectCorrection ? 384 : 320, 240, true, true);
      windowWidth  = (size.width  > 320 ? 384 : 320) * scale;
      windowHeight = (size.height > 240 ? 288 : 240) * scale;
    } else {
      windowWidth  = 320 * scale;
      windowHeight = 240 * scale;
    }
  } else {
    integerScale = settings["Video/Shader"].text() == "None";
    windowWidth  = geometry().width();
    windowHeight = geometry().height();
  }
  if(!fullScreen()) setSize({windowWidth, windowHeight});

  if(!emulator) {
    viewport.setGeometry({0, 0, windowWidth, windowHeight});
  } else {
    uint overscanWidth = 0, overscanHeight = 0;
    if(emulator->information.overscan && settings["Video/Overscan/Mask"].boolean()) {
      overscanWidth  = settings["Video/Overscan/Horizontal"].natural();
      overscanHeight = settings["Video/Overscan/Vertical"  ].natural();
    }
    auto videoSize = emulator->videoSize(windowWidth, windowHeight, aspectCorrection, integerScale, overscanWidth, overscanHeight);
    viewport.setGeometry({
      (windowWidth - videoSize.width) / 2, (windowHeight - videoSize.height) / 2,
      videoSize.width, videoSize.height
    });
  }

  //clear video area again to ensure entire viewport area has been painted in
  clearViewport();
}

auto Presentation::toggleFullScreen() -> void {
  if(!fullScreen()) {
    menuBar.setVisible(false);
    statusBar.setVisible(false);
    setResizable(true);
    setFullScreen(true);
    if(!input->acquired()) input->acquire();
  } else {
    if(input->acquired()) input->release();
    setFullScreen(false);
    setResizable(false);
    menuBar.setVisible(true);
    statusBar.setVisible(settings["UserInterface/ShowStatusBar"].boolean());
  }
  resizeViewport();
}

/*
auto Presentation::draw(image logo) -> void {
  if(!video) return;

  uint32_t* output;
  uint length = 0;
  uint width = viewport.geometry().width();
  uint height = viewport.geometry().height();
  if(video->lock(output, length, width, height)) {
    uint cx = (width - logo.width()) - 10;
    uint cy = (height - logo.height()) - 10;

    image backdrop;
    backdrop.allocate(width, height);
    if(logo && !program->hasQuit) {
    //backdrop.sphericalGradient(0xff0000bf, 0xff000000, logo.width(), logo.height() / 2, width, height);
      backdrop.impose(image::blend::sourceAlpha, cx, cy, logo, 0, 0, logo.width(), logo.height());
    } else {
      backdrop.fill(0xff000000);
    }

    auto data = (uint32_t*)backdrop.data();
    for(auto y : range(height)) {
      auto dp = output + y * (length >> 2);
      auto sp = data + y * width;
      for(auto x : range(width)) *dp++ = *sp++;
    }

    video->unlock();
    video->refresh();
  }
}
*/

auto Presentation::loadShaders() -> void {
  auto pathname = locate("Video Shaders/");

  if(settings["Video/Driver"].text() == "OpenGL") {
    for(auto shader : directory::folders(pathname, "*.shader")) {
      if(videoShaders.objectCount() == 2) videoShaderMenu.append(MenuSeparator());
      MenuRadioItem item{&videoShaderMenu};
      item.setText(string{shader}.trimRight(".shader/", 1L)).onActivate([=] {
        settings["Video/Shader"].setValue({pathname, shader});
        program->updateVideoShader();
      });
      videoShaders.append(item);
    }
  }

  if(settings["Video/Shader"].text() == "None") videoShaderNone.setChecked();
  if(settings["Video/Shader"].text() == "Blur") videoShaderBlur.setChecked();

  for(auto radioItem : videoShaders.objects<MenuRadioItem>()) {
    if(settings["Video/Shader"].text() == string{pathname, radioItem.text(), ".shader/"}) {
      radioItem.setChecked();
    }
  }
}
