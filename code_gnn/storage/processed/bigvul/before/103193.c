void Browser::ShowBookmarkManagerTab() {
  UserMetrics::RecordAction(UserMetricsAction("ShowBookmarks"), profile_);
  ShowSingletonTab(GURL(chrome::kChromeUIBookmarksURL));
}
