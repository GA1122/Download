void Browser::Exit() {
  UserMetrics::RecordAction(UserMetricsAction("Exit"));
  BrowserList::AttemptUserExit();
}
