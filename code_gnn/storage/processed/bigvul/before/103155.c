void Browser::OpenLanguageOptionsDialog() {
  UserMetrics::RecordAction(UserMetricsAction("OpenLanguageOptionsDialog"),
                            profile_);
  ShowOptionsTab(chrome::kLanguageOptionsSubPage);
}
