  virtual ~ScopedExtensionPrefUpdate() {
    prefs_->ScheduleSavePersistentPrefs();
  }
