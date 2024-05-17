void Browser::OpenUpdateChromeDialog() {
  UserMetrics::RecordAction(UserMetricsAction("UpdateChrome"));
  window_->ShowUpdateChromeDialog();
}
