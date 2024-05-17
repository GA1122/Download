void Browser::ShowHistoryTab() {
  UserMetrics::RecordAction(UserMetricsAction("ShowHistory"));
  ShowSingletonTabOverwritingNTP(
      GetSingletonTabNavigateParams(GURL(chrome::kChromeUIHistoryURL)));
}
