void Browser::OpenUpdateChromeDialog() {
  UserMetrics::RecordAction(UserMetricsAction("UpdateChrome"), profile_);
  window_->ShowUpdateChromeDialog();
}
