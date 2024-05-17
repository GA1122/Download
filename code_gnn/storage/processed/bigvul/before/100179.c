bool LaunchBrowser(const CommandLine& command_line, Profile* profile,
                   const std::wstring& cur_dir, bool process_startup,
                   int* return_code, BrowserInit* browser_init) {
  in_startup = process_startup;
  DCHECK(profile);

  if (command_line.HasSwitch(switches::kIncognito))
    profile = profile->GetOffTheRecordProfile();

  BrowserInit::LaunchWithProfile lwp(cur_dir, command_line, browser_init);
  bool launched = lwp.Launch(profile, process_startup);
  in_startup = false;

  if (!launched) {
    LOG(ERROR) << "launch error";
    if (return_code)
      *return_code = ResultCodes::INVALID_CMDLINE_URL;
    return false;
  }

#if defined(OS_CHROMEOS)
  TabOverviewMessageListener::instance();

  const CommandLine& parsed_command_line = *CommandLine::ForCurrentProcess();
  if (parsed_command_line.HasSwitch(switches::kEnableGView)) {
    chromeos::GViewRequestInterceptor::GetGViewRequestInterceptor();
  }
  if (process_startup) {
    chromeos::MountLibrary* lib = chromeos::MountLibrary::Get();
    chromeos::USBMountObserver* observe = chromeos::USBMountObserver::Get();
    observe->set_profile(profile);
    lib->AddObserver(observe);
  }
#endif
  return true;
}
