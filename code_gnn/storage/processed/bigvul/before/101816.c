void Browser::ShowExtensionsTab() {
  UserMetrics::RecordAction(UserMetricsAction("ShowExtensions"));
  ShowOptionsTab(chrome::kExtensionsSubPage);
}
