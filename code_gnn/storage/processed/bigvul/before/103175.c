void Browser::Reload(WindowOpenDisposition disposition) {
  UserMetrics::RecordAction(UserMetricsAction("Reload"), profile_);
  ReloadInternal(disposition, false);
}
