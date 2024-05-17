void Browser::FocusLocationBar() {
  UserMetrics::RecordAction(UserMetricsAction("FocusLocation"));
  window_->SetFocusToLocationBar(true);
}
