void Browser::Find() {
  UserMetrics::RecordAction(UserMetricsAction("Find"), profile_);
  FindInPage(false, false);
}
