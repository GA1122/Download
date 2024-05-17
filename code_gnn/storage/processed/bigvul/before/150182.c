bool StartupBrowserCreator::LaunchBrowserForLastProfiles(
    const base::CommandLine& command_line,
    const base::FilePath& cur_dir,
    bool process_startup,
    Profile* last_used_profile,
    const Profiles& last_opened_profiles) {
  chrome::startup::IsProcessStartup is_process_startup = process_startup ?
      chrome::startup::IS_PROCESS_STARTUP :
      chrome::startup::IS_NOT_PROCESS_STARTUP;
  chrome::startup::IsFirstRun is_first_run = first_run::IsChromeFirstRun() ?
      chrome::startup::IS_FIRST_RUN : chrome::startup::IS_NOT_FIRST_RUN;

  bool was_windows_notification_launch = false;
#if defined(OS_WIN)
  was_windows_notification_launch =
      command_line.HasSwitch(switches::kNotificationLaunchId);
#endif   

  if (last_opened_profiles.empty() || was_windows_notification_launch) {
    if (CanOpenProfileOnStartup(last_used_profile)) {
      Profile* profile_to_open =
          last_used_profile->IsGuestSession()
              ? last_used_profile->GetOffTheRecordProfile()
              : last_used_profile;

      return LaunchBrowser(command_line, profile_to_open, cur_dir,
                           is_process_startup, is_first_run);
    }

    ShowUserManagerOnStartup(command_line);
    return true;
  }
  return ProcessLastOpenedProfiles(command_line, cur_dir, is_process_startup,
                                   is_first_run, last_used_profile,
                                   last_opened_profiles);
}
