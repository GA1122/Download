void Browser::OpenInternetOptionsDialog() {
  UserMetrics::RecordAction(UserMetricsAction("OpenInternetOptionsDialog"));
  ShowOptionsTab(chrome::kInternetOptionsSubPage);
}
