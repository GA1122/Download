void Browser::FindNext() {
  UserMetrics::RecordAction(UserMetricsAction("FindNext"));
  FindInPage(true, true);
}
