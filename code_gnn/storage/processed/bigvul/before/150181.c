bool StartupBrowserCreator::LaunchBrowser(
    const base::CommandLine& command_line,
    Profile* profile,
    const base::FilePath& cur_dir,
    chrome::startup::IsProcessStartup process_startup,
    chrome::startup::IsFirstRun is_first_run) {
  DCHECK(profile);
  in_synchronous_profile_launch_ =
      process_startup == chrome::startup::IS_PROCESS_STARTUP;

  if (IncognitoModePrefs::ShouldLaunchIncognito(command_line,
                                                profile->GetPrefs())) {
    profile = profile->GetOffTheRecordProfile();
  } else if (command_line.HasSwitch(switches::kIncognito)) {
    LOG(WARNING) << "Incognito mode disabled by policy, launching a normal "
                 << "browser session.";
  }

  if (IsGuestModeEnforced(command_line,   true)) {
    profile = g_browser_process->profile_manager()
                  ->GetProfile(ProfileManager::GetGuestProfilePath())
                  ->GetOffTheRecordProfile();
  }

#if defined(OS_WIN)
  if (command_line.HasSwitch(credential_provider::kGcpwSigninSwitch))
    profile = profile->GetOffTheRecordProfile();
#endif

  if (!IsSilentLaunchEnabled(command_line, profile)) {
    StartupBrowserCreatorImpl lwp(cur_dir, command_line, this, is_first_run);
    const std::vector<GURL> urls_to_launch =
        GetURLsFromCommandLine(command_line, cur_dir, profile);
    const bool launched =
        lwp.Launch(profile, urls_to_launch, in_synchronous_profile_launch_);
    in_synchronous_profile_launch_ = false;
    if (!launched) {
      LOG(ERROR) << "launch error";
      return false;
    }
  } else {
    in_synchronous_profile_launch_ = false;
  }

  profile_launch_observer.Get().AddLaunched(profile);

  return true;
}
