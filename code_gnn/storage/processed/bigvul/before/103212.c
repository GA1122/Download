void Browser::ToggleBookmarkBar() {
  UserMetrics::RecordAction(UserMetricsAction("ShowBookmarksBar"), profile_);
  window_->ToggleBookmarkBar();
}
