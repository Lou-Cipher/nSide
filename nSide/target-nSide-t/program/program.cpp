#include "../nSide.hpp"
#include <a2600/interface/interface.hpp>
#include <fc/interface/interface.hpp>
#if defined(PROFILE_BALANCED)
#include <sfc-balanced/interface/interface.hpp>
#else
#include <sfc/interface/interface.hpp>
#endif
#include <ms/interface/interface.hpp>
#include <md/interface/interface.hpp>
#include <pce/interface/interface.hpp>
#include <gb/interface/interface.hpp>
#include <gba/interface/interface.hpp>
#include <ws/interface/interface.hpp>
#include "interface.cpp"
#include "medium.cpp"
#include "state.cpp"
#include "utility.cpp"
unique_pointer<Program> program;

Program::Program(string_vector args) {
  program = this;

  Emulator::platform = this;
  emulators.append(new Atari2600::Interface);
  emulators.append(new Famicom::FamicomInterface);
  emulators.append(new SuperFamicom::Interface);
  emulators.append(new MasterSystem::SG1000Interface);
  emulators.append(new MasterSystem::MasterSystemInterface);
  emulators.append(new MegaDrive::Interface);
  emulators.append(new PCEngine::PCEngineInterface);
  emulators.append(new PCEngine::SuperGrafxInterface);
  emulators.append(new GameBoy::GameBoyInterface);
  emulators.append(new GameBoy::GameBoyColorInterface);
  emulators.append(new GameBoyAdvance::Interface);
  emulators.append(new MasterSystem::GameGearInterface);
  emulators.append(new WonderSwan::WonderSwanInterface);
  emulators.append(new WonderSwan::WonderSwanColorInterface);
  emulators.append(new Famicom::VSSystemInterface);
  emulators.append(new Famicom::PlayChoice10Interface);
  emulators.append(new Famicom::FamicomBoxInterface);

  new Presentation;
  presentation->setVisible();

  if(settings["Crashed"].boolean()) {
    MessageDialog().setText("Driver crash detected. Video/Audio/Input drivers have been disabled.").information();
    settings["Video/Driver"].setValue("None");
    settings["Audio/Driver"].setValue("None");
    settings["Input/Driver"].setValue("None");
  }

  settings["Crashed"].setValue(true);
  settings.save();

  if(!Video::availableDrivers().find(settings["Video/Driver"].text())) {
    settings["Video/Driver"].setValue("None");
  }
  video = Video::create(settings["Video/Driver"].text());
  video->setContext(presentation->viewport.handle());
  video->setBlocking(settings["Video/Synchronize"].boolean());
  if(!video->ready()) MessageDialog().setText("Failed to initialize video driver").warning();
  presentation->clearViewport();

  if(!Audio::availableDrivers().find(settings["Audio/Driver"].text())) {
    settings["Audio/Driver"].setValue("None");
  }
  audio = Audio::create(settings["Audio/Driver"].text());
  audio->setExclusive(settings["Audio/Exclusive"].boolean());
  audio->setContext(presentation->viewport.handle());
  audio->setDevice(settings["Audio/Device"].text());
  audio->setBlocking(settings["Audio/Synchronize"].boolean());
  audio->setChannels(2);
  if(!audio->ready()) MessageDialog().setText("Failed to initialize audio driver").warning();

  if(!Input::availableDrivers().find(settings["Input/Driver"].text())) {
    settings["Input/Driver"].setValue("None");
  }
  input = Input::create(settings["Input/Driver"].text());
  input->setContext(presentation->viewport.handle());
  input->onChange({&InputManager::onChange, &inputManager()});
  if(!input->ready()) MessageDialog().setText("Failed to initialize input driver").warning();

  settings["Crashed"].setValue(false);
  settings.save();

  new InputManager;
  new SettingsManager;
  new CheatDatabase;
  new ToolsManager;
  new DipSwitches;
  new AboutWindow;

  updateVideoShader();  //Must be moved below the following block to support the "Auto" shader
  updateAudioDriver();
  updateAudioEffects();

  args.takeLeft();  //ignore program location in argument parsing
  for(auto& argument : args) {
    if(argument == "--fullscreen") {
      presentation->toggleFullScreen();
    } else if(directory::exists(argument.split("|", 1L).right())) {
      if(!argument.transform("\\", "/").endsWith("/")) argument.append("/");
      mediumQueue.append(argument);
    } else if(file::exists(argument)) {
      if(auto result = execute("cart-pal", "--import", argument)) {
        mediumQueue.append(result.output.strip());
      }
    }
  }
  loadMedium();

  Application::onMain({&Program::main, this});
}

auto Program::main() -> void {
  updateStatusText();
  inputManager->poll();
  inputManager->pollHotkeys();

  if(!emulator || !emulator->loaded() || pause || (!focused() && settings["Input/FocusLoss/Pause"].boolean())) {
    audio->clear();
    usleep(20 * 1000);
    return;
  }

  emulator->run();
}

auto Program::quit() -> void {
  hasQuit = true;
  unloadMedium();
  settings.save();
  inputManager->quit();
  video.reset();
  audio.reset();
  input.reset();
  Application::quit();
}
