void Browser::OpenSystemOptionsDialog() {
  UserMetrics::RecordAction(UserMetricsAction("OpenSystemOptionsDialog"),
                            profile_);
  if (!CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kDisableTabbedOptions)) {
    ShowOptionsTab(chrome::kSystemOptionsSubPage);
  } else {
    ShowOptionsWindow(OPTIONS_PAGE_SYSTEM, OPTIONS_GROUP_NONE,
                      profile_);
  }
}
