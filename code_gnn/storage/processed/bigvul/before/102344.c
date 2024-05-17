  virtual ~ScopedExtensionControlledPrefUpdate() {
    prefs_->ScheduleSavePersistentPrefs();
  }
