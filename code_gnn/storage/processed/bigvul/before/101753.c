void Browser::OpenSearchEngineOptionsDialog() {
  UserMetrics::RecordAction(UserMetricsAction("EditSearchEngines"));
  ShowOptionsTab(chrome::kSearchEnginesSubPage);
}
