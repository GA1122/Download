bool BrowserInit::LaunchWithProfile::ProcessStartupURLs(
    const std::vector<GURL>& urls_to_open) {
  SessionStartupPref pref = GetSessionStartupPref(command_line_, profile_);
  if (command_line_.HasSwitch(switches::kTestingChannelID) &&
      !command_line_.HasSwitch(switches::kRestoreLastSession) &&
      browser_defaults::kDefaultSessionStartupType !=
      SessionStartupPref::DEFAULT) {
    return false;
  }

  if (pref.type == SessionStartupPref::LAST) {
    if (!profile_->DidLastSessionExitCleanly() &&
        !command_line_.HasSwitch(switches::kRestoreLastSession)) {
      return false;
    }

  uint32 restore_behavior = SessionRestore::SYNCHRONOUS |
                            SessionRestore::ALWAYS_CREATE_TABBED_BROWSER;
#if defined(OS_MACOSX)
    if (base::mac::WasLaunchedAsLoginOrResumeItem()) {
      restore_behavior = restore_behavior &
                         ~SessionRestore::ALWAYS_CREATE_TABBED_BROWSER;
    }
#endif

    Browser* browser = SessionRestore::RestoreSession(profile_,
                                                      NULL,
                                                      restore_behavior,
                                                      urls_to_open);
    AddInfoBarsIfNecessary(browser);
    return true;
  }

  Browser* browser = ProcessSpecifiedURLs(urls_to_open);
  if (!browser)
    return false;

  AddInfoBarsIfNecessary(browser);
  return true;
}
