void Browser::NewWindow() {
  IncognitoModePrefs::Availability incognito_avail =
      IncognitoModePrefs::GetAvailability(profile_->GetPrefs());
  if (browser_defaults::kAlwaysOpenIncognitoWindow &&
      incognito_avail != IncognitoModePrefs::DISABLED &&
      (CommandLine::ForCurrentProcess()->HasSwitch(switches::kIncognito) ||
       incognito_avail == IncognitoModePrefs::FORCED)) {
    NewIncognitoWindow();
    return;
  }
  UserMetrics::RecordAction(UserMetricsAction("NewWindow"));
  SessionService* session_service =
      SessionServiceFactory::GetForProfile(profile_->GetOriginalProfile());
  if (!session_service ||
      !session_service->RestoreIfNecessary(std::vector<GURL>())) {
    Browser::OpenEmptyWindow(profile_->GetOriginalProfile());
  }
}
