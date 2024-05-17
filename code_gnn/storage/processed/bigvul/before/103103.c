void Browser::FindNext() {
  UserMetrics::RecordAction(UserMetricsAction("FindNext"), profile_);
  FindInPage(true, true);
}
