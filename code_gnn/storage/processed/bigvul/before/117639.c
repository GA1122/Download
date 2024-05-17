SessionStartupPref GetSessionStartupPref(const CommandLine& command_line,
                                         Profile* profile) {
  SessionStartupPref pref = SessionStartupPref::GetStartupPref(profile);
  if (command_line.HasSwitch(switches::kRestoreLastSession) ||
      BrowserInit::WasRestarted()) {
    pref.type = SessionStartupPref::LAST;
  }
  if (pref.type == SessionStartupPref::LAST &&
      IncognitoModePrefs::ShouldLaunchIncognito(command_line,
                                                profile->GetPrefs())) {
    pref.type = SessionStartupPref::DEFAULT;
  }
  return pref;
}
