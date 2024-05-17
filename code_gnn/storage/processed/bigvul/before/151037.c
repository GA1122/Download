void DevToolsUIBindings::SetPreference(const std::string& name,
                                   const std::string& value) {
  DictionaryPrefUpdate update(profile_->GetPrefs(),
                              prefs::kDevToolsPreferences);
  update.Get()->SetStringWithoutPathExpansion(name, value);
}
