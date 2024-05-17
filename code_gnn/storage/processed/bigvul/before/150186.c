void StartupBrowserCreator::ProcessCommandLineAlreadyRunning(
    const base::CommandLine& command_line,
    const base::FilePath& cur_dir,
    const base::FilePath& profile_path) {
  ProfileManager* profile_manager = g_browser_process->profile_manager();
  Profile* profile = profile_manager->GetProfileByPath(profile_path);

  if (!profile) {
    profile_manager->CreateProfileAsync(
        profile_path,
        base::BindRepeating(&ProcessCommandLineOnProfileCreated, command_line,
                            cur_dir),
        base::string16(), std::string());
    return;
  }
  StartupBrowserCreator startup_browser_creator;
  startup_browser_creator.ProcessCmdLineImpl(
      command_line, cur_dir,  false, profile, Profiles());
}
