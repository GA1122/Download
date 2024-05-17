void Browser::Cut() {
  UserMetrics::RecordAction(UserMetricsAction("Cut"), profile_);
  window()->Cut();
}
