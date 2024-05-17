void Browser::FocusNextPane() {
  UserMetrics::RecordAction(UserMetricsAction("FocusNextPane"));
  window_->RotatePaneFocus(true);
}
