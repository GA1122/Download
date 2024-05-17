void ChromeBrowserMainPartsChromeos::PostMainMessageLoopStart() {
  MessageLoopForUI* message_loop = MessageLoopForUI::current();
  message_loop->AddObserver(g_message_loop_observer.Pointer());

  chromeos::DBusThreadManager::Initialize();

  brightness_observer_.reset(new chromeos::BrightnessObserver());
  chromeos::DBusThreadManager::Get()->GetPowerManagerClient()->AddObserver(
      brightness_observer_.get());
  session_manager_observer_.reset(new chromeos::SessionManagerObserver);
  chromeos::DBusThreadManager::Get()->GetSessionManagerClient()->
      AddObserver(session_manager_observer_.get());

  chromeos::disks::DiskMountManager::Initialize();

  chromeos::accessibility::SystemEventObserver::Initialize();

  chromeos::CrosNetworkChangeNotifierFactory::GetInstance()->Init();

  UpgradeDetectorChromeos::GetInstance()->Init();

  if (chromeos::system::runtime_environment::IsRunningOnChromeOS()) {
#if defined(GOOGLE_CHROME_BUILD)
      chromeos::input_method::InputMethodManager::GetInstance()->
          GetXKeyboard()->SetNumLockEnabled(true);
#endif

#if defined(USE_AURA)
      initial_browser_window_observer_.reset(
          new chromeos::InitialBrowserWindowObserver);
#endif
  }

  ChromeBrowserMainPartsLinux::PostMainMessageLoopStart();
}
