void Browser::ReloadIgnoringCache(WindowOpenDisposition disposition) {
  UserMetrics::RecordAction(UserMetricsAction("ReloadIgnoringCache"));
  ReloadInternal(disposition, true);
}
