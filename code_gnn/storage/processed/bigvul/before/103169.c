void Browser::Paste() {
  UserMetrics::RecordAction(UserMetricsAction("Paste"), profile_);
  window()->Paste();
}
