void Browser::ToggleBookmarkBar() {
  UserMetrics::RecordAction(UserMetricsAction("ShowBookmarksBar"));
  window_->ToggleBookmarkBar();
}
