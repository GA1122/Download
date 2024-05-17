bool BrowserInit::LaunchBrowser(const CommandLine& command_line,
                                Profile* profile,
                                const FilePath& cur_dir,
                                IsProcessStartup process_startup,
                                IsFirstRun is_first_run,
                                int* return_code) {
  in_startup = process_startup == IS_PROCESS_STARTUP;
  DCHECK(profile);

  if (IncognitoModePrefs::ShouldLaunchIncognito(command_line,
                                                profile->GetPrefs())) {
    profile = profile->GetOffTheRecordProfile();
  } else if (command_line.HasSwitch(switches::kIncognito)) {
    LOG(WARNING) << "Incognito mode disabled by policy, launching a normal "
                 << "browser session.";
  }

  BrowserInit::LaunchWithProfile lwp(cur_dir, command_line, this, is_first_run);
  std::vector<GURL> urls_to_launch = BrowserInit::GetURLsFromCommandLine(
      command_line, cur_dir, profile);
  bool launched = lwp.Launch(profile, urls_to_launch, in_startup);
  in_startup = false;

  if (!launched) {
    LOG(ERROR) << "launch error";
    if (return_code)
      *return_code = chrome::RESULT_CODE_INVALID_CMDLINE_URL;
    return false;
  }

#if defined(OS_CHROMEOS)
  profile->InitChromeOSPreferences();

#if defined(TOOLKIT_USES_GTK)
  chromeos::WmMessageListener::GetInstance();
#endif

  if (process_startup) {
    static chromeos::LowBatteryObserver* low_battery_observer =
        new chromeos::LowBatteryObserver(profile);
    chromeos::DBusThreadManager::Get()->GetPowerManagerClient()->AddObserver(
        low_battery_observer);

    static chromeos::NetworkMessageObserver* network_message_observer =
        new chromeos::NetworkMessageObserver(profile);
    chromeos::CrosLibrary::Get()->GetNetworkLibrary()
        ->AddNetworkManagerObserver(network_message_observer);
    chromeos::CrosLibrary::Get()->GetNetworkLibrary()
        ->AddCellularDataPlanObserver(network_message_observer);
    chromeos::CrosLibrary::Get()->GetNetworkLibrary()
        ->AddUserActionObserver(network_message_observer);

    static chromeos::SmsObserver* sms_observer =
        new chromeos::SmsObserver(profile);
    chromeos::CrosLibrary::Get()->GetNetworkLibrary()
        ->AddNetworkManagerObserver(sms_observer);

    profile->SetupChromeOSEnterpriseExtensionObserver();
  }
#endif
  return true;
}
