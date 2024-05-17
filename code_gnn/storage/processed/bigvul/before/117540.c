void ChromeBrowserMainPartsChromeos::PreEarlyInitialization() {
  if (parsed_command_line().HasSwitch(switches::kGuestSession)) {
    CommandLine* singleton_command_line = CommandLine::ForCurrentProcess();
    singleton_command_line->AppendSwitch(switches::kDisableSync);
    singleton_command_line->AppendSwitch(switches::kDisableExtensions);
    browser_defaults::bookmarks_enabled = false;
  }

  ChromeBrowserMainPartsLinux::PreEarlyInitialization();
}
