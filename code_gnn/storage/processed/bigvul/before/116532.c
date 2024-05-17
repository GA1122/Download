bool ChromeContentClient::SandboxPlugin(CommandLine* command_line,
                                        sandbox::TargetPolicy* policy) {
  std::wstring plugin_dll = command_line->
      GetSwitchValueNative(switches::kPluginPath);

  FilePath builtin_flash;
  if (!PathService::Get(chrome::FILE_FLASH_PLUGIN, &builtin_flash))
    return false;

  FilePath plugin_path(plugin_dll);
  if (plugin_path.BaseName() != builtin_flash.BaseName())
    return false;

  if (base::win::GetVersion() <= base::win::VERSION_XP ||
      CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kDisableFlashSandbox)) {
    return false;
  }

  if (policy->AddRule(sandbox::TargetPolicy::SUBSYS_NAMED_PIPES,
                      sandbox::TargetPolicy::NAMEDPIPES_ALLOW_ANY,
                      L"\\\\.\\pipe\\chrome.*") != sandbox::SBOX_ALL_OK) {
    NOTREACHED();
    return false;
  }

  base::win::RegKey talk_key(HKEY_CURRENT_USER,
                             L"Software\\Google\\Google Talk Plugin",
                             KEY_READ);
  if (talk_key.Valid()) {
    string16 install_dir;
    if (talk_key.ReadValue(L"install_dir", &install_dir) == ERROR_SUCCESS) {
      if (install_dir[install_dir.size() - 1] != '\\')
        install_dir.append(L"\\*");
      else
        install_dir.append(L"*");
      if (policy->AddRule(sandbox::TargetPolicy::SUBSYS_FILES,
                          sandbox::TargetPolicy::FILES_ALLOW_READONLY,
                          install_dir.c_str()) != sandbox::SBOX_ALL_OK) {
        DVLOG(ERROR) << "Failed adding sandbox rule for Talk plugin";
      }
    }
    talk_key.Close();
  }

  if (LoadFlashBroker(plugin_path, command_line)) {
    policy->SetJobLevel(sandbox::JOB_UNPROTECTED, 0);
    policy->SetTokenLevel(sandbox::USER_RESTRICTED_SAME_ACCESS,
                          sandbox::USER_INTERACTIVE);
    if (base::win::GetVersion() == base::win::VERSION_VISTA) {
      ::ChangeWindowMessageFilter(WM_MOUSEWHEEL, MSGFLT_ADD);
      ::ChangeWindowMessageFilter(WM_APPCOMMAND, MSGFLT_ADD);
    }
    policy->SetIntegrityLevel(sandbox::INTEGRITY_LEVEL_LOW);
  } else {
    DLOG(WARNING) << "Failed to start flash broker";
    policy->SetJobLevel(sandbox::JOB_UNPROTECTED, 0);
    policy->SetTokenLevel(
        sandbox::USER_UNPROTECTED, sandbox::USER_UNPROTECTED);
  }

  return true;
}
