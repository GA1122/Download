void Browser::FocusLocationBar() {
  UserMetrics::RecordAction(UserMetricsAction("FocusLocation"), profile_);
  window_->SetFocusToLocationBar(true);
}
