void Browser::DuplicateTab() {
  UserMetrics::RecordAction(UserMetricsAction("Duplicate"), profile_);
  DuplicateContentsAt(active_index());
}
