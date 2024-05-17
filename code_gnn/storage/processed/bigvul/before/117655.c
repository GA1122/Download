void BrowserInit::ProcessCommandLineOnProfileCreated(
    const CommandLine& cmd_line,
    const FilePath& cur_dir,
    Profile* profile,
    Profile::CreateStatus status) {
  if (status == Profile::CREATE_STATUS_INITIALIZED)
    ProcessCmdLineImpl(cmd_line, cur_dir, false, profile, NULL, NULL);
}
