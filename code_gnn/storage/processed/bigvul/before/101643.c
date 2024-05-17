void Browser::FindPrevious() {
  UserMetrics::RecordAction(UserMetricsAction("FindPrevious"));
  FindInPage(true, false);
}
