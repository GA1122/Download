void Browser::Stop() {
  UserMetrics::RecordAction(UserMetricsAction("Stop"), profile_);
  GetSelectedTabContentsWrapper()->tab_contents()->Stop();
}
