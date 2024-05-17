void Browser::ReloadIgnoringCache(WindowOpenDisposition disposition) {
  UserMetrics::RecordAction(UserMetricsAction("ReloadIgnoringCache"), profile_);
  ReloadInternal(disposition, true);
}
