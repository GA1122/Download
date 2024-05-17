void Browser::Reload(WindowOpenDisposition disposition) {
  UserMetrics::RecordAction(UserMetricsAction("Reload"));
  ReloadInternal(disposition, false);
}
