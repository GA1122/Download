void Browser::ShowAboutConflictsTab() {
  UserMetrics::RecordAction(UserMetricsAction("AboutConflicts"), profile_);
  ShowSingletonTab(GURL(chrome::kChromeUIConflictsURL), false);
}
