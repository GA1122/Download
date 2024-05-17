void Browser::ShowAboutConflictsTab() {
  UserMetrics::RecordAction(UserMetricsAction("AboutConflicts"));
  ShowSingletonTab(GURL(chrome::kChromeUIConflictsURL));
}
