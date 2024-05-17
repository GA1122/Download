MasterPreferences::MasterPreferences(const std::string& prefs)
    : distribution_(NULL),
      preferences_read_from_file_(false),
      chrome_(true),
      multi_install_(false) {
  InitializeFromString(prefs);
}
