MasterPreferences::MasterPreferences(const base::FilePath& prefs_path)
    : distribution_(NULL),
      preferences_read_from_file_(false),
      chrome_(true),
      multi_install_(false) {
  InitializeFromFilePath(prefs_path);
}
