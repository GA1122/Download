void Browser::OpenBookmarkManager() {
  UserMetrics::RecordAction(UserMetricsAction("ShowBookmarkManager"), profile_);
  ShowBookmarkManagerTab();
}
