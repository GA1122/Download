void Browser::OpenSystemOptionsDialog() {
  UserMetrics::RecordAction(UserMetricsAction("OpenSystemOptionsDialog"));
  ShowOptionsTab(chrome::kSystemOptionsSubPage);
}
