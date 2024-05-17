void ChromeBrowserMainPartsChromeos::PostBrowserStart() {

  chromeos::system::StatisticsProvider::GetInstance();

  if (parsed_command_line().HasSwitch(switches::kEnableBluetooth)) {
    chromeos::BluetoothManager::Initialize();
  }

#if defined(USE_AURA)
  power_button_observer_.reset(new chromeos::PowerButtonObserver);
  chromeos::DBusThreadManager::Get()->GetPowerManagerClient()->AddObserver(
      power_button_observer_.get());
#endif

  ChromeBrowserMainPartsLinux::PostBrowserStart();
}
