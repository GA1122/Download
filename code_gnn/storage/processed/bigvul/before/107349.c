void Browser::ShowExtensionsTab() {
  UserMetrics::RecordAction(UserMetricsAction("ShowExtensions"), profile_);
  ShowSingletonTab(GURL(chrome::kChromeUIExtensionsURL), false);
}
