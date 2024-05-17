void Browser::OpenLanguageOptionsDialog() {
  UserMetrics::RecordAction(UserMetricsAction("OpenLanguageOptionsDialog"),
                            profile_);
  if (!CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kDisableTabbedOptions)) {
    ShowOptionsTab(chrome::kLanguageOptionsSubPage);
  } else {
    chromeos::LanguageConfigView::Show(profile_, NULL);
  }
}
