void Browser::FocusChromeOSStatus() {
  UserMetrics::RecordAction(UserMetricsAction("FocusChromeOSStatus"), profile_);
  window_->FocusChromeOSStatus();
}
