void Browser::FocusToolbar() {
  UserMetrics::RecordAction(UserMetricsAction("FocusToolbar"));
  window_->FocusToolbar();
}
