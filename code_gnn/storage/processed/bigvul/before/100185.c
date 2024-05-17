bool BrowserInit::ProcessCmdLineImpl(const CommandLine& command_line,
                                     const std::wstring& cur_dir,
                                     bool process_startup,
                                     Profile* profile,
                                     int* return_code,
                                     BrowserInit* browser_init) {
  DCHECK(profile);
  if (process_startup) {
    const std::string popup_count_string =
        command_line.GetSwitchValueASCII(switches::kOmniBoxPopupCount);
    if (!popup_count_string.empty()) {
      int count = 0;
      if (StringToInt(popup_count_string, &count)) {
        const int popup_count = std::max(0, count);
        AutocompleteResult::set_max_matches(popup_count);
        AutocompleteProvider::set_max_matches(popup_count / 2);
      }
    }

    if (command_line.HasSwitch(switches::kDisablePromptOnRepost))
      NavigationController::DisablePromptOnRepost();

    const std::string tab_count_string = command_line.GetSwitchValueASCII(
        switches::kTabCountToLoadOnSessionRestore);
    if (!tab_count_string.empty()) {
      int count = 0;
      if (StringToInt(tab_count_string, &count)) {
        const int tab_count = std::max(0, count);
        SessionRestore::num_tabs_to_load_ = static_cast<size_t>(tab_count);
      }
    }

    if (command_line.HasSwitch(switches::kTestingChannelID)) {
      std::string testing_channel_id = command_line.GetSwitchValueASCII(
          switches::kTestingChannelID);
      int expected_tab_count = 1;
      if (command_line.HasSwitch(switches::kRestoreLastSession)) {
        std::string restore_session_value(
            command_line.GetSwitchValueASCII(switches::kRestoreLastSession));
        StringToInt(restore_session_value, &expected_tab_count);
      } else {
        expected_tab_count =
            std::max(1, static_cast<int>(command_line.GetLooseValues().size()));
      }
      CreateAutomationProvider<TestingAutomationProvider>(
          testing_channel_id,
          profile,
          static_cast<size_t>(expected_tab_count));
    }

    if (command_line.HasSwitch(switches::kPackExtension)) {
      FilePath src_dir = command_line.GetSwitchValuePath(
          switches::kPackExtension);
      FilePath private_key_path;
      if (command_line.HasSwitch(switches::kPackExtensionKey)) {
        private_key_path = command_line.GetSwitchValuePath(
            switches::kPackExtensionKey);
      }

      FilePath output(src_dir.DirName().Append(src_dir.BaseName().value()));
      FilePath crx_path(output);
      crx_path = crx_path.ReplaceExtension(chrome::kExtensionFileExtension);
      FilePath output_private_key_path;
      if (private_key_path.empty()) {
        output_private_key_path = FilePath(output);
        output_private_key_path =
            output_private_key_path.ReplaceExtension(FILE_PATH_LITERAL("pem"));
      }

      scoped_ptr<ExtensionCreator> creator(new ExtensionCreator());
      if (creator->Run(src_dir, crx_path, private_key_path,
          output_private_key_path)) {
        std::wstring message;
        if (private_key_path.value().empty()) {
          message = StringPrintf(
              L"Created the following files:\n\n"
              L"Extension: %ls\n"
              L"Key File: %ls\n\n"
              L"Keep your key file in a safe place. You will need it to create "
              L"new versions of your extension.",
              crx_path.ToWStringHack().c_str(),
              output_private_key_path.ToWStringHack().c_str());
        } else {
          message = StringPrintf(L"Created the extension:\n\n%ls",
                                 crx_path.ToWStringHack().c_str());
        }
        ShowPackExtensionMessage(L"Extension Packaging Success", message);
      } else {
        ShowPackExtensionMessage(L"Extension Packaging Error",
            UTF8ToWide(creator->error_message()));
        return false;
      }
      return false;
    }
  }

  bool silent_launch = false;
  if (command_line.HasSwitch(switches::kAutomationClientChannelID)) {
    std::string automation_channel_id = command_line.GetSwitchValueASCII(
        switches::kAutomationClientChannelID);
    size_t expected_tabs =
        std::max(static_cast<int>(command_line.GetLooseValues().size()),
                 0);
    if (expected_tabs == 0)
      silent_launch = true;

    if (command_line.HasSwitch(switches::kChromeFrame)) {
      CreateAutomationProvider<ChromeFrameAutomationProvider>(
          automation_channel_id, profile, expected_tabs);
    } else {
      CreateAutomationProvider<AutomationProvider>(automation_channel_id,
                                                   profile, expected_tabs);
    }
  }

  if (command_line.HasSwitch(switches::kUseFlip)) {
    std::string flip_mode =
        command_line.GetSwitchValueASCII(switches::kUseFlip);
    net::HttpNetworkLayer::EnableFlip(flip_mode);
  }

  if (command_line.HasSwitch(switches::kExplicitlyAllowedPorts)) {
    std::wstring allowed_ports =
      command_line.GetSwitchValue(switches::kExplicitlyAllowedPorts);
    net::SetExplicitlyAllowedPorts(allowed_ports);
  }

  if (command_line.HasSwitch(switches::kEnableUserDataDirProfiles)) {
    UserDataManager::Get()->RefreshUserDataDirProfiles();
  }

  if (!silent_launch) {
    return LaunchBrowser(command_line, profile, cur_dir, process_startup,
                         return_code, browser_init);
  }
  return true;
}
