void Browser::FocusAppMenu() {
  UserMetrics::RecordAction(UserMetricsAction("FocusAppMenu"));
  window_->FocusAppMenu();
}
