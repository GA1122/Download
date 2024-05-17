void Browser::Cut() {
  UserMetrics::RecordAction(UserMetricsAction("Cut"));
  window()->Cut();
}
