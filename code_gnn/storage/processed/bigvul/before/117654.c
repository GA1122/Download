void BrowserInit::ProcessCommandLineAlreadyRunning(const CommandLine& cmd_line,
                                                   const FilePath& cur_dir) {
  if (cmd_line.HasSwitch(switches::kProfileDirectory)) {
    ProfileManager* profile_manager = g_browser_process->profile_manager();
    FilePath path = cmd_line.GetSwitchValuePath(switches::kProfileDirectory);
    path = profile_manager->user_data_dir().Append(path);
    profile_manager->CreateProfileAsync(path,
        base::Bind(&BrowserInit::ProcessCommandLineOnProfileCreated,
                   cmd_line, cur_dir));
    return;
  }

  Profile* profile = ProfileManager::GetLastUsedProfile();
  if (!profile) {
    NOTREACHED();
    return;
  }
  ProcessCmdLineImpl(cmd_line, cur_dir, false, profile, NULL, NULL);
}
