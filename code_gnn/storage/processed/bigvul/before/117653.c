bool BrowserInit::ProcessCmdLineImpl(const CommandLine& command_line,
                                     const FilePath& cur_dir,
                                     bool process_startup,
                                     Profile* profile,
                                     int* return_code,
                                     BrowserInit* browser_init) {
  DCHECK(profile);
  if (process_startup) {
    if (command_line.HasSwitch(switches::kDisablePromptOnRepost))
      content::NavigationController::DisablePromptOnRepost();

    RegisterComponentsForUpdate(command_line);

    if (command_line.HasSwitch(switches::kTestingChannelID)) {
      std::string testing_channel_id = command_line.GetSwitchValueASCII(
          switches::kTestingChannelID);
      int expected_tab_count = 1;
      if (command_line.HasSwitch(switches::kNoStartupWindow)) {
        expected_tab_count = 0;
#if defined(OS_CHROMEOS)
      } else if (command_line.HasSwitch(switches::kLoginManager)) {
        expected_tab_count = 0;
#endif
      } else if (command_line.HasSwitch(switches::kRestoreLastSession)) {
        std::string restore_session_value(
            command_line.GetSwitchValueASCII(switches::kRestoreLastSession));
        base::StringToInt(restore_session_value, &expected_tab_count);
      } else {
        std::vector<GURL> urls_to_open = GetURLsFromCommandLine(
            command_line, cur_dir, profile);
        expected_tab_count =
            std::max(1, static_cast<int>(urls_to_open.size()));
      }
      if (!CreateAutomationProvider<TestingAutomationProvider>(
          testing_channel_id,
          profile,
          static_cast<size_t>(expected_tab_count)))
        return false;
    }
  }

  bool silent_launch = false;

  if (command_line.HasSwitch(switches::kAutomationClientChannelID)) {
    std::string automation_channel_id = command_line.GetSwitchValueASCII(
        switches::kAutomationClientChannelID);
    std::vector<GURL> urls_to_open = GetURLsFromCommandLine(
        command_line, cur_dir, profile);
    size_t expected_tabs =
        std::max(static_cast<int>(urls_to_open.size()), 0);
    if (expected_tabs == 0)
      silent_launch = true;

    if (command_line.HasSwitch(switches::kChromeFrame)) {
#if !defined(USE_AURA)
      if (!CreateAutomationProvider<ChromeFrameAutomationProvider>(
          automation_channel_id, profile, expected_tabs))
        return false;
#endif
    } else {
      if (!CreateAutomationProvider<AutomationProvider>(
          automation_channel_id, profile, expected_tabs))
        return false;
    }
  }

  if (command_line.HasSwitch(switches::kNotifyCloudPrintTokenExpired)) {
    silent_launch = true;
    CloudPrintProxyServiceFactory::GetForProfile(profile)->
        ShowTokenExpiredNotification();
  }

  if (command_line.HasSwitch(switches::kCloudPrintFile) &&
      print_dialog_cloud::CreatePrintDialogFromCommandLine(command_line)) {
    silent_launch = true;
  }

  if (command_line.HasSwitch(switches::kExplicitlyAllowedPorts)) {
    std::string allowed_ports =
        command_line.GetSwitchValueASCII(switches::kExplicitlyAllowedPorts);
    net::SetExplicitlyAllowedPorts(allowed_ports);
  }

#if defined(OS_CHROMEOS)
  if (command_line.HasSwitch(switches::kLoginManager) ||
      command_line.HasSwitch(switches::kLoginPassword)) {
    silent_launch = true;
  }
#endif

#if defined(TOOLKIT_VIEWS) && defined(OS_LINUX)
  std::string touch_devices =
    command_line.GetSwitchValueASCII(switches::kTouchDevices);

  if (!touch_devices.empty()) {
    std::vector<std::string> devs;
    std::vector<unsigned int> device_ids;
    unsigned int devid;
    base::SplitString(touch_devices, ',', &devs);
    for (std::vector<std::string>::iterator iter = devs.begin();
        iter != devs.end(); ++iter) {
      if (base::StringToInt(*iter, reinterpret_cast<int*>(&devid)))
        device_ids.push_back(devid);
      else
        DLOG(WARNING) << "Invalid touch-device id: " << *iter;
    }
    ui::TouchFactory::GetInstance()->SetTouchDeviceList(device_ids);
  }
#endif

  if (!silent_launch) {
    IsProcessStartup is_process_startup = process_startup ?
        IS_PROCESS_STARTUP : IS_NOT_PROCESS_STARTUP;
    IsFirstRun is_first_run = first_run::IsChromeFirstRun() ?
        IS_FIRST_RUN : IS_NOT_FIRST_RUN;
    return browser_init->LaunchBrowser(command_line, profile, cur_dir,
        is_process_startup, is_first_run, return_code);
  }
  return true;
}
