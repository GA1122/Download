base::FilePath GetStartupProfilePath(const base::FilePath& user_data_dir,
                                     const base::CommandLine& command_line) {
#if defined(OS_WIN)
  if (command_line.HasSwitch(switches::kNotificationLaunchId)) {
    std::string profile_id = NotificationLaunchId::GetProfileIdFromLaunchId(
        command_line.GetSwitchValueNative(switches::kNotificationLaunchId));
    if (!profile_id.empty()) {
      return user_data_dir.Append(
          base::FilePath(base::UTF8ToUTF16(profile_id)));
    }
  }
#endif   

  if (IsGuestModeEnforced(command_line,   false))
    return profiles::GetDefaultProfileDir(user_data_dir);

  if (command_line.HasSwitch(switches::kProfileDirectory)) {
    return user_data_dir.Append(
        command_line.GetSwitchValuePath(switches::kProfileDirectory));
  }

  return g_browser_process->profile_manager()->GetLastUsedProfileDir(
      user_data_dir);
}
