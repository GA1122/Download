void Browser::DuplicateTab() {
  UserMetrics::RecordAction(UserMetricsAction("Duplicate"), profile_);
  DuplicateContentsAt(selected_index());
}
