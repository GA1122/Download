void Browser::OpenOptionsDialog() {
  UserMetrics::RecordAction(UserMetricsAction("ShowOptions"));
  ShowOptionsTab("");
}
