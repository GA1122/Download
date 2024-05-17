void Browser::Copy() {
  UserMetrics::RecordAction(UserMetricsAction("Copy"));
  window()->Copy();
}
