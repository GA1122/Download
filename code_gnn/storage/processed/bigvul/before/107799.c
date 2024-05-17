void Browser::NewWindow() {
  if (browser_defaults::kAlwaysOpenIncognitoWindow &&
      CommandLine::ForCurrentProcess()->HasSwitch(switches::kIncognito)) {
    NewIncognitoWindow();
    return;
  }
  UserMetrics::RecordAction(UserMetricsAction("NewWindow"), profile_);
  SessionService* session_service =
      profile_->GetOriginalProfile()->GetSessionService();
  if (!session_service ||
      !session_service->RestoreIfNecessary(std::vector<GURL>())) {
    Browser::OpenEmptyWindow(profile_->GetOriginalProfile());
  }
}
