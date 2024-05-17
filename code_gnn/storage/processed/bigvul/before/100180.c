BrowserInit::LaunchWithProfile::LaunchWithProfile(
    const std::wstring& cur_dir,
    const CommandLine& command_line)
        : cur_dir_(cur_dir),
          command_line_(command_line),
          profile_(NULL),
          browser_init_(NULL) {
}
