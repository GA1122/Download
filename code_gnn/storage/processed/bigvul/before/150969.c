void DevToolsUIBindings::ClearPreferences() {
  DictionaryPrefUpdate update(profile_->GetPrefs(),
                              prefs::kDevToolsPreferences);
  update.Get()->Clear();
}
