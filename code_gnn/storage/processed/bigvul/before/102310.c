void ExtensionPrefs::SavePrefs() {
  prefs_->ScheduleSavePersistentPrefs();
}
