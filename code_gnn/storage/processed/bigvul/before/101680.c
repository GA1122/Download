void Browser::GoForward(WindowOpenDisposition disposition) {
  UserMetrics::RecordAction(UserMetricsAction("Forward"));
  if (CanGoForward())
    GetOrCloneTabForDisposition(disposition)->controller().GoForward();
}
