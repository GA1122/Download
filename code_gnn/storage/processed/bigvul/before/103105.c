void Browser::FocusAppMenu() {
  UserMetrics::RecordAction(UserMetricsAction("FocusAppMenu"), profile_);
  window_->FocusAppMenu();
}
