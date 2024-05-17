void Browser::GoForward(WindowOpenDisposition disposition) {
  UserMetrics::RecordAction(UserMetricsAction("Forward"), profile_);
  if (GetSelectedTabContentsWrapper()->controller().CanGoForward())
    GetOrCloneTabForDisposition(disposition)->controller().GoForward();
}
