BrowserInit::LaunchWithProfile::LaunchWithProfile(
    const FilePath& cur_dir,
    const CommandLine& command_line,
    IsFirstRun is_first_run)
        : cur_dir_(cur_dir),
          command_line_(command_line),
          profile_(NULL),
          browser_init_(NULL),
          is_first_run_(is_first_run == IS_FIRST_RUN) {
}