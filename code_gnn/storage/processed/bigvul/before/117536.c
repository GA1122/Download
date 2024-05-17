void ChromeBrowserMainPartsChromeos::PostMainMessageLoopRun() {
  chromeos::BootTimesLoader::Get()->AddLogoutTimeMarker("UIMessageLoopEnded",
                                                        true);

  g_browser_process->oom_priority_manager()->Stop();

  if (UpgradeDetectorChromeos::GetInstance())
    UpgradeDetectorChromeos::GetInstance()->Shutdown();

  if (chromeos::CrosNetworkChangeNotifierFactory::GetInstance())
    chromeos::CrosNetworkChangeNotifierFactory::GetInstance()->Shutdown();

  if (session_manager_observer_.get()) {
    chromeos::DBusThreadManager::Get()->GetSessionManagerClient()->
        RemoveObserver(session_manager_observer_.get());
  }
  if (brightness_observer_.get()) {
    chromeos::DBusThreadManager::Get()->GetPowerManagerClient()
        ->RemoveObserver(brightness_observer_.get());
  }

  chromeos::accessibility::SystemEventObserver::Shutdown();
  chromeos::BluetoothManager::Shutdown();

  chromeos::XInputHierarchyChangedEventListener::GetInstance()->Stop();

  chromeos::SystemKeyEventListener::Shutdown();
  chromeos::AudioHandler::Shutdown();

  ChromeBrowserMainPartsLinux::PostMainMessageLoopRun();
}
