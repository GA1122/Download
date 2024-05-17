void Browser::OpenKeywordEditor() {
  UserMetrics::RecordAction(UserMetricsAction("EditSearchEngines"), profile_);
  if (!CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kDisableTabbedOptions)) {
    ShowOptionsTab(chrome::kSearchEnginesSubPage);
  } else {
    window_->ShowSearchEnginesDialog();
  }
}
