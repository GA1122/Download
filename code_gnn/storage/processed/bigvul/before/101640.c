void Browser::Find() {
  UserMetrics::RecordAction(UserMetricsAction("Find"));
  FindInPage(false, false);
}
