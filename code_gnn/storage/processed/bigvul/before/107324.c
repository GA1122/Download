void Browser::OpenInternetOptionsDialog() {
  UserMetrics::RecordAction(UserMetricsAction("OpenInternetOptionsDialog"),
                            profile_);
  if (!CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kDisableTabbedOptions)) {
    ShowOptionsTab(chrome::kInternetOptionsSubPage);
  } else {
    ShowOptionsWindow(OPTIONS_PAGE_INTERNET, OPTIONS_GROUP_DEFAULT_SEARCH,
                      profile_);
  }
}
