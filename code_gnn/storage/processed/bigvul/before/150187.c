void StartupBrowserCreator::ProcessCommandLineOnProfileCreated(
    const base::CommandLine& command_line,
    const base::FilePath& cur_dir,
    Profile* profile,
    Profile::CreateStatus status) {
  if (status != Profile::CREATE_STATUS_INITIALIZED)
    return;
  StartupBrowserCreator startup_browser_creator;
  startup_browser_creator.ProcessCmdLineImpl(command_line, cur_dir, false,
                                             profile, Profiles());
}
