void Browser::OpenInternetOptionsDialog() {
  UserMetrics::RecordAction(UserMetricsAction("OpenInternetOptionsDialog"),
                            profile_);
  ShowOptionsTab(chrome::kInternetOptionsSubPage);
}
