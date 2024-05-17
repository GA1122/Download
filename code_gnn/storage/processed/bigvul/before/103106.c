void Browser::FocusBookmarksToolbar() {
  UserMetrics::RecordAction(UserMetricsAction("FocusBookmarksToolbar"),
                            profile_);
  window_->FocusBookmarksToolbar();
}
