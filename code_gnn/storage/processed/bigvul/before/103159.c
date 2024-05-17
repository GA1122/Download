void Browser::OpenSearchEngineOptionsDialog() {
  UserMetrics::RecordAction(UserMetricsAction("EditSearchEngines"), profile_);
  ShowOptionsTab(chrome::kSearchEnginesSubPage);
}
