void Browser::Paste() {
  UserMetrics::RecordAction(UserMetricsAction("Paste"));
  window()->Paste();
}
