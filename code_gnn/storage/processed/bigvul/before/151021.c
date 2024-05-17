void DevToolsUIBindings::RemovePreference(const std::string& name) {
  DictionaryPrefUpdate update(profile_->GetPrefs(),
                              prefs::kDevToolsPreferences);
  update.Get()->RemoveWithoutPathExpansion(name, nullptr);
}
