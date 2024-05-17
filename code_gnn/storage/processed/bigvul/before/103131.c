void Browser::NewIncognitoWindow() {
  if (!incognito_mode_allowed_.GetValue()) {
    NewWindow();
    return;
  }

  UserMetrics::RecordAction(UserMetricsAction("NewIncognitoWindow"), profile_);
  Browser::OpenEmptyWindow(profile_->GetOffTheRecordProfile());
}
