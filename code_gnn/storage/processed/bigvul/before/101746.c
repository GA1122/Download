void Browser::OpenLanguageOptionsDialog() {
  UserMetrics::RecordAction(UserMetricsAction("OpenLanguageOptionsDialog"));
  ShowOptionsTab(chrome::kLanguageOptionsSubPage);
}
