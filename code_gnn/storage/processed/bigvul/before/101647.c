void Browser::FocusBookmarksToolbar() {
  UserMetrics::RecordAction(UserMetricsAction("FocusBookmarksToolbar"));
  window_->FocusBookmarksToolbar();
}
