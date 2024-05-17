bool SessionService::RestoreIfNecessary(const std::vector<GURL>& urls_to_open,
                                        Browser* browser) {
  if (ShouldNewWindowStartSession()) {
    if (move_on_new_browser_) {
      MoveCurrentSessionToLastSession();
      move_on_new_browser_ = false;
    }
    SessionStartupPref pref = BrowserInit::GetSessionStartupPref(
        *CommandLine::ForCurrentProcess(), profile());
    if (pref.type == SessionStartupPref::LAST) {
      SessionRestore::RestoreSession(
          profile(), browser,
          browser ? 0 : SessionRestore::ALWAYS_CREATE_TABBED_BROWSER,
          urls_to_open);
      return true;
    }
  }
  return false;
}
