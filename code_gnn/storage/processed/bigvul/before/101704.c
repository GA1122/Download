void Browser::NewIncognitoWindow() {
  if (IncognitoModePrefs::GetAvailability(profile_->GetPrefs()) ==
          IncognitoModePrefs::DISABLED) {
    NewWindow();
    return;
  }

  UserMetrics::RecordAction(UserMetricsAction("NewIncognitoWindow"));
  Browser::OpenEmptyWindow(profile_->GetOffTheRecordProfile());
}
