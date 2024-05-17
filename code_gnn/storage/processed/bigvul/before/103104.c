void Browser::FindPrevious() {
  UserMetrics::RecordAction(UserMetricsAction("FindPrevious"), profile_);
  FindInPage(true, false);
}
