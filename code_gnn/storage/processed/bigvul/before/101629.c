void Browser::DuplicateTab() {
  UserMetrics::RecordAction(UserMetricsAction("Duplicate"));
  DuplicateContentsAt(active_index());
}
