void Browser::NewIncognitoWindow() {
  UserMetrics::RecordAction(UserMetricsAction("NewIncognitoWindow"), profile_);
  Browser::OpenEmptyWindow(profile_->GetOffTheRecordProfile());
}
