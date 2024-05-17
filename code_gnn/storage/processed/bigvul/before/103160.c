void Browser::OpenSystemOptionsDialog() {
  UserMetrics::RecordAction(UserMetricsAction("OpenSystemOptionsDialog"),
                            profile_);
  ShowOptionsTab(chrome::kSystemOptionsSubPage);
}
