void Browser::Copy() {
  UserMetrics::RecordAction(UserMetricsAction("Copy"), profile_);
  window()->Copy();
}
