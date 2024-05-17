void Browser::OpenOptionsDialog() {
  UserMetrics::RecordAction(UserMetricsAction("ShowOptions"), profile_);
  if (!CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kDisableTabbedOptions)) {
    ShowOptionsTab(chrome::kDefaultOptionsSubPage);
  } else {
    ShowOptionsWindow(OPTIONS_PAGE_DEFAULT, OPTIONS_GROUP_NONE, profile_);
  }
}
