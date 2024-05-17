void Browser::OpenSearchEngineOptionsDialog() {
  if (!CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kDisableTabbedOptions)) {
    OpenKeywordEditor();
  } else {
    ShowOptionsWindow(OPTIONS_PAGE_GENERAL, OPTIONS_GROUP_DEFAULT_SEARCH,
                      profile_);
  }
}
