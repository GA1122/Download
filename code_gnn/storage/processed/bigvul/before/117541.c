void ChromeBrowserMainPartsChromeos::PreMainMessageLoopRun() {
  chromeos::AudioHandler::Initialize();

  if (chromeos::system::runtime_environment::IsRunningOnChromeOS() &&
      !parameters().ui_task) {   
    chromeos::SystemKeyEventListener::Initialize();
  }

  chromeos::XInputHierarchyChangedEventListener::GetInstance();

  ChromeBrowserMainPartsLinux::PreMainMessageLoopRun();
}
