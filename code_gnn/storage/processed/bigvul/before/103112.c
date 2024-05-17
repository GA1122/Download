void Browser::FocusToolbar() {
  UserMetrics::RecordAction(UserMetricsAction("FocusToolbar"), profile_);
  window_->FocusToolbar();
}
