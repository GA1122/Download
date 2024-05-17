void Browser::FocusPreviousPane() {
  UserMetrics::RecordAction(UserMetricsAction("FocusPreviousPane"), profile_);
  window_->RotatePaneFocus(false);
}
