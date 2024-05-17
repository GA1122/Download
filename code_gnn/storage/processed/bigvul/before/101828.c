void Browser::Stop() {
  UserMetrics::RecordAction(UserMetricsAction("Stop"));
  GetSelectedTabContentsWrapper()->tab_contents()->Stop();
}
