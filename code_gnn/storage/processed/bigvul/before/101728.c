void Browser::OpenBookmarkManagerWithHash(const std::string& action,
                                          int64 node_id) {
  UserMetrics::RecordAction(UserMetricsAction("ShowBookmarkManager"));
  UserMetrics::RecordAction(UserMetricsAction("ShowBookmarks"));
  browser::NavigateParams params(GetSingletonTabNavigateParams(
      GURL(chrome::kChromeUIBookmarksURL).Resolve(
      StringPrintf("/#%s%s", action.c_str(),
      base::Int64ToString(node_id).c_str()))));
  params.path_behavior = browser::NavigateParams::IGNORE_AND_NAVIGATE;
  ShowSingletonTabOverwritingNTP(params);
}
