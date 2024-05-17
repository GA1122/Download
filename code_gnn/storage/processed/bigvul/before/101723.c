void Browser::OpenBookmarkManager() {
  UserMetrics::RecordAction(UserMetricsAction("ShowBookmarkManager"));
  UserMetrics::RecordAction(UserMetricsAction("ShowBookmarks"));
  ShowSingletonTabOverwritingNTP(
      GetSingletonTabNavigateParams(GURL(chrome::kChromeUIBookmarksURL)));
}
