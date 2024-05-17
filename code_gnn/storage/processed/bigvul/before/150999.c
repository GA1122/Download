void DevToolsUIBindings::GetPreferences(const DispatchCallback& callback) {
  const DictionaryValue* prefs =
      profile_->GetPrefs()->GetDictionary(prefs::kDevToolsPreferences);
  callback.Run(prefs);
}
