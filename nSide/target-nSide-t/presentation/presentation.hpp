struct AboutWindow : Window {
  AboutWindow();

  VerticalLayout layout{this};
    Canvas canvas{&layout, Size{256, 96}, 15};
    HorizontalLayout informationLayout{&layout, Size{~0, 0}};
      Label informationLeft{&informationLayout, Size{~0, 0}, 3};
      Label informationRight{&informationLayout, Size{~0, 0}};
    Label informationBottom{&layout, Size{~0, 0}};
};

struct Presentation : Window {
  Presentation();
  auto refreshLibraryMenu() -> void;
  auto updateEmulator() -> void;
  auto resizeViewport() -> void;
  auto toggleFullScreen() -> void;
  auto draw(image logo = {}) -> void;
  auto loadShaders() -> void;

  MenuBar menuBar{this};
    Menu libraryMenu{&menuBar};
      vector<MenuItem*> loadBootableMedia;
      MenuSeparator librarySeparator;
    Menu systemMenu{&menuBar};
      MenuItem powerSystem{&systemMenu};
      MenuItem resetSystem{&systemMenu};
      MenuSeparator systemMenuSeparatorPorts{&systemMenu};
      Menu inputPort1{&systemMenu};
      Menu inputPort2{&systemMenu};
      Menu inputPort3{&systemMenu};
      Menu inputPort4{&systemMenu};
      MenuSeparator systemMenuSeparatorUnload{&systemMenu};
      MenuItem unloadSystem{&systemMenu};
    Menu settingsMenu{&menuBar};
      Menu videoScaleMenu{&settingsMenu};
        MenuRadioItem videoScaleTiny{&videoScaleMenu};
        MenuRadioItem videoScaleSmall{&videoScaleMenu};
        MenuRadioItem videoScaleMedium{&videoScaleMenu};
        MenuRadioItem videoScaleLarge{&videoScaleMenu};
          Group videoScales{&videoScaleTiny, &videoScaleSmall, &videoScaleMedium, &videoScaleLarge};
        MenuSeparator videoScaleSeparator{&videoScaleMenu};
        MenuCheckItem aspectCorrection{&videoScaleMenu};
      Menu videoEmulationMenu{&settingsMenu};
        MenuCheckItem blurEmulation{&videoEmulationMenu};
        MenuCheckItem colorEmulation{&videoEmulationMenu};
        MenuCheckItem scanlineEmulation{&videoEmulationMenu};
        MenuCheckItem maskOverscan{&videoEmulationMenu};
      Menu videoShaderMenu{&settingsMenu};
        MenuRadioItem videoShaderNone{&videoShaderMenu};
        MenuRadioItem videoShaderBlur{&videoShaderMenu};
        Group videoShaders{&videoShaderNone, &videoShaderBlur};
      MenuSeparator videoSettingsSeparator{&settingsMenu};
      MenuCheckItem synchronizeVideo{&settingsMenu};
      MenuCheckItem synchronizeAudio{&settingsMenu};
      MenuCheckItem muteAudio{&settingsMenu};
      MenuCheckItem showStatusBar{&settingsMenu};
      MenuSeparator showConfigurationSeparator{&settingsMenu};
      MenuItem showConfiguration{&settingsMenu};
    Menu toolsMenu{&menuBar};
      Menu saveStateMenu{&toolsMenu};
        MenuItem saveSlot0{&saveStateMenu};
        MenuItem saveSlot1{&saveStateMenu};
        MenuItem saveSlot2{&saveStateMenu};
        MenuItem saveSlot3{&saveStateMenu};
        MenuItem saveSlot4{&saveStateMenu};
        MenuItem saveSlot5{&saveStateMenu};
        MenuItem saveSlot6{&saveStateMenu};
        MenuItem saveSlot7{&saveStateMenu};
        MenuItem saveSlot8{&saveStateMenu};
        MenuItem saveSlot9{&saveStateMenu};
      Menu loadStateMenu{&toolsMenu};
        MenuItem loadSlot0{&loadStateMenu};
        MenuItem loadSlot1{&loadStateMenu};
        MenuItem loadSlot2{&loadStateMenu};
        MenuItem loadSlot3{&loadStateMenu};
        MenuItem loadSlot4{&loadStateMenu};
        MenuItem loadSlot5{&loadStateMenu};
        MenuItem loadSlot6{&loadStateMenu};
        MenuItem loadSlot7{&loadStateMenu};
        MenuItem loadSlot8{&loadStateMenu};
        MenuItem loadSlot9{&loadStateMenu};
      MenuSeparator toolsMenuSeparator{&toolsMenu};
      MenuItem cheatEditor{&toolsMenu};
      MenuItem stateManager{&toolsMenu};
      MenuItem manifestViewer{&toolsMenu};
    Menu helpMenu{&menuBar};
      MenuItem documentation{&helpMenu};
      MenuItem about{&helpMenu};

  FixedLayout layout{this};
    Viewport viewport{&layout, Geometry{0, 0, 1, 1}};

  StatusBar statusBar{this};
};

extern unique_pointer<AboutWindow> aboutWindow;
extern unique_pointer<Presentation> presentation;
